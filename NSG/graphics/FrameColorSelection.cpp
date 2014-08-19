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
#include "FrameColorSelection.h"
#include "Log.h"
#include "Check.h"
#include "SceneNode.h"
#include "Camera.h"
#include "App.h"
#include "Graphics.h"
#include "Pass.h"
#include "Context.h"
#include "ProgramColorSelection.h"
#include "Material.h"
#include "Technique.h"
#include "Mesh.h"

namespace NSG
{
    FrameColorSelection::FrameColorSelection(bool createDepthBuffer, bool createDepthStencilBuffer)
        : material_(new Material),
          windowWidth_(0),
          windowHeight_(0),
          pixelX_(0),
          pixelY_(0),
          createDepthBuffer_(createDepthBuffer),
          createDepthStencilBuffer_(createDepthStencilBuffer)
    {
        Program *program = new ProgramColorSelection;
        material_->SetProgram(PProgram(program));
        PTechnique technique(new Technique);
        PPass pass(new Pass);
        technique->Add(pass);
        material_->SetTechnique(technique);
        pass->SetBlendMode(BLEND_NONE);
        App::Add(this);
    }

    FrameColorSelection::~FrameColorSelection()
    {
        App::Remove(this);
		Context::RemoveObject(this);
    }

    void FrameColorSelection::OnViewChanged(int32_t width, int32_t height)
    {
        windowWidth_ =  width;
        windowHeight_ = height;
    }

    bool FrameColorSelection::IsValid()
    {
        return material_->IsReady();
    }

    void FrameColorSelection::AllocateResources()
    {
        CHECK_ASSERT(windowWidth_ > 0 && windowHeight_ > 0, __FILE__, __LINE__);

        memset(selected_, 0, sizeof(selected_));

        CHECK_GL_STATUS(__FILE__, __LINE__);

        glGenFramebuffers(1, &framebuffer_);

        Graphics::this_->SetFrameBuffer(framebuffer_);

        {
            // Color buffer
            glGenRenderbuffers(1, &colorRenderbuffer_);
            glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);
#if defined(GLES2)
            glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, windowWidth_, windowHeight_);
#else
            glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, windowWidth_, windowHeight_);
#endif
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer_);
        }

        if (createDepthStencilBuffer_)
        {
            // The depth stencil buffer
            glGenRenderbuffers(1, &depthStencilRenderBuffer_);
            glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRenderBuffer_);
#if defined(GLES2)
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, windowWidth_, windowHeight_);
#else
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_EXT, windowWidth_, windowHeight_);
#endif
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderBuffer_);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderBuffer_);
        }
        else if (createDepthBuffer_)
        {
            // The depth buffer
            glGenRenderbuffers(1, &depthRenderBuffer_);
            glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer_);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, windowWidth_, windowHeight_);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer_);
        }

        CHECK_GL_STATUS(__FILE__, __LINE__);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (GL_FRAMEBUFFER_COMPLETE != status)
        {
            TRACE_LOG("Frame buffer failed with error = 0x" << std::hex << status << " in file = " << __FILE__ << " line = " << __LINE__);
            CHECK_ASSERT(!"Frame buffer failed", __FILE__, __LINE__);
        }

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    void FrameColorSelection::ReleaseResources()
    {
        if (createDepthStencilBuffer_)
            glDeleteRenderbuffers(1, &depthStencilRenderBuffer_);
        else if (createDepthBuffer_)
            glDeleteRenderbuffers(1, &depthRenderBuffer_);

        glDeleteRenderbuffers(1, &colorRenderbuffer_);
        glDeleteFramebuffers(1, &framebuffer_);
        Graphics::this_->SetFrameBuffer(0);
    }

    void FrameColorSelection::Begin(float screenX, float screenY)
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        pixelX_ = (GLint)((1 + screenX) / 2.0f * windowWidth_);
        pixelY_ = (GLint)((1 + screenY) / 2.0f * windowHeight_);

        Graphics::this_->SetFrameBuffer(framebuffer_);
        Graphics::this_->ClearBuffers(true, false, false);

#ifndef ANDROID
        glEnable(GL_SCISSOR_TEST);
        glScissor(pixelX_, pixelY_, 1, 1);
#endif
        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    void FrameColorSelection::End()
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        glReadPixels(pixelX_, pixelY_, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &selected_);
        Graphics::this_->SetFrameBuffer(0);

#ifndef ANDROID
        glDisable(GL_SCISSOR_TEST);
#endif
        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    GLushort FrameColorSelection::GetSelected() const
    {
        GLushort b3 = (GLushort)selected_[3] / 0x10;
        GLushort b2 = (GLushort)selected_[2] / 0x10;
        GLushort b1 = (GLushort)selected_[1] / 0x10;
        GLushort b0 = (GLushort)selected_[0] / 0x10;
        GLushort index = b3 << 12 | b2 << 8 | b1 << 4 | b0;
        return index;
    }

    Color FrameColorSelection::TransformSelectedId2Color(GLushort id)
    {
        Color color;
        color[0] = (id & 0x000F) / 15.0f;
        color[1] = ((id & 0x00F0) >> 4) / 15.0f;
        color[2] = ((id & 0x0F00) >> 8) / 15.0f;
        color[3] = ((id & 0xF000) >> 12) / 15.0f;
        return color;
    }

    bool FrameColorSelection::Render(GLushort id, float screenX, float screenY, const std::vector<SceneNode*>& nodes)
    {
        if (IsReady())
        {
            Begin(screenX, screenY);
            {
                material_->SetColor(TransformSelectedId2Color(id));
                
                Graphics::this_->Set(material_.get());
                
                for(auto& obj : nodes)
                {
                    Graphics::this_->Set(obj);
                    Graphics::this_->Set(obj->GetMesh().get());
                    material_->GetTechnique()->Render();
                }
            }
            End();
            return true;
        }
        return false;
    }

    bool FrameColorSelection::Hit(GLushort id, float screenX, float screenY, const std::vector<SceneNode*>& nodes)
    {
        if (Render(id, screenX, screenY, nodes))
            return id == GetSelected();

        return false;
    }

    void FrameColorSelection::ClearDepthStencil()
    {
        if (IsReady())
        {
            Graphics::this_->SetFrameBuffer(framebuffer_);
            Graphics::this_->ClearBuffers(false, true, true);
        }
    }
}
