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
#include "TextMesh.h"
#include "FontAtlasTextureManager.h"
#include "Program.h"
#include "FontAtlasTexture.h"
#include "BufferManager.h"
#include "Graphics.h"
#include "Context.h"

static const char* vShader = STRINGIFY(
	uniform mat4 u_mvp;
	attribute vec4 a_position;
	attribute vec2 a_texcoord;
	varying vec2 v_texcoord;
	
	void main() 
	{
		gl_Position = u_mvp * a_position;
		v_texcoord = a_texcoord;
	}
);

static const char* fShader = STRINGIFY(
	varying vec2 v_texcoord;
	uniform sampler2D u_texture0;
	struct Material
	{
		vec4 diffuse;
	};

	uniform Material u_material;

	void main()
	{
		gl_FragColor = vec4(u_material.diffuse.w, u_material.diffuse.w, u_material.diffuse.w, texture2D(u_texture0, v_texcoord).a) * vec4(u_material.diffuse.x, u_material.diffuse.y, u_material.diffuse.z, 1);
	}
);

namespace NSG
{
	TextMesh::TextMesh(size_t maxLength, const char* filename, int fontSize, GLenum usage)
	: Mesh(usage),
	pProgram_(new Program(vShader, fShader)),
	screenWidth_(0),
	screenHeight_(0),
	fontSize_(fontSize),
	hAlignment_(LEFT_ALIGNMENT),
	vAlignment_(BOTTOM_ALIGNMENT),
	alignmentOffsetX_(0),
	alignmentOffsetY_(0),
	maxLength_(maxLength)
    {
    	pAtlas_ = FontAtlasTextureManager::this_->GetAtlas(FontAtlasTextureManager::Key(filename, fontSize));
	}

	TextMesh::~TextMesh() 
	{
	}

	bool TextMesh::IsValid()
	{
        return pProgram_->IsReady() && pAtlas_->IsReady() && !vertexsData_.empty();
	}

	void TextMesh::UpdateBuffers()
	{
		if (vertexsData_.empty())
			return;

		vertexDataCopy_ = vertexsData_;
		Move(vertexsData_, alignmentOffsetX_, alignmentOffsetY_);

		CHECK_GL_STATUS(__FILE__, __LINE__);

		//CHECK_ASSERT(!vertexsData_.empty(), __FILE__, __LINE__);
		CHECK_ASSERT(!indexes_.empty(), __FILE__, __LINE__);
		CHECK_ASSERT(GetSolidDrawMode() != GL_TRIANGLES || indexes_.size() % 3 == 0, __FILE__, __LINE__);

		CHECK_ASSERT(pVBuffer_ && bufferVertexData_->data_, __FILE__, __LINE__);
		{
			CHECK_ASSERT(Context::this_->bufferManager_->IsValidBufferPtr(pVBuffer_), __FILE__, __LINE__);
			CHECK_ASSERT(bufferVertexData_, __FILE__, __LINE__);
			pVBuffer_->UpdateData(*bufferVertexData_, vertexsData_);
		}
		
		CHECK_ASSERT(pIBuffer_ && bufferIndexData_->data_, __FILE__, __LINE__);
		{
			CHECK_ASSERT(Context::this_->bufferManager_->IsValidBufferPtr(pIBuffer_), __FILE__, __LINE__);

			CHECK_ASSERT(bufferIndexData_, __FILE__, __LINE__);

			GLintptr indexBase = bufferVertexData_->offset_ / sizeof(VertexData);

			pIBuffer_->UpdateData(*bufferIndexData_, indexes_, indexBase);
		}

		CHECK_GL_STATUS(__FILE__, __LINE__);
		
		vertexsData_ = vertexDataCopy_;
	}

	
	void TextMesh::AllocateResources()
	{
		vertexDataCopy_ = vertexsData_;
		Move(vertexsData_, alignmentOffsetX_, alignmentOffsetY_);

		CHECK_GL_STATUS(__FILE__, __LINE__);

		CHECK_ASSERT(!vertexsData_.empty(), __FILE__, __LINE__);
		CHECK_ASSERT(!indexes_.empty(), __FILE__, __LINE__);
		CHECK_ASSERT(GetSolidDrawMode() != GL_TRIANGLES || indexes_.size() % 3 == 0, __FILE__, __LINE__);

		CHECK_ASSERT(!pVBuffer_, __FILE__, __LINE__);
		{
			const size_t VERTEX_PER_CHAR = 6;
			
			const GLsizeiptr MAX_BYTES_VERTEX_BUFFER = VERTEX_PER_CHAR * maxLength_ * sizeof(VertexData);
			GLsizeiptr bytesNeeded = sizeof(VertexData) * vertexsData_.size();
			CHECK_ASSERT(bytesNeeded <= MAX_BYTES_VERTEX_BUFFER, __FILE__, __LINE__);
			pVBuffer_ = Context::this_->bufferManager_->GetDynamicVertexBuffer(MAX_BYTES_VERTEX_BUFFER, bytesNeeded, &vertexsData_[0]);
			//pVBuffer_ = Context::this_->bufferManager_->GetStaticVertexBuffer(MAX_BYTES_VERTEX_BUFFER, bytesNeeded, &vertexsData_[0]);
			bufferVertexData_ = pVBuffer_->GetLastAllocation();
			CHECK_ASSERT(bufferVertexData_->maxSize_ && bufferVertexData_->size_, __FILE__, __LINE__);
		}
		
		CHECK_ASSERT(!pIBuffer_, __FILE__, __LINE__);
		{
			const size_t INDEXES_PER_CHAR = 6;
			const GLsizeiptr MAX_BYTES_INDEX_BUFFER = INDEXES_PER_CHAR * maxLength_ * sizeof(IndexType);
			GLsizeiptr bytesNeeded = sizeof(IndexType) * indexes_.size();
			CHECK_ASSERT(bytesNeeded <= MAX_BYTES_INDEX_BUFFER, __FILE__, __LINE__);

			GLintptr indexBase = bufferVertexData_->offset_ / sizeof(VertexData);
			pIBuffer_ = Context::this_->bufferManager_->GetDynamicIndexBuffer(MAX_BYTES_INDEX_BUFFER, bytesNeeded, &indexes_[0], indexBase);
			//pIBuffer_ = Context::this_->bufferManager_->GetStaticIndexBuffer(MAX_BYTES_INDEX_BUFFER, bytesNeeded, &indexes_[0], indexBase);
			bufferIndexData_ = pIBuffer_->GetLastAllocation();
			CHECK_ASSERT(bufferIndexData_->maxSize_ && bufferIndexData_->size_, __FILE__, __LINE__);
		}

		CHECK_GL_STATUS(__FILE__, __LINE__);
		
		vertexsData_ = vertexDataCopy_;
	}
	
	void TextMesh::ReleaseResources()
	{
		Mesh::ReleaseResources();

		text_.clear(); // Force SetText (when window's resized)
	}
	
	GLfloat TextMesh::GetWidthForCharacterPosition(unsigned int charPos) const
	{
		return pAtlas_->GetWidthForCharacterPosition(text_.c_str(), charPos);
	}

	unsigned int TextMesh::GetCharacterPositionForWidth(float width) const
	{
		return pAtlas_->GetCharacterPositionForWidth(text_.c_str(), width);
	}

	void TextMesh::Move(VertexsData& obj, float offsetX, float offsetY)
	{
		auto it = obj.begin();
		while(it != obj.end())
		{
			VertexData& data = *it;
			data.position_.x += offsetX;
			data.position_.y += offsetY;
			++it;
		}
	}

	void TextMesh::SetText(const std::string& text, HorizontalAlignment hAlign, VerticalAlignment vAlign)
	{
		bool changed = false;
		std::string tmpText = text;
		if(tmpText.size() > maxLength_)
			tmpText.resize(maxLength_);

		if (text_ != tmpText)
		{
			if (pAtlas_->SetTextMesh(tmpText, vertexsData_, indexes_, screenWidth_, screenHeight_))
			{
				text_ = tmpText;

				changed = true;
			}
		}
		
		if (changed || hAlign != hAlignment_ || vAlign != vAlignment_)
		{
			if (hAlign == CENTER_ALIGNMENT)
				alignmentOffsetX_ = -screenWidth_ / 2;
			else if (hAlign == RIGHT_ALIGNMENT)
				alignmentOffsetX_ = -screenWidth_;
			else
				alignmentOffsetX_ = 0;

			if (vAlign == MIDDLE_ALIGNMENT)
				alignmentOffsetY_ = 0;
			else if (vAlign == TOP_ALIGNMENT)
				alignmentOffsetY_ = -screenHeight_;
			else
				alignmentOffsetY_ = screenHeight_;

			hAlignment_ = hAlign;
			vAlignment_ = vAlign;

			changed = true;
		}

		if (changed && IsReady())
		{
			UpdateBuffers();
		}
	}	

	GLenum TextMesh::GetWireFrameDrawMode() const
	{
		return GL_LINE_LOOP;
	}

	GLenum TextMesh::GetSolidDrawMode() const
	{
		return GL_TRIANGLES;
	}
}