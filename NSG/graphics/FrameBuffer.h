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
#pragma once

#include "Types.h"
#include "GPUObject.h"
#include "FlagSet.h"

namespace NSG
{
    class FrameBuffer : public GPUObject
    {
    public:
        enum Flag
        {
            COLOR = 1 << 0,
            DEPTH = 1 << 1,
            STENCIL = 1 << 2,
            COLOR_USE_TEXTURE = 1 << 3,
            DEPTH_USE_TEXTURE = 1 << 4
        };

        typedef FlagSet<Flag> Flags;

        FrameBuffer(int width, int height, Flags flags);
        ~FrameBuffer();
        PTexture GetColorTexture() const
        {
            return colorTexture_;
        }
        PTexture GetDepthTexture() const
        {
            return depthTexture_;
        }
        GLuint GetId() const
        {
            return framebuffer_;
        }
    private:
        virtual bool IsValid() override;
        virtual void AllocateResources() override;
        virtual void ReleaseResources() override;
        Flags flags_;
        int width_;
        int height_;
        PTexture colorTexture_;
        PTexture depthTexture_;
        GLuint framebuffer_;
        GLuint colorRenderbuffer_;
        GLuint depthStencilRenderBuffer_;
		GLuint stencilRenderBuffer_;
    };
}