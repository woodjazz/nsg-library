/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

-------------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#if defined(IS_TARGET_LINUX)
#include "LinuxWindow.h"
#include "Engine.h"
#include "Tick.h"
#include "Keys.h"
#include "Log.h"
#include "Check.h"
#include "UTF8String.h"
#include "AppConfiguration.h"
#include "imgui.h"
#include <memory>
#include <string>
#include <locale>
#include <map>
#ifndef __GNUC__
#include <codecvt>
#endif

namespace NSG
{
    static std::map<::Window, LinuxWindow*> windowHwndMap;
    Display* LinuxWindow::display_ = nullptr;
    XIM LinuxWindow::im_ = nullptr;
    XIC LinuxWindow::ic_ = nullptr;

    LinuxWindow::LinuxWindow(const std::string& name, WindowFlags flags)
        : Window(name),
          flags_(0),
          context_(0),
          visualInfo_(nullptr),
          hwnd_(0),
          keyModifier_(0)
    {
        const AppConfiguration& conf = Engine::GetPtr()->GetAppConfiguration();
        Initialize(conf.x_, conf.y_, conf.width_, conf.height_, flags);
        LOGI("Window %s created.", name_.c_str());
    }

    LinuxWindow::LinuxWindow(const std::string& name, int x, int y, int width, int height, WindowFlags flags)
        : Window(name),
          flags_(0),
          context_(0),
          visualInfo_(nullptr),
          hwnd_(0),
          keyModifier_(0)
    {
        Initialize(x, y, width, height, flags);
        LOGI("Window %s created.", name_.c_str());
    }

    LinuxWindow::~LinuxWindow()
    {
        Close();
    }

    void LinuxWindow::Initialize(int x, int y, int width, int height, WindowFlags flags)
    {
        if (!LinuxWindow::display_)
            LinuxWindow::display_ = XOpenDisplay(nullptr);

        XLockDisplay(LinuxWindow::display_);
        CHECK_CONDITION(LinuxWindow::display_ && "Cannot connect to X server");

        int major, minor;
        bool version = glXQueryVersion(LinuxWindow::display_, &major, &minor);
        CHECK_CONDITION(version && "Failed to query GLX version");
        CHECK_CONDITION((major == 1 && minor >= 2) || major > 1 && "GLX version is not >=1.2");
        int32_t screen = DefaultScreen(LinuxWindow::display_);

        const int attrsGlx[] =
        {
            GLX_RENDER_TYPE, GLX_RGBA_BIT,
            GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
            GLX_DOUBLEBUFFER, true,
            GLX_RED_SIZE, 8,
            GLX_BLUE_SIZE, 8,
            GLX_GREEN_SIZE, 8,
            //GLX_ALPHA_SIZE, 8,
            GLX_DEPTH_SIZE, 24,
            GLX_STENCIL_SIZE, 8,
            0,
        };

        GLXFBConfig bestConfig = nullptr;
        int numConfigs = 0;
        auto configs = glXChooseFBConfig(LinuxWindow::display_, screen, attrsGlx, &numConfigs);
        for (int i = 0; i < numConfigs; ++i)
        {
            visualInfo_ = glXGetVisualFromFBConfig(LinuxWindow::display_, configs[i]);
            if (visualInfo_)
            {
                bool valid = true;
                for (uint32_t attr = 6; attr < sizeof(attrsGlx) - 1 && attrsGlx[attr] != None; attr += 2)
                {
                    int value = 0;
                    glXGetFBConfigAttrib(LinuxWindow::display_, configs[i], attrsGlx[attr], &value);
                    if (value < attrsGlx[attr + 1])
                    {
                        valid = false;
                        break;
                    }
                }
                if (valid)
                {
                    bestConfig = configs[i];
                    break;
                }
            }
            XFree(visualInfo_);
            visualInfo_ = nullptr;
        }
        XFree(configs);

        CHECK_CONDITION(visualInfo_ && "Failed to find a suitable X11 display configuration.");

        auto root = DefaultRootWindow(LinuxWindow::display_);
        auto cmap = XCreateColormap(LinuxWindow::display_, root, visualInfo_->visual, AllocNone);
        XSetWindowAttributes swa;
        swa.colormap = cmap;
        swa.event_mask = 0
                         | ButtonPressMask
                         | ButtonReleaseMask
                         | ExposureMask
                         | KeyPressMask
                         | KeyReleaseMask
                         | PointerMotionMask
                         | StructureNotifyMask
                         ;
        hwnd_ = XCreateWindow(LinuxWindow::display_, root, x, y, width, height, 0,
                              visualInfo_->depth, InputOutput, visualInfo_->visual,
                              CWColormap | CWEventMask, &swa);

        windowHwndMap[hwnd_] = this;
        if (flags & (int)WindowFlag::SHOWN)
            Show();
        else if (flags & (int)WindowFlag::HIDDEN)
            Hide();

        XStoreName(LinuxWindow::display_, hwnd_, name_.c_str());

        if (Window::mainWindow_)
        {
            isMainWindow_ = false;
            // Do not create a new context. Instead, share the main window's context.
            SetContext();
        }
        else
        {
            LOGI("Create GL 2.1 context.");
            context_ = glXCreateContext(LinuxWindow::display_, visualInfo_, 0, GL_TRUE);
            CHECK_CONDITION(context_ && "Failed to create GL 2.1 context.");
            glXMakeCurrent(LinuxWindow::display_, hwnd_, context_);
            Window::SetMainWindow(this);
            LinuxWindow::im_ = XOpenIM(LinuxWindow::display_, nullptr, nullptr, nullptr);
            CHECK_ASSERT(im_);
            LinuxWindow::ic_ = XCreateIC(im_
                                         , XNInputStyle
                                         , 0
                                         | XIMPreeditNothing
                                         | XIMStatusNothing
                                         , XNClientWindow
                                         , hwnd_
                                         , nullptr
                                        );
            CHECK_ASSERT(ic_);
        }
        XUnlockDisplay(LinuxWindow::display_);

        auto glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress( (const GLubyte*)"glXSwapIntervalEXT");
        if (glXSwapIntervalEXT)
        {
            LOGI("Using glXSwapIntervalEXT.");
            glXSwapIntervalEXT(LinuxWindow::display_, hwnd_, 0);
        }
        else
        {
            auto glXSwapIntervalMESA = (PFNGLXSWAPINTERVALMESAPROC)glXGetProcAddress( (const GLubyte*)"glXSwapIntervalMESA");
            if (glXSwapIntervalMESA)
            {
                LOGI("Using glXSwapIntervalMESA.");
                glXSwapIntervalMESA(0);
            }
            else
            {
                auto glXSwapIntervalSGI = (PFNGLXSWAPINTERVALSGIPROC)glXGetProcAddress( (const GLubyte*)"glXSwapIntervalSGI");
                if (glXSwapIntervalSGI)
                {
                    LOGI("Using glXSwapIntervalSGI.");
                    glXSwapIntervalSGI(0);
                }
            }
        }
        SetSize(width, height);
    }

    void LinuxWindow::Close()
    {
        Window::Close();
    }

    void LinuxWindow::SetContext()
    {
        context_ = glXGetCurrentContext();
        glXMakeCurrent(LinuxWindow::display_, hwnd_, context_);
    }

    void LinuxWindow::Destroy()
    {
        if (!isClosed_)
        {
            isClosed_ = true;
            Window::NotifyOneWindow2Remove();
            if (isMainWindow_)
            {
                XDestroyIC(LinuxWindow::ic_);
                XCloseIM(LinuxWindow::im_);
                glXDestroyContext(LinuxWindow::display_, context_);
                Window::SetMainWindow(nullptr);
            }
            XUnmapWindow(LinuxWindow::display_, hwnd_);
            XDestroyWindow(LinuxWindow::display_, hwnd_);
        }
    }

    void LinuxWindow::SwapWindowBuffers()
    {
        glXSwapBuffers(LinuxWindow::display_, hwnd_);
    }

    void LinuxWindow::HandleEvents()
    {
        if (!XPending(LinuxWindow::display_))
            return;

        XEvent event;
        XNextEvent(LinuxWindow::display_, &event);

        switch (event.type)
        {
            case Expose:
                break;

            case ClientMessage:
                break;

            case ButtonPress:
            case ButtonRelease:
                {
                    const XButtonEvent& xbutton = event.xbutton;
                    Window* window = windowHwndMap.find(xbutton.window)->second;

                    auto button = xbutton.button == Button1 ? NSG_BUTTON_LEFT : (xbutton.button == Button2 ? NSG_BUTTON_MIDDLE : NSG_BUTTON_RIGHT);
                    if (event.type == ButtonPress)
                        window->OnMouseDown(button, xbutton.x, xbutton.y);
                    else
                        window->OnMouseUp(button, xbutton.x, xbutton.y);
                }
                break;

            case MotionNotify:
                {
                    const XMotionEvent& xmotion = event.xmotion;
                    Window* window = windowHwndMap.find(xmotion.window)->second;
                    window->OnMouseMove(xmotion.x, xmotion.y);
                }
                break;

            case KeyPress:
                {
                    XKeyEvent& xkey = event.xkey;
                    LinuxWindow* window = windowHwndMap.find(xkey.window)->second;
                    KeySym keysym = XLookupKeysym(&xkey, 0);
                    int action = NSG_KEY_PRESS;
                    window->keyModifier_ |= keysym == XK_Shift_L ? NSG_KEY_MOD_SHIFT : 0;
                    window->keyModifier_ |= keysym == XK_Control_L ? NSG_KEY_MOD_CONTROL : 0;
                    window->keyModifier_ |= keysym == XK_Alt_L ? NSG_KEY_MOD_ALT : 0;

                    Status status = 0;
                    uint8_t utf8[4];
                    int len = Xutf8LookupString(LinuxWindow::ic_, &xkey, (char*)utf8, sizeof(utf8), &keysym, &status);
                    switch (status)
                    {
                        case XLookupChars:
                        case XLookupBoth:
                            {
                                std::string text;
                                text.resize(4);
                                text[0] = utf8[0];
                                text[1] = utf8[1];
                                text[2] = utf8[2];
                                text[3] = utf8[3];
                                //window->OnText(text);
                                UTF8String utf8(text.c_str());
                                unsigned unicode = utf8.AtUTF8(0);
                                if (unicode)
                                    window->OnChar(unicode);
                                break;
                            }

                        default:
                            break;
                    }
                    window->OnKey(keysym, action, window->keyModifier_);
                }
                break;


            case KeyRelease:
                {
                    XKeyEvent& xkey = event.xkey;
                    LinuxWindow* window = windowHwndMap.find(xkey.window)->second;
                    KeySym keysym = XLookupKeysym(&xkey, 0);
                    int action = NSG_KEY_RELEASE;
                    window->keyModifier_ &= keysym == XK_Shift_L ? ~NSG_KEY_MOD_SHIFT : 0;
                    window->keyModifier_ &= keysym == XK_Control_L ? ~NSG_KEY_MOD_CONTROL : 0;
                    window->keyModifier_ &= keysym == XK_Alt_L ? ~NSG_KEY_MOD_ALT : 0;
                    window->OnKey(keysym, action, window->keyModifier_);
                }
                break;

            case ConfigureNotify:
                {
                    const XConfigureEvent& xev = event.xconfigure;
                    if (xev.width > 0 && xev.height > 0)
                    {
                        Window* window = windowHwndMap.find(xev.window)->second;
                        window->ViewChanged(xev.width, xev.height);
                    }
                }
                break;
        }

    }

    void LinuxWindow::Show()
    {
        XMapWindow(LinuxWindow::display_, hwnd_);
    }

    void LinuxWindow::Hide()
    {
        XUnmapWindow(LinuxWindow::display_, hwnd_);
    }

    void LinuxWindow::Raise()
    {
        XMapRaised(LinuxWindow::display_, hwnd_);
    }

    static const char* ImGuiGetClipboardText()
    {
        return "";
    }

    static void ImGuiSetClipboardText(const char* text)
    {
    }

    void LinuxWindow::SetupImgui()
    {
        Window::SetupImgui();
        ImGuiIO& io = ImGui::GetIO();
        io.SetClipboardTextFn = ImGuiSetClipboardText;
        io.GetClipboardTextFn = ImGuiGetClipboardText;
    }

    void LinuxWindow::BeginImguiRender()
    {
        ImGuiIO& io = ImGui::GetIO();
        io.ImeWindowHandle = (void*)hwnd_;
        // Hide OS mouse cursor if ImGui is drawing it
        //ShowCursor(io.MouseDrawCursor ? FALSE : TRUE);
    }
}
#endif
