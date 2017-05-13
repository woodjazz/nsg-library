/*
   -------------------------------------------------------------------------------
   This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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
#include "LinuxX11Window.h"
#include "AppConfiguration.h"
#include "Check.h"
#include "Engine.h"
#include "Keys.h"
#include "Log.h"
#include "Tick.h"
#include "UTF8String.h"
#include "imgui.h"
#include <X11/Xutil.h>
#include <locale>
#include <map>
#include <memory>
#include <string>
#ifndef __GNUC__
#include <codecvt>
#endif

namespace NSG {
static std::map<::Window, LinuxX11Window*> windowHwndMap;
Display* LinuxX11Window::display_ = nullptr;
XIM LinuxX11Window::im_ = nullptr;
XIC LinuxX11Window::ic_ = nullptr;

LinuxX11Window::LinuxX11Window(const std::string& name, WindowFlags flags)
    : Window(name), flags_(0), hwnd_(0), keyModifier_(0) {}

LinuxX11Window::LinuxX11Window(const std::string& name, int x, int y, int width,
                               int height, WindowFlags flags)
    : Window(name), flags_(0), hwnd_(0), keyModifier_(0) {}

LinuxX11Window::~LinuxX11Window() {}

void LinuxX11Window::Initialize(int x, int y, int width, int height,
                                WindowFlags flags) {
    if (!LinuxX11Window::display_)
        LinuxX11Window::display_ = XOpenDisplay(nullptr);

    XLockDisplay(LinuxX11Window::display_);
    CHECK_CONDITION(LinuxX11Window::display_ && "Cannot connect to X server");
    auto root = DefaultRootWindow(LinuxX11Window::display_);
    // auto cmap = XCreateColormap(LinuxX11Window::display_, root,
    // visualInfo->visual, AllocNone);
    XSetWindowAttributes swa;
    // swa.colormap = cmap;
    swa.event_mask = 0 | ButtonPressMask | ButtonReleaseMask | ExposureMask |
                     KeyPressMask | KeyReleaseMask | PointerMotionMask |
                     StructureNotifyMask;
    hwnd_ = XCreateWindow(LinuxX11Window::display_, root, x, y, width, height,
                          0, CopyFromParent, InputOutput, CopyFromParent,
                          CWEventMask, &swa);

    windowHwndMap[hwnd_] = this;
    if (flags & (int)WindowFlag::SHOWN)
        Show();
    else if (flags & (int)WindowFlag::HIDDEN)
        Hide();

    XStoreName(LinuxX11Window::display_, hwnd_, name_.c_str());
    LOGI("XSupportsLocale = %d", XSupportsLocale());
    LOGI("XSetLocaleModifiers = %s", XSetLocaleModifiers(""));
    LinuxX11Window::im_ =
        XOpenIM(LinuxX11Window::display_, nullptr, nullptr, nullptr);
    if (!LinuxX11Window::im_) {
        XSetLocaleModifiers("@im=none");
        LinuxX11Window::im_ =
            XOpenIM(LinuxX11Window::display_, nullptr, nullptr, nullptr);
    }
    CHECK_ASSERT(im_);
    LinuxX11Window::ic_ =
        XCreateIC(im_, XNInputStyle, 0 | XIMPreeditNothing | XIMStatusNothing,
                  XNClientWindow, hwnd_, nullptr);
    CHECK_ASSERT(ic_);
    CreateContext();
    XUnlockDisplay(LinuxX11Window::display_);
    SetSize(width, height);
}

void LinuxX11Window::Close() { Window::Close(); }

void LinuxX11Window::Destroy() {
    if (!isClosed_) {
        isClosed_ = true;
        Window::NotifyOneWindow2Remove();
        if (isMainWindow_) {
            XDestroyIC(LinuxX11Window::ic_);
            XCloseIM(LinuxX11Window::im_);
            DestroyContext();
            Window::SetMainWindow(nullptr);
        }
        XUnmapWindow(LinuxX11Window::display_, hwnd_);
        XDestroyWindow(LinuxX11Window::display_, hwnd_);
    }
}

static int MapKeyCode(int keyCode) {
    switch (keyCode) {
    case XK_Alt_L:
        keyCode = NSG_KEY_LALT;
        break;
    case XK_Shift_L:
        keyCode = NSG_KEY_LSHIFT;
        break;
    case XK_Escape:
        keyCode = NSG_KEY_ESC;
        break;
    case XK_space:
        keyCode = NSG_KEY_SPACE;
        break;
    case XK_Tab:
        keyCode = NSG_KEY_TAB;
        break;
    case XK_BackSpace:
        keyCode = NSG_KEY_BACKSPACE;
        break;
    case XK_Return:
        keyCode = NSG_KEY_ENTER;
        break;
    case XK_Delete:
        keyCode = NSG_KEY_DELETE;
        break;
    case XK_Right:
        keyCode = NSG_KEY_RIGHT;
        break;
    case XK_Left:
        keyCode = NSG_KEY_LEFT;
        break;
    case XK_Up:
        keyCode = NSG_KEY_UP;
        break;
    case XK_Down:
        keyCode = NSG_KEY_DOWN;
        break;
    case XK_Home:
        keyCode = NSG_KEY_HOME;
        break;
    case XK_KP_End:
        keyCode = NSG_KEY_END;
        break;
    case XK_Page_Up:
        keyCode = NSG_KEY_PAGEUP;
        break;
    case XK_Page_Down:
        keyCode = NSG_KEY_PAGEDOWN;
        break;
    }
    return keyCode;
}

void LinuxX11Window::HandleEvents() {
    if (!XPending(LinuxX11Window::display_))
        return;

    XEvent event;
    XNextEvent(LinuxX11Window::display_, &event);

    switch (event.type) {
    case Expose:
        break;

    case ClientMessage:
        break;

    case ButtonPress:
    case ButtonRelease: {
        const XButtonEvent& xbutton = event.xbutton;
        Window* window = windowHwndMap.find(xbutton.window)->second;

        if (xbutton.button == Button4)
            window->OnMouseWheel(0, 1);
        else if (xbutton.button == Button5)
            window->OnMouseWheel(0, -1);
        else {
            auto button = xbutton.button == Button1
                              ? NSG_BUTTON_LEFT
                              : (xbutton.button == Button2 ? NSG_BUTTON_MIDDLE
                                                           : NSG_BUTTON_RIGHT);
            if (event.type == ButtonPress)
                window->OnMouseDown(button, xbutton.x, xbutton.y);
            else
                window->OnMouseUp(button, xbutton.x, xbutton.y);
        }
    } break;

    case MotionNotify: {
        const XMotionEvent& xmotion = event.xmotion;
        Window* window = windowHwndMap.find(xmotion.window)->second;
        window->OnMouseMove(xmotion.x, xmotion.y);
    } break;

    case KeyPress: {
        XKeyEvent& xkey = event.xkey;
        LinuxX11Window* window = windowHwndMap.find(xkey.window)->second;
        KeySym keysym = XLookupKeysym(&xkey, 0);
        int action = NSG_KEY_PRESS;
        window->keyModifier_ |= keysym == XK_Shift_L ? NSG_KEY_MOD_SHIFT : 0;
        window->keyModifier_ |=
            keysym == XK_Control_L ? NSG_KEY_MOD_CONTROL : 0;
        window->keyModifier_ |= keysym == XK_Alt_L ? NSG_KEY_MOD_ALT : 0;

        Status status = 0;
        uint8_t utf8[4];
        Xutf8LookupString(LinuxX11Window::ic_, &xkey, (char*)utf8, sizeof(utf8),
                          &keysym, &status);
        switch (status) {
        case XLookupChars:
        case XLookupBoth: {
            std::string text;
            text.resize(4);
            text[0] = utf8[0];
            text[1] = utf8[1];
            text[2] = utf8[2];
            text[3] = utf8[3];
            // window->OnText(text);
            UTF8String utf8(text.c_str());
            unsigned unicode = utf8.AtUTF8(0);
            if (unicode)
                window->OnChar(unicode);
            break;
        }

        default:
            break;
        }
        window->OnKey(MapKeyCode(keysym), action, window->keyModifier_);
    } break;

    case KeyRelease: {
        XKeyEvent& xkey = event.xkey;
        LinuxX11Window* window = windowHwndMap.find(xkey.window)->second;
        KeySym keysym = XLookupKeysym(&xkey, 0);
        int action = NSG_KEY_RELEASE;
        window->keyModifier_ &= keysym == XK_Shift_L ? ~NSG_KEY_MOD_SHIFT : 0;
        window->keyModifier_ &=
            keysym == XK_Control_L ? ~NSG_KEY_MOD_CONTROL : 0;
        window->keyModifier_ &= keysym == XK_Alt_L ? ~NSG_KEY_MOD_ALT : 0;
        window->OnKey(MapKeyCode(keysym), action, window->keyModifier_);
    } break;

    case ConfigureNotify: {
        const XConfigureEvent& xev = event.xconfigure;
        if (xev.width > 0 && xev.height > 0) {
            Window* window = windowHwndMap.find(xev.window)->second;
            window->ViewChanged(xev.width, xev.height);
        }
    } break;
    }
}

void LinuxX11Window::Show() { XMapWindow(LinuxX11Window::display_, hwnd_); }

void LinuxX11Window::Hide() { XUnmapWindow(LinuxX11Window::display_, hwnd_); }

void LinuxX11Window::Raise() { XMapRaised(LinuxX11Window::display_, hwnd_); }

void LinuxX11Window::SetupImgui() { Window::SetupImgui(); }

void LinuxX11Window::BeginImguiRender() {
    ImGuiIO& io = ImGui::GetIO();
    io.ImeWindowHandle = (void*)hwnd_;
    // Hide OS mouse cursor if ImGui is drawing it
    // ShowCursor(io.MouseDrawCursor ? FALSE : TRUE);
}
}
#endif
