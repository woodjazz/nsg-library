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
#include "GLES2Text.h"
#include "FontAtlasTextureManager.h"
#include "GLES2Program.h"
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
	GLES2Text::GLES2Text(const char* filename, int fontSize, GLenum usage)
	: GLES2Mesh(usage),
	pProgram_(new GLES2Program(vShader, fShader)),
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

	GLES2Text::~GLES2Text() 
	{
	}

	bool GLES2Text::IsValid()
	{
        return pProgram_->IsReady() && pAtlas_->IsReady() && !vertexsData_.empty();
	}

	void GLES2Text::AllocateResources()
	{
		CHECK_GL_STATUS(__FILE__, __LINE__);

		CHECK_ASSERT(pVBuffer_ == nullptr, __FILE__, __LINE__);
		CHECK_ASSERT(pIBuffer_ == nullptr, __FILE__, __LINE__);

		CHECK_ASSERT(!vertexsData_.empty(), __FILE__, __LINE__);

		Move(alignmentOffsetX_, alignmentOffsetY_);
		
		CHECK_ASSERT(GetSolidDrawMode() != GL_TRIANGLES || indexes_.size() % 3 == 0, __FILE__, __LINE__);
		
		pVBuffer_ = PGLES2VertexBuffer(new GLES2VertexBuffer(sizeof(VertexData) * vertexsData_.size(), &vertexsData_[0], usage_));
        
        CHECK_GL_STATUS(__FILE__, __LINE__);
	}

	void GLES2Text::ReleaseResources()
	{
        pVBuffer_ = nullptr;
	}

	GLfloat GLES2Text::GetWidthForCharacterPosition(unsigned int charPos) const
	{
		return pAtlas_->GetWidthForCharacterPosition(text_.c_str(), charPos);
	}

	unsigned int GLES2Text::GetCharacterPositionForWidth(float width) const
	{
		return pAtlas_->GetCharacterPositionForWidth(text_.c_str(), width);
	}

	void GLES2Text::SetTextHorizontalAlignment(HorizontalAlignment align)
	{
		if(align != hAlignment_)
		{
			alignmentOffsetX_ = 0;

			if(hAlignment_ == CENTER_ALIGNMENT)
				alignmentOffsetX_ = screenWidth_/2;
			else if(hAlignment_ == RIGHT_ALIGNMENT)
				alignmentOffsetX_ = screenWidth_;

			if(align == CENTER_ALIGNMENT)
				alignmentOffsetX_ -= screenWidth_/2;
			else if(align == RIGHT_ALIGNMENT)
				alignmentOffsetX_ -= screenWidth_;

			hAlignment_ = align;
		}
	}

	void GLES2Text::SetTextVerticalAlignment(VerticalAlignment align)
	{
		if(align != vAlignment_)
		{
			alignmentOffsetY_ = 0;

			if(vAlignment_ == MIDDLE_ALIGNMENT)
				alignmentOffsetY_ = screenHeight_/2;
			else if(vAlignment_ == TOP_ALIGNMENT)
				alignmentOffsetY_ = screenHeight_;

			if(align == MIDDLE_ALIGNMENT)
				alignmentOffsetY_ -= screenHeight_/2;
			else if(align == TOP_ALIGNMENT)
				alignmentOffsetY_ -= screenHeight_;

			vAlignment_ = align;
		}
	}


	void GLES2Text::Move(float offsetX, float offsetY)
	{
		auto it = vertexsData_.begin();
		while(it != vertexsData_.end())
		{
			VertexData& data = *it;
			data.position_.x += offsetX;
			data.position_.y += offsetY;
			++it;
		}
	}

	void GLES2Text::SetText(const std::string& text) 
	{
		if(text_ != text)
		{
			if(pAtlas_->SetTextMesh(text, vertexsData_, screenWidth_, screenHeight_))
            {
			    text_ = text;

			    HorizontalAlignment hAlign = hAlignment_;
			    VerticalAlignment vAlign = vAlignment_;
			    hAlignment_ = LEFT_ALIGNMENT;
			    vAlignment_ = BOTTOM_ALIGNMENT;
			    SetTextHorizontalAlignment(hAlign);
			    SetTextVerticalAlignment(vAlign);

                Invalidate();
            }
		}
	}	

	GLenum GLES2Text::GetWireFrameDrawMode() const
	{
		return GL_LINE_LOOP;
	}

	GLenum GLES2Text::GetSolidDrawMode() const
	{
		return GL_TRIANGLES;
	}
}
