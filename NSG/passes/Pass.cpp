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
#include "Pass.h"
#include "Material.h"
#include "Mesh.h"
#include "Check.h"
#include "Graphics.h"
#include "Camera.h"
#include "BoundingBox.h"
#include "Frustum.h"
#include "Context.h"
#include "Program.h"
#include "pugixml.hpp"
#include <sstream>

namespace NSG
{
    Pass::Pass()
        : graphics_(*Graphics::this_),
          blendMode_(BLEND_ALPHA),
          enableDepthTest_(true),
          enableStencilTest_(false),
          stencilMask_(~GLuint(0)),
          sfailStencilOp_(GL_KEEP),
          dpfailStencilOp_(GL_KEEP),
          dppassStencilOp_(GL_KEEP),
          stencilFunc_(GL_ALWAYS),
          stencilRefValue_(0),
          stencilMaskValue_(~GLuint(0)),
          enableColorBuffer_(true),
          enableDepthBuffer_(true),
          drawMode_(DrawMode::SOLID),
          enableCullFace_(true),
          cullFaceMode_(CullFaceMode::DEFAULT),
          frontFaceMode_(FrontFaceMode::DEFAULT)
    {

    }

    Pass::~Pass()
    {
        Context::RemoveObject(this);
    }

    void Pass::SetBlendMode(BLEND_MODE mode)
    {
        blendMode_ = mode;
    }

    void Pass::EnableColorBuffer(bool enable)
    {
        enableColorBuffer_ = enable;
    }

    void Pass::EnableDepthBuffer(bool enable)
    {
        enableDepthBuffer_ = enable;
    }

    void Pass::EnableDepthTest(bool enable)
    {
        enableDepthTest_ = enable;
    }

    void Pass::EnableStencilTest(bool enable)
    {
        enableStencilTest_ = enable;
    }

    void Pass::SetStencilMask(GLuint mask)
    {
        stencilMask_ = mask;
    }

    void Pass::SetStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
    {
        sfailStencilOp_ = sfail;
        dpfailStencilOp_ = dpfail;
        dppassStencilOp_ = dppass;
    }

    void Pass::SetStencilFunc(GLenum func, GLint ref, GLuint mask)
    {
        stencilFunc_ = func;
        stencilRefValue_ = ref;
        stencilMaskValue_ = mask;
    }

    void Pass::SetDrawMode(DrawMode mode)
    {
        drawMode_ = mode;
    }

    void Pass::EnableCullFace(bool enable)
    {
        enableCullFace_ = enable;
    }

    void Pass::SetCullFace(CullFaceMode mode)
    {
        cullFaceMode_ = mode;
    }

    void Pass::SetFrontFace(FrontFaceMode mode)
    {
        frontFaceMode_ = mode;
    }

    PProgram Pass::GetProgram() const
    {
        return pProgram_;
    }

    void Pass::SetupPass()
    {
		CHECK_GL_STATUS(__FILE__, __LINE__);

        graphics_.SetColorMask(enableColorBuffer_);
        graphics_.SetStencilTest(enableStencilTest_, stencilMask_, sfailStencilOp_,
                                 dpfailStencilOp_, dppassStencilOp_, stencilFunc_,
                                 stencilRefValue_, stencilMaskValue_);

        graphics_.SetBlendModeTest(blendMode_);
        graphics_.EnableDepthTest(enableDepthTest_);
        if (enableDepthTest_)
            graphics_.SetDepthMask(enableDepthBuffer_);

        graphics_.EnableCullFace(enableCullFace_);
        if (enableCullFace_)
        {
            graphics_.SetCullFace(cullFaceMode_);
            graphics_.SetFrontFace(frontFaceMode_);
        }

        graphics_.SetProgram(pProgram_.get());

		CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    bool Pass::Render()
    {
        if (IsReady())
        {
            SetupPass();
            return graphics_.Draw(drawMode_ == DrawMode::SOLID);
        }

        return false;
    }

    bool Pass::Render(Batch& batch)
    {
        if (IsReady())
        {
            SetupPass();
            return graphics_.Draw(drawMode_ == DrawMode::SOLID, batch);
        }
		return false;
    }

    void Pass::SetProgram(PProgram pProgram)
    {
        if (pProgram_ != pProgram)
        {
            pProgram_ = pProgram;
            SetUniformsNeedUpdate();
            Invalidate();
        }
    }

    bool Pass::IsValid()
    {
        return pProgram_ && pProgram_->IsReady();
    }

    void Pass::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Pass");

        if (pProgram_)
            pProgram_->Save(child);

        {
            std::stringstream ss;
            ss << blendMode_;
            child.append_attribute("blendMode") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << enableDepthTest_;
            child.append_attribute("enableDepthTest") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << enableStencilTest_;
            child.append_attribute("enableStencilTest") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << stencilMask_;
            child.append_attribute("stencilMask") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << sfailStencilOp_;
            child.append_attribute("sfailStencilOp") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << dpfailStencilOp_;
            child.append_attribute("dpfailStencilOp") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << dppassStencilOp_;
            child.append_attribute("dppassStencilOp") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << stencilFunc_;
            child.append_attribute("stencilFunc") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << stencilRefValue_;
            child.append_attribute("stencilRefValue") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << stencilMaskValue_;
            child.append_attribute("stencilMaskValue") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << enableColorBuffer_;
            child.append_attribute("enableColorBuffer") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << enableDepthBuffer_;
            child.append_attribute("enableDepthBuffer") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << (int)drawMode_;
            child.append_attribute("drawMode") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << enableCullFace_;
            child.append_attribute("enableCullFace") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << (int)cullFaceMode_;
            child.append_attribute("cullFaceMode") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << (int)frontFaceMode_;
            child.append_attribute("frontFaceMode") = ss.str().c_str();
        }

    }

    void Pass::Load(const pugi::xml_node& node)
    {
        pugi::xml_node programChild = node.child("Program");

        if (programChild)
            SetProgram(Program::CreateFrom(programChild));

        SetBlendMode((BLEND_MODE)node.attribute("blendMode").as_int());
        EnableDepthTest(node.attribute("enableDepthTest").as_bool());
        EnableStencilTest(node.attribute("enableStencilTest").as_bool());
        SetStencilMask(node.attribute("stencilMask").as_int());
        SetStencilOp(node.attribute("sfailStencilOp").as_int(), node.attribute("dpfailStencilOp").as_int(), node.attribute("dppassStencilOp").as_int());
        SetStencilFunc(node.attribute("stencilFunc").as_int(), node.attribute("stencilRefValue").as_int(), node.attribute("stencilMaskValue").as_int());
        EnableColorBuffer(node.attribute("enableColorBuffer").as_bool());
        EnableDepthBuffer(node.attribute("enableDepthBuffer").as_bool());
        SetDrawMode((DrawMode)node.attribute("drawMode").as_int());
        EnableCullFace(node.attribute("enableCullFace").as_bool());
        SetCullFace((CullFaceMode)node.attribute("cullFaceMode").as_int());
        SetFrontFace((FrontFaceMode)node.attribute("frontFaceMode").as_int());
    }
}