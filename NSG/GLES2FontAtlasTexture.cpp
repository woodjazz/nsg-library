#include "GLES2FontAtlasTexture.h"
#include "GLES2PlaneMesh.h"
#include "SOIL.h"
#include "Log.h"
#include "Check.h"
#include "App.h"
#if NACL
#include "AppNaCl.h"
#endif
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <assert.h>

static const char* vShader = STRINGIFY(
	attribute vec4 a_position;
	attribute vec2 a_texcoord;
	varying vec2 v_texcoord;

	void main()
	{
		gl_Position = a_position;
		v_texcoord = vec2(a_texcoord.x, 1.0 - a_texcoord.y);
	}
);

static const char* fShader = STRINGIFY(
	uniform sampler2D u_texture0;
	varying vec2 v_texcoord;
	void main()
	{
        gl_FragColor = vec4(1, 1, 1, texture2D(u_texture0, v_texcoord).a);
	}
);

static const int MAXCHARS = 256;
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

namespace NSG
{
	GLES2FontAtlasTexture::GLES2FontAtlasTexture(const char* filename, int fontSize) 
	: GLES2Texture(filename), 
	atlasWidth_(0),
	atlasHeight_(0),
	fontSize_(fontSize)
	{
		pMaterial_->SetBlendMode(ALPHA);
        PGLES2Program pProgram(new GLES2Program(vShader, fShader));
        pMaterial_->SetProgram(pProgram);

		if(fontSize_ > 0)
		{
			Initialize();
		}
	}

	GLES2FontAtlasTexture::~GLES2FontAtlasTexture()
	{
	}

	bool GLES2FontAtlasTexture::IsReady()
	{
		if(!isLoaded_  && pResource_->IsReady())
		{
			CreateTextureAtlas();

			pResource_ = nullptr;

			isLoaded_ = true;
		}

		return isLoaded_;
	}

    #define MAXWIDTH 1024
	void GLES2FontAtlasTexture::CreateTextureAtlas()
	{
		FT_Face face;

		FT_New_Memory_Face(s_ft, pResource_->GetData(), pResource_->GetBytes(), 0, &face);
		FT_Set_Pixel_Sizes(face, 0, fontSize_);

		FT_GlyphSlot g = face->glyph;
		
		int roww = 0;
		int rowh = 0;
		 
		for(int i = 32; i < MAXCHARS; i++) 
		{
			if(FT_Load_Char(face, i, FT_LOAD_RENDER)) 
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
		std::vector<GLubyte> emptyData(atlasWidth_ * atlasHeight_, 0);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, atlasWidth_, atlasHeight_, GL_ALPHA, GL_UNSIGNED_BYTE, &emptyData[0]);		

		int ox = 0;
        int oy = 0;
        rowh = 0;

        charInfo_.resize(MAXCHARS) ;

		for(int i = 32; i < MAXCHARS; i++) 
		{
			if(FT_Load_Char(face, i, FT_LOAD_RENDER))
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

            rowh = std::max(rowh, g->bitmap.rows);
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlign);	
	}
}