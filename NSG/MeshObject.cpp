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
#include "MeshObject.h"
#include "NSG/Log.h"

#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

namespace NSG
{
	VertexData::VertexData(Vertex3 position, Vertex3 color, Vertex2 uv)
	: position_(position), color_(color), uv_(uv)
	{

	}

	MeshObject::MeshObject(PGLES2Program pProgram, PGLES2Texture pTexture) 
	: pProgram_(pProgram),
	pTexture_(pTexture),
	texture_loc_(0),
	position_loc_(0),
	color_loc_(0),
	dirty_(false)
	{
		texture_loc_ = pProgram_->GetUniformLocation("u_texture");
		position_loc_ = pProgram_->GetAttributeLocation("a_position");
		texcoord_loc_ = pProgram_->GetAttributeLocation("a_texcoord");
		color_loc_ = pProgram_->GetAttributeLocation("a_color");
	}

	MeshObject::~MeshObject() 
	{
	}

	void MeshObject::AddVertexData(const VertexData& data)
	{
		vertexsData_.push_back(data);
		dirty_ = true;
	}

	void MeshObject::SetIndices(const Indexes& indexes)
	{
		indexes_ = indexes;
		dirty_ = true;
	}

	void MeshObject::Redo()
	{
		if(dirty_)
		{
			pVBuffer_ = nullptr;
			pIBuffer_ = nullptr;

			if(!vertexsData_.empty())
			{
				pVBuffer_ = PGLES2VertexBuffer(new GLES2VertexBuffer(sizeof(VertexData) * vertexsData_.size(), &vertexsData_[0]));
				pIBuffer_ = PGLES2IndexBuffer(new GLES2IndexBuffer(sizeof(IndexType) * indexes_.size(), &indexes_[0]));
			}

			dirty_ = false;
		}
	}

	void MeshObject::SetPosition(const Vertex3& position)
	{
		position_ = position;
		Update();
	}

	void MeshObject::SetRotation(const Quaternion& q)
	{
		q_ = q;
		Update();
	}

	void MeshObject::Update()
	{
		matModelView_ = glm::translate(glm::mat4(), position_) * glm::mat4_cast(q_);
	}

	void MeshObject::Render() 
	{
		if(!pTexture_->Done() || !pVBuffer_) 
			return;

		//set what program to use
		pProgram_->Use();

		glActiveTexture(GL_TEXTURE0);
		pTexture_->Bind();
		glUniform1i(texture_loc_, 0);

		//define the attributes of the vertex
		pVBuffer_->Bind();

		glVertexAttribPointer(position_loc_,
				3,
				GL_FLOAT,
				GL_FALSE,
				sizeof(VertexData),
				reinterpret_cast<void*>(offsetof(VertexData, position_)));

		glEnableVertexAttribArray(position_loc_);

		glVertexAttribPointer(color_loc_,
				3,
				GL_FLOAT,
				GL_FALSE,
				sizeof(VertexData),
				reinterpret_cast<void*>(offsetof(VertexData, color_)));
		
		glEnableVertexAttribArray(color_loc_);

		glVertexAttribPointer(texcoord_loc_,
				2,
				GL_FLOAT,
				GL_FALSE,
				sizeof(VertexData),
				reinterpret_cast<void*>(offsetof(VertexData, uv_)));
		
		glEnableVertexAttribArray(texcoord_loc_);

		pIBuffer_->Bind();

        glDrawElements(GL_TRIANGLES, indexes_.size(), GL_UNSIGNED_BYTE, 0);
	}
}