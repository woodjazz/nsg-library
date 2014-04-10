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
#include "GLES2FrameColorSelection.h"
#include "Log.h"
#include "Check.h"
#include "SceneNode.h"
#include "GLES2Camera.h"
#include <assert.h>

#define STRINGIFY(S) #S

static const char* vShader = STRINGIFY(
    uniform mat4 u_mvp;
    attribute vec4 a_position;
    
    void main() 
    {
        gl_Position = u_mvp * a_position;
    }
);

static const char* fShader = STRINGIFY(
    uniform vec4 u_color;
    void main()
    {
        gl_FragColor = u_color;
    }
);

namespace NSG
{
	GLES2FrameColorSelection::GLES2FrameColorSelection()
	: pProgram_(new GLES2Program(vShader, fShader)),
	position_loc_(pProgram_->GetAttributeLocation("a_position")),
	color_loc_(pProgram_->GetUniformLocation("u_color")),
	mvp_loc_(pProgram_->GetUniformLocation("u_mvp")),
	windowWidth_(0),
	windowHeight_(0),
	screenX_(0),
	screenY_(0),
	pixelX_(0),
	pixelY_(0)
	{
        CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);

        glGenFramebuffers(1, &framebuffer_);
        glGenRenderbuffers(1, &colorRenderbuffer_);
        glGenRenderbuffers(1, &depthRenderBuffer_);

        memset(selected_, 0, sizeof(selected_));

        CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);

	}

	GLES2FrameColorSelection::~GLES2FrameColorSelection()
	{
        glDeleteRenderbuffers(1, &depthRenderBuffer_);
        glDeleteRenderbuffers(1, &colorRenderbuffer_);
        glDeleteFramebuffers(1, &framebuffer_);
	}

	void GLES2FrameColorSelection::ViewChanged(int32_t windowWidth, int32_t windowHeight)
	{
        CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);

		windowWidth_ = windowWidth;
		windowHeight_ = windowHeight;
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

        //////////////////////////////////////////////////////////////////////////////////
        // Color buffer
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);
#if defined(NACL) || defined(ANDROID)
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, windowWidth, windowHeight);
#else                
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, windowWidth, windowHeight);
#endif  
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer_);
        //////////////////////////////////////////////////////////////////////////////////              

        //////////////////////////////////////////////////////////////////////////////////
        // The depth buffer
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, windowWidth, windowHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer_);
        //////////////////////////////////////////////////////////////////////////////////

        CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(GL_FRAMEBUFFER_COMPLETE != status)
        {
            TRACE_LOG("Frame buffer failed with error=" << status);
            CHECK_ASSERT(!"Frame buffer failed", __FILE__, __LINE__);
        }
	}

    void GLES2FrameColorSelection::Begin(float screenX, float screenY)
    {
        enabled_ = true;

    	screenX_ = screenX;
    	screenY_ = screenY;

        pixelX_ = (GLint)((1 + screenX)/2.0f * windowWidth_);
        pixelY_ = (GLint)((1 + screenY)/2.0f * windowHeight_);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
#ifndef ANDROID
        glEnable(GL_SCISSOR_TEST);
        glScissor(pixelX_,pixelY_,1,1);
#endif
        glClearColor(0, 0, 0, 0);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GLES2FrameColorSelection::End()
    {
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glReadPixels(pixelX_, pixelY_, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &selected_);
#ifndef ANDROID
        glDisable(GL_SCISSOR_TEST);
#endif        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        enabled_ = false;
    }

    GLushort GLES2FrameColorSelection::GetSelected() const
    {
        GLushort b3 = (GLushort)selected_[3] / 0x10;
        GLushort b2 = (GLushort)selected_[2] / 0x10;
        GLushort b1 = (GLushort)selected_[1] / 0x10;
        GLushort b0 = (GLushort)selected_[0] / 0x10;
        GLushort index = b3 << 12 | b2 << 8 | b1 << 4 | b0;
        return index;
    }

    Color GLES2FrameColorSelection::TransformSelectedId2Color(GLushort id)
    {
        Color color;
        color[0] = (id & 0x000F) / 15.0f;
        color[1] = ((id & 0x00F0) >> 4) / 15.0f;
        color[2] = ((id & 0x0F00) >> 8) / 15.0f;
        color[3] = ((id & 0xF000) >> 12)/ 15.0f;
        return color;
    }

    void GLES2FrameColorSelection::Render(SceneNode* pSceneNode)
    {
        Node* pNode = pSceneNode;

        Render(pNode->GetId(), pSceneNode->GetMesh().get(), pNode);
    }

    void GLES2FrameColorSelection::Render(GLushort id, GLES2Mesh* pMesh, Node* pNode)
    {
        UseProgram useProgram(*pProgram_);

        Color color = TransformSelectedId2Color(id);
        
        glUniform4fv(color_loc_, 1, &color[0]);

        Matrix4 mvp = GLES2Camera::GetModelViewProjection(pNode);
        glUniformMatrix4fv(mvp_loc_, 1, GL_FALSE, glm::value_ptr(mvp));        

        pMesh->Render(pMesh->GetSolidDrawMode(), position_loc_, -1, -1);
    }


}
