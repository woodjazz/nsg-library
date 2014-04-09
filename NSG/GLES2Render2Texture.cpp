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
#include "GLES2Render2Texture.h"
#include "Log.h"
#include "Check.h"
#include "App.h"
#include <assert.h>
#include <algorithm>

namespace NSG
{
	static GLES2Render2Texture::Renderers s_Renderers;

	GLES2Render2Texture::GLES2Render2Texture(PGLES2Texture pTexture)
	: pTexture_(pTexture),
    windowWidth_(0),
    windowHeight_(0)
	{
        CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);
        CHECK_ASSERT(pTexture != nullptr, __FILE__, __LINE__);

        glGenFramebuffers(1, &framebuffer_);
        glGenRenderbuffers(1, &depthRenderBuffer_);

        App* pApp = App::GetPtrInstance();

        if(pApp)
        {
		    auto viewSize = pApp->GetViewSize();
		    ViewChanged(viewSize.first, viewSize.second);
        }


        s_Renderers.push_back(this);
	}

	GLES2Render2Texture::~GLES2Render2Texture()
	{
		auto it = std::find(s_Renderers.begin(), s_Renderers.end(), this);
		assert(it != s_Renderers.end());
		s_Renderers.erase(it);

		glDeleteRenderbuffers(1, &depthRenderBuffer_);
		glDeleteFramebuffers(1, &framebuffer_);
	}

	GLES2Render2Texture::Renderers& GLES2Render2Texture::GetRenderers()
	{
		return s_Renderers;
	}

	void GLES2Render2Texture::ViewChanged(int32_t windowWidth, int32_t windowHeight)
	{
		windowWidth_ = windowWidth;
		windowHeight_ = windowHeight;
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

        // The color buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture_->GetID(), 0);

        //////////////////////////////////////////////////////////////////////////////////
        // The depth buffer
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, windowWidth, windowHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer_);
        //////////////////////////////////////////////////////////////////////////////////

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(GL_FRAMEBUFFER_COMPLETE != status)
        {
            TRACE_LOG("Frame buffer failed with error=" << status);
            CHECK_ASSERT(!"Frame buffer failed", __FILE__, __LINE__);
        }
	}


	void GLES2Render2Texture::Begin()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
        glClearColor(0, 0, 0, 0);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLES2Render2Texture::End()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}