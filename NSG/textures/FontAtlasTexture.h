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
#include "VertexData.h"
#include "Path.h"
#include "Object.h"
#include <string>
#include <map>

namespace NSG
{
	class FontAtlasTexture : public Object
	{
	public:
		FontAtlasTexture(const Path& path, int viewWidth, int viewHeight);
		~FontAtlasTexture();
		void GenerateMesh(const std::string& text, VertexsData& vertexsData, Indexes& indexes, GLfloat& screenWidth, GLfloat& screenHeight);
		GLfloat GetWidthForCharacterPosition(const char* text, unsigned int charPos);
		unsigned int GetCharacterPositionForWidth(const char* text, float width);
		PTexture GetTexture() const { return texture_; }
		void SetViewSize(int width, int height);
	private:
		bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        bool ParseXML();
        PTexture texture_;
        PResource xmlResource_;
		Path path_;
		int viewWidth_;
		int viewHeight_;

        struct CharInfo
        {
        	int width;
        	int height;
        	Vertex2 offset;
        	Rect rect;
        };
		
		typedef std::map<int, CharInfo> CharsMap;
        CharsMap charsMap_;
		SignalViewChanged::PSlot slotViewChanged_;
	};

}