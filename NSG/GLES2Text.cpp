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
#include "Check.h"
#include "App.h"
#include "GLES2Camera.h"
#include <algorithm>
#include <vector>
#include <map>

struct Point 
{
	GLfloat x;
	GLfloat y;
	GLfloat s;
	GLfloat t;
};

#define STRINGIFY(S) #S

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
	uniform sampler2D u_texture;
	struct Material
	{
		vec4 diffuse;
	};

	uniform Material u_material;

	void main()
	{
		gl_FragColor = vec4(u_material.diffuse.w, u_material.diffuse.w, u_material.diffuse.w, texture2D(u_texture, v_texcoord).a) * vec4(u_material.diffuse.x, u_material.diffuse.y, u_material.diffuse.z, 1);
	}
);

namespace NSG
{
	typedef std::pair<std::string, int> Key;
	typedef std::map<Key, PGLES2FontAtlasTexture> Atlas;
	Atlas fontAtlas;

	GLES2Text::GLES2Text(const char* filename, int fontSize, GLenum usage)
	: GLES2Mesh(usage),
	pApp_(App::GetPtrInstance()),
	pProgram_(new GLES2Program(vShader, fShader)),
	screenWidth_(0),
	screenHeight_(0),
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
    		pAtlas_ = PGLES2FontAtlasTexture(new GLES2FontAtlasTexture(filename, fontSize));
			fontAtlas.insert(Atlas::value_type(k, pAtlas_));
    	}
	}

	GLES2Text::~GLES2Text() 
	{
	}

	void GLES2Text::ReleaseAtlasCollection()
	{
		fontAtlas.clear();
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

        const GLES2FontAtlasTexture::CharsInfo& charInfo = pAtlas_->GetCharInfo();

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

        const GLES2FontAtlasTexture::CharsInfo& charInfo = pAtlas_->GetCharInfo();

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
        if(!pAtlas_->IsReady())
            return;

		auto viewSize = pApp_->GetViewSize();

		if(lastText_ != text || viewSize.first != width_ || viewSize.second != height_)
		{
		    vertexsData_.clear();

			width_ = viewSize.first;
			height_ = viewSize.second;

			if(width_ > 0 && height_ > 0)
			{
				float x = 0;
				float y = 0;

				float sx = 2.0f/width_;
			    float sy = 2.0f/height_;    

		        vertexsData_.resize(6 * text.size());

	            screenHeight_ = 0;

				const GLES2FontAtlasTexture::CharsInfo& charInfo = pAtlas_->GetCharInfo();
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

					VertexData data;
					data.normal_ = Vertex3(0, 0, 1); // always facing forward
					
					data.position_ = Vertex3(x2, -y2, 0);
					data.uv_ = Vertex2(charInfo[idx].tx, charInfo[idx].ty);
					vertexsData_.push_back(data);

					data.position_ = Vertex3(x2 + w, -y2, 0);
					data.uv_ = Vertex2(charInfo[idx].tx + charInfo[idx].bw / atlasWidth, charInfo[idx].ty);
					vertexsData_.push_back(data);

					data.position_ = Vertex3(x2, -y2 - h, 0);
					data.uv_ = Vertex2(charInfo[idx].tx, charInfo[idx].ty + charInfo[idx].bh / atlasHeight);
					vertexsData_.push_back(data);

					data.position_ = Vertex3(x2 + w, -y2, 0);
					data.uv_ = Vertex2(charInfo[idx].tx + charInfo[idx].bw / atlasWidth, charInfo[idx].ty);
					vertexsData_.push_back(data);

					data.position_ = Vertex3(x2, -y2 - h, 0);
					data.uv_ = Vertex2(charInfo[idx].tx, charInfo[idx].ty + charInfo[idx].bh / atlasHeight);
					vertexsData_.push_back(data);

					data.position_ = Vertex3(x2 + w, -y2 - h, 0);
					data.uv_ = Vertex2(charInfo[idx].tx + charInfo[idx].bw / atlasWidth, charInfo[idx].ty + charInfo[idx].bh / atlasHeight);
					vertexsData_.push_back(data);

	                screenHeight_ = std::max(screenHeight_, h);
				}

	            screenWidth_ = x;

				lastText_ = text;
			}
            
            Redo();
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
