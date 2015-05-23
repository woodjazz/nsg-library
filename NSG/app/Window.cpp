/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "Engine.h"
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
#include "Texture.h"
#include "UTF8String.h"
#include "Renderer.h"
#include <algorithm>
#include <thread>

#ifndef __GNUC__
#include <codecvt>
#endif

namespace NSG
{
    std::vector<PWeakWindow> Window::windows_;
    Window* Window::mainWindow_ = nullptr;
    int Window::nWindows2Remove_ = 0;
    PGraphics Window::graphics_;
    std::once_flag Window::onceFlag_;

    Window::Window(const std::string& name)
        : name_(name),
          isClosed_(false),
          minimized_(false),
          isMainWindow_(true),
          width_(0),
          height_(0),
          filtersEnabled_(true),
          scene_(nullptr),
          signalViewChanged_(new Signal<int, int>()),
          signalFloatFloat_(new Signal<float, float>()),
          signalMouseDown_(new Signal<int, float, float>()),
          signalMouseUp_(new Signal<int, float, float>()),
          signalMouseWheel_(new Signal<float, float>()),
          signalKey_(new Signal<int, int, int>()),
          signalUnsigned_(new Signal<unsigned int>()),
          signalMultiGesture_(new Signal<int, float, float, float, float, int>()),
          signalDropFile_(new Signal<const std::string & >()),
          signalJoystickDown_(new SignalJoystickButton),
          signalJoystickUp_(new SignalJoystickButton),
          signalJoystickAxisMotion_(new Signal<int, JoystickAxis, float>)

    {
        CHECK_CONDITION(Window::AllowWindowCreation(), __FILE__, __LINE__);
    }

    Window::~Window()
    {
        TRACE_PRINTF("Window %s terminated.\n", name_.c_str());
    }

    PWindow Window::Create(const std::string& name, WindowFlags flags)
    {
        if (Window::AllowWindowCreation())
        {
            auto window = std::make_shared<SDLWindow>(name, flags);
            Window::AddWindow(window);
            return window;
        }
        return nullptr;
    }

    PWindow Window::Create(const std::string& name, int x, int y, int width, int height, WindowFlags flags)
    {
        if (Window::AllowWindowCreation())
        {
            auto window = std::make_shared<SDLWindow>(name, x, y, width, height, flags);
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
		frameBuffer_ = std::make_shared<FrameBuffer>(GetUniqueName("WindowFrameBuffer"), frameBufferFlags);
        frameBuffer_->SetWindow(this);
        CHECK_ASSERT(!showMap_, __FILE__, __LINE__);
        showMap_ = std::make_shared<ShowTexture>();
        showMap_->SetNormal(frameBuffer_->GetColorTexture());
    }

    void Window::ShowMap(PTexture texture)
    {
        showTexture_ = texture;
        if (texture)
            showMap_->SetColortexture(texture);
        else
            showMap_->SetNormal(frameBuffer_->GetColorTexture());
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
        auto hasFilters = HasFilters();
        if (hasFilters)
            for (auto& filter : filters_)
                filter->Draw();
        if (hasFilters || showTexture_)
        {
			Graphics::this_->SetFrameBuffer(nullptr); //use system framebuffer to show the texture
            showMap_->Show();
        }
    }

    void Window::Close()
    {
        TRACE_PRINTF("Closing %s window.\n", name_.c_str());

        if (Window::mainWindow_ == this)
        {
            Graphics::this_->ResetCachedState();
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

    void Window::OnReady()
    {
        std::call_once(onceFlag_, [&]()
        {
            CHECK_ASSERT(!graphics_, __FILE__, __LINE__);
            graphics_ = PGraphics(new Graphics);
        });

        CreateFrameBuffer(); // used when filters are enabled
        Window::SigReady()->Run(this);
    }

    void Window::SetSize(int width, int height)
    {
        if (width_ != width || height_ != height)
        {
            TRACE_PRINTF("WindowChanged: %d,%d", width, height);
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
        signalFloatFloat_->Run(x, y);
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
        signalUnsigned_->Run(character);
    }

    void Window::OnJoystickDown(int joystickID, unsigned button)
    {
        SigJoystickDown()->Run(joystickID, button);
    }

    void Window::OnJoystickUp(int joystickID, unsigned button)
    {
        SigJoystickUp()->Run(joystickID, button);
    }

    void Window::OnJoystickAxisMotion(int joystickID, JoystickAxis axis, float position)
    {
        SigJoystickAxisMotion()->Run(joystickID, axis, position);
    }

    void Window::EnterBackground()
    {
        minimized_ = true;
        if (Window::mainWindow_ == this)
        {
            if (Music::this_ && Engine::GetAppConfiguration().pauseMusicOnBackground_)
                Music::this_->Pause();
        }
    }

    void Window::EnterForeground()
    {
        if (Window::mainWindow_ == this)
        {
            if (Music::this_ && Engine::GetAppConfiguration().pauseMusicOnBackground_)
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
        std::string name = GetUniqueName("FilterBlur");
        if (filters_.empty())
            blur = std::make_shared<Filter>(name, frameBuffer_->GetColorTexture());
        else
            blur = std::make_shared<Filter>(name, filters_.back()->GetTexture());
        blur->GetMaterial()->SetRenderPass(RenderPass::BLUR);
        blur->GetMaterial()->FlipYTextureCoords(true);
        AddFilter(blur);
        return blur;
    }

    PFilter Window::AddBlendFilter()
    {
        //CHECK_ASSERT(filters_.size() > 0, __FILE__, __LINE__);
        PFilter blend;
        std::string name = GetUniqueName("FilterBlend");
        size_t n = filters_.size();
        if (n > 1)
        {
            blend = std::make_shared<Filter>(name, filters_[n - 2]->GetTexture());
            auto texture = filters_[n - 1]->GetTexture();
            texture->SetMapType(TextureType::NORM);
            blend->GetMaterial()->SetTexture(texture);
        }
        else
        {
            blend = std::make_shared<Filter>(name, frameBuffer_->GetColorTexture());
            if (n == 1)
            {
                auto texture = filters_[0]->GetTexture();
                texture->SetMapType(TextureType::NORM);
                blend->GetMaterial()->SetTexture(texture);
            }
        }
        blend->GetMaterial()->SetRenderPass(RenderPass::BLEND);
        blend->GetMaterial()->FlipYTextureCoords(true);
        AddFilter(blend);
        return blend;
    }

    PFilter Window::AddWaveFilter()
    {
        PFilter wave;
        std::string name = GetUniqueName("FilterWave");
        if (filters_.empty())
            wave = std::make_shared<Filter>(name, frameBuffer_->GetColorTexture());
        else
            wave = std::make_shared<Filter>(name, filters_.back()->GetTexture());
        wave->GetMaterial()->SetRenderPass(RenderPass::WAVE);
        wave->GetMaterial()->FlipYTextureCoords(true);
        AddFilter(wave);
        return wave;
    }

    void Window::AddFilter(PFilter filter)
    {
        filter->SetWindow(this);
        filters_.push_back(filter);
        showMap_->SetColortexture(filter->GetTexture());
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
                TRACE_PRINTF("Only one window is allowed for this platform!!!");
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

    void Window::UpdateScenes(float delta)
    {
        for (auto& obj : windows_)
        {
            auto window(obj.lock());
            if (window && window->scene_)
                window->scene_->UpdateAll(delta);
        }
    }

    void Window::RenderFrame()
    {
        if (BeginFrameRender())
        {
            Renderer::GetPtr()->Render(this, scene_);
            EndFrameRender();
            SwapWindowBuffers();
        }
    }

    bool Window::RenderWindows()
    {
        for (auto& obj : windows_)
        {
            auto window(obj.lock());
            if (!window || window->IsClosed())
                continue;
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

    void Window::HandleEvents()
    {
        #if SDL
        SDLWindow::HandleEvents();
        #else
        CHEKC_ASSERT(!"So far only support for SDL...!!!", __FILE__, __LINE__);
        #endif
    }

    void Window::SetScene(Scene* scene)
    {
        scene_ = scene;
    }

    void Window::NotifyOneWindow2Remove()
    {
        ++nWindows2Remove_;
    }

    SignalWindow::PSignal Window::SigReady()
    {
        static SignalWindow::PSignal signalReady(new SignalWindow);
        return signalReady;
    }

}