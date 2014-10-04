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
#include "ResourceFile.h"
#include "Check.h"
#include "Context.h"
#include "App.h"
#include "Util.h"
#if NACL
#include "AppNaCl.h"
#include "NaClURLLoader.h"
#elif ANDROID
#include <android/asset_manager.h>
#endif
#include <fstream>

namespace NSG
{
	ResourceFile::ResourceFile(const char* fileFullPath)
		: fileFullPath_(fileFullPath)
	{
		ReplaceChar(fileFullPath_, '\\', '/');
		filePath_ = ExtractPath(fileFullPath_);
		fileName_ = ExtractFileName(fileFullPath_);

	#if NACL
		pLoader_ = NaCl::NaClURLLoader::Create(NaCl::NaCl3DInstance::GetInstance(), filename_.c_str());
	#elif defined(ANDROID) && !defined(SDL)
		pAAssetManager_ = App::this_->GetAssetManager();
	#endif
	}

	ResourceFile::~ResourceFile()
	{
		Context::RemoveResource(this);
	}

	bool ResourceFile::IsLoaded()
	{
		if (!loaded_ && !fileFullPath_.empty())
		{
		#if NACL
			if(!pLoader_->IsDone()) return false;
			buffer_.resize(pLoader_->GetData().size());
			memcpy(&buffer_[0], pLoader_->GetData().c_str(), pLoader_->GetData().size());
			pLoader_ = nullptr;
		#elif ANDROID
			#if SDL
	    		SDL_RWops* assetHandle = SDL_RWFromFile(filename_.c_str(), "rb");
				off_t filelength = assetHandle->hidden.androidio.size;
		        buffer_.resize((int)filelength);
	    		SDL_RWread(assetHandle, &buffer_[0], filelength, 1);
	    		SDL_RWclose(assetHandle);
			#else
				assert(pAAssetManager_ != nullptr);
				AAsset* pAsset = AAssetManager_open(pAAssetManager_, filename_.c_str(), AASSET_MODE_BUFFER);
				if(!pAsset)
				{
					TRACE_LOG("Cannot open file: " << filename_);
					return false;
				}
				off_t filelength = AAsset_getLength(pAsset);
		        buffer_.resize((int)filelength);
		        AAsset_read(pAsset, &buffer_[0],filelength);
				AAsset_close(pAsset); 
			#endif
		#else
			std::ifstream file(fileFullPath_, std::ios::binary);
            
	        #if __APPLE__
	    	if(!file.is_open())
	    	{
	    		const char* base_path = SDL_GetBasePath();
                std::string path(base_path);
                path += fileFullPath_;
	    		file.open(path.c_str(), std::ios::binary);
	    	}
	    	#endif
	    	if(!file.is_open())
				TRACE_LOG("Cannot open filename=" << fileFullPath_);
	        CHECK_ASSERT(file.is_open(), __FILE__, __LINE__);
	        file.seekg(0,std::ios::end);
	        std::streampos filelength = file.tellg();
	        file.seekg(0,std::ios::beg);
	        buffer_.resize((int)filelength);
	        file.read(&buffer_[0],filelength);
		#endif	
			TRACE_LOG("Resource::File=" << fileFullPath_ << " has been loaded with size=" << buffer_.size());
			loaded_ = true;
		}

		return loaded_;
	}
}