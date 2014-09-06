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
#include "Graphics.h"
#include "TextureFile.h"
#include "TextureMemory.h"
#include "image_helper.h"
#include "pugixml.hpp"

namespace NSG
{
    Texture::Texture(Flags flags)
        : flags_(flags),
          texture_(0),
          width_(0),
          height_(0),
          format_(GL_RGBA),
          type_(GL_UNSIGNED_BYTE),
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
        CHECK_GL_STATUS(__FILE__, __LINE__);

        glGenTextures(1, &texture_);

        Graphics::this_->SetTexture(0, this);

        const unsigned char* img = GetImageData();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if (img != nullptr)
        {
            int max_supported_size = 0;

            glGetIntegerv( GL_MAX_TEXTURE_SIZE, &max_supported_size );

            if (width_ > max_supported_size || height_ > max_supported_size || !Graphics::this_->HasNonPowerOfTwo())
            {
                int new_width = 1;
                int new_height = 1;

                while (new_width < width_)
                    new_width *= 2;

                while (new_height < height_)
                    new_height *= 2;


                if (new_width != width_ || new_height != height_)
                {
                    unsigned char* resampled = (unsigned char*)malloc(channels_ * new_width * new_height);
                    up_scale_image(img, width_, height_, channels_, resampled, new_width, new_height);
                    free((void*)img); // same as stbi_image_free
                    img = resampled;
                    width_ = new_width;
                    height_ = new_height;
                }
            }
        }

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     format_,
                     width_,
                     height_,
                     0,
                     format_,
                     type_,
                     img);

        FreeImageData(img);

        if (flags_ & Flag::GENERATE_MIPMAPS)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }

        Graphics::this_->SetTexture(0, nullptr);

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    void Texture::ReleaseResources()
    {
        glDeleteTextures(1, &texture_);
    }

    PTexture Texture::CreateFrom(const pugi::xml_node& node)
    {
        std::string type = node.attribute("type").as_string();

        if (type == "TextureFile")
        {
			std::string flags = node.attribute("flags").as_string();
            std::string filename = node.attribute("filename").as_string();
            return PTextureFile(new TextureFile(filename.c_str(), Texture::Flags(flags)));
        }
        else
        {
            int format = node.attribute("format").as_int();
            int width = node.attribute("width").as_int();
            int height = node.attribute("height").as_int();
            return PTextureMemory(new TextureMemory(format, width, height, nullptr));
        }
    }
}