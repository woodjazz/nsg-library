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
#include "Camera.h"
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
#if NACL
#include "ppapi/cpp/var.h"
#endif

namespace NSG
{
    App::App() 
    : width_(0),
    height_(0),
    configuration_(new AppConfiguration)
    {
    }

    App::App(PAppConfiguration configuration) 
    : width_(0),
    height_(0),
    configuration_(configuration)
    {
    }

    App::~App()
    {
        TRACE_LOG("App Terminated");
    }

    void App::SetViewSize(int32_t width, int32_t height)
    {
        width_ = width;
        height_ = height;
    }

    std::pair<int32_t, int32_t> App::GetViewSize() const
    {
        return std::pair<int32_t, int32_t>(width_, height_);
    }

    void App::HandleMessage(const pp::Var& var_message)
    {
#if NACL
        TRACE_LOG("App::HandleMessage");

        if(var_message.is_string())
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

    void InternalApp::BeginTick()
    {
        // Set up texture data read/write alignment
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        SetFrameBuffer(0);

        CHECK_ASSERT(pApp_->width_ > 0 && pApp_->height_ > 0, __FILE__, __LINE__);
        glViewport(0, 0, pApp_->width_, pApp_->height_);

        if(AppStatistics::this_)
            AppStatistics::this_->Reset();

        pApp_->Start();
    }
    
    void InternalApp::DoTick(float delta)
    {
        pApp_->DoTick(delta);
    }
    
    void InternalApp::EndTick()
    {
    }

    void InternalApp::ViewChanged(int32_t width, int32_t height)
    {
        TRACE_LOG("ViewChanged: width=" << width << " height=" << height);

        pApp_->SetViewSize(width, height);

        Context::this_->InvalidateGPUResources();

#if 0
		Context::this_->ReleaseResourcesFromMemory();
#endif        

        IMGUI::Context::this_->Invalidate();

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
        PerformTick();

        ClearAllBuffers();

        pApp_->RenderFrame();

        Camera* camera(Camera::GetActiveCamera());

        IMGUI::Context::this_->RenderGUI();

		Camera::Activate(camera);

        if(AppStatistics::this_)
            AppStatistics::this_->NewFrame();
    }

    bool InternalApp::ShallExit() const
    {
        return pApp_->ShallExit();
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