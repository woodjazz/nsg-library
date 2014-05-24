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
#include "TextureFile.h"
#include "SOIL.h"
#include "Check.h"
#include "ResourceFile.h"
#include "Context.h"

namespace NSG
{
	TextureFile::TextureFile(const char* filename) 
	: filename_(filename)
	{
        pResource_ = PResourceFile(new ResourceFile(filename));
	}

	TextureFile::~TextureFile()
	{
		Context::this_->Remove(this);
	}

	void TextureFile::AllocateResources()
	{
		Texture::AllocateResources();

        int channels = 0;
		unsigned char* img = SOIL_load_image_from_memory((const unsigned char*)pResource_->GetData(), pResource_->GetBytes(), &width_, &height_, &channels, 0);

        if(channels == 4)
        {
            format_ = GL_RGBA;
        }
        else if(channels == 3)
        {
            format_ = GL_RGB;
        }
        else
        {
            format_ = GL_RGB;
            TRACE_LOG("Unknown internalformat");
        }

        glBindTexture(GL_TEXTURE_2D, texture_);

		glTexImage2D(GL_TEXTURE_2D,
			0,
			format_,
			width_,
			height_,
			0,
			format_,
			GL_UNSIGNED_BYTE,
			img);

		glBindTexture(GL_TEXTURE_2D, 0);

		SOIL_free_image_data(img);
	}
}