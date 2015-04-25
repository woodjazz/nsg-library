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
#include "Pass.h"
#include "Material.h"
#include "Mesh.h"
#include "Check.h"
#include "Graphics.h"
#include "Camera.h"
#include "BoundingBox.h"
#include "Frustum.h"
#include "Program.h"
#include "pugixml.hpp"
#include "Batch.h"
#include "Technique.h"
#include <sstream>

namespace NSG
{
    PassData::PassData()
        : blendMode_(BLEND_NONE),
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
          frontFaceMode_(FrontFaceMode::DEFAULT),
          depthFunc_(DepthFunc::LESS)
    {

    }

    Pass::Pass(Technique* technique)
        : technique_(technique)
    {
        data_.pProgram_ = std::make_shared<Program>(technique->GetMaterial());
    }

    Pass::~Pass()
    {
    }

    PPass Pass::Clone(Material* material) const
    {
        auto pass = std::make_shared<Pass>(technique_);
        pass->data_ = data_;
        pass->data_.pProgram_ = data_.pProgram_->Clone(material);
        return pass;
    }

    void Pass::SetBlendMode(BLEND_MODE mode)
    {
        data_.blendMode_ = mode;
    }

    void Pass::EnableColorBuffer(bool enable)
    {
        data_.enableColorBuffer_ = enable;
    }

    void Pass::EnableDepthBuffer(bool enable)
    {
        data_.enableDepthBuffer_ = enable;
    }

    void Pass::EnableDepthTest(bool enable)
    {
        data_.enableDepthTest_ = enable;
    }

    void Pass::EnableStencilTest(bool enable)
    {
        data_.enableStencilTest_ = enable;
    }

    void Pass::SetDepthFunc(DepthFunc depthFunc)
    {
        data_.depthFunc_ = depthFunc;
    }

    void Pass::SetStencilMask(GLuint mask)
    {
        data_.stencilMask_ = mask;
    }

    void Pass::SetStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
    {
        data_.sfailStencilOp_ = sfail;
        data_.dpfailStencilOp_ = dpfail;
        data_.dppassStencilOp_ = dppass;
    }

    void Pass::SetStencilFunc(GLenum func, GLint ref, GLuint mask)
    {
        data_.stencilFunc_ = func;
        data_.stencilRefValue_ = ref;
        data_.stencilMaskValue_ = mask;
    }

    void Pass::SetDrawMode(DrawMode mode)
    {
        data_.drawMode_ = mode;
    }

    void Pass::EnableCullFace(bool enable)
    {
        data_.enableCullFace_ = enable;
    }

    void Pass::SetCullFace(CullFaceMode mode)
    {
        data_.cullFaceMode_ = mode;
    }

    void Pass::SetFrontFace(FrontFaceMode mode)
    {
        data_.frontFaceMode_ = mode;
    }

    bool Pass::IsTransparent() const
    {
        return data_.blendMode_ == BLEND_MODE::BLEND_ALPHA;
    }

    bool Pass::IsText() const
    {
        return data_.pProgram_->GetFlags() & (int)ProgramFlag::TEXT ? true : false;
    }

    void Pass::Draw()
    {
        Graphics::this_->DrawActiveMesh(this);
    }

    void Pass::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Pass");

        if (data_.pProgram_)
            data_.pProgram_->Save(child);

        child.append_attribute("blendMode").set_value((int)data_.blendMode_);
        child.append_attribute("enableDepthTest").set_value(data_.enableDepthTest_);
        child.append_attribute("enableStencilTest").set_value(data_.enableStencilTest_);
        child.append_attribute("stencilMask").set_value(data_.stencilMask_);
        child.append_attribute("sfailStencilOp").set_value(data_.sfailStencilOp_);
        child.append_attribute("dpfailStencilOp").set_value(data_.dpfailStencilOp_);
        child.append_attribute("dppassStencilOp").set_value(data_.dppassStencilOp_);
        child.append_attribute("stencilFunc").set_value(data_.stencilFunc_);
        child.append_attribute("stencilRefValue").set_value(data_.stencilRefValue_);
        child.append_attribute("stencilMaskValue").set_value(data_.stencilMaskValue_);
        child.append_attribute("enableColorBuffer").set_value(data_.enableColorBuffer_);
        child.append_attribute("enableDepthBuffer").set_value(data_.enableDepthBuffer_);
        child.append_attribute("drawMode").set_value((int)data_.drawMode_);
        child.append_attribute("enableCullFace").set_value(data_.enableCullFace_);
        child.append_attribute("cullFaceMode").set_value((int)data_.cullFaceMode_);
        child.append_attribute("frontFaceMode").set_value((int)data_.frontFaceMode_);
        child.append_attribute("depthFunc").set_value((int)data_.depthFunc_);
    }

    void Pass::Load(const pugi::xml_node& node, Material* material)
    {
        pugi::xml_node programChild = node.child("Program");
        std::string flags = programChild.attribute("flags").as_string();
        data_.pProgram_->SetFlags(flags);

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
        SetDepthFunc((DepthFunc)node.attribute("depthFunc").as_int());
    }

    void Pass::Invalidate()
    {
        data_.pProgram_->Invalidate();
    }
}