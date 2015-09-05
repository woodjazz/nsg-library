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

struct FONScontext;
namespace NSG
{
	class Font
	{
	public:
		Font(PWindow window, int width, int height, int flags);
		~Font();
	private:
		void InternalDraw(const float* verts, const float* tcoords, const unsigned int* colors, int nverts);
		int InternalCreate(int width, int height);
		void InternalUpdate(int* rect, const unsigned char* data);
		void InternalDelete();
		static int RenderCreate(void* userPtr, int width, int height);
		static int RenderResize(void* userPtr, int width, int height);
		static void RenderUpdate(void* userPtr, int* rect, const unsigned char* data);
		static void RenderDraw(void* userPtr, const float* verts, const float* tcoords, const unsigned int* colors, int nverts);
		static void RenderDelete(void* userPtr);
		void Create(int width, int height, int flags);
		void Destroy();
		PWindow window_;
        FONScontext* context_;
        PGraphics graphics_;
		PPass pass_;
		PProgram program_;
		PCamera camera_;
		GLuint tex_;
		int width_;
		int height_;
	};
}