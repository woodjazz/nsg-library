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
	TextMesh::TextMesh(const char* filename, int fontSize, GLenum usage)
	: Mesh(usage),
	pProgram_(new Program(vShader, fShader)),
	screenWidth_(0),
	screenHeight_(0),
	fontSize_(fontSize),
	hAlignment_(LEFT_ALIGNMENT),
	vAlignment_(BOTTOM_ALIGNMENT),
	alignmentOffsetX_(0),
	alignmentOffsetY_(0)
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

	void TextMesh::AllocateResources()
	{
		CHECK_GL_STATUS(__FILE__, __LINE__);

		CHECK_ASSERT(pVBuffer_ == nullptr, __FILE__, __LINE__);
		CHECK_ASSERT(pIBuffer_ == nullptr, __FILE__, __LINE__);

		CHECK_ASSERT(!vertexsData_.empty(), __FILE__, __LINE__);

        VertexsData tmp(vertexsData_);

        Move(tmp, alignmentOffsetX_, alignmentOffsetY_);
		
		CHECK_ASSERT(GetSolidDrawMode() != GL_TRIANGLES || indexes_.size() % 3 == 0, __FILE__, __LINE__);
		
		pVBuffer_ = PVertexBuffer(new VertexBuffer(sizeof(VertexData) * tmp.size(), &tmp[0], usage_));
        
        CHECK_GL_STATUS(__FILE__, __LINE__);
	}

	void TextMesh::ReleaseResources()
	{
        pVBuffer_ = nullptr;
	}

	GLfloat TextMesh::GetWidthForCharacterPosition(unsigned int charPos) const
	{
		return pAtlas_->GetWidthForCharacterPosition(text_.c_str(), charPos);
	}

	unsigned int TextMesh::GetCharacterPositionForWidth(float width) const
	{
		return pAtlas_->GetCharacterPositionForWidth(text_.c_str(), width);
	}

	void TextMesh::SetTextHorizontalAlignment(HorizontalAlignment align)
	{
		if(align != hAlignment_)
		{
			if(align == CENTER_ALIGNMENT)
				alignmentOffsetX_ = -screenWidth_/2;
			else if(align == RIGHT_ALIGNMENT)
				alignmentOffsetX_ = -screenWidth_;
            else
                alignmentOffsetX_ = 0;

			hAlignment_ = align;

            Invalidate();
		}
	}

	void TextMesh::SetTextVerticalAlignment(VerticalAlignment align)
	{
		if(align != vAlignment_)
		{
			if(align == MIDDLE_ALIGNMENT)
				alignmentOffsetY_ = -screenHeight_/2;
			else if(align == TOP_ALIGNMENT)
				alignmentOffsetY_ = -screenHeight_;
            else
                alignmentOffsetY_ = 0;

			vAlignment_ = align;

            Invalidate();
		}
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

	void TextMesh::SetText(const std::string& text) 
	{
		if(text_ != text)
		{
			if(pAtlas_->SetTextMesh(text, vertexsData_, screenWidth_, screenHeight_))
            {
			    text_ = text;

                Invalidate();
            }
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
