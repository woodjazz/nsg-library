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
#include "Resource.h"
#include "Log.h"
#include "App.h"
#include "Check.h"
#include <fstream>
#if NACL
#include "AppNaCl.h"
#include "NaClURLLoader.h"
#endif

#ifdef __APPLE__
#include "CoreFoundation/CFBundle.h"
#define MAXPATHLEN 1024
char* AppleGetBundleDirectory()
{
	CFURLRef bundleURL;
	CFStringRef pathStr;
	static char path[MAXPATHLEN];
	memset(path, 0, MAXPATHLEN);
	CFBundleRef mainBundle = CFBundleGetMainBundle();

	bundleURL = CFBundleCopyBundleURL(mainBundle);
	pathStr = CFURLCopyFileSystemPath(bundleURL, kCFURLPOSIXPathStyle);
	CFStringGetCString(pathStr, path, MAXPATHLEN, kCFStringEncodingASCII);
	CFRelease(pathStr);
	CFRelease(bundleURL);
	return path;
}
#endif

namespace NSG
{
	Resource::Resource(const char* filename)
	: loaded_(false),
	filename_(filename),
	pData_(nullptr),
	bytes_(0)
	{
#if NACL
		pLoader_ = NaCl::NaClURLLoader::Create(NaCl::NaCl3DInstance::GetInstance(), filename);
#elif ANDROID			
		pAAssetManager_ = App::this_->GetAssetManager();
#endif
	}

	Resource::Resource(const char* buffer, size_t bytes)
	: pData_(nullptr),
	bytes_(0),
	loaded_(true)
	{
		if(buffer != nullptr)
		{
			CHECK_ASSERT(bytes > 0, __FILE__, __LINE__);
			bytes_ = bytes;
			buffer_.resize(bytes);
			pData_ = buffer_.c_str();
			memcpy((void*)pData_, buffer, bytes);
		}
	}

	Resource::~Resource()
	{
	}

	const char* const Resource::GetData() const
	{
		return pData_;
	}

	size_t Resource::GetBytes() const
	{
		return bytes_;
	}

	bool Resource::IsLoaded()
	{
		if(loaded_) return true;

	#if NACL
		if(!pLoader_->IsDone()) return false;
		pData_ = pLoader_->GetData().c_str();
		bytes_ = pLoader_->GetData().size();
	#elif ANDROID
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
		pData_ = buffer_.c_str();
		bytes_ = buffer_.size();    
		AAsset_close(pAsset);    
	#else
        std::ifstream file(filename_, std::ios::binary);

        #if __APPLE__
    	if(!file.is_open())
    	{
    		std::string newName;
    		newName.resize(2048);
    		sprintf(&newName[0], "%s/Contents/Resources/Data/%s", AppleGetBundleDirectory(), filename_.c_str());
    		file.open(newName, std::ios::binary);
    	}
        #endif

        CHECK_ASSERT(file.is_open(), __FILE__, __LINE__);
        file.seekg(0,std::ios::end);
        std::streampos filelength = file.tellg();
        file.seekg(0,std::ios::beg);
        buffer_.resize((int)filelength);
        file.read(&buffer_[0],filelength);
		pData_ = buffer_.c_str();
		bytes_ = buffer_.size();
	#endif	
		loaded_ = true;
		TRACE_LOG("File=" << filename_ << " has been loaded with size=" << bytes_);
		return true;
	}

}