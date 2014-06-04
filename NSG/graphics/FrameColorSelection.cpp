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
#include "ProgramSimpleColor.h"
#include "Material.h"
#include "Technique.h"
#include "Mesh.h"

namespace NSG
{
	FrameColorSelection::FrameColorSelection(bool createDepthBuffer)
	: material_(new Material),
    windowWidth_(0),
    windowHeight_(0),
	pixelX_(0),
	pixelY_(0),
    createDepthBuffer_(createDepthBuffer)
	{
        Program* program = new ProgramSimpleColor;
        material_->SetProgram(PProgram(program));
        material_->SetBlendMode(BLEND_NONE);
	}

	FrameColorSelection::~FrameColorSelection()
	{
        Context::this_->Remove(this);
	}

	bool FrameColorSelection::IsValid()
    {
        return material_->IsReady();
    }

	void FrameColorSelection::AllocateResources()
    {
        auto windowSize = App::this_->GetViewSize();
        
        windowWidth_ = windowSize.first;
        
        windowHeight_ = windowSize.second;

        CHECK_ASSERT(windowWidth_ > 0 && windowHeight_ > 0, __FILE__, __LINE__);

        CHECK_GL_STATUS(__FILE__, __LINE__);

        glGenFramebuffers(1, &framebuffer_);
        glGenRenderbuffers(1, &colorRenderbuffer_);

        if(createDepthBuffer_)
        {
            glGenRenderbuffers(1, &depthRenderBuffer_);
        }

        memset(selected_, 0, sizeof(selected_));

        CHECK_GL_STATUS(__FILE__, __LINE__);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

        //////////////////////////////////////////////////////////////////////////////////
        // Color buffer
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);
#if defined(GLES2)
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, windowWidth_, windowHeight_);
#else                
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, windowWidth_, windowHeight_);
#endif  
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer_);
        //////////////////////////////////////////////////////////////////////////////////              

        if(createDepthBuffer_)
        {
            //////////////////////////////////////////////////////////////////////////////////
            // The depth buffer
            glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer_);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, windowWidth_, windowHeight_);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer_);
            //////////////////////////////////////////////////////////////////////////////////
        }

        CHECK_GL_STATUS(__FILE__, __LINE__);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(GL_FRAMEBUFFER_COMPLETE != status)
        {
            TRACE_LOG("Frame buffer failed with error=" << status);
            CHECK_ASSERT(!"Frame buffer failed", __FILE__, __LINE__);
        }

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

	void FrameColorSelection::ReleaseResources()
    {
        if(createDepthBuffer_)
        {
            glDeleteRenderbuffers(1, &depthRenderBuffer_);
        }

        glDeleteRenderbuffers(1, &colorRenderbuffer_);
        glDeleteFramebuffers(1, &framebuffer_);
    }

    void FrameColorSelection::Begin(float screenX, float screenY)
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        pixelX_ = (GLint)((1 + screenX)/2.0f * windowWidth_);
        pixelY_ = (GLint)((1 + screenY)/2.0f * windowHeight_);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
        ClearBuffers(true, true, false);

#ifndef ANDROID
        glEnable(GL_SCISSOR_TEST);
        glScissor(pixelX_,pixelY_,1,1);
#endif
        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    void FrameColorSelection::End()
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        glReadPixels(pixelX_, pixelY_, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &selected_);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
        color[3] = ((id & 0xF000) >> 12)/ 15.0f;
        return color;
    }

    bool FrameColorSelection::Render(GLushort id, float screenX, float screenY, Technique* technique)
    {
        if(IsReady())
        {
            Begin(screenX, screenY);
            {
                Mesh* lastMesh = nullptr;
			    Node* lastNode = nullptr;

                material_->SetColor(TransformSelectedId2Color(id));

                const PASSES& passes = technique->GetPasses();
                auto it = passes.begin();
                while(it != passes.end())
                {
                    PPass pass = *(it++);
                    if(pass)
                    {
                        const MESHNODES& meshNodes = pass->GetMeshNodes();
                        auto meshNodeIt = meshNodes.begin();
                        while(meshNodeIt != meshNodes.end())
                        {
                            Mesh* mesh = meshNodeIt->second.get();
                            Node* node = meshNodeIt->first.get();

                            if(lastMesh != mesh || lastNode != node) // optimization to not render always the same
                            {
                                material_->Render(true, node, mesh);
                                lastMesh = mesh;
                                lastNode = node;
                            }
                            ++meshNodeIt;
                        }
                    }
                }
            }
            End();
            return true;
        }
        return false;
    }
}
