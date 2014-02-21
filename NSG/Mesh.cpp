/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 N�stor Silveira Gorski

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
#include "Mesh.h"
#include "Log.h"
#include "IApp.h"

#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

namespace NSG
{
	VertexData::VertexData(Vertex3 position, Vertex3 color, Vertex2 uv)
	: position_(position), color_(color), uv_(uv)
	{

	}

	Mesh::Mesh(PGLES2Program pProgram, PGLES2Texture pTexture, GLenum usage) 
	: pProgram_(pProgram),
	pTexture_(pTexture),
	texture_loc_( pProgram_->GetUniformLocation("u_texture")),
	texcoord_loc_(pProgram_->GetAttributeLocation("a_texcoord")),
	position_loc_(pProgram_->GetAttributeLocation("a_position")),
	color_loc_(pProgram_->GetAttributeLocation("a_color")),
    mvp_loc_(pProgram_->GetUniformLocation("u_mvp")),
    usage_(usage)
	{
	}

	Mesh::~Mesh() 
	{
	}

	void Mesh::AddVertexData(const VertexData& data)
	{
		vertexsData_.push_back(data);
	}

	void Mesh::SetIndices(const Indexes& indexes)
	{
		indexes_ = indexes;
	}

	void Mesh::Redo()
	{
		pVBuffer_ = nullptr;
		pIBuffer_ = nullptr;

		if(!vertexsData_.empty())
		{
			pVBuffer_ = PGLES2VertexBuffer(new GLES2VertexBuffer(sizeof(VertexData) * vertexsData_.size(), &vertexsData_[0], usage_));
			pIBuffer_ = PGLES2IndexBuffer(new GLES2IndexBuffer(sizeof(IndexType) * indexes_.size(), &indexes_[0], usage_));
		}
	}

	void Mesh::SetModelViewProjection(GLuint mvp_loc, PNode pNode)
	{
		Camera* pCamera = Camera::GetActiveCamera();

		if (pCamera)
		{
			Matrix4 matModelViewProjection = pCamera->GetViewProjection() * pNode->GetModelView();
			glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(matModelViewProjection));
		}
		else
		{
			Matrix4 matModelViewProjection = pNode->GetModelView();
			glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(matModelViewProjection));			
		}
	}

	void Mesh::Render(PNode pNode) 
	{
		if(!pTexture_->IsReady() || !pVBuffer_) 
			return;

		UseProgram useProgram(*pProgram_);

		SetModelViewProjection(mvp_loc_, pNode);

		glActiveTexture(GL_TEXTURE0);
		BindTexture binTexture(*pTexture_);

		glUniform1i(texture_loc_, 0);

		BindBuffer bindVBuffer(*pVBuffer_);

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

		BindBuffer bindIBuffer(*pIBuffer_);

        glDrawElements(GL_TRIANGLES, indexes_.size(), GL_UNSIGNED_BYTE, 0);
	}

	void Mesh::RenderForSelect(PNode pNode, GLuint position_loc, GLuint mvp_loc) 
	{
		if(!pVBuffer_) 
			return;

		SetModelViewProjection(mvp_loc, pNode);

		BindBuffer bindVBuffer(*pVBuffer_);

		glVertexAttribPointer(position_loc,
				3,
				GL_FLOAT,
				GL_FALSE,
				sizeof(VertexData),
				reinterpret_cast<void*>(offsetof(VertexData, position_)));

		glEnableVertexAttribArray(position_loc);

		BindBuffer bindIBuffer(*pIBuffer_);

        glDrawElements(GL_TRIANGLES, indexes_.size(), GL_UNSIGNED_BYTE, 0);
	}	
}