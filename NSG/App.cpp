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
#include "Log.h"
#include "GLES2IMGUI.h"
#include <cassert>

namespace NSG
{
    App* s_pApp = nullptr;

    App::App() 
    : width_(0),
    height_(0)

    {
	    assert(s_pApp == nullptr);

	    s_pApp = this;
    }

    App::~App()
    {
	    assert(s_pApp != nullptr);

	    s_pApp = nullptr;
    }

    int App::GetFPS() const
    {
        return 24;
    }


    App* App::GetPtrInstance()
    {
	    return s_pApp;
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

#if NACL
    void App::HandleMessage(const pp::Var& var_message)
    {
        TRACE_LOG("App::HandleMessage");

        if(var_message.is_string())
        {
            std::string message = var_message.AsString();
        }
    }
#endif    

    InternalApp::InternalApp(NSG::PApp pApp) 
    : pApp_(pApp)
    {
    }

    void InternalApp::Initialize()
    {
        IMGUI::AllocateResources();
        Tick::Initialize(pApp_->GetFPS());
    }

    void InternalApp::Release()
    {
        IMGUI::ReleaseResources();
    }

    void InternalApp::BeginTick()
    {
        
        pApp_->Start();
    }
    
    void InternalApp::DoTick(float delta)
    {
        pApp_->Update(delta);
    }
    
    void InternalApp::EndTick()
    {
        pApp_->LateUpdate();
    }

    void InternalApp::ViewChanged(int32_t width, int32_t height)
    {
        IMGUI::ViewChanged(width, height);

        pApp_->SetViewSize(width, height);

        if(width > 0 && height > 0)
        {
            TRACE_LOG("ViewChanged width=" << width << " height=" << height);  

            pApp_->ViewChanged(width, height);
        }
    }

    void InternalApp::OnMouseMove(double x, double y) 
    {
        pApp_->OnMouseMove(x, y);
    }

    void InternalApp::OnMouseDown(double x, double y) 
    {
        pApp_->OnMouseDown(x, y);
    }

    void InternalApp::OnMouseUp() 
    {
        pApp_->OnMouseUp();
    }

    void InternalApp::RenderFrame()
    {
        pApp_->RenderFrame();
        IMGUI::Begin();
        pApp_->RenderGUIFrame();
        IMGUI::End();
    }

#if NACL
    void InternalApp::HandleMessage(const pp::Var& var_message)
    {
        pApp_->HandleMessage(var_message);
    }
#elif ANDROID
    void InternalApp::SetAssetManager(AAssetManager* pAAssetManager)
    {
        pApp_->SetAssetManager(pAAssetManager);
    }
#endif

}