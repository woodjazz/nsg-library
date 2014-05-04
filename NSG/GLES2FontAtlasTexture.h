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
#pragma once

#include "GLES2Includes.h"
#include "GLES2Texture.h"
#include <memory>
#include <string>
#include <vector>
#include "Resource.h"
#include "ft2build.h"
#include FT_FREETYPE_H

namespace NSG
{
	class GLES2FontAtlasTexture : public GLES2Texture
	{
	public:
		GLES2FontAtlasTexture(const char* filename, int fontSize);
		~GLES2FontAtlasTexture();
		bool IsReady();

		struct CharacterInfo 
		{
			float ax; // advance.x
			float ay; // advance.y

			float bw; // bitmap.width;
			float bh; // bitmap.rows;

			float bl; // bitmap_left;
			float bt; // bitmap_top;

			float tx; // x offset of glyph in texture coordinates
			float ty; // y offset of glyph in texture coordinates
		};

		typedef std::vector<CharacterInfo> CharsInfo;

		const CharsInfo& GetCharInfo() const { return charInfo_;}
		int GetAtlasWidth() const { return atlasWidth_; }
		int GetAtlasHeight() const {return atlasHeight_; }

	private:
		void CreateTextureAtlas();
		int atlasWidth_;
		int atlasHeight_;
		CharsInfo charInfo_;
		int fontSize_;

	};

	typedef std::shared_ptr<GLES2FontAtlasTexture> PGLES2FontAtlasTexture;
}