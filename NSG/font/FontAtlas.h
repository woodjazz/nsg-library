/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "WeakFactory.h"
#include <string>
#include <map>

namespace NSG
{
	class FontAtlas : public std::enable_shared_from_this<FontAtlas>, public Object, WeakFactory<std::string, TextMesh>
	{
	public:
		FontAtlas(const std::string& name = GetUniqueName("FontAtlas"));
		~FontAtlas();
		void Set(PResourceFile xmlResource);
		void SetWindow(Window* window);
		void GenerateMeshData(const std::string& text, VertexsData& vertexsData, Indexes& indexes, GLfloat& screenWidth, GLfloat& screenHeight);
		void SetTexture(PTexture texture);
		PTexture GetTexture() const { return texture_; }
		void SetViewSize(int width, int height);
		PTextMesh GetOrCreateMesh(const std::string& text, HorizontalAlignment hAlign, VerticalAlignment vAlign);
	private:
		bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        void ParseXML();
		PResourceFile xmlResource_;
		int viewWidth_;
		int viewHeight_;
        PTexture texture_;
        int height_;
        struct CharInfo
        {
        	int width;
        	Vertex2 offset;
        	Rect rect;
        };
		typedef std::map<int, CharInfo> CharsMap;
        CharsMap charsMap_;
        SignalSizeChanged::PSlot slotViewChanged_;
        PGraphics graphics_;
	};

}