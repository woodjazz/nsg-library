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
#include <assert.h>

static const char kSelectFragShaderSource[] = {
#include "shaders/gles2SelectFragmentShader.h"
};

static const char kSelectVertexShaderSource[] = {
#include "shaders/gles2SelectVertexShader.h"
};

namespace NSG
{
	FrameColorSelection::FrameColorSelection()
	: pProgram_(new GLES2Program(kSelectVertexShaderSource, kSelectFragShaderSource)),
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
        glGenFramebuffers(1, &framebuffer_);
        glGenRenderbuffers(1, &colorRenderbuffer_);
        memset(selected_, 0, sizeof(selected_));
	}

	FrameColorSelection::~FrameColorSelection()
	{

	}

	void FrameColorSelection::ViewChanged(int32_t windowWidth, int32_t windowHeight)
	{
		windowWidth_ = windowWidth;
		windowHeight_ = windowHeight;
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);
#if defined(NACL) || defined(ANDROID)
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, windowWidth, windowHeight);
#else                
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, windowWidth, windowHeight);
#endif                
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer_);
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(GL_FRAMEBUFFER_COMPLETE != status)
        {
            TRACE_LOG("Frame buffer failed with error=" << status << "\n");
            assert(false);
        }
	}

    void FrameColorSelection::Begin(double screenX, double screenY)
    {
    	screenX_ = screenX;
    	screenY_ = screenY;

        pixelX_ = (GLint)((1 + screenX)/2.0 * windowWidth_);
        pixelY_ = (GLint)((1 + screenY)/2.0 * windowHeight_);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
#ifndef ANDROID
        glEnable(GL_SCISSOR_TEST);
        glScissor(pixelX_,pixelY_,1,1);
#endif
        glClearColor(0, 0, 0, 0);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
    }

    void FrameColorSelection::End()
    {
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glReadPixels(pixelX_, pixelY_, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &selected_);
#ifndef ANDROID
        glDisable(GL_SCISSOR_TEST);
#endif        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

    void FrameColorSelection::Render(GLushort id, PMesh pMesh, PNode pNode)
    {
        UseProgram useProgram(*pProgram_);

		Color color = TransformSelectedId2Color(id);
		
		glUniform4fv(color_loc_, 1, &color[0]);

    	pMesh->RenderForSelect(pNode, position_loc_, mvp_loc_);
    }

}