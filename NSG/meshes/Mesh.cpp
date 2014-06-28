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
#include "Check.h"
#include "App.h"
#include "Graphics.h"
#include "AppStatistics.h"
#include "Context.h"
#include "BufferManager.h"

namespace NSG
{
	const size_t MAX_VERTEXES_PER_MESH = 10000;
	const IndexType MAX_INDEXES_PER_MESH = ~IndexType(0);

	Mesh::Mesh(GLenum usage) 
	: pVBuffer_(nullptr),
	pIBuffer_(nullptr),
	usage_(usage),
	bufferVertexData_(nullptr),
	bufferIndexData_(nullptr)
	{
		// we do not want reallocation of these buffers
		vertexsData_.reserve(MAX_VERTEXES_PER_MESH);
		indexes_.reserve(MAX_INDEXES_PER_MESH);
	}

	Mesh::~Mesh() 
	{
		Context::this_->Remove(this);
	}


	void Mesh::Render(bool solid, GLuint position_loc, GLuint texcoord_loc, GLuint normal_loc, GLuint color_loc)
	{
	   	if(IsReady())
	   	{
	   		CHECK_ASSERT(vertexsData_.size() < MAX_VERTEXES_PER_MESH, __FILE__, __LINE__); // we do not want reallocation of these buffers
			CHECK_ASSERT(indexes_.size() < MAX_INDEXES_PER_MESH, __FILE__, __LINE__); // we do not want reallocation of these buffers

			CHECK_GL_STATUS(__FILE__, __LINE__);

			CHECK_ASSERT(pVBuffer_, __FILE__, __LINE__);
			
			SetVertexBuffer(pVBuffer_);

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
				SetIndexBuffer(pIBuffer_);

				const GLvoid* offset = reinterpret_cast<const GLvoid*>(bufferIndexData_->offset_);

				glDrawElements(mode, indexes_.size(), GL_UNSIGNED_SHORT, offset);

				CHECK_GL_STATUS(__FILE__, __LINE__);

				if (AppStatistics::this_)
				{
					CHECK_ASSERT(GetSolidDrawMode() == GL_TRIANGLES && indexes_.size() % 3 == 0, __FILE__, __LINE__);
					AppStatistics::this_->NewTriangles(indexes_.size() / 3);
				}
		    }
		    else
		    {
				GLint first = bufferVertexData_->offset_ / sizeof(VertexData);

				glDrawArrays(mode, first, vertexsData_.size());

				CHECK_GL_STATUS(__FILE__, __LINE__);

				if (AppStatistics::this_ && solid)
				{
					CHECK_ASSERT(GetSolidDrawMode() != GL_TRIANGLES || vertexsData_.size() % 3 == 0, __FILE__, __LINE__);
					AppStatistics::this_->NewTriangles(vertexsData_.size() / 3);
				}
		    }

            if(AppStatistics::this_)
		        AppStatistics::this_->NewDrawCall();

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

	bool Mesh::IsValid()
	{
		if(resource_)
			return resource_->IsLoaded();
		else
			return !vertexsData_.empty();
	}

	void Mesh::AllocateResources()
	{
		CHECK_GL_STATUS(__FILE__, __LINE__);

		CHECK_ASSERT(pVBuffer_ == nullptr, __FILE__, __LINE__);
		CHECK_ASSERT(pIBuffer_ == nullptr, __FILE__, __LINE__);

		CHECK_ASSERT(!vertexsData_.empty(), __FILE__, __LINE__);
		CHECK_ASSERT(GetSolidDrawMode() != GL_TRIANGLES || indexes_.size() % 3 == 0, __FILE__, __LINE__);

		GLsizeiptr bytesNeeded = sizeof(VertexData) * vertexsData_.size();
		pVBuffer_ = Context::this_->bufferManager_->GetStaticVertexBuffer(bytesNeeded, bytesNeeded, &vertexsData_[0]);
		bufferVertexData_ = pVBuffer_->GetLastAllocation();
		CHECK_ASSERT(bufferVertexData_->maxSize_ && bufferVertexData_->size_, __FILE__, __LINE__);
		
		if(!indexes_.empty())
		{
			GLintptr indexBase = bufferVertexData_->offset_ / sizeof(VertexData);
			GLsizeiptr bytesNeeded = sizeof(IndexType) * indexes_.size();
			pIBuffer_ = Context::this_->bufferManager_->GetStaticIndexBuffer(bytesNeeded, bytesNeeded, &indexes_[0], indexBase);
			bufferIndexData_ = pIBuffer_->GetLastAllocation();
			CHECK_ASSERT(bufferIndexData_->maxSize_ && bufferIndexData_->size_, __FILE__, __LINE__);
		}

		CHECK_GL_STATUS(__FILE__, __LINE__);
	}

	void Mesh::ReleaseResources()
	{
		pVBuffer_ = nullptr;
		pIBuffer_ = nullptr;
		bufferVertexData_ = nullptr;
		bufferIndexData_ = nullptr;
	}
}