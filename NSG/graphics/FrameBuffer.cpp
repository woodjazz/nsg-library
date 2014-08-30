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
#include "FrameBuffer.h"
#include "Log.h"
#include "Check.h"
#include "App.h"
#include "Context.h"
#include "Texture.h"
#include "TextureMemory.h"
#include "Graphics.h"
#include <algorithm>

namespace NSG
{
    FrameBuffer::FrameBuffer(int width, int height, Flags flags)
        : flags_(flags),
          width_(width),
          height_(height),
          framebuffer_(0),
          colorRenderbuffer_(0),
          depthStencilRenderBuffer_(0)
    {
		if (Flag::STENCIL & flags_)
        {
            // If we want stencil buffer then we force also the depth buffer.
            // depth and stencil buffers are shared (only one buffer)
            flags_ |= Flag::DEPTH;
        }
#if IOS
        if (flags_ & FrameBuffer::DEPTH)
        {
            //Force use depth texture on IOS
            //I do not know why but depth render buffer does not work on IOS
            //The alternative solution is to use a depth texture
            flags_ |= FrameBuffer::DEPTH_USE_TEXTURE;
        }
#endif

		if (flags_ & Flag::COLOR_USE_TEXTURE)
        {
            colorTexture_ = PTexture(new TextureMemory(GL_RGBA, width_, height_, nullptr));
        }

		if (flags_ & Flag::DEPTH_USE_TEXTURE)
        {
            if (Graphics::this_->HasDepthTexture())
            {
                depthTexture_ = PTexture(new TextureMemory(GL_DEPTH_COMPONENT, width_, height_, nullptr));
            }
            else
            {
                TRACE_LOG("Warning: We are trying to use a depth texture when graphics does not support it!!!");
                // clean out the flag that is not supported by the driver
                flags_ &= ~Flag::DEPTH_USE_TEXTURE;
#if IOS
                // in case of IOS also clean out render depth and stencil render buffers
                // this will probably will make the app not working (but at least will not crash)
                flags_ &= ~(Flag::DEPTH | Flag::STENCIL);
#endif
            }
        }
    }

    FrameBuffer::~FrameBuffer()
    {
        Context::RemoveObject(this);
    }

    bool FrameBuffer::IsValid()
    {
        return (!colorTexture_ || colorTexture_->IsReady()) && (!depthTexture_ || depthTexture_->IsReady());
    }

    void FrameBuffer::AllocateResources()
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        glGenFramebuffers(1, &framebuffer_);

        Graphics::this_->SetFrameBuffer(framebuffer_);

		if (flags_ & Flag::COLOR)
        {
			if (flags_ & Flag::COLOR_USE_TEXTURE)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture_->GetID(), 0);
            }
            else
            {
                glGenRenderbuffers(1, &colorRenderbuffer_);
                glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);
#if defined(GLES2)
                glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, width_, height_);
#else
                glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width_, height_);
#endif
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer_);
            }
        }

		if (flags_ & Flag::DEPTH)
        {
			if (flags_ & Flag::DEPTH_USE_TEXTURE)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture_->GetID(), 0);
				if (flags_ & Flag::STENCIL)
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture_->GetID(), 0);
            }
            else
            {
                glGenRenderbuffers(1, &depthStencilRenderBuffer_);
                glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRenderBuffer_);
				if (flags_ & Flag::STENCIL)
                {
                    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
                    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderBuffer_);
                    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderBuffer_);
                }
                else
                {
                    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width_, height_);
                    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderBuffer_);
                }
            }
        }

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (GL_FRAMEBUFFER_COMPLETE != status)
        {
            TRACE_LOG("Frame buffer failed with error = 0x" << std::hex << status << " in file = " << __FILE__ << " line = " << __LINE__);
            CHECK_ASSERT(!"Frame buffer failed", __FILE__, __LINE__);
        }

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    void FrameBuffer::ReleaseResources()
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        if (depthStencilRenderBuffer_)
            glDeleteRenderbuffers(1, &depthStencilRenderBuffer_);
        
        if (colorRenderbuffer_)
            glDeleteRenderbuffers(1, &colorRenderbuffer_);

        glDeleteFramebuffers(1, &framebuffer_);

        CHECK_GL_STATUS(__FILE__, __LINE__);

        Graphics::this_->SetFrameBuffer(0);
    }
}