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
#include "FrameBuffer.h"
#include "Log.h"
#include "Check.h"
#include "App.h"
#include "Context.h"
#include "Graphics.h"
#include <algorithm>

namespace NSG
{
    Render2Texture::Render2Texture(int width, int height, UseBuffer buffer)
        : buffer_(buffer),
          width_(width),
          height_(height),
          enabled_(false)
    {
		FrameBuffer::Flags frameBufferFlags((unsigned int)(FrameBuffer::COLOR | FrameBuffer::COLOR_USE_TEXTURE)); // always use texture for color buffer
		if (buffer_ == UseBuffer::DEPTH)
            frameBufferFlags |= FrameBuffer::DEPTH;
        else if (buffer_ == UseBuffer::DEPTH_STENCIL)
            frameBufferFlags |= FrameBuffer::STENCIL;

        frameBuffer_ = PFrameBuffer(new FrameBuffer(width, height, frameBufferFlags));
    }

    Render2Texture::~Render2Texture()
    {
        Context::RemoveObject(this);
    }

    PTexture Render2Texture::GetTexture() const
    {
        return frameBuffer_->GetColorTexture();
    }

    bool Render2Texture::IsValid()
    {
        return frameBuffer_->IsReady();
    }

    bool Render2Texture::Begin()
    {
        if (IsReady())
        {
            CHECK_GL_STATUS(__FILE__, __LINE__);

            glGetIntegerv(GL_VIEWPORT, &viewport_[0]);

            Graphics::this_->SetFrameBuffer(frameBuffer_->GetId());

            Graphics::this_->SetViewport(Recti {0, 0, width_, height_});

            if (buffer_ == UseBuffer::DEPTH_STENCIL)
                Graphics::this_->ClearAllBuffers();
            else if (buffer_ == UseBuffer::DEPTH)
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