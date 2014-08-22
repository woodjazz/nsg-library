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
#include "Render2Texture.h"
#include "SDL.h"
#include "Log.h"
#include "Check.h"
#include "App.h"
#include "Context.h"
#include "Texture.h"
#include "TextureMemory.h"
#include "Graphics.h"
#include <algorithm>

#if defined(GLES2)
#define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
#else
//#define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_EXT
#endif

namespace NSG
{
    Render2Texture::Render2Texture(PTexture pTexture, bool createDepthBuffer, bool createDepthStencilBuffer)
        : pTexture_(pTexture),
          depthRenderBuffer_(0),
          depthStencilRenderBuffer_(0),
          createDepthBuffer_(createDepthBuffer),
          createDepthStencilBuffer_(createDepthStencilBuffer),
          enabled_(false),
          windowWidth_(0),
          windowHeight_(0)
    {
    }

    Render2Texture::~Render2Texture()
    {
        Context::RemoveObject(this);
    }

    bool Render2Texture::IsValid()
    {
        return pTexture_->IsReady();
    }

    void Render2Texture::AllocateResources()
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        CHECK_ASSERT(pTexture_ != nullptr, __FILE__, __LINE__);

        GLint width  = pTexture_->GetWidth();
        GLint height = pTexture_->GetHeight();
        //glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
        //glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);

        glGenFramebuffers(1, &framebuffer_);

        Graphics::this_->SetFrameBuffer(framebuffer_);

        {
            // The color buffer
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture_->GetID(), 0);
        }

        if (createDepthStencilBuffer_)
        {
            // The depth stencil buffer
#if IOS
            //I do not know why but depth render buffer does not work on IOS
            //The alternative solution is to use a depth texture
            CHECK_CONDITION(Graphics::this_->HasDepthTexture(), __FILE__, __LINE__);
            depthTexture_ = PTexture(new TextureMemory(GL_DEPTH_COMPONENT, width, height, nullptr));
            CHECK_CONDITION(depthTexture_->IsReady(), __FILE__, __LINE__);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture_->GetID(), 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture_->GetID(), 0);

#else
            glGenRenderbuffers(1, &depthStencilRenderBuffer_);
            glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRenderBuffer_);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderBuffer_);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderBuffer_);
#endif
        }
        else if (createDepthBuffer_)
        {
#if IOS
            //I do not know why but depth render buffer does not work on IOS
            //The alternative solution is to use a depth texture
            CHECK_CONDITION(Graphics::this_->HasDepthTexture(), __FILE__, __LINE__);
            depthTexture_ = PTexture(new TextureMemory(GL_DEPTH_COMPONENT, width, height, nullptr));
            CHECK_CONDITION(depthTexture_->IsReady(), __FILE__, __LINE__);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture_->GetID(), 0);
#else
            // The depth buffer
            glGenRenderbuffers(1, &depthRenderBuffer_);
            glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer_);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer_);
#endif
        }

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (GL_FRAMEBUFFER_COMPLETE != status)
        {
            TRACE_LOG("Frame buffer failed with error = 0x" << std::hex << status << " in file = " << __FILE__ << " line = " << __LINE__);
            CHECK_ASSERT(!"Frame buffer failed", __FILE__, __LINE__);
        }

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    void Render2Texture::ReleaseResources()
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        if (depthStencilRenderBuffer_)
            glDeleteRenderbuffers(1, &depthStencilRenderBuffer_);
        else if (depthRenderBuffer_)
            glDeleteRenderbuffers(1, &depthRenderBuffer_);

        glDeleteFramebuffers(1, &framebuffer_);

        CHECK_GL_STATUS(__FILE__, __LINE__);

        Graphics::this_->SetFrameBuffer(0);

        enabled_ = false;
    }

    bool Render2Texture::Begin()
    {
        if (IsReady())
        {
            CHECK_GL_STATUS(__FILE__, __LINE__);

            glGetIntegerv(GL_VIEWPORT, &viewport_[0]);

            Graphics::this_->SetFrameBuffer(framebuffer_);

            Graphics::this_->SetViewport(Recti {0, 0, pTexture_->GetWidth(), pTexture_->GetHeight()});

            if (createDepthStencilBuffer_)
                Graphics::this_->ClearAllBuffers();
            else if (createDepthBuffer_)
                Graphics::this_->ClearBuffers(true, true, false);
            else
                Graphics::this_->ClearBuffers(true, false, false);

            CHECK_GL_STATUS(__FILE__, __LINE__);

            enabled_ = true;
        }

        return enabled_;

    }

    void Render2Texture::End()
    {
        if (IsReady() && enabled_)
        {
            CHECK_GL_STATUS(__FILE__, __LINE__);

            Graphics::this_->DiscardFramebuffer();

            Graphics::this_->SetFrameBuffer(0);

            Graphics::this_->SetViewport(viewport_);

            CHECK_GL_STATUS(__FILE__, __LINE__);

            enabled_ = false;
        }
    }
}