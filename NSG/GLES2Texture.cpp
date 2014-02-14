#include "GLES2Texture.h"
#include "soil/SOIL.h"
#include "Log.h"
#include "IApp.h"
#if NACL
#include "NaCl3DInstance.h"
#endif
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <assert.h>

namespace NSG
{
	GLES2Texture::GLES2Texture(const char* filename) 
	: texture_(0), filename_(filename), loaded_(false)
	{
#if NACL
		pLoader_ = NaCl::NaClURLLoader::Create(NaCl::NaCl3DInstance::GetInstance(), filename);
#elif ANDROID			
		pAAssetManager_ = IApp::GetPtrInstance()->GetAssetManager();
#endif	
	}

	GLES2Texture::~GLES2Texture()
	{
		glDeleteTextures(1, &texture_);
	}

	bool GLES2Texture::Done()
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
		int img_width = 0;
		int img_height = 0;
		unsigned char* img = SOIL_load_image_from_memory((const unsigned char *const)pData, bytes, &img_width, &img_height, nullptr, 0);
		glGenTextures(1, &texture_);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGB,
			img_width,
			img_height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			img);
		SOIL_free_image_data(img);
		loaded_ = true;
		return true;
	}
}