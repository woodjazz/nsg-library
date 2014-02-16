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
#include "Text.h"
#include "Log.h"
#if NACL
#include "NaCl3DInstance.h"
#endif
#include <fstream>
#include <sstream>
#include <assert.h>


static FT_Library s_ft;
static bool s_initialized = false;

static void Initialize()
{
	if(!s_initialized)
	{
		if(FT_Init_FreeType(&s_ft)) 
		{
			TRACE_LOG("Could not init freetype library.");
		}
		else
		{
			s_initialized = true;
		}
	}
}

namespace NSG
{
	Text::Text(const char* filename, int fontSize)
	: filename_(filename),
	fontSize_(fontSize),
	loaded_(false)
	{
		Initialize();

#if NACL
		pLoader_ = NaCl::NaClURLLoader::Create(NaCl::NaCl3DInstance::GetInstance(), filename);
#elif ANDROID			
		pAAssetManager_ = IApp::GetPtrInstance()->GetAssetManager();
#endif
	}

	Text::~Text()
	{
	}

	bool Text::Done()
	{
		if(loaded_) return true;
		const char* pData = nullptr;
		size_t bytes = 0;
	#if NACL
		if(!pLoader_->IsDone()) return false;
		pData = pLoader_->GetData().c_str();
		bytes = pLoader_->GetData().size();
	#elif ANDROID
		assert(pAAssetManager_ != nullptr);
		AAsset* pAsset = AAssetManager_open(pAAssetManager_, filename_.c_str(), AASSET_MODE_BUFFER);
		off_t filelength = AAsset_seek(pAsset, 0, SEEK_END);
		AAsset_seek(pAsset, 0, SEEK_SET);
        std::string buffer;
        buffer.resize((int)filelength);
        AAsset_read(pAsset, &buffer[0],filelength);
		pData = buffer.c_str();
		bytes = buffer.size();    
		AAsset_close(pAsset);    
	#else
        std::ifstream file(filename_, std::ios::binary);
        file.seekg(0,std::ios::end);
        std::streampos filelength = file.tellg();
        file.seekg(0,std::ios::beg);
        std::string buffer;
        buffer.resize((int)filelength);
        file.read(&buffer[0],filelength);
		pData = buffer.c_str();
		bytes = buffer.size();
	#endif	
		TRACE_LOG("File=" << filename_ << " has been loaded with size=" << bytes);
		FT_New_Memory_Face(s_ft, (const unsigned char*)pData, bytes, 0, &face_);
		FT_Set_Pixel_Sizes(face_, 0, fontSize_);
		loaded_ = true;
		return true;
	}
}
