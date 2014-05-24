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
#include "Context.h"

namespace NSG
{
	VertexData::VertexData()
	: color_(1,1,1,1)
	{
	}

	GLES2Mesh::GLES2Mesh(GLenum usage) 
	: usage_(usage)
	{
	}

	GLES2Mesh::~GLES2Mesh() 
	{
		Context::this_->Remove(this);
	}


	void GLES2Mesh::Render(bool solid, GLuint position_loc, GLuint texcoord_loc, GLuint normal_loc, GLuint color_loc)
	{
	   	if(IsReady())
	   	{
			CHECK_GL_STATUS(__FILE__, __LINE__);

			BindBuffer bindVBuffer(*pVBuffer_);

			if(position_loc != -1)
			{
				glVertexAttribPointer(position_loc,
						3,
						GL_FLOAT,
						GL_FALSE,
						sizeof(VertexData),
						reinterpret_cast<void*>(offsetof(VertexData, position_)));

				glEnableVertexAttribArray(position_loc);
			}

			if(normal_loc != -1)
			{
				glVertexAttribPointer(normal_loc,
						3,
						GL_FLOAT,
						GL_FALSE,
						sizeof(VertexData),
						reinterpret_cast<void*>(offsetof(VertexData, normal_)));
				
				glEnableVertexAttribArray(normal_loc);
			}

			if(texcoord_loc != -1)
			{
				glVertexAttribPointer(texcoord_loc,
						2,
						GL_FLOAT,
						GL_FALSE,
						sizeof(VertexData),
						reinterpret_cast<void*>(offsetof(VertexData, uv_)));
				
				glEnableVertexAttribArray(texcoord_loc);
			}

			if(color_loc != -1)
			{
				glVertexAttribPointer(color_loc,
						3,
						GL_FLOAT,
						GL_FALSE,
						sizeof(VertexData),
						reinterpret_cast<void*>(offsetof(VertexData, color_)));
				
				glEnableVertexAttribArray(color_loc);
			}


		    CHECK_GL_STATUS(__FILE__, __LINE__);

		    GLenum mode = solid ? GetSolidDrawMode() : GetWireFrameDrawMode();

			if(!indexes_.empty())
			{
				BindBuffer bindIBuffer(*pIBuffer_);

		    	glDrawElements(mode, indexes_.size(), GL_UNSIGNED_SHORT, 0);
		    }
		    else
		    {
				glDrawArrays(mode, 0, vertexsData_.size());
		    }

			if(position_loc != -1)
			{
				glDisableVertexAttribArray(position_loc);	
			}

			if(normal_loc != -1)
			{
				glDisableVertexAttribArray(normal_loc);	
			}

			if(texcoord_loc != -1)
			{
				glDisableVertexAttribArray(texcoord_loc);	
			}

			if(color_loc != -1)
			{
				glDisableVertexAttribArray(color_loc);	
			}


			CHECK_GL_STATUS(__FILE__, __LINE__);
		}
	}

	bool GLES2Mesh::IsValid()
	{
		if(resource_)
			return resource_->IsLoaded();
		else
			return !vertexsData_.empty();
	}

	void GLES2Mesh::AllocateResources()
	{
		CHECK_GL_STATUS(__FILE__, __LINE__);

		CHECK_ASSERT(pVBuffer_ == nullptr, __FILE__, __LINE__);
		CHECK_ASSERT(pIBuffer_ == nullptr, __FILE__, __LINE__);

		CHECK_ASSERT(!vertexsData_.empty(), __FILE__, __LINE__);
		CHECK_ASSERT(GetSolidDrawMode() != GL_TRIANGLES || indexes_.size() % 3 == 0, __FILE__, __LINE__);
		pVBuffer_ = PGLES2VertexBuffer(new GLES2VertexBuffer(sizeof(VertexData) * vertexsData_.size(), &vertexsData_[0], usage_));

		if(!indexes_.empty())
		{
			pIBuffer_ = PGLES2IndexBuffer(new GLES2IndexBuffer(sizeof(IndexType) * indexes_.size(), &indexes_[0], usage_));
		}

		CHECK_GL_STATUS(__FILE__, __LINE__);
	}

	void GLES2Mesh::ReleaseResources()
	{
		pVBuffer_ = nullptr;
		pIBuffer_ = nullptr;
	}
}