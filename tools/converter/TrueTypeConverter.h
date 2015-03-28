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
#include "Path.h"
#include "stb_truetype.h"
#include <vector>
namespace NSG
{
	class ResourceConverter;
	class TrueTypeConverter
	{
	public:
		TrueTypeConverter(const Path& path, int sChar, int eChar, int fontPixelsHeight, int bitmapWidth, int bitmapHeight);
		bool Load();
		bool Save(const Path& outputDir, bool compress) const;
	private:
		Path path_;
		int sChar_;
		int eChar_;
		int fontPixelsHeight_;
		int bitmapWidth_;
		int bitmapHeight_;
		std::shared_ptr<ResourceConverter> texture_;
		std::vector<stbtt_bakedchar> cdata_;
	};
}