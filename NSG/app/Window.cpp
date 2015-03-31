/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 Néstor Silveira Gorski

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
#include "Window.h"
#include "AppConfiguration.h"
#include "SignalSlots.h"
#include "TextMesh.h"
#include "Scene.h"
#include "Graphics.h"
#include "Music.h"
#include "Filter.h"
#include "FrameBuffer.h"
#include "Program.h"
#include "Material.h"
#include "ShowTexture.h"
#include "SDLWindow.h"
#include "Keys.h"

#include "UTF8String.h"
#include <algorithm>
#include <thread>

#ifndef __GNUC__
#include <codecvt>
#endif

namespace NSG
{
    SignalWindow::PSignal Window::signalWindowCreated_(new SignalWindow());
    std::vector<PWeakWindow> Window::windows_;
    Window* Window::mainWindow_ = nullptr;
    int Window::nWindows2Remove_ = 0;
    PGraphics Window::graphics_;
    std::once_flag Window::onceFlag_;
    AppConfiguration Window::conf_;

    Window::Window(const std::string& name)
        : Tick(Window::GetAppConfiguration().fps_),
          signalViewChanged_(new Signal<int, int>()),
          signalMouseMoved_(new Signal<float, float>()),
          signalMouseDown_(new Signal<int, float, float>()),
          signalMouseUp_(new Signal<int, float, float>()),
          signalMouseWheel_(new Signal<float, float>()),
          signalKey_(new Signal<int, int, int>()),
          signalChar_(new Signal<unsigned int>()),
          signalMultiGesture_(new Signal<int, float, float, float, float, int>()),
          signalUpdate_(new Signal<float>()),
          signalRender_(new Signal<>()),
          signalDropFile_(new Signal<const std::string & >()),
          name_(name),
          isClosed_(false),
          minimized_(false),
          isMainWindow_(true),
          width_(0),
          height_(0),
          filtersEnabled_(true)

    {
        CHECK_CONDITION(Window::AllowWindowCreation(), __FILE__, __LINE__);
        TRACE_PRINTF("Window %s created\n", name_.c_str());
    }

    Window::~Window()
    {
        TRACE_PRINTF("Window %s terminated\n", name_.c_str());
    }

    PWindow Window::Create(const std::string& name)
    {
        if (Window::AllowWindowCreation())
        {
            auto window = std::make_shared<SDLWindow>(name);
            Window::AddWindow(window);
            return window;
        }
        return nullptr;
    }

    PWindow Window::Create(const std::string& name, int x, int y, int width, int height)
    {
        if (Window::AllowWindowCreation())
        {
			auto window = std::make_shared<SDLWindow>(name, x, y, width, height);
            Window::AddWindow(window);
            return window;
        }
        return nullptr;
    }

    void Window::CreateFrameBuffer()
    {
        CHECK_ASSERT(!frameBuffer_, __FILE__, __LINE__);
        FrameBuffer::Flags frameBufferFlags((unsigned int)(FrameBuffer::COLOR | FrameBuffer::COLOR_USE_TEXTURE | FrameBuffer::DEPTH));
        //frameBufferFlags |= FrameBuffer::STENCIL;
        frameBuffer_ = std::make_shared<FrameBuffer>("GraphicsFrameBuffer", frameBufferFlags);
        frameBuffer_->SetWindow(this);
        CHECK_ASSERT(!showFrameBuffer_, __FILE__, __LINE__);
        showFrameBuffer_ = std::make_shared<ShowTexture>();
        showFrameBuffer_->SetNormal(frameBuffer_->GetColorTexture());
    }

    bool Window::BeginFrameRender()
    {
        if (HasFilters())
        {
            if (!frameBuffer_->IsReady())
                return false;
            Graphics::this_->SetFrameBuffer(frameBuffer_.get());
        }
        else
            Graphics::this_->SetFrameBuffer(nullptr);
        return true;
    }

    void Window::EndFrameRender()
    {
        if (HasFilters())
        {
            for (auto& filter : filters_)
                filter->Draw();
            Graphics::this_->SetFrameBuffer(nullptr); //use system framebuffer to show the texture
            showFrameBuffer_->Show();
        }
    }

    void Window::Close()
    {
        TRACE_PRINTF("Closing %s window...", name_.c_str());

        if (Window::mainWindow_ == this)
        {
            // destroy other windows
            auto windows = Window::GetWindows();
            for (auto& obj : windows)
            {
                PWindow window = obj.lock();
                if (window && window.get() != this)
                    window->Destroy();
            }
        }
        Destroy();
    }

    float Window::GetDeltaTime() const
    {
        return deltaTime_;
    }

    void Window::InitializeTicks()
    {
        std::call_once(onceFlag_, [&]()
        {
            CHECK_ASSERT(!graphics_, __FILE__, __LINE__);
            graphics_ = PGraphics(new Graphics);
        });

        CreateFrameBuffer(); // used when filters are enabled
		signalWindowCreated_->Run(this);
    }

    void Window::BeginTicks()
    {
    }

    void Window::DoTick(float delta)
    {
        deltaTime_ = delta;
        signalUpdate_->Run(deltaTime_);
    }

    void Window::EndTicks()
    {
        if (Graphics::this_->BeginFrameRender())
        {
            signalRender_->Run();
            UniformsUpdate::ClearAllUpdates();
            Graphics::this_->EndFrameRender();
        }
    }

    void Window::SetSize(int width, int height)
    {
        if (width_ != width || height_ != height)
        {
            TRACE_LOG("WindowChanged: " << width << "," << height);
            width_ = width;
            height_ = height;

            if (Graphics::this_ && Graphics::this_->GetWindow() == this)
            {
                Graphics::this_->SetViewport(GetViewport(), true);
                signalViewChanged_->Run(width, height);
            }
        }
    }

    void Window::ViewChanged(int width, int height)
    {
        SetSize(width, height);
    }

    void Window::OnMouseMove(float x, float y)
    {
        signalMouseMoved_->Run(x, y);
    }

    void Window::OnMouseDown(int button, float x, float y)
    {
        signalMouseDown_->Run(button, x, y);
    }

    void Window::OnMouseUp(int button, float x, float y)
    {
        signalMouseUp_->Run(button, x, y);
    }

    void Window::OnMultiGesture(int timestamp, float x, float y, float dTheta, float dDist, int numFingers)
    {
        signalMultiGesture_->Run(timestamp, x, y, dTheta, dDist, numFingers);
    }

    void Window::OnMouseWheel(float x, float y)
    {
        signalMouseWheel_->Run(x, y);
    }

    void Window::OnKey(int key, int action, int modifier)
    {
        signalKey_->Run(key, action, modifier);
    }

    void Window::OnChar(unsigned int character)
    {
        signalChar_->Run(character);
    }

    void Window::EnterBackground()
    {
        minimized_ = true;
        if (Window::mainWindow_ == this)
        {
            if (Music::this_ && Window::GetAppConfiguration().pauseMusicOnBackground_)
                Music::this_->Pause();
        }
    }

    void Window::EnterForeground()
    {
        if (Window::mainWindow_ == this)
        {
            if (Music::this_ && Window::GetAppConfiguration().pauseMusicOnBackground_)
                Music::this_->Resume();
        }

        minimized_ = false;
    }

    void Window::DropFile(const std::string& filePath)
    {
        signalDropFile_->Run(filePath);
    }

    Recti Window::GetViewport() const
    {
        return Recti(0, 0, width_, height_);
    }

    PFilter Window::AddBlurFilter()
    {
        PFilter blur;
        ProgramFlags flags = (int)ProgramFlag::BLUR | (int)ProgramFlag::FLIP_Y;
        std::string name = GetUniqueName("FilterBlur");
        if (filters_.empty())
            blur = std::make_shared<Filter>(name, frameBuffer_->GetColorTexture(), flags);
        else
            blur = std::make_shared<Filter>(name, filters_.back()->GetTexture(), flags);
        AddFilter(blur);
        return blur;
    }

    PFilter Window::AddBlendFilter()
    {
        CHECK_ASSERT(filters_.size() > 0, __FILE__, __LINE__);
        PFilter blend;
        ProgramFlags flags = (int)ProgramFlag::BLEND | (int)ProgramFlag::FLIP_Y;
        std::string name = GetUniqueName("FilterBlend");
        size_t n = filters_.size();
        if (n > 1)
        {
            blend = std::make_shared<Filter>(name, filters_[n - 2]->GetTexture(), flags);
            blend->GetMaterial()->SetTexture(1, filters_[n - 1]->GetTexture());
        }
        else
        {
            blend = std::make_shared<Filter>(name, frameBuffer_->GetColorTexture(), flags);
            blend->GetMaterial()->SetTexture(1, filters_[0]->GetTexture());
        }
        AddFilter(blend);
        return blend;
    }

    PFilter Window::AddWaveFilter()
    {
        PFilter wave;
        ProgramFlags flags = (int)ProgramFlag::WAVE | (int)ProgramFlag::FLIP_Y;
        std::string name = GetUniqueName("FilterWave");
        if (filters_.empty())
            wave = std::make_shared<Filter>(name, frameBuffer_->GetColorTexture(),  flags);
        else
            wave = std::make_shared<Filter>(name, filters_.back()->GetTexture(),  flags);
        AddFilter(wave);
        return wave;
    }


    PFilter Window::AddUserFilter(PResource fragmentShader)
    {
        PFilter filter;

        if (filters_.empty())
            filter = std::make_shared<Filter>("UserFilter", frameBuffer_->GetColorTexture());
        else
            filter = std::make_shared<Filter>("UserFilter", filters_.back()->GetTexture());

        filter->GetProgram()->SetFragmentShader(fragmentShader);

        AddFilter(filter);

        return filter;
    }

    void Window::AddFilter(PFilter filter)
    {
        filter->SetWindow(this);
        filters_.push_back(filter);
        showFrameBuffer_->SetColortexture(filter->GetTexture());
    }

    void Window::EnableFilters(bool enable)
    {
        if (filtersEnabled_ != enable)
        {
            filtersEnabled_ = enable;
            if (!enable)
                frameBuffer_->Invalidate();
        }
    }

    bool Window::AllowWindowCreation()
    {
        #if defined(IS_TARGET_MOBILE) || defined(IS_TARGET_WEB)
        {
            if (windows_.size())
            {
                TRACE_LOG("Only one window is allowed for this platform!!!");
                return false;
            }
        }
        #endif
        return true;
    }

    void Window::SetMainWindow(Window* window)
    {
        if (mainWindow_ != window)
            mainWindow_ = window;
    }

    void Window::AddWindow(PWindow window)
    {
        CHECK_ASSERT(AllowWindowCreation(), __FILE__, __LINE__);
        windows_.push_back(window);
    }

    bool Window::RenderWindows()
    {
        mainWindow_->HandleEvents();
        for (auto& obj : windows_)
        {
            PWindow window(obj.lock());
            if (!window || window->IsClosed())
                break;
            if (!window->IsMinimized())
                window->RenderFrame();
        }

        while (nWindows2Remove_)
        {
            windows_.erase(std::remove_if(windows_.begin(), windows_.end(), [&](PWeakWindow window)
            {
                if (!window.lock() || window.lock()->IsClosed())
                {
                    --nWindows2Remove_;
                    return true;
                }
                return false;
            }), windows_.end());
        }

        if (!mainWindow_)
            return false;
        else if (mainWindow_->IsMinimized())
        {
            std::this_thread::sleep_for(Milliseconds(100));
        }
        return true;
    }

    int Window::RunApp()
    {
        if (mainWindow_)
            return mainWindow_->Run();
        return 0;
    }

}