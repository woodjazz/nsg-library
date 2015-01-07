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
#include "UniformsUpdate.h"
#include "Object.h"
#include <vector>

namespace pugi
{
    class xml_node;
}

namespace NSG
{
    class Pass : public Object, UniformsUpdate
    {
    public:
        Pass(Technique* technique);
        virtual ~Pass();
        void SetBlendMode(BLEND_MODE mode);
        void EnableColorBuffer(bool enable);
        void EnableDepthBuffer(bool enable);
        void EnableDepthTest(bool enable);
        void EnableStencilTest(bool enable);
        void SetStencilMask(GLuint mask);
        void SetStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
        void SetStencilFunc(GLenum func, GLint ref, GLuint mask);
        void SetDrawMode(DrawMode mode);
        DrawMode GetDrawMode() const { return drawMode_; }
        void EnableCullFace(bool enable);
        void SetCullFace(CullFaceMode mode);
        void SetFrontFace(FrontFaceMode mode);
        void SetProgram(PProgram pProgram);
        PProgram GetProgram() const;
        virtual void Render();
        void Render(Batch& batch);
        void Save(pugi::xml_node& node);
        void Load(const pugi::xml_node& node, PMaterial material);
        PPass Clone(PMaterial material) const;
    protected:
        virtual bool IsValid() override;
        void SetupPass();
    private:
        Technique* technique_;
        Graphics& graphics_;
        PProgram pProgram_;
        BLEND_MODE blendMode_;
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
        DrawMode drawMode_;
        bool enableCullFace_;
        CullFaceMode cullFaceMode_;
        FrontFaceMode frontFaceMode_;
    };
}