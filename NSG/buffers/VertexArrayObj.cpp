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
#include "VertexArrayObj.h"
#include "Program.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InstanceBuffer.h"
#include "Graphics.h"
#include "Check.h"

namespace NSG
{
    VertexArrayObj::VertexArrayObj(Program* program, VertexBuffer* vBuffer, IndexBuffer* iBuffer)
        : vao_(0),
          program_(program),
          vBuffer_(vBuffer),
          iBuffer_(iBuffer),
          graphics_(*Graphics::this_)
    {
    }

    VertexArrayObj::~VertexArrayObj()
    {
		Invalidate();
    }

	bool VertexArrayObj::IsValid()
	{
		return program_->IsReady();
	}

	void VertexArrayObj::AllocateResources()
	{
		CHECK_GL_STATUS(__FILE__, __LINE__);

		glGenVertexArrays(1, &vao_);

		CHECK_ASSERT(vao_ != 0, __FILE__, __LINE__);

		graphics_.SetVertexArrayObj(this);

		graphics_.SetVertexBuffer(vBuffer_, true);

		GLuint position_loc = program_->GetAttPositionLoc();
		GLuint texcoord_loc0 = program_->GetAttTextCoordLoc0();
		GLuint texcoord_loc1 = program_->GetAttTextCoordLoc1();
		GLuint normal_loc = program_->GetAttNormalLoc();
		GLuint color_loc = program_->GetAttColorLoc();
		GLuint tangent_loc = program_->GetAttTangentLoc();
		GLuint bones_id_loc = program_->GetAttBonesIDLoc();
		GLuint bones_weight = program_->GetAttBonesWeightLoc();

		if (position_loc != -1)
			glEnableVertexAttribArray((int)AttributesLoc::POSITION);

		if (normal_loc != -1)
			glEnableVertexAttribArray((int)AttributesLoc::NORMAL);

		if (texcoord_loc0 != -1)
			glEnableVertexAttribArray((int)AttributesLoc::TEXTURECOORD0);

		if (texcoord_loc1 != -1)
			glEnableVertexAttribArray((int)AttributesLoc::TEXTURECOORD1);

		if (color_loc != -1)
			glEnableVertexAttribArray((int)AttributesLoc::COLOR);

		if (tangent_loc != -1)
			glEnableVertexAttribArray((int)AttributesLoc::TANGENT);

		if (bones_id_loc != -1)
			glEnableVertexAttribArray((int)AttributesLoc::BONES_ID);

		if (bones_weight != -1)
			glEnableVertexAttribArray((int)AttributesLoc::BONES_WEIGHT);
	
		graphics_.SetVertexAttrPointers();

		graphics_.SetIndexBuffer(iBuffer_, true);

		graphics_.SetInstanceAttrPointers(program_);

		CHECK_GL_STATUS(__FILE__, __LINE__);
	}

	void VertexArrayObj::ReleaseResources()
	{
		if (graphics_.GetVertexArrayObj() == this)
			graphics_.SetVertexArrayObj(nullptr);

		glDeleteVertexArrays(1, &vao_);
		vao_ = 0;
	}

    void VertexArrayObj::Use()
    {
		if(IsReady())
			graphics_.SetVertexArrayObj(this);
    }

    void VertexArrayObj::Bind()
    {
        glBindVertexArray(vao_);
    }

    void VertexArrayObj::Unbind()
    {
        glBindVertexArray(0);
    }
}
