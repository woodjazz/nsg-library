/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "Texture2D.h"
#include "TextureCube.h"
#include "Graphics.h"
#include "Util.h"
#include "Window.h"
#include <algorithm>

namespace NSG
{
    FrameBuffer::FrameBuffer(const std::string& name, Flags flags)
        : Object(name),
          originalWidth_(0),
          originalHeight_(0),
          width_(0),
          height_(0),
          flags_(flags),
		  depthTexture_(std::make_shared<Texture2D>(name + "DepthBuffer")),
		  stencilTexture_(std::make_shared<Texture2D>(name + "StencilBuffer")),
          framebuffer_(0),
          colorRenderbuffer_(0),
          depthStencilRenderBuffer_(0),
          stencilRenderBuffer_(0),
          window_(nullptr),
		  autoSize_(true)
    {
        if (Flag::COLOR_CUBE_TEXTURE & flags_)
            colorTexture_ = std::make_shared<TextureCube>(name + "ColorCubeBuffer");
        else
            colorTexture_ = std::make_shared<Texture2D>(name + "ColorBuffer");
    }

    FrameBuffer::~FrameBuffer()
    {
    }

    bool FrameBuffer::IsValid()
    {
        if (width_ == 0 || height_ == 0)
            return false;

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
			colorTexture_->SetSize(width_, height_);
        else
            colorTexture_->SetSize(0, 0);

        if (flags_ & Flag::DEPTH_USE_TEXTURE)
        {
            if (Graphics::this_->HasDepthTexture())
            {
                depthTexture_->SetSize(width_, height_);
                if (flags_ & Flag::STENCIL)
                    stencilTexture_->SetSize(width_, height_);
            }
            else
            {
                depthTexture_->SetSize(0, 0);
                LOGW("We are trying to use a depth texture when graphics does not support it");
                // clean out the flag that is not supported by the driver
                flags_ &= ~Flag::DEPTH_USE_TEXTURE;
                #if IOS
                // in case of IOS also clean out render depth and stencil render buffers
                // this will probably will make the app not working (but at least will not crash)
                flags_ &= ~(Flag::DEPTH | Flag::STENCIL);
                #endif
            }
        }

        return (!(flags_ & Flag::COLOR_USE_TEXTURE) || colorTexture_->IsReady()) && (!(flags_ & Flag::DEPTH_USE_TEXTURE) || depthTexture_->IsReady());
    }

    TextureTarget FrameBuffer::GetDefaultTextureTarget() const
    {
        if (Flag::COLOR_CUBE_TEXTURE & flags_)
            return TextureTarget::TEXTURE_CUBE_MAP_POSITIVE_X;
        else
            return TextureTarget::TEXTURE_2D;
    }

    void FrameBuffer::AllocateResources()
    {
        LOGI("Framebuffer width=%d, height=%d", width_, height_);
        CHECK_GL_STATUS(__FILE__, __LINE__);

        glGenFramebuffers(1, &framebuffer_);

        Graphics::this_->SetFrameBuffer(this, GetDefaultTextureTarget());

        CHECK_GL_STATUS(__FILE__, __LINE__);

        if (flags_ & Flag::COLOR && !(flags_ & Flag::COLOR_USE_TEXTURE))
        {
            glGenRenderbuffers(1, &colorRenderbuffer_);
            glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);
            #if defined(GLES2)
            {
                glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB565, width_, height_);
            }
            #else
            {
                glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width_, height_);
            }
            #endif
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer_);
        }

        if (flags_ & Flag::DEPTH && !(flags_ & Flag::DEPTH_USE_TEXTURE))
        {
            glGenRenderbuffers(1, &depthStencilRenderBuffer_);
            glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRenderBuffer_);
            if (flags_ & Flag::STENCIL)
            {
                if (Graphics::this_->HasPackedDepthStencil())
                {
                    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
                    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderBuffer_);
                    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderBuffer_);
                }
                else
                {
                    if (Graphics::this_->HasDepthComponent24())
                    {
                        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width_, height_);
                        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderBuffer_);
                    }
                    else
                    {
                        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width_, height_);
                        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderBuffer_);
                    }

                    glGenRenderbuffers(1, &stencilRenderBuffer_);
                    glBindRenderbuffer(GL_RENDERBUFFER, stencilRenderBuffer_);

                    glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, width_, height_);
                    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilRenderBuffer_);
                }
            }
            else
            {
                if (Graphics::this_->HasDepthComponent24())
                {
                    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width_, height_);
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
            LOGE("Frame buffer failed with error = 0x%x in file = %s line = %d", status, __FILE__, __LINE__);
        }

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    void FrameBuffer::ReleaseResources()
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        Graphics::this_->UnboundTextures();

        if (stencilRenderBuffer_)
        {
            glDeleteRenderbuffers(1, &stencilRenderBuffer_);
            stencilRenderBuffer_ = 0;
        }

        if (depthStencilRenderBuffer_)
        {
            glDeleteRenderbuffers(1, &depthStencilRenderBuffer_);
            depthStencilRenderBuffer_ = 0;
        }

        if (colorRenderbuffer_)
        {
            glDeleteRenderbuffers(1, &colorRenderbuffer_);
            colorRenderbuffer_ = 0;
        }

        CHECK_ASSERT(framebuffer_, __FILE__, __LINE__);

        glDeleteFramebuffers(1, &framebuffer_);

        framebuffer_ = 0;

        Graphics::this_->SetFrameBuffer(0);

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    void FrameBuffer::SetSize(int width, int height)
    {
        CHECK_ASSERT(width >= 0 && height >= 0, __FILE__, __LINE__);
        auto maxSize = Graphics::this_->GetMaxTextureSize();
        width = glm::clamp(width, 0, maxSize);
        height = glm::clamp(height, 0, maxSize);

        if (originalWidth_ != width || originalHeight_ != height)
        {
            originalWidth_ = width;
            originalHeight_ = height;

            if (!Graphics::this_->IsTextureSizeCorrect(width, height))
                GetPowerOfTwoValues(width, height);

            width_ = width;
            height_ = height;
            Invalidate();
        }
    }

    void FrameBuffer::SetWindow(Window* window)
    {
        if (window_ != window)
        {
            window_ = window;
            if (window)
            {
                SetSize(window->GetWidth(), window->GetHeight());
                slotViewChanged_ = window->SigSizeChanged()->Connect([&](int width, int height)
                {
					if (autoSize_)
						SetSize(width, height);
                });
            }
            else
            {
                slotViewChanged_ = nullptr;
            }
        }
    }

    void FrameBuffer::SetColorTexture(PTexture texture)
    {
        if (colorTexture_ != texture)
        {
            colorTexture_ = texture;
            Invalidate();
        }
    }

    void FrameBuffer::SetDepthTexture(PTexture texture)
    {
        if (depthTexture_ != texture)
        {
            depthTexture_ = texture;
            Invalidate();
        }
    }

    void FrameBuffer::AttachTarget(TextureTarget colorTexTarget)
    {
        if (flags_ & Flag::COLOR_USE_TEXTURE)
        {
            CHECK_ASSERT(colorTexTarget != TextureTarget::UNKNOWN, __FILE__, __LINE__);
            CHECK_ASSERT(this == Graphics::this_->GetFrameBuffer(), __FILE__, __LINE__);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, (GLenum)colorTexTarget, colorTexture_->GetID(), 0);
        }

        if (flags_ & Flag::DEPTH_USE_TEXTURE)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture_->GetID(), 0);
            if (flags_ & Flag::STENCIL)
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencilTexture_->GetID(), 0);
        }

    }
}