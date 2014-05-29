#include "FontAtlasTexture.h"
#include "FreeTypeClass.h"
#include "Check.h"
#include "Context.h"
#include "ResourceFile.h"
#include "App.h"
#if NACL
#include "AppNaCl.h"
#endif
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <assert.h>

namespace NSG
{
	FontAtlasTexture::FontAtlasTexture(const char* filename, int fontSize) 
	: filename_(filename),
    atlasWidth_(0),
	atlasHeight_(0),
	fontSize_(fontSize),
	viewWidth_(0),
	viewHeight_(0)
	{
        pResource_ = PResource(new ResourceFile(filename));
	}

	FontAtlasTexture::~FontAtlasTexture()
	{
        Context::this_->Remove(this);
	}

    void FontAtlasTexture::AllocateResources()
    {
    	Texture::AllocateResources();
        CreateTextureAtlas();
        GenerateMeshesForAllChars();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void FontAtlasTexture::ReleaseResources()
    {
    	charsMesh_.clear();
        charInfo_.clear();
        atlasWidth_ = atlasHeight_ = 0;
        Texture::ReleaseResources();
    }

    #define MAXWIDTH 1024
	void FontAtlasTexture::CreateTextureAtlas()
	{
		TRACE_LOG("FontAtlasTexture::CreateTextureAtlas: File=" << filename_ << " fontSize=" << fontSize_);

		FT_Face face;

		FT_New_Memory_Face(Context::this_->freeType_->GetHandle(), (const FT_Byte*)pResource_->GetData(), pResource_->GetBytes(), 0, &face);
		FT_Set_Pixel_Sizes(face, 0, fontSize_);

		FT_GlyphSlot g = face->glyph;
		
		int roww = 0;
		int rowh = 0;
		 
		for(int i = 32; i < FontAtlasTexture::MAXCHARS; i++) 
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

		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, atlasWidth_, atlasHeight_, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
		std::vector<GLubyte> emptyData(atlasWidth_ * atlasHeight_, 0);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, atlasWidth_, atlasHeight_, GL_ALPHA, GL_UNSIGNED_BYTE, &emptyData[0]);		

		int ox = 0;
        int oy = 0;
        rowh = 0;

        charInfo_.resize(FontAtlasTexture::MAXCHARS) ;

		for(int i = 32; i < FontAtlasTexture::MAXCHARS; i++) 
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

		TRACE_LOG("FontAtlasTexture::CreateTextureAtlas done.");
	}

	void FontAtlasTexture::GenerateMeshesForAllChars()
	{
		TRACE_LOG("FontAtlasTexture::GenerateMeshesForAllChars...");

		auto viewSize = App::this_->GetViewSize();

		viewWidth_ = viewSize.first;
		viewHeight_ = viewSize.second;

		CHECK_ASSERT(viewWidth_ > 0 && viewHeight_ > 0, __FILE__, __LINE__);

		float sx = 2.0f/viewWidth_;
	    float sy = 2.0f/viewHeight_;   

		for(int idx = 32; idx < FontAtlasTexture::MAXCHARS; idx++) 
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

		TRACE_LOG("FontAtlasTexture::GenerateMeshesForAllChars done.");
	}

	bool FontAtlasTexture::SetTextMesh(const std::string& text, VertexsData& vertexsData, GLfloat& screenWidth, GLfloat& screenHeight)
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

            const CharacterInfo& charInfo = charInfo_[idx];

			float x2 = x + charInfo.bl * sx;
			float y2 = -y - charInfo.bt * sy;
			float w = charInfo.bw * sx;
			float h = charInfo.bh * sy;

            float charBottom = (charInfo.bt - charInfo.bh) * sy;

			// Advance the cursor to the start of the next character
			x += charInfo.ax * sx;
			y += charInfo.ay * sy;

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

	GLfloat FontAtlasTexture::GetWidthForCharacterPosition(const char* text, unsigned int charPos)
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

	unsigned int FontAtlasTexture::GetCharacterPositionForWidth(const char* text, float width)
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