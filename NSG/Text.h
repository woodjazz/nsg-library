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
#include "ft2build.h"
#include FT_FREETYPE_H
#include <string>
#if NACL
#include "NSG/NaClURLLoader.h"
#elif ANDROID
#include <android/asset_manager.h>
#endif

namespace NSG
{
	class Text
	{
	public:
		Text(const char* filename, int fontSize);
		~Text();
	private:
		bool Done();

		std::string filename_;
		FT_Face face_;
		int fontSize_;
		bool loaded_;

	#if NACL		
		NaCl::PNaClURLLoader pLoader_;
	#elif ANDROID		
		AAssetManager* pAAssetManager_;		
	#endif
	};

	typedef std::shared_ptr<Text> PText;
}