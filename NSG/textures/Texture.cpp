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
#include "Texture.h"
#include "Context.h"
#include "Resource.h"
#include "Check.h"

namespace NSG
{
	Texture::Texture() 
	: texture_(0), 
	width_(0),
	height_(0),
	format_(GL_RGBA),
	channels_(0)
	{
	}

	Texture::~Texture()
	{
	}

	bool Texture::IsValid()
	{
		return pResource_->IsLoaded();
	}

	void Texture::AllocateResources()
	{
		glGenTextures(1, &texture_);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void Texture::ReleaseResources()
	{
		glDeleteTextures(1, &texture_);
	}

	BindTexture::BindTexture(Texture& obj)
	: obj_(obj)
	{
		obj.Bind();
	}

	BindTexture::~BindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}