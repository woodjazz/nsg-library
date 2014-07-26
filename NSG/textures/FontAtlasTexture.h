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

#include "Types.h"
#include <string>
#include <map>

namespace NSG
{
	class FontAtlasTexture
	{
	public:
		FontAtlasTexture(const std::string& filename);
		~FontAtlasTexture();
		bool GenerateMesh(const std::string& text, VertexsData& vertexsData, Indexes& indexes, GLfloat& screenWidth, GLfloat& screenHeight);
		GLfloat GetWidthForCharacterPosition(const char* text, unsigned int charPos);
		unsigned int GetCharacterPositionForWidth(const char* text, float width);
		bool IsReady();
		PTexture GetTexture() const { return texture_; }
	private:
        void ParseXML();
        PTexture texture_;
        PResource xmlResource_;
		int32_t viewWidth_;
		int32_t viewHeight_;

        struct CharInfo
        {
        	int width;
        	int height;
        	Vertex2 offset;
        	Rect rect;
        };
		
		typedef std::map<int, CharInfo> CharsMap;
        CharsMap charsMap_;
		std::string filename_;
	};

}