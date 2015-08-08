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
#include "UniformsUpdate.h"
#include "Object.h"
#include <string>

namespace pugi
{
    class xml_node;
}

namespace NSG
{
    class Pass;
    struct PassData
    {
        bool enableDepthTest_;
        bool enableStencilTest_;
        GLuint stencilMask_;
        GLenum sfailStencilOp_;
        GLenum dpfailStencilOp_;
        GLenum dppassStencilOp_;
        GLenum stencilFunc_;
        GLint stencilRefValue_;
        GLuint stencilMaskValue_;
        bool enableColorBuffer_;
        bool enableDepthBuffer_;
        FrontFaceMode frontFaceMode_;
        DepthFunc depthFunc_;
		bool enableScissorTest_;
		int scissorX_;
		int scissorY_;
		int scissorWidth_;
		int scissorHeight_;
		CullFaceMode cullFaceMode_;
        PassData();
    private:
        BLEND_MODE blendMode_;
        friend class Pass;
    };

    class Pass : UniformsUpdate
    {
    public:
        Pass();
        virtual ~Pass();
        void SetType(PassType type);
        PassType GetType() const { return type_; }
        void SetBlendMode(BLEND_MODE mode);
        BLEND_MODE GetBlendMode() const;
        void EnableColorBuffer(bool enable);
        void EnableDepthBuffer(bool enable);
        void EnableDepthTest(bool enable);
        void EnableStencilTest(bool enable);
		void EnableScissorTest(bool enable);
		void SetScissor(int x, int y, int width, int height);
        void SetDepthFunc(DepthFunc depthFunc);
        void SetStencilMask(GLuint mask);
        void SetStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
        void SetStencilFunc(GLenum func, GLint ref, GLuint mask);
        void SetFrontFace(FrontFaceMode mode);
		void SetCullFace(CullFaceMode mode);
        const PassData& GetData() const { return data_; }
    private:
        PassData data_;
        PassType type_;
    };
}