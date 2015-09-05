/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "Texture2D.h"
#include "Resource.h"
#include "Util.h"
#include "Image.h"
#include "StringConverter.h"
#include "pugixml.hpp"

namespace NSG
{
    Texture2D::Texture2D(PResource resource, const TextureFlags& flags)
        : Texture(resource, flags),
		pixels_(nullptr)
    {

    }

    Texture2D::Texture2D(const std::string& name)
        : Texture(name),
		pixels_(nullptr)
    {

    }

    Texture2D::~Texture2D()
    {
    }

    GLenum Texture2D::GetTarget() const
    {
        return GL_TEXTURE_2D;
    }

	PTexture Texture2D::CreateFrom(const pugi::xml_node& node)
    {
        std::string flags = node.attribute("flags").as_string();
        std::string resourceName = node.attribute("resource").as_string();
        std::string uvName = node.attribute("uvName").as_string();
        TextureWrapMode wrapMode = ToTextureWrapMode(node.attribute("wrapMode").as_string());
        TextureFilterMode filterMode = ToTextureFilterMode(node.attribute("filterMode").as_string());
        TextureBlend blendType = ToTextureBlend(node.attribute("blendType").as_string());
        TextureType mapType = ToTextureType(node.attribute("mapType").as_string());
        bool useAlpha = node.attribute("useAlpha").as_bool();
        Vector4 uvTransform = ToVertex4(node.attribute("uvTransform").as_string());
        auto res = Resource::Get(resourceName);
        PTexture texture;
        texture = std::make_shared<Texture2D>(res);
        texture->SetName(resourceName);
        texture->SetUVName(uvName);
        texture->SetFlags(flags);
        texture->SetWrapMode(wrapMode);
        texture->SetFilterMode(filterMode);
        texture->SetBlendType(blendType);
        texture->SetMapType(mapType);
        texture->SetUseAlpha(useAlpha);
        texture->SetUVTransform(uvTransform);
        return texture;
    }

    void Texture2D::Define()
    {
		CHECK_GL_STATUS(__FILE__, __LINE__);

        if (image_ && image_->IsCompressed())
        {
            glCompressedTexImage2D(GL_TEXTURE_2D,
                                   0,
                                   format_,
                                   width_,
                                   height_,
                                   0,
                                   image_->GetCompressedDataSize(),
                                   image_->GetData());
        }
        else if (image_)
        {
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         format_,
                         width_,
                         height_,
                         0,
                         format_,
                         type_,
                         image_->GetData());
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         format_,
                         width_,
                         height_,
                         0,
                         format_,
						 type_,
						 pixels_);
        }

		CHECK_GL_STATUS(__FILE__, __LINE__);
    }

	void Texture2D::SetData(unsigned char* pixels)
	{
		if (pixels_ != pixels)
		{
			CHECK_ASSERT(!image_, __FILE__, __LINE__);
			pixels_ = pixels;
			Invalidate();
		}
	}
}
