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
#include "GLES2FontAtlasTextureManager.h"
#include "GLES2FontAtlasTexture.h"
#include "Check.h"

namespace NSG
{
	GLES2FontAtlasTextureManager::GLES2FontAtlasTextureManager()
	{
	}

	GLES2FontAtlasTextureManager::~GLES2FontAtlasTextureManager()
	{
	}

	PGLES2FontAtlasTexture GLES2FontAtlasTextureManager::GetAtlas(const Key& key)
	{
    	auto it = fontAtlas_.find(key);
    	
    	if(it != fontAtlas_.end())
    	{
    		return it->second;
    	}
    	else
    	{
    		PGLES2FontAtlasTexture pAtlas = PGLES2FontAtlasTexture(new GLES2FontAtlasTexture(key.first.c_str(), key.second));
			fontAtlas_.insert(Atlas::value_type(key, pAtlas));
			return pAtlas;
    	}
    }
}

