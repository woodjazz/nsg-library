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
#pragma once

#include "Types.h"
#include "Object.h"
#include <string>

namespace NSG
{
    class FrameBuffer : public Object
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
		FrameBuffer(const std::string& name, Flags flags);
        ~FrameBuffer();
        PTexture GetColorTexture() const { return colorTexture_; }
        PTexture GetDepthTexture() const { return depthTexture_; }
        GLuint GetId() const { return framebuffer_; }
        void SetSize(int width, int height);
        void SetWindow(Window* window);
		int GetWidth() const { return width_; }
		int GetHeight() const { return height_; }
        void SetColorTexture(PTexture texture);
        void SetDepthTexture(PTexture texture);
    private:
		bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        int originalWidth_;
		int originalHeight_;
		int width_;
		int height_;
		Flags flags_;
        PTexture colorTexture_;
        PTexture depthTexture_;
        PTexture stencilTexture_;
        GLuint framebuffer_;
        GLuint colorRenderbuffer_;
        GLuint depthStencilRenderBuffer_;
		GLuint stencilRenderBuffer_;
        SignalViewChanged::PSlot slotViewChanged_;
        Window* window_;
    };
}