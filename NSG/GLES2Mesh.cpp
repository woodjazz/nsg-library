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
#include "GLES2Mesh.h"
#include "Log.h"
#include "Check.h"
#include "App.h"

#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

namespace NSG
{
	VertexData::VertexData()
	{
	}

	VertexData::VertexData(Vertex3 position, Vertex2 uv)
	: position_(position), uv_(uv)
	{
	}

	GLES2Mesh::GLES2Mesh(GLenum usage) 
	: usage_(usage),
    mode_(GL_TRIANGLES),
    selectMode_(GL_TRIANGLES),
    loaded_(false),
    blendMode_(ALPHA),
    enableDepthTest_(true)
	{
	}

	GLES2Mesh::~GLES2Mesh() 
	{
	}

	void GLES2Mesh::SetBlendMode(BLEND_MODE mode)
	{
		blendMode_ = mode;
	}

	void GLES2Mesh::EnableDepthTest(bool enable)
	{
		enableDepthTest_ = enable;
	}

	void GLES2Mesh::AddVertexData(const VertexData& data)
	{
		vertexsData_.push_back(data);
	}

	void GLES2Mesh::SetIndices(const Indexes& indexes)
	{
		indexes_ = indexes;
	}

	void GLES2Mesh::Redo()
	{
		pVBuffer_ = nullptr;
		pIBuffer_ = nullptr;

		if(!vertexsData_.empty())
		{
			assert(indexes_.size() % 3 == 0);
			pVBuffer_ = PGLES2VertexBuffer(new GLES2VertexBuffer(sizeof(VertexData) * vertexsData_.size(), &vertexsData_[0], usage_));
			if(!indexes_.empty())
			{
				pIBuffer_ = PGLES2IndexBuffer(new GLES2IndexBuffer(sizeof(IndexType) * indexes_.size(), &indexes_[0], usage_));
			}
		}
	}

	void GLES2Mesh::SetMaterial(PGLES2Material pMaterial)
	{
        CHECK_ASSERT(pMaterial, __FILE__, __LINE__);
		pMaterial_ = pMaterial;
		texcoord_loc_ = pMaterial_->GetTextCoordAttLocation();
		position_loc_ = pMaterial_->GetPositionAttLocation();
		normal_loc_ = pMaterial_->GetNormalAttLocation();
	}

	void GLES2Mesh::Render(PNode pNode)
	{
		Render(pNode.get());
	}

	void GLES2Mesh::Render(Node* pNode) 
	{
        CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);

		App* pApp = App::GetPtrInstance();

		PGLES2FrameColorSelection pSelection = pApp->GetSelection();

		if(pSelection && pNode && pNode->IsSelectionEnabled())
		{
			pSelection->Render(this, pNode);
		}
		else
		{
			if(!pMaterial_ || !pMaterial_->IsReady()) 
				return;

	        if(!loaded_)
	        {
		        texcoord_loc_ = pMaterial_->GetTextCoordAttLocation();
		        position_loc_ = pMaterial_->GetPositionAttLocation();
		        normal_loc_ = pMaterial_->GetNormalAttLocation();

		        loaded_ = true;
	        }

			CHECK_ASSERT(pVBuffer_, __FILE__, __LINE__);

	        CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);

	        switch(blendMode_)
	       	{
	        	case NONE:
		        	glDisable(GL_BLEND);
		        	break;

		        case ALPHA:
		        	glEnable(GL_BLEND);
		        	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		        	break;

		        default:
		        	CHECK_ASSERT(false && "Undefined blend mode", __FILE__, __LINE__);
		        	break;
	        }
            
            if(enableDepthTest_)
            {
            	glEnable(GL_DEPTH_TEST);
            }
            else
            {
            	glDisable(GL_DEPTH_TEST);
            }

			UseMaterial useMaterial(*pMaterial_, pNode);

            CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);

			BindBuffer bindVBuffer(*pVBuffer_);

            CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);

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

            CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);

			if(!indexes_.empty())
			{
				BindBuffer bindIBuffer(*pIBuffer_);

	        	glDrawElements(mode_, indexes_.size(), GL_UNSIGNED_SHORT, 0);
	        }
	        else
	        {
				glDrawArrays(mode_, 0, vertexsData_.size());
	        }

			if(position_loc_ != -1)
			{
				glDisableVertexAttribArray(position_loc_);	
			}

			if(normal_loc_ != -1)
			{
				glDisableVertexAttribArray(normal_loc_);	
			}

			if(texcoord_loc_ != -1)
			{
				glDisableVertexAttribArray(texcoord_loc_);	
			}

			CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);
	    }
	}

	void GLES2Mesh::RenderForSelect(PNode pNode, GLuint position_loc, GLuint mvp_loc) 
	{
		RenderForSelect(pNode.get(), position_loc, mvp_loc);
	}	

	void GLES2Mesh::RenderForSelect(Node* pNode, GLuint position_loc, GLuint mvp_loc) 
	{
		if(!pVBuffer_) 
			return;

        assert(glGetError() == GL_NO_ERROR);

		Matrix4 mvp = GLES2Camera::GetModelViewProjection(pNode);
		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));

		BindBuffer bindVBuffer(*pVBuffer_);

		glVertexAttribPointer(position_loc,
				3,
				GL_FLOAT,
				GL_FALSE,
				sizeof(VertexData),
				reinterpret_cast<void*>(offsetof(VertexData, position_)));

		glEnableVertexAttribArray(position_loc);

        if(!indexes_.empty())
		{
    		BindBuffer bindIBuffer(*pIBuffer_);

            glDrawElements(selectMode_, indexes_.size(), GL_UNSIGNED_SHORT, 0);   
        }
        else
        {
			glDrawArrays(selectMode_, 0, vertexsData_.size());
        }

        assert(glGetError() == GL_NO_ERROR);
	}	

}