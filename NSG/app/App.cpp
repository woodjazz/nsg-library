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
#include "App.h"
#include "AppConfiguration.h"
#include "AppStatistics.h"
#include "Log.h"
#include "IMGUI.h"
#include "IMGUIContext.h"
#include "IMGUISkin.h"
#include "TextMesh.h"
#include "Check.h"
#include "Context.h"
#include "Keyboard.h"
#include "Graphics.h"
#include "FrameColorSelection.h"
#include "AppStatistics.h"
#include "IMGUIContext.h"
#include "Graphics.h"
#if NACL
#include "ppapi/cpp/var.h"
#endif

namespace NSG
{
    template <> App* Singleton<App>::this_ = nullptr;

    App::App()
        : width_(0),
          height_(0)
    {
        context_ = PContext(new Context);
        configuration_ = PAppConfiguration(new AppConfiguration);

    }

    App::App(PAppConfiguration configuration)
        : width_(0),
          height_(0)
    {
        context_ = PContext(new Context);
        configuration_ = configuration;
    }

    App::~App()
    {
        App::this_ = nullptr;
        TRACE_LOG("App Terminated");
    }

    void App::SetViewSize(int32_t width, int32_t height)
    {
        width_ = width;
        height_ = height;

        for (auto& listener : viewChangedListeners_)
            listener->OnViewChanged(width, height);
    }

    std::pair<int32_t, int32_t> App::GetViewSize() const
    {
        return std::pair<int32_t, int32_t>(width_, height_);
    }

    void App::HandleMessage(const pp::Var& var_message)
    {
#if NACL
        TRACE_LOG("App::HandleMessage");

        if (var_message.is_string())
        {
            std::string message = var_message.AsString();
        }
#endif
    }
    void App::DoTick(float delta)
    {
        deltaTime_ = delta;
        IMGUI::DoTick();
        Update();
    }

    void App::AddListener(IViewChangedListener* listener)
    {
        viewChangedListeners_.push_back(listener);
        listener->OnViewChanged(width_, height_); //notify inmediately to have correct view size
    }

    void App::RemoveListener(IViewChangedListener* listener)
    {
        viewChangedListeners_.erase(std::find(viewChangedListeners_.begin(), viewChangedListeners_.end(), listener));
    }

    void App::Add(IViewChangedListener* listener)
    {
        if (App::this_)
        {
            App::this_->AddListener(listener);
        }
    }

    void App::Remove(IViewChangedListener* listener)
    {
        if (App::this_)
        {
            App::this_->RemoveListener(listener);
        }
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    InternalApp::InternalApp(App* pApp)
        : Tick(pApp->configuration_->fps_),
          pApp_(pApp),
          screenX_(0),
          screenY_(0)
    {
    }

    InternalApp::~InternalApp()
    {
        pApp_ = nullptr;
    }

    void InternalApp::InitializeTicks()
    {
        Context::this_->Initialize();

        pApp_->Start();
    }

    void InternalApp::BeginTicks()
    {
        Graphics::this_->BeginFrame();
    }

    void InternalApp::DoTick(float delta)
    {
        pApp_->DoTick(delta);
    }

    void InternalApp::EndTicks()
    {
        Graphics::this_->ClearAllBuffers();
        pApp_->RenderFrame();
#if 1
        IMGUI::Context::this_->RenderGUI();
#endif

        Graphics::this_->EndFrame();
     }

    void InternalApp::SetViewSize(int32_t width, int32_t height)
    {
        TRACE_LOG("SetViewSize: width=" << width << " height=" << height);
        pApp_->SetViewSize(width, height);
    }

    void InternalApp::ViewChanged(int32_t width, int32_t height)
    {
        //TRACE_LOG("ViewChanged: width=" << width << " height=" << height);
        pApp_->SetViewSize(width, height);
        pApp_->ViewChanged(width, height);
    }

    void InternalApp::OnMouseMove(float x, float y)
    {
        screenX_ = x;
        screenY_ = y;

        IMGUI::OnMouseMove(x, y);
        pApp_->OnMouseMove(x, y);
    }

    void InternalApp::OnMouseDown(float x, float y)
    {
        //TRACE_LOG("Mouse Down");
        screenX_ = x;
        screenY_ = y;

        IMGUI::OnMouseDown(x, y);
        pApp_->OnMouseDown(x, y);
    }

    void InternalApp::OnMouseUp(float x, float y)
    {
        //TRACE_LOG("Mouse Up");
        IMGUI::OnMouseUp(x, y);
        pApp_->OnMouseUp(x, y);
    }

    void InternalApp::OnMouseWheel(float x, float y)
    {
        IMGUI::OnMouseWheel(x, y);
        pApp_->OnMouseWheel(x, y);
    }

    void InternalApp::OnKey(int key, int action, int modifier)
    {
        //TRACE_LOG("key=" << key << " action=" << action << " modifier=" << modifier);
        IMGUI::OnKey(key, action, modifier);
        pApp_->OnKey(key, action, modifier);
    }

    void InternalApp::OnChar(unsigned int character)
    {
        //TRACE_LOG("character=" << character);
        IMGUI::OnChar(character);
        pApp_->OnChar(character);
    }

    void InternalApp::RenderFrame()
    {
        PerformTicks();

        AppStatistics::this_->NewFrame();
    }

    bool InternalApp::ShallExit() const
    {
        return pApp_->ShallExit();
    }

    void InternalApp::ReleaseResourcesFromMemory()
    {
        Context::this_->ReleaseResourcesFromMemory();
    }

    void InternalApp::InvalidateGPUContext()
    {
        Context::this_->InvalidateGPUResources();
    }

    void InternalApp::HandleMessage(const pp::Var& var_message)
    {
        pApp_->HandleMessage(var_message);
    }

    void InternalApp::SetAssetManager(AAssetManager* pAAssetManager)
    {
        pApp_->SetAssetManager(pAAssetManager);
    }

    void InternalApp::SetActivity(ANativeActivity* pActivity)
    {
        Keyboard::this_->SetActivity(pActivity);
    }

}