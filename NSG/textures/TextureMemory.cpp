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
#include "TextureMemory.h"
#include "Check.h"
#include "ResourceMemory.h"
#include "Context.h"
#include <vector>

namespace NSG
{
	TextureMemory::TextureMemory(GLint format, GLsizei width, GLsizei height, const char* pixels) 
	: channels_(0)
	{
       	switch(format_)
    	{
    		case GL_ALPHA:
    		case GL_LUMINANCE:
    			channels_ = 1;
    			break;
    		case GL_LUMINANCE_ALPHA:
    			channels_ = 2;
    			break;
    		case GL_RGB:
    			channels_ = 3;
    			break;
    		case GL_RGBA:
    			channels_ = 4;
    			break;
    		default:
    			CHECK_ASSERT(false && "Unknown format", __FILE__, __LINE__);
    			break; 
    	}	   

		pResource_ = PResource(new ResourceMemory(pixels, width*height*channels_));
		width_ = width;
		height_ = height;
		format_ = format;

	}

	TextureMemory::~TextureMemory()
	{
		Context::this_->Remove(this);		
	}

	void TextureMemory::AllocateResources()
	{
		Texture::AllocateResources();

		const char* data = nullptr;

		if(pResource_->GetBytes())
		{
        	CHECK_ASSERT(width_*height_*channels_ == pResource_->GetBytes(), __FILE__, __LINE__);
        	data = pResource_->GetData();
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
			data);

        if(data == nullptr)
        {
	        std::vector<GLubyte> emptyData(width_ * height_ * channels_, 0);
	        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, format_, GL_UNSIGNED_BYTE, &emptyData[0]);		
        }

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}