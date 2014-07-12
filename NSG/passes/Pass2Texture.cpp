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
#include "Pass2Texture.h"
#include "Render2Texture.h"

namespace NSG
{
	Pass2Texture::Pass2Texture(PTexture texture, bool createDepthBuffer, bool createDepthStencilBuffer)
	: texture_(texture),
	render2Texture_(new Render2Texture(texture, createDepthBuffer, createDepthStencilBuffer))
	{

	}

	Pass2Texture::~Pass2Texture()
	{

	}

	void Pass2Texture::Add(PPass pass)
	{
		passes_.push_back(pass);
	}

	void Pass2Texture::Render()
	{
		if(render2Texture_->Begin())
		{
			auto it = passes_.begin();
			while(it != passes_.end())
			{
				PPass pass = *it;
				pass->Render();
				++it;
			}
			render2Texture_->End();
		}
	}
}