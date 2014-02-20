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
#include "IApp.h"
#include "Log.h"
#include <assert.h>

static const char kSelectFragShaderSource[] = {
#include "shaders/gles2SelectFragmentShader.h"
};

static const char kSelectVertexShaderSource[] = {
#include "shaders/gles2SelectVertexShader.h"
};


namespace NSG
{
    IApp* s_pIApp = nullptr;

    IApp::IApp() : selectionFramebuffer_(0), selectionColorRenderbuffer_(0), viewWidth_(0), viewHeight_(0)
    {
	    assert(s_pIApp == nullptr);

        memset(selectedIndex_, 0, sizeof(selectedIndex_));

	    s_pIApp = this;
    }

    IApp::~IApp()
    {
	    assert(s_pIApp != nullptr);

	    s_pIApp = nullptr;
    }


    IApp* IApp::GetPtrInstance()
    {
	    return s_pIApp;
    }

    void IApp::Setup(SetupStep step)
    {
        GLenum glError = GL_NO_ERROR;

        if(step == START)
        {
            pSelectProgram_ = PGLES2Program(new GLES2Program(kSelectVertexShaderSource, kSelectFragShaderSource));
            glGenFramebuffers(1, &selectionFramebuffer_);
            glGenRenderbuffers(1, &selectionColorRenderbuffer_);
        }

        if(step == START || step == VIEW_CHANGED)
        {
            if(viewWidth_ > 0 && viewHeight_ > 0)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, selectionFramebuffer_);
                glBindRenderbuffer(GL_RENDERBUFFER, selectionColorRenderbuffer_);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, viewWidth_, viewHeight_);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, selectionColorRenderbuffer_);
                GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
                if(GL_FRAMEBUFFER_COMPLETE != status)
                {
                    TRACE_LOG("Frame buffer failed with error=" << status << "\n");
                }
            }
        }
    }

    void IApp::StartSelection()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, selectionFramebuffer_);

        glClearColor(0, 0, 0, 0);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
    }

    GLushort IApp::EndSelection(double screenX, double screenY)
    {
        GLint pixelX = (GLint)((1 + screenX)/2.0 * viewWidth_);
        GLint pixelY = (GLint)((1 + screenY)/2.0 * viewHeight_);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glReadPixels(pixelX, pixelY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &selectedIndex_);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return GetSelectedIndex();
    }

    GLushort IApp::GetSelectedIndex() const
    {
        GLushort index = 0;
        index |= selectedIndex_[3] << 12;
        index |= selectedIndex_[2] << 8;
        index |= selectedIndex_[1] << 4;
        index |= selectedIndex_[0];
        return index;
    }

    Color IApp::TransformSelectedId2Color(GLushort id)
    {
        Color color;
        color[0] = (id & 0x000F) / 15.0;
        color[1] = ((id & 0x00F0) >> 4) / 15.0;
        color[2] = ((id & 0x0F00) >> 8) / 15.0;
        color[3] = ((id & 0xF000) >> 12)/ 15.0;
        return color;
    }

    void IApp::SetViewDimensions(int32_t viewWidth, int32_t viewHeight)
    {
        viewWidth_ = viewWidth;
        viewHeight_ = viewHeight;
    }

#if NACL
    void IApp::HandleMessage(const pp::Var& var_message)
    {
        TRACE_LOG("IApp::HandleMessage");

        if(var_message.is_string())
        {
            std::string message = var_message.AsString();
        }
    }
#endif    

    InternalApp::InternalApp(NSG::PApp pApp) : pApp_(pApp)
    {
    }

    void InternalApp::Initialize(int fps)
    {
        pApp_->Setup(IApp::START);
        Tick::Initialize(fps);
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
        pApp_->SetViewDimensions(width, height);

        pApp_->Setup(IApp::VIEW_CHANGED);

        pApp_->ViewChanged(width, height);
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