#include "GLES2FontAtlasTexture.h"
#include "GLES2PlaneMesh.h"
#include "SOIL.h"
#include "FreeType.h"
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

namespace NSG
{
    static FreeType freeTypeObj;

	GLES2FontAtlasTexture::GLES2FontAtlasTexture(const char* filename, int fontSize) 
	: GLES2Texture(filename), 
	atlasWidth_(0),
	atlasHeight_(0),
	filename_(filename),
	fontSize_(fontSize),
	viewWidth_(0),
	viewHeight_(0)
	{
        CHECK_GL_STATUS(__FILE__, __LINE__);

		pMaterial_->SetBlendMode(ALPHA);

        PGLES2Program pProgram(new GLES2Program(vShader, fShader));

        pMaterial_->SetProgram(pProgram);

		CHECK_GL_STATUS(__FILE__, __LINE__);
	}
	

	GLES2FontAtlasTexture::~GLES2FontAtlasTexture()
	{
	}

    bool GLES2FontAtlasTexture::IsValid()
    {
        return pResource_->IsLoaded();
    }

    void GLES2FontAtlasTexture::AllocateResources()
    {
        CreateTextureAtlas();
        GenerateMeshesForAllChars();
    }

    void GLES2FontAtlasTexture::ReleaseResources()
    {
    	charsMesh_.clear();
        charInfo_.clear();
        atlasWidth_ = atlasHeight_ = 0;
        glDeleteTextures(1, &texture_);
    }

    #define MAXWIDTH 1024
	void GLES2FontAtlasTexture::CreateTextureAtlas()
	{
		TRACE_LOG("GLES2FontAtlasTexture::CreateTextureAtlas: File=" << filename_ << " fontSize=" << fontSize_);

		FT_Face face;

		FT_New_Memory_Face(freeTypeObj.GetHandle(), (const FT_Byte*)pResource_->GetData(), pResource_->GetBytes(), 0, &face);
		FT_Set_Pixel_Sizes(face, 0, fontSize_);

		FT_GlyphSlot g = face->glyph;
		
		int roww = 0;
		int rowh = 0;
		 
		for(int i = 32; i < GLES2FontAtlasTexture::MAXCHARS; i++) 
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
 
        /* Linear filtering usually looks best for text */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, atlasWidth_, atlasHeight_, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
		std::vector<GLubyte> emptyData(atlasWidth_ * atlasHeight_, 0);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, atlasWidth_, atlasHeight_, GL_ALPHA, GL_UNSIGNED_BYTE, &emptyData[0]);		

		int ox = 0;
        int oy = 0;
        rowh = 0;

        charInfo_.resize(GLES2FontAtlasTexture::MAXCHARS) ;

		for(int i = 32; i < GLES2FontAtlasTexture::MAXCHARS; i++) 
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

		TRACE_LOG("GLES2FontAtlasTexture::CreateTextureAtlas done.");
	}

	void GLES2FontAtlasTexture::GenerateMeshesForAllChars()
	{
		TRACE_LOG("GLES2FontAtlasTexture::GenerateMeshesForAllChars...");

		auto viewSize = App::this_->GetViewSize();

		viewWidth_ = viewSize.first;
		viewHeight_ = viewSize.second;

		CHECK_ASSERT(viewWidth_ > 0 && viewHeight_ > 0, __FILE__, __LINE__);

		float sx = 2.0f/viewWidth_;
	    float sy = 2.0f/viewHeight_;   

		for(int idx = 32; idx < GLES2FontAtlasTexture::MAXCHARS; idx++) 
		{ 
			float w = charInfo_[idx].bw * sx;
			float h = charInfo_[idx].bh * sy;

			/* Skip glyphs that have no pixels */
			if(!w || !h)
				continue;

			VertexData data;
			CharMesh mesh;
            
			data.normal_ = Vertex3(0, 0, 1); // always facing forward
		
			data.position_ = Vertex3(0, 0, 0);
			data.uv_ = Vertex2(charInfo_[idx].tx, charInfo_[idx].ty);

			mesh.push_back(data);
            
			data.position_ = Vertex3(w, 0, 0);
			data.uv_ = Vertex2(charInfo_[idx].tx + charInfo_[idx].bw / atlasWidth_, charInfo_[idx].ty);
			mesh.push_back(data);

			data.position_ = Vertex3(0, -h, 0);
			data.uv_ = Vertex2(charInfo_[idx].tx, charInfo_[idx].ty + charInfo_[idx].bh / atlasHeight_);
			mesh.push_back(data);

			data.position_ = Vertex3(w, 0, 0);
			data.uv_ = Vertex2(charInfo_[idx].tx + charInfo_[idx].bw / atlasWidth_, charInfo_[idx].ty);
			mesh.push_back(data);

			data.position_ = Vertex3(0, -h, 0);
			data.uv_ = Vertex2(charInfo_[idx].tx, charInfo_[idx].ty + charInfo_[idx].bh / atlasHeight_);
			mesh.push_back(data);

			data.position_ = Vertex3(w, -h, 0);
			data.uv_ = Vertex2(charInfo_[idx].tx + charInfo_[idx].bw / atlasWidth_, charInfo_[idx].ty + charInfo_[idx].bh / atlasHeight_);
			mesh.push_back(data);
            
			charsMesh_[idx] = mesh;
		}

		TRACE_LOG("GLES2FontAtlasTexture::GenerateMeshesForAllChars done.");
	}

	bool GLES2FontAtlasTexture::SetTextMesh(const std::string& text, VertexsData& vertexsData, GLfloat& screenWidth, GLfloat& screenHeight)
	{
        if(!IsReady())
            return false;

		CHECK_ASSERT(viewWidth_ > 0 && viewHeight_ > 0, __FILE__, __LINE__);
			
		vertexsData.clear();

	    vertexsData.reserve(6 * text.size());

        screenWidth = screenHeight = 0;

		float sx = 2.0f/viewWidth_;
		float sy = 2.0f/viewHeight_;

		float x = 0;
		float y = 0;

		for(const char *p = text.c_str(); *p; p++) 
		{ 
            int idx = (unsigned char)(*p);

			float x2 = x + charInfo_[idx].bl * sx;
			float y2 = -y - charInfo_[idx].bt * sy;
			float w = charInfo_[idx].bw * sx;
			float h = charInfo_[idx].bh * sy;

			// Advance the cursor to the start of the next character
			x += charInfo_[idx].ax * sx;
			y += charInfo_[idx].ay * sy;

            auto it = charsMesh_.find(idx);
            if(it == charsMesh_.end())
            {
            	continue;
            }

			screenHeight = std::max(screenHeight, h);

			auto it0 = it->second.begin();

			while(it0 != it->second.end())
			{
				VertexData data = *it0;
                data.position_.x += x2;
                data.position_.y -= y2;
				vertexsData.push_back(data);
				++it0;
			}
		}

		screenWidth = x;

        return true;
	}	

	GLfloat GLES2FontAtlasTexture::GetWidthForCharacterPosition(const char* text, unsigned int charPos)
	{
		GLfloat pos = 0;

        if(IsReady())
        {
		    const char* p = text;

            float sx = 2.0f/viewWidth_;

		    for(unsigned int i=0; i<charPos && *p; i++) 
		    { 
                int idx = (unsigned char)(*p);

			    pos += charInfo_[idx].ax * sx;
			    ++p;
		    }
        }

		return pos;
	}

	unsigned int GLES2FontAtlasTexture::GetCharacterPositionForWidth(const char* text, float width)
	{
		unsigned int charPos = 0;

        if(IsReady())
        {
		    GLfloat pos = 0;

		    const char *p = text;

            float sx = 2.0f/viewWidth_;

		    while(*p) 
		    { 
			    if(pos >= viewWidth_)
				    break;

                int idx = (unsigned char)(*p);

			    pos += charInfo_[idx].ax * sx;

			    ++p;

			    ++charPos;
		    }
        }

		return charPos;
	}
}