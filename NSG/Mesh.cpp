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
#include "Mesh.h"
#include "Log.h"
#include "IApp.h"

#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

namespace NSG
{
	VertexData::VertexData()
	{
	}

	VertexData::VertexData(Vertex3 position, Color color, Vertex2 uv)
	: position_(position), color_(color), uv_(uv)
	{
	}

	Mesh::Mesh(PGLES2Program pProgram, PGLES2Texture pTexture, GLenum usage) 
	: pProgram_(pProgram),
	pTexture_(pTexture),
    mvp_loc_(pProgram_->GetUniformLocation("u_mvp")),
    m_loc_(pProgram_->GetUniformLocation("u_m")),
    vp_loc_(pProgram_->GetUniformLocation("u_vp")),
	model_inv_transp_loc_(pProgram_->GetUniformLocation("u_model_inv_transp")),
	texture_loc_( pProgram_->GetUniformLocation("u_texture")),
	texcoord_loc_(pProgram_->GetAttributeLocation("a_texcoord")),
	position_loc_(pProgram_->GetAttributeLocation("a_position")),
	normal_loc_(pProgram_->GetAttributeLocation("a_normal")),
	color_loc_(pProgram_->GetAttributeLocation("a_color")),
    usage_(usage),
    mode_(GL_TRIANGLES)
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
			assert(indexes_.size() % 3 == 0);
			pVBuffer_ = PGLES2VertexBuffer(new GLES2VertexBuffer(sizeof(VertexData) * vertexsData_.size(), &vertexsData_[0], usage_));
			pIBuffer_ = PGLES2IndexBuffer(new GLES2IndexBuffer(sizeof(IndexType) * indexes_.size(), &indexes_[0], usage_));
		}
	}

	void Mesh::SetMode(Mode mode)
	{
		switch(mode)
		{
			case POINTS:
				mode_ = GL_POINTS;
				break;
			case LINES:
				mode_ = GL_LINES;
				break;
			case TRIANGLES:
				mode_ = GL_TRIANGLES;
				break;
			default:
			assert(false);
		}
	}

	void Mesh::CalculateFlatNormals()
	{
		for (int i = 0; i < indexes_.size(); i++) 
		{
			if ((i % 3) == 2) 
			{
				GLushort ia = indexes_[i-2];
				GLushort ib = indexes_[i-1];
				GLushort ic = indexes_[i];
				
				vertexsData_[ic].normal_ = vertexsData_[ib].normal_ = vertexsData_[ia].normal_ = glm::normalize(glm::cross(
					vertexsData_[ic].position_ - vertexsData_[ia].position_,
					vertexsData_[ib].position_ - vertexsData_[ia].position_));
			}
		}	
	}

	void Mesh::CalculateAverageNormals()
	{
		std::vector<int> nb_seen;
		nb_seen.resize(vertexsData_.size(), 0);

		for (int i = 0; i < indexes_.size(); i+=3) 
		{
			GLushort ia = indexes_[i];
			GLushort ib = indexes_[i+1];
			GLushort ic = indexes_[i+2];

			glm::vec3 normal = glm::normalize(glm::cross(
			  vertexsData_[ib].position_ - vertexsData_[ia].position_,
			  vertexsData_[ic].position_ - vertexsData_[ia].position_));

			int v[3];  v[0] = ia;  v[1] = ib;  v[2] = ic;

			for (int j = 0; j < 3; j++) 
			{
				GLushort cur_v = v[j];
				nb_seen[cur_v]++;
				if (nb_seen[cur_v] == 1) 
				{
					vertexsData_[cur_v].normal_ = normal;
				} 
				else 
				{
					// average
					vertexsData_[cur_v].normal_.x = vertexsData_[cur_v].normal_.x * (1.0 - 1.0/nb_seen[cur_v]) + normal.x * 1.0/nb_seen[cur_v];
					vertexsData_[cur_v].normal_.y = vertexsData_[cur_v].normal_.y * (1.0 - 1.0/nb_seen[cur_v]) + normal.y * 1.0/nb_seen[cur_v];
					vertexsData_[cur_v].normal_.z = vertexsData_[cur_v].normal_.z * (1.0 - 1.0/nb_seen[cur_v]) + normal.z * 1.0/nb_seen[cur_v];
					vertexsData_[cur_v].normal_ = glm::normalize(vertexsData_[cur_v].normal_);
				}
			}
		}		
	}

	void Mesh::Render(PNode pNode) 
	{
		if((pTexture_ && !pTexture_->IsReady()) || !pVBuffer_) 
			return;

        assert(glGetError() == GL_NO_ERROR);

		UseProgram useProgram(*pProgram_);

		if(mvp_loc_ != -1)
		{
			Matrix4 m = Camera::GetModelViewProjection(pNode);
			glUniformMatrix4fv(mvp_loc_, 1, GL_FALSE, glm::value_ptr(m));
		}

		if(m_loc_ != -1)
		{
			Matrix4 m = pNode->GetModelMatrix();
			glUniformMatrix4fv(m_loc_, 1, GL_FALSE, glm::value_ptr(m));
		}

		if(vp_loc_ != -1)
		{
			Matrix4 m = Camera::GetViewProjectionMatrix();
			glUniformMatrix4fv(vp_loc_, 1, GL_FALSE, glm::value_ptr(m));
		}

		if(model_inv_transp_loc_ != -1)
		{
			Matrix3 m = pNode->GetModelInvTRanspMatrix();
			glUniformMatrix3fv(model_inv_transp_loc_, 1, GL_FALSE, glm::value_ptr(m));			
		}

		if(texture_loc_ != -1 && pTexture_)
		{
			glActiveTexture(GL_TEXTURE0);
			pTexture_->Bind();
			glUniform1i(texture_loc_, 0);
		}

		BindBuffer bindVBuffer(*pVBuffer_);

		if(position_loc_ != -1)
		{
			glVertexAttribPointer(position_loc_,
					3,
					GL_FLOAT,
					GL_FALSE,
					sizeof(VertexData),
					reinterpret_cast<void*>(offsetof(VertexData, position_)));

			glEnableVertexAttribArray(position_loc_);
		}

		if(normal_loc_ != -1)
		{
			glVertexAttribPointer(normal_loc_,
					3,
					GL_FLOAT,
					GL_FALSE,
					sizeof(VertexData),
					reinterpret_cast<void*>(offsetof(VertexData, normal_)));
			
			glEnableVertexAttribArray(normal_loc_);
		}

		if(color_loc_ != -1)
		{
			glVertexAttribPointer(color_loc_,
					4,
					GL_FLOAT,
					GL_FALSE,
					sizeof(VertexData),
					reinterpret_cast<void*>(offsetof(VertexData, color_)));
			
			glEnableVertexAttribArray(color_loc_);
		}

		if(texcoord_loc_ != -1)
		{
			glVertexAttribPointer(texcoord_loc_,
					2,
					GL_FLOAT,
					GL_FALSE,
					sizeof(VertexData),
					reinterpret_cast<void*>(offsetof(VertexData, uv_)));
			
			glEnableVertexAttribArray(texcoord_loc_);
		}

		BindBuffer bindIBuffer(*pIBuffer_);

        glDrawElements(mode_, indexes_.size(), GL_UNSIGNED_SHORT, 0);

        glBindTexture(GL_TEXTURE_2D, 0);

        assert(glGetError() == GL_NO_ERROR);
	}

	void Mesh::RenderForSelect(PNode pNode, GLuint position_loc, GLuint mvp_loc) 
	{
		if(!pVBuffer_) 
			return;

        assert(glGetError() == GL_NO_ERROR);

		Matrix4 mvp = Camera::GetModelViewProjection(pNode);
		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));

		BindBuffer bindVBuffer(*pVBuffer_);

		glVertexAttribPointer(position_loc,
				3,
				GL_FLOAT,
				GL_FALSE,
				sizeof(VertexData),
				reinterpret_cast<void*>(offsetof(VertexData, position_)));

		glEnableVertexAttribArray(position_loc);

		BindBuffer bindIBuffer(*pIBuffer_);

        glDrawElements(GL_TRIANGLES, indexes_.size(), GL_UNSIGNED_SHORT, 0);   

        assert(glGetError() == GL_NO_ERROR);
	}	
}