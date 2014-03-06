#include "GLES2Texture.h"
#include "soil/SOIL.h"
#include "Log.h"
#include "App.h"
#if NACL
#include "AppNaCl.h"
#endif
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <assert.h>

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
	GLES2Texture::GLES2Texture(const char* filename, bool createFontAtlas, int fontSize) 
	: texture_(0), 
	pResource_(new Resource(filename)), 
	createFontAtlas_(createFontAtlas),
	atlasWidth_(0),
	atlasHeight_(0),
	fontSize_(fontSize)
	{
		Initialize();
	}

	GLES2Texture::~GLES2Texture()
	{
		glDeleteTextures(1, &texture_);
	}

	bool GLES2Texture::IsReady()
	{
		if(pResource_ != nullptr && pResource_->IsReady())
		{
			if(createFontAtlas_)
			{
				CreateTextureAtlas();
			}
			else
			{
				int img_width = 0;
				int img_height = 0;
				unsigned char* img = SOIL_load_image_from_memory(pResource_->GetData(), pResource_->GetBytes(), &img_width, &img_height, nullptr, 0);
				glGenTextures(1, &texture_);
				glBindTexture(GL_TEXTURE_2D, texture_);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D,
					0,
					GL_RGB,
					img_width,
					img_height,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					img);
				SOIL_free_image_data(img);
			}

			pResource_ = nullptr;
		}

		return pResource_ == nullptr;
	}

	#define MAXWIDTH 1024
	void GLES2Texture::CreateTextureAtlas()
	{
		FT_Face face;

		FT_New_Memory_Face(s_ft, pResource_->GetData(), pResource_->GetBytes(), 0, &face);
		FT_Set_Pixel_Sizes(face, 0, fontSize_);

		FT_GlyphSlot g = face->glyph;
		
		int roww = 0;
		int rowh = 0;
		 
		for(int i = 32; i < 128; i++) 
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

		glGenTextures(1, &texture_);
		GLint unpackAlign = 0;
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpackAlign);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        /* Clamping to edges is important to prevent artifacts when scaling */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 

		//if(fontSize_ > 20)
		{
	        /* Linear filtering usually looks best for text */
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
/*		else
		{
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}        */

		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, atlasWidth_, atlasHeight_, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
		std::vector<GLubyte> emptyData(atlasWidth_ * atlasHeight_, 0);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, atlasWidth_, atlasHeight_, GL_ALPHA, GL_UNSIGNED_BYTE, &emptyData[0]);		

		int ox = 0;
        int oy = 0;
        rowh = 0;

        charInfo_.resize(128) ;

		for(int i = 32; i < 128; i++) 
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


	BindTexture::BindTexture(GLES2Texture& obj)
	: obj_(obj)
	{
		obj.Bind();
	}

	BindTexture::~BindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}