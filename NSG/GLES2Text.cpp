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
#include "Log.h"
#include "App.h"
#include "GLES2Camera.h"
#include <algorithm>
#include <vector>
#include <map>

#define STRINGIFY(S) #S

static const char* vShader = STRINGIFY(
	uniform mat4 u_mvp;
	attribute vec4 a_position;
	varying vec2 v_texcoord;
	
	void main() 
	{
		gl_Position = u_mvp * vec4(a_position.xy, 0, 1);
		v_texcoord = a_position.zw;
	}
);

static const char* fShader = STRINGIFY(
	varying vec2 v_texcoord;
	uniform sampler2D u_texture;
	uniform vec4 u_color;
	void main()
	{
		gl_FragColor = vec4(1, 1, 1, texture2D(u_texture, v_texcoord).a) * u_color;
	}
);

#define STRINGIFY(S) #S

static const char* vAtlasShader = STRINGIFY(
	attribute vec4 a_position;
	attribute vec2 a_texcoord;
	varying vec2 v_texcoord;
	void main()
	{
		gl_Position = a_position;
		v_texcoord = a_texcoord;
	}
);

static const char* fAtlasShader = STRINGIFY(
	uniform sampler2D u_texture;
	varying vec2 v_texcoord;
	void main()
	{
		gl_FragColor = vec4(1, 1, 1, texture2D(u_texture, v_texcoord).a);
	}
);


namespace NSG
{
	typedef std::pair<std::string, int> Key;
	typedef std::map<Key, PGLES2Texture> Atlas;
	Atlas fontAtlas;

	GLES2Text::GLES2Text(const char* filename, int fontSize, GLenum usage)
	: pProgram_(new GLES2Program(vShader, fShader)),
	texture_loc_(pProgram_->GetUniformLocation("u_texture")),
	position_loc_(pProgram_->GetAttributeLocation("a_position")),
	color_loc_(pProgram_->GetUniformLocation("u_color")),
	mvp_loc_(pProgram_->GetUniformLocation("u_mvp")),
	screenWidth_(0),
	screenHeight_(0),
	usage_(usage),
	width_(0),
	height_(0),
	fontSize_(fontSize)
    {
    	Key k(filename, fontSize);
    	auto it = fontAtlas.find(k);
    	if(it != fontAtlas.end())
    	{
    		pAtlas_ = it->second;
    	}
    	else
    	{
    		pAtlas_ = PGLES2Texture(new GLES2Texture(filename, true, fontSize));
			fontAtlas.insert(Atlas::value_type(k, pAtlas_));
    	}

		PGLES2Program pProgram(new GLES2Program(vAtlasShader, fAtlasShader));
		PGLES2Material pMaterial = PGLES2Material(new GLES2Material ());
        pMaterial->SetProgram(pProgram);
		pMaterial->SetMainTexture(pAtlas_);
		pMesh_ = PGLES2PlaneMesh(new GLES2PlaneMesh(2, 2, 2, 2, pMaterial, GL_STATIC_DRAW));

	}

	GLES2Text::~GLES2Text() 
	{
	}

	void GLES2Text::ReleaseAtlasCollection()
	{
		fontAtlas.clear();
	}

	void GLES2Text::ShowAtlas()
	{
		GLboolean isBlendEnabled = glIsEnabled(GL_BLEND);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

		pMesh_->Render(nullptr);		

		if(!isBlendEnabled)
			glDisable(GL_BLEND);
	}

	void GLES2Text::Render(PNode pNode, Color color) 
	{
		Render(pNode.get(), color);
	}

	void GLES2Text::Render(Node* pNode, Color color) 
	{
		SetText(lastText_);

		if(pVBuffer_ != nullptr)
		{
            assert(glGetError() == GL_NO_ERROR);

			GLboolean isBlendEnabled = glIsEnabled(GL_BLEND);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

			UseProgram useProgram(*pProgram_);

			Matrix4 mvp = GLES2Camera::GetModelViewProjection(pNode);
			glUniformMatrix4fv(mvp_loc_, 1, GL_FALSE, glm::value_ptr(mvp));

			glActiveTexture(GL_TEXTURE0);

			BindTexture bindTexture(*pAtlas_);
			glUniform1i(texture_loc_, 0);
			glUniform4fv(color_loc_, 1, &color[0]);

			BindBuffer bindVBuffer(*pVBuffer_);

			glEnableVertexAttribArray(position_loc_);
			glVertexAttribPointer(position_loc_, 4, GL_FLOAT, GL_FALSE, 0, 0);

			glDrawArrays(GL_TRIANGLES, 0, coords_.size());

			if(!isBlendEnabled)
				glDisable(GL_BLEND);

            assert(glGetError() == GL_NO_ERROR);
		}
	}	

    static int GetIndex(char ch)
    {
        int idx = ch;
        if(idx < 0)
            idx += 256;
        return idx;
    }

	GLfloat GLES2Text::GetWidthForCharacterPosition(unsigned int charPos) const
	{
		GLfloat pos = 0;

		const char *p = lastText_.c_str();

        float sx = 2.0f/width_;

        const GLES2Texture::CharsInfo& charInfo = pAtlas_->GetCharInfo();

		for(unsigned int i=0; i<charPos && *p; i++) 
		{ 
            int idx = GetIndex(*p);

			pos += charInfo[idx].ax * sx;
			++p;
		}

		return pos;
	}

	unsigned int GLES2Text::GetCharacterPositionForWidth(float width) const
	{
		unsigned int charPos = 0;

		GLfloat pos = 0;

		const char *p = lastText_.c_str();

        float sx = 2.0f/width_;

        const GLES2Texture::CharsInfo& charInfo = pAtlas_->GetCharInfo();

		while(*p) 
		{ 
			if(pos >= width)
				break;

            int idx = GetIndex(*p);

			pos += charInfo[idx].ax * sx;

			++p;

			++charPos;
		}

		return charPos;
	}

	void GLES2Text::SetText(const std::string& text) 
	{
		auto viewSize = App::GetPtrInstance()->GetViewSize();

		if(lastText_ != text || viewSize.first != width_ || viewSize.second != height_)
		{
			width_ = viewSize.first;
			height_ = viewSize.second;
			lastText_.clear();
			pVBuffer_ = nullptr;
			screenWidth_ = 0;
			screenHeight_ = 0;
		}

		if(!pAtlas_->IsReady() || text.empty())
			return;

		if(!pVBuffer_ && width_ > 0 && height_ > 0)
		{
			float x = 0;
			float y = 0;

			float sx = 2.0f/width_;
		    float sy = 2.0f/height_;    

	        size_t length = 6 * text.size();

	        coords_.clear();

	        coords_.resize(length);

            screenHeight_ = 0;

			int c = 0;

			const GLES2Texture::CharsInfo& charInfo = pAtlas_->GetCharInfo();
			int atlasWidth = pAtlas_->GetAtlasWidth();
			int atlasHeight = pAtlas_->GetAtlasHeight();

			for(const char *p = text.c_str(); *p; p++) 
			{ 
                int idx = GetIndex(*p);

				float x2 =  x + charInfo[idx].bl * sx;
				float y2 = -y - charInfo[idx].bt * sy;
				float w = charInfo[idx].bw * sx;
				float h = charInfo[idx].bh * sy;

				/* Advance the cursor to the start of the next character */
				x += charInfo[idx].ax * sx;
				y += charInfo[idx].ay * sy;

				/* Skip glyphs that have no pixels */
				if(!w || !h)
					continue;

		        Point point1 = {x2, -y2, charInfo[idx].tx, charInfo[idx].ty};
				Point point2 = {x2 + w, -y2, charInfo[idx].tx + charInfo[idx].bw / atlasWidth, charInfo[idx].ty};
				Point point3 = {x2, -y2 - h, charInfo[idx].tx, charInfo[idx].ty + charInfo[idx].bh / atlasHeight};
				Point point4 = {x2 + w, -y2, charInfo[idx].tx + charInfo[idx].bw / atlasWidth, charInfo[idx].ty};
				Point point5 = {x2, -y2 - h, charInfo[idx].tx, charInfo[idx].ty + charInfo[idx].bh / atlasHeight};
				Point point6 = {x2 + w, -y2 - h, charInfo[idx].tx + charInfo[idx].bw / atlasWidth, charInfo[idx].ty + charInfo[idx].bh / atlasHeight};

	            coords_[c++] = point1;
	            coords_[c++] = point2;
	            coords_[c++] = point3;
	            coords_[c++] = point4;
	            coords_[c++] = point5;
	            coords_[c++] = point6;

                screenHeight_ = std::max(screenHeight_, h);
			}

            screenWidth_ = x;

			pVBuffer_ = PGLES2VertexBuffer(new GLES2VertexBuffer(sizeof(Point) * coords_.size(), &coords_[0], usage_));

			lastText_ = text;
		}
	}	

}
