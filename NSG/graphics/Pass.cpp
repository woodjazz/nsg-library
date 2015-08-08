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
#include <sstream>

namespace NSG
{
    PassData::PassData()
        : enableDepthTest_(true),
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
          frontFaceMode_(FrontFaceMode::DEFAULT),
          depthFunc_(DepthFunc::LESS),
          blendMode_(BLEND_MODE::NONE),
		  enableScissorTest_(false),
		  scissorX_(0),
		  scissorY_(0),
		  scissorWidth_(0),
		  scissorHeight_(0),
		  cullFaceMode_(CullFaceMode::DEFAULT)
    {

    }

    Pass::Pass()
        : type_(PassType::DEFAULT)
    {
    }

    Pass::~Pass()
    {
    }

    void Pass::SetType(PassType type)
    {
        type_ = type;
    }

    void Pass::SetBlendMode(BLEND_MODE mode)
    {
        data_.blendMode_ = mode;
    }

    BLEND_MODE Pass::GetBlendMode() const
    {
        return data_.blendMode_;
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

	void Pass::EnableScissorTest(bool enable)
	{
		data_.enableScissorTest_ = enable;
	}

	void Pass::SetScissor(int x, int y, int width, int height)
	{
		data_.scissorX_ = x;
		data_.scissorY_ = x;
		data_.scissorWidth_ = width;
		data_.scissorHeight_ = height;
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

    void Pass::SetFrontFace(FrontFaceMode mode)
    {
        data_.frontFaceMode_ = mode;
    }

	void Pass::SetCullFace(CullFaceMode mode)
	{
		data_.cullFaceMode_ = mode;
	}
}