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
	: texture_(0), pResource_(new Resource(filename))
	{
	}

	GLES2Texture::~GLES2Texture()
	{
		glDeleteTextures(1, &texture_);
	}

	bool GLES2Texture::IsReady()
	{
		if(pResource_ != nullptr && pResource_->IsReady())
		{
			int img_width = 0;
			int img_height = 0;
			unsigned char* img = SOIL_load_image_from_memory(pResource_->GetData(), pResource_->GetBytes(), &img_width, &img_height, nullptr, 0);
			glGenTextures(1, &texture_);
			glBindTexture(GL_TEXTURE_2D, texture_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
			pResource_ = nullptr;
		}

		return pResource_ == nullptr;
	}

	BindTexture::BindTexture(GLES2Texture& obj)
	: obj_(obj)
	{
		obj.Bind();
	}

	BindTexture::~BindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}