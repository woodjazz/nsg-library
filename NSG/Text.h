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
#include <memory>
#include <vector>
#include <string>
#include "ft2build.h"
#include FT_FREETYPE_H
#include "Resource.h"
#include "GLES2Includes.h"
#include "GLES2Program.h"
#include "GLES2VertexBuffer.h"
#include "Types.h"

namespace NSG
{
	class Text
	{
	public:
		Text(const char* filename, int fontSize);
		~Text();
		GLuint GetId() const { return texture_; }
		void Bind() { glBindTexture(GL_TEXTURE_2D, texture_); }
		void RenderText(Color color, const std::string& text, float x, float y, float sx, float sy, GLenum usage);
		GLfloat GetWidth() const { return screenWidth_; }
		GLfloat GetHeight() const { return screenHeight_; }
	private:
		bool IsReady();
		void CreateTextureAtlas();
		FT_Face face_;
		int fontSize_;
		PResource pResource_;
		int atlasWidth_;
		int atlasHeight_;
		GLuint texture_;
		PGLES2Program pProgram_;
		PGLES2VertexBuffer pVBuffer_;
		GLuint texture_loc_;
		GLuint position_loc_;
		GLuint texcoord_loc_;
		GLuint color_loc_;

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

		CharacterInfo charInfo_[128];

		struct Point 
		{
			GLfloat x;
			GLfloat y;
			GLfloat s;
			GLfloat t;
		};

		std::vector<Point> coords_;
		std::string lastText_;
		GLfloat screenWidth_;
		GLfloat screenHeight_;
	};

	typedef std::shared_ptr<Text> PText;
}