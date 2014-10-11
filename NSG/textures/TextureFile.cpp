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
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "TextureFile.h"
#include "stb_image.h"
#include "jpgd.h"
#include "Check.h"
#include "Util.h"
#include "ResourceFile.h"
#include "ResourceMemory.h"
#include "Context.h"
#include "Graphics.h"
#include "Path.h"
#include "pugixml.hpp"

namespace NSG
{
	TextureFile::TextureFile(const char* filename, Flags flags) 
	: Texture(flags)
	{
		ResourceFile* resourceFile = new ResourceFile(filename);
		pResource_ = PResourceFile(resourceFile);
		filename_ = resourceFile->GetPath().GetFilePath();
	}

	TextureFile::TextureFile(PResourceFile resource, Flags flags)
	: Texture(flags),
	filename_(resource->GetPath().GetFilePath())
	{
		pResource_ = resource;
	}

	TextureFile::TextureFile(PResourceMemory resource, Flags flags)
	: Texture(flags)
	{
		pResource_ = resource;
	}


	TextureFile::~TextureFile()
	{
		Context::RemoveObject(this);
	}

	const unsigned char* TextureFile::GetImageData()
	{
		const unsigned char* img = stbi_load_from_memory((const unsigned char*)pResource_->GetData(), pResource_->GetBytes(), &width_, &height_, &channels_, 0);

		if (!img)
			img = jpgd::decompress_jpeg_image_from_memory((const unsigned char*)pResource_->GetData(), pResource_->GetBytes(), &width_, &height_, &channels_, 4);

		if (!img)
			TRACE_LOG("Filename=" << filename_.c_str() << " failed with reason: " << stbi_failure_reason());
		
		if (img)
		{
			if (channels_ == 4)
			{
				format_ = GL_RGBA;
			}
			else if (channels_ == 3)
			{
				format_ = GL_RGB;
			}
			else
			{
				format_ = GL_RGB;
				TRACE_LOG("Filename=" << filename_.c_str() << " unknown internalformat. Channels = " << channels_ << " Width=" << width_ << " Height=" << height_);
				CHECK_ASSERT(false && "Unknown internalformat", __FILE__, __LINE__);
			}
		}
		return img;
	}

	void TextureFile::FreeImageData(const unsigned char* img)
	{
		stbi_image_free((void*)img);
	}

	void TextureFile::Save(pugi::xml_node& node)
	{
		CHECK_ASSERT(!filename_.empty(), __FILE__, __LINE__);
		node.append_attribute("type") = "TextureFile";
		//CopyFile(filename_, filename);
		node.append_attribute("filename") = filename_.c_str();
		node.append_attribute("flags") = flags_.to_string().c_str();
	}
}