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

static FT_Library s_ft;
static bool s_initialized = false;

static void Initialize()
{
	if(!s_initialized)
	{
		if(FT_Init_FreeType(&s_ft)) 
		{
			TRACE_LOG("Could not init freetype library.");
		}
		else
		{
			s_initialized = true;
		}
	}
}


const char s_fragShaderSource[] = {
#include "shaders/gles2TextFragmentShader.h"
};

const char s_vertexShaderSource[] = {
#include "shaders/gles2TextVertexShader.h"
};

namespace NSG
{
	GLES2Text::GLES2Text(const char* filename, int fontSize, GLenum usage)
	: fontSize_(fontSize),
	pResource_(new Resource(filename)),
	atlasWidth_(0),
	atlasHeight_(0),
	texture_(0),
	pProgram_(new GLES2Program(s_vertexShaderSource, s_fragShaderSource)),
	texture_loc_(pProgram_->GetUniformLocation("u_texture")),
	position_loc_(pProgram_->GetAttributeLocation("a_position")),
	color_loc_(pProgram_->GetUniformLocation("u_color")),
	mvp_loc_(pProgram_->GetUniformLocation("u_mvp")),
	screenWidth_(0),
	screenHeight_(0),
	usage_(usage),
	width_(0),
	height_(0)
    {
		memset(charInfo_, 0, sizeof(charInfo_));

		Initialize();

	}

	GLES2Text::~GLES2Text() 
	{
		glDeleteTextures(1, &texture_);
	}

	#define MAXWIDTH 1024

	void GLES2Text::CreateTextureAtlas()
	{
		FT_New_Memory_Face(s_ft, pResource_->GetData(), pResource_->GetBytes(), 0, &face_);
		FT_Set_Pixel_Sizes(face_, 0, fontSize_);

		FT_GlyphSlot g = face_->glyph;
		
		int roww = 0;
		int rowh = 0;
		 
		for(int i = 32; i < 128; i++) 
		{
			if(FT_Load_Char(face_, i, FT_LOAD_RENDER)) 
			{
				TRACE_LOG("Loading character " << (char)i << " failed!");
				continue;
			}
			
			if (roww + g->bitmap.width + 1 >= MAXWIDTH) 
			{
                atlasWidth_ = std::max(atlasWidth_, roww);
                atlasHeight_ += rowh;
                roww = 0;
                rowh = 0;
            }

            roww += g->bitmap.width + 1;
            rowh = std::max(rowh, g->bitmap.rows);		
        }

		atlasWidth_ = std::max(atlasWidth_, roww);
        atlasHeight_ += rowh;        

		//glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &texture_);
		GLint unpackAlign = 0;
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpackAlign);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        /* Clamping to edges is important to prevent artifacts when scaling */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 
        /* Linear filtering usually looks best for text */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, atlasWidth_, atlasHeight_, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

		int ox = 0;
        int oy = 0;
        rowh = 0; 
		for(int i = 32; i < 128; i++) 
		{
			if(FT_Load_Char(face_, i, FT_LOAD_RENDER))
			{
				TRACE_LOG("Loading character " << (char)i << " failed!");
				continue;
			}

			if(ox + g->bitmap.width + 1 >= MAXWIDTH) 
			{
                oy += rowh;
                rowh = 0;
                ox = 0;
            }			

			glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			charInfo_[i].ax = g->advance.x >> 6;
			charInfo_[i].ay = g->advance.y >> 6;
			charInfo_[i].bw = g->bitmap.width;
			charInfo_[i].bh = g->bitmap.rows;
			charInfo_[i].bl = g->bitmap_left;
			charInfo_[i].bt = g->bitmap_top;
			charInfo_[i].tx = (float)ox / atlasWidth_;			
			charInfo_[i].ty = (float)oy / atlasHeight_;	

			ox += g->bitmap.width + 1;
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlign);	
	}

	bool GLES2Text::IsReady()
	{
		if(pResource_ != nullptr && pResource_->IsReady())
		{
			CreateTextureAtlas();
			pResource_ = nullptr;
		}

		return pResource_ == nullptr;
	}
	void GLES2Text::Render(PNode pNode, Color color, const std::string& text) 
	{
		Render(pNode.get(), color, text);
	}

	void GLES2Text::Render(Node* pNode, Color color, const std::string& text) 
	{
		if(!IsReady() || text.empty())
			return;

		auto viewSize = App::GetPtrInstance()->GetViewSize();

		if(lastText_ != text || viewSize.first != width_ || viewSize.second != height_)
		{
			width_ = viewSize.first;
			height_ = viewSize.second;
			lastText_.clear();
			pVBuffer_ = nullptr;
		}

		if(!pVBuffer_ && width_ > 0 && height_ > 0)
		{
			float x = 0;
			float y = 0;

			float sx = 2.0/width_;
		    float sy = 2.0/height_;    

	        size_t length = 6 * text.size();

	        coords_.clear();

	        coords_.resize(length);

            screenHeight_ = 0;

			int c = 0;

			for(const char *p = text.c_str(); *p; p++) 
			{ 
				float x2 =  x + charInfo_[*p].bl * sx;
				float y2 = -y - charInfo_[*p].bt * sy;
				float w = charInfo_[*p].bw * sx;
				float h = charInfo_[*p].bh * sy;

				/* Advance the cursor to the start of the next character */
				x += charInfo_[*p].ax * sx;
				y += charInfo_[*p].ay * sy;

				/* Skip glyphs that have no pixels */
				if(!w || !h)
					continue;

				int idx = (int)*p;

		        Point point1 = {x2, -y2, charInfo_[*p].tx, charInfo_[*p].ty};
				Point point2 = {x2 + w, -y2, charInfo_[*p].tx + charInfo_[*p].bw / atlasWidth_, charInfo_[*p].ty};
				Point point3 = {x2, -y2 - h, charInfo_[*p].tx, charInfo_[*p].ty + charInfo_[*p].bh / atlasHeight_};
				Point point4 = {x2 + w, -y2, charInfo_[*p].tx + charInfo_[*p].bw / atlasWidth_, charInfo_[*p].ty};
				Point point5 = {x2, -y2 - h, charInfo_[*p].tx, charInfo_[*p].ty + charInfo_[*p].bh / atlasHeight_};
				Point point6 = {x2 + w, -y2 - h, charInfo_[*p].tx + charInfo_[*p].bw / atlasWidth_, charInfo_[*p].ty + charInfo_[*p].bh / atlasHeight_};

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

		if(pVBuffer_ != nullptr)
		{
            //bool isDepthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
            //glEnable(GL_DEPTH_TEST);

            assert(glGetError() == GL_NO_ERROR);

			GLboolean isBlendEnabled = glIsEnabled(GL_BLEND);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

			UseProgram useProgram(*pProgram_);

			Matrix4 mvp = GLES2Camera::GetModelViewProjection(pNode);
			glUniformMatrix4fv(mvp_loc_, 1, GL_FALSE, glm::value_ptr(mvp));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_);
			glUniform1i(texture_loc_, 0);
			glUniform4fv(color_loc_, 1, &color[0]);

			BindBuffer bindVBuffer(*pVBuffer_);

			glEnableVertexAttribArray(position_loc_);
			glVertexAttribPointer(position_loc_, 4, GL_FLOAT, GL_FALSE, 0, 0);

			glDrawArrays(GL_TRIANGLES, 0, coords_.size());

			glBindTexture(GL_TEXTURE_2D, 0);

			if(!isBlendEnabled)
				glDisable(GL_BLEND);

            //if(!isDepthTestEnabled)
            //    glDisable(GL_DEPTH_TEST);
            
            assert(glGetError() == GL_NO_ERROR);
		}
	}	
}
