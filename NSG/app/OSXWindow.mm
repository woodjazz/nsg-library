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
#if defined(IS_TARGET_OSX) && !defined(SDL)
#include "OSXWindow.h"
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
    OSXWindow::OSXWindow(const std::string& name, WindowFlags flags)
        : Window(name),
          flags_(0)
    {
        const AppConfiguration& conf = Engine::GetPtr()->GetAppConfiguration();
        Initialize(conf.x_, conf.y_, conf.width_, conf.height_, flags);
        LOGI("Window %s created.", name_.c_str());
    }

    OSXWindow::OSXWindow(const std::string& name, int x, int y, int width, int height, WindowFlags flags)
        : Window(name),
          flags_(0)
    {
        Initialize(x, y, width, height, flags);
        LOGI("Window %s created.", name_.c_str());
    }

    OSXWindow::~OSXWindow()
    {
        Close();
    }

    void OSXWindow::Initialize(int x, int y, int width, int height, WindowFlags flags)
    {

    }

    void OSXWindow::Close()
    {
        Window::Close();
    }

    void OSXWindow::SetContext()
    {
    }

    void OSXWindow::Destroy()
    {
        if (!isClosed_)
        {
            isClosed_ = true;
            Window::NotifyOneWindow2Remove();
            if (isMainWindow_)
            {
                Window::SetMainWindow(nullptr);
            }
        }
    }

    void OSXWindow::ViewChanged(int width, int height)
    {
        if (width_ != width || height_ != height)
        {
            Window::ViewChanged(width, height);
        }
    }

    void OSXWindow::SwapWindowBuffers()
    {
    }

    void OSXWindow::HandleEvents()
    {
    }

    void OSXWindow::Show()
    {
    }

    void OSXWindow::Hide()
    {
    }

    void OSXWindow::Raise()
    {
    }

    static const char* ImGuiGetClipboardText()
    {
        return "";
    }

    static void ImGuiSetClipboardText(const char* text)
    {
    }

    void OSXWindow::SetupImgui()
    {
        Window::SetupImgui();
        ImGuiIO& io = ImGui::GetIO();
        io.SetClipboardTextFn = ImGuiSetClipboardText;
        io.GetClipboardTextFn = ImGuiGetClipboardText;
    }

    void OSXWindow::BeginImguiRender()
    {
        ImGuiIO& io = ImGui::GetIO();
        //io.ImeWindowHandle = (void*)hwnd_;
        // Hide OS mouse cursor if ImGui is drawing it
        //ShowCursor(io.MouseDrawCursor ? FALSE : TRUE);
    }
}
#endif
