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
	: usage_(usage)
	{
	}

	GLES2Mesh::~GLES2Mesh() 
	{
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

	void GLES2Mesh::Render(GLenum mode, GLuint position_loc, GLuint texcoord_loc, GLuint normal_loc)
	{
        if(!pVBuffer_)
            return;

		BindBuffer bindVBuffer(*pVBuffer_);

	    CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);

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

	    CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);

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

		CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);
	}
}