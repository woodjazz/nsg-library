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
#if defined(IS_TARGET_WINDOWS) && !defined(SDL)
#include "WinWindow.h"
#include "Engine.h"
#include "RenderingContext.h"
#include "Tick.h"
#include "Keys.h"
#include "Log.h"
#include "Check.h"
#include "UTF8String.h"
#include "AppConfiguration.h"
#include "Object.h"
#include "RenderingContext.h"
#include "Scene.h"
#include "imgui.h"
#include <memory>
#include <string>
#include <locale>
#include <thread>
#include <mutex>
#include <strsafe.h>
#ifndef __GNUC__
#include <codecvt>
#endif

namespace NSG
{
    WinWindow::WinWindow(const std::string& name, WindowFlags flags)
        : Window(name),
          flags_(0),
          hwnd_(0)
    {
        const AppConfiguration& conf = Engine::GetPtr()->GetAppConfiguration();
        Initialize(conf.x_, conf.y_, conf.width_, conf.height_, flags);
        LOGI("Window %s created.", name_.c_str());
    }

    WinWindow::WinWindow(const std::string& name, int x, int y, int width, int height, WindowFlags flags)
        : Window(name),
          flags_(0),
          hwnd_(0)
    {
        Initialize(x, y, width, height, flags);
        LOGI("Window %s created.", name_.c_str());
    }

    WinWindow::~WinWindow()
    {
        Close();
    }

	static HGLRC CreateContext(HDC hdc)
    {
        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(pfd));
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cAlphaBits = 8;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;

        int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        CHECK_ASSERT(0 != pixelFormat && "ChoosePixelFormat failed!");

        DescribePixelFormat(hdc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

        LOGI("Pixel format:\n"
             "\tiPixelType %d\n"
             "\tcColorBits %d\n"
             "\tcAlphaBits %d\n"
             "\tcDepthBits %d\n"
             "\tcStencilBits %d\n"
             , pfd.iPixelType
             , pfd.cColorBits
             , pfd.cAlphaBits
             , pfd.cDepthBits
             , pfd.cStencilBits
            );

        int result = SetPixelFormat(hdc, pixelFormat, &pfd);
        CHECK_ASSERT(0 != result && "SetPixelFormat failed");

        HGLRC context = wglCreateContext(hdc);
        CHECK_ASSERT(nullptr != context && "wglCreateContext failed!");

        result = wglMakeCurrent(hdc, context);
        CHECK_ASSERT(0 != result && "wglMakeCurrent failed!");

        return context;
    }

    LRESULT CALLBACK WndProc(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam)
    {
        auto window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (window)
        {
            switch (id)
            {
                case WM_CLOSE:
                    {
                        window->Close();
                    }
                    break;
                case WM_SIZE:
                    {
                        auto width = LOWORD(lparam);
                        auto height = HIWORD(lparam);
                        if (width > 0 && height > 0)
                            window->ViewChanged(width, height);
                    }
                    break;
                case WM_MOUSEMOVE:
                    {
                        window->OnMouseMove(LOWORD(lparam), HIWORD(lparam));
                    }
                    break;

                case WM_MOUSEWHEEL:
                    {
                        POINT pt = { LOWORD(lparam), HIWORD(lparam) };
                        ScreenToClient(hwnd, &pt);
                        auto mx = pt.x;
                        auto my = pt.y;
                        window->OnMouseWheel((float)mx, (float)my);
                    }
                    break;

                case WM_LBUTTONDOWN:
                case WM_MBUTTONDOWN:
                case WM_RBUTTONDOWN:
                    {
                        auto mx = LOWORD(lparam);
                        auto my = HIWORD(lparam);
                        auto button = id == WM_LBUTTONDOWN ? NSG_BUTTON_LEFT : (id == WM_MBUTTONDOWN ? NSG_BUTTON_MIDDLE : NSG_BUTTON_RIGHT);
                        window->OnMouseDown(button, mx, my);
                    }
                    break;

                case WM_LBUTTONUP:
                case WM_MBUTTONUP:
                case WM_RBUTTONUP:
                    {
                        auto mx = LOWORD(lparam);
                        auto my = HIWORD(lparam);
                        auto button = id == WM_LBUTTONUP ? NSG_BUTTON_LEFT : (id == WM_MBUTTONUP ? NSG_BUTTON_MIDDLE : NSG_BUTTON_RIGHT);
                        window->OnMouseUp(button, mx, my);
                    }
                    break;
                case WM_KEYDOWN:
                case WM_SYSKEYDOWN:
                    {
                        int action = NSG_KEY_PRESS;
                        int modifier = GetKeyState(VK_LSHIFT) ? NSG_KEY_MOD_SHIFT : 0;
                        modifier |= GetKeyState(VK_LCONTROL) ? NSG_KEY_MOD_CONTROL : 0;
                        modifier |= GetKeyState(VK_LMENU) ? NSG_KEY_MOD_ALT : 0;
                        window->OnKey((int)wparam, action, modifier);
                    }
                    break;
                case WM_KEYUP:
                case WM_SYSKEYUP:
                    {
                        int action = NSG_KEY_RELEASE;
                        int modifier = GetKeyState(VK_LSHIFT) ? NSG_KEY_MOD_SHIFT : 0;
                        modifier |= GetKeyState(VK_LCONTROL) ? NSG_KEY_MOD_CONTROL : 0;
                        modifier |= GetKeyState(VK_LMENU) ? NSG_KEY_MOD_ALT : 0;
                        window->OnKey((int)wparam, action, modifier);
                    }
                    break;
                case WM_CHAR:
                    {
                        std::string text(1, (char)wparam);
                        window->OnText(text);
                        UTF8String utf8(text.c_str());
                        unsigned unicode = utf8.AtUTF8(0);
                        if (unicode)
                            window->OnChar(unicode);
                    }
                    break;
                default:
                    break;
            }
        }
        return DefWindowProc(hwnd, id, wparam, lparam);
    }

    void WinWindow::Initialize(int x, int y, int width, int height, WindowFlags flags)
    {
        HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(nullptr);

        WNDCLASSEX wcx;
        wcx.cbSize = sizeof(wcx);
        wcx.style = CS_HREDRAW | CS_VREDRAW;
        wcx.lpfnWndProc = WndProc;
        wcx.cbClsExtra = 0;
        wcx.cbWndExtra = 0;
        wcx.hInstance = hInstance;
        wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wcx.lpszMenuName = "MainMenu";
        wcx.lpszClassName = name_.c_str();
        wcx.hIconSm = (HICON)LoadImage(hInstance,
                                       MAKEINTRESOURCE(5),
                                       IMAGE_ICON,
                                       GetSystemMetrics(SM_CXSMICON),
                                       GetSystemMetrics(SM_CYSMICON),
                                       LR_DEFAULTCOLOR);

        auto result = RegisterClassExA(&wcx);

        CHECK_ASSERT(result && "RegisterClassExA failed!");

        hwnd_ = CreateWindowA(wcx.lpszClassName
                              , name_.c_str()
                              , WS_OVERLAPPEDWINDOW |
                              ((int)WindowFlag::SHOWN & flags ? WS_VISIBLE : 0)
                              , 0
                              , 0
                              , width
                              , height
                              , nullptr
                              , nullptr
                              , hInstance
                              , this
                             );

        SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR)this);

        if (Window::mainWindow_)
        {
            isMainWindow_ = false;
            // Do not create a new context. Instead, share the main window's context.
            SetContext();
        }
        else
        {
            HDC hdc = GetDC(hwnd_);
            CreateContext(hdc);
            Window::SetMainWindow(this);
        }

        SetSize(width, height);
    }

    void WinWindow::Close()
    {
        Window::Close();
        UnregisterClass(name_.c_str(), GetModuleHandle(nullptr));
    }

    void WinWindow::SetContext()
    {
        auto context = wglGetCurrentContext();
        int result = wglMakeCurrent(wglGetCurrentDC(), context);
        CHECK_CONDITION(0 != result && "wglMakeCurrent failed!");
    }

    void WinWindow::Destroy()
    {
        if (!isClosed_)
        {
            isClosed_ = true;
            Window::NotifyOneWindow2Remove();
            if (isMainWindow_)
            {
                wglDeleteContext(wglGetCurrentContext());
                Window::SetMainWindow(nullptr);
            }
            DestroyWindow(hwnd_);
        }
    }

    void WinWindow::ViewChanged(int width, int height)
    {
        if (width_ != width || height_ != height)
        {
            Window::ViewChanged(width, height);
        }
    }

    void WinWindow::SwapWindowBuffers()
    {
        SwapBuffers(wglGetCurrentDC());
    }

    void WinWindow::HandleEvents()
    {
        MSG msg;
        if (PeekMessage(&msg, (HWND)nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void WinWindow::Show()
    {
        ShowWindow(hwnd_, SW_SHOW);
    }

    void WinWindow::Hide()
    {
        ShowWindow(hwnd_, SW_HIDE);
    }

    void WinWindow::Raise()
    {
        SwitchToThisWindow(hwnd_, FALSE);
    }

    static const char* ImGuiGetClipboardText()
    {
        if (!IsClipboardFormatAvailable(CF_TEXT))
            return "";
        auto hwnd = static_cast<WinWindow*>(Window::GetMainWindow())->GetHWND();
        if (!OpenClipboard(hwnd))
            return "";
        auto hglb = GetClipboardData(CF_TEXT);
        if (hglb)
        {
            auto lptstr = GlobalLock(hglb);
            static std::string data;
            data = (const char*)lptstr;
            GlobalUnlock(hglb);
            return data.c_str();
        }
        return "";
    }

    static void ImGuiSetClipboardText(const char* text)
    {
        auto hwnd = static_cast<WinWindow*>(Window::GetMainWindow())->GetHWND();
        if (OpenClipboard(hwnd))
        {
            auto n = strlen(text);
            EmptyClipboard();
            auto hglbCopy = GlobalAlloc(GMEM_MOVEABLE, n + 1 * sizeof(char));
            if (hglbCopy)
            {
                LPTSTR lptstrCopy = (LPTSTR)GlobalLock(hglbCopy);
                memcpy(lptstrCopy, text, n * sizeof(char));
                lptstrCopy[n] = '\0';
                GlobalUnlock(hglbCopy);
                SetClipboardData(CF_TEXT, hglbCopy);
            }
            CloseClipboard();
        }
    }

    void WinWindow::SetupImgui()
    {
        Window::SetupImgui();
        ImGuiIO& io = ImGui::GetIO();
        io.SetClipboardTextFn = ImGuiSetClipboardText;
        io.GetClipboardTextFn = ImGuiGetClipboardText;
    }

    void WinWindow::BeginImguiRender()
    {
        ImGuiIO& io = ImGui::GetIO();
        io.ImeWindowHandle = hwnd_;
        // Hide OS mouse cursor if ImGui is drawing it
        ShowCursor(io.MouseDrawCursor ? FALSE : TRUE);
    }
}
#endif