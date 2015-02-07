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
#include "App.h"
#include "Resource.h"
#include "Check.h"
#include "Graphics.h"
#include "ResourceMemory.h"
#include "ResourceFile.h"
#include "Texture.h"
#include "Path.h"
#include "Util.h"
#include "image_helper.h"
#include "pugixml.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "jpgd.h"
#include <algorithm>

namespace NSG
{
    Texture::Texture(const std::string& name, GLint format, GLsizei width, GLsizei height, const char* pixels)
        : fromKnownImgFormat_(false),
          flags_((int)TextureFlag::NONE),
          texture_(0),
          width_(width),
          height_(height),
          format_(format),
          type_(GL_UNSIGNED_BYTE),
          channels_(0),
          serializable_(false),
          wrapMode_(TextureWrapMode::REPEAT),
          mipmapLevels_(0),
          filterMode_(TextureFilterMode::BILINEAR)

    {
        switch (format_)
        {
            case GL_ALPHA:
                channels_ = 1;
                break;
            case GL_RGB:
                channels_ = 3;
                break;
            case GL_RGBA:
                channels_ = 4;
                break;
            case GL_DEPTH_COMPONENT:
                channels_ = 0;
                type_ = GL_UNSIGNED_INT;
                break;
            default:
                CHECK_ASSERT(false && "Unknown format!", __FILE__, __LINE__);
                break;
        }

        auto resource = App::this_->GetOrCreateResourceMemory(name);
		resource->SetData(pixels, width * height * channels_);
		pResource_ = resource;
    }

    Texture::Texture(PResource resource, const TextureFlags& flags)
        : fromKnownImgFormat_(true),
          flags_(flags),
          texture_(0),
          pResource_(resource),
          width_(0),
          height_(0),
          format_(GL_RGBA),
          type_(GL_UNSIGNED_BYTE),
          channels_(0),
          serializable_(true),
          wrapMode_(TextureWrapMode::REPEAT),
          mipmapLevels_(0),
          filterMode_(TextureFilterMode::BILINEAR)
    {
    }

    Texture::~Texture()
    {
        Invalidate();
    }

    GLuint Texture::GetID() const
    {
        return texture_;
    }

    GLsizei Texture::GetWidth() const
    {
        return width_;
    }

    GLsizei Texture::GetHeight() const
    {
        return height_;
    }

    GLint Texture::GetFormat() const
    {
        return format_;
    }

    int Texture::GetChannels() const
    {
        return channels_;
    }

    void Texture::SetSerializable(bool serializable)
    {
        serializable_ = serializable;
    }

    bool Texture::IsSerializable() const
    {
        return serializable_;
    }

    bool Texture::IsValid()
    {
        return pResource_->IsReady();
    }

	const unsigned char* Texture::GetImageData(bool fromKnownImgFormat, PResource resource, bool& allocated, GLint& format, GLsizei& width, GLsizei& height, int& channels)
    {
		CHECK_ASSERT(resource->IsReady(), __FILE__, __LINE__);
        if (fromKnownImgFormat)
        {
			allocated = true;
			const unsigned char* img = stbi_load_from_memory((const unsigned char*)resource->GetData(), resource->GetBytes(), &width, &height, &channels, 0);

            if (!img)
				img = jpgd::decompress_jpeg_image_from_memory((const unsigned char*)resource->GetData(), resource->GetBytes(), &width, &height, &channels, 4);

            if (!img)
				TRACE_LOG("Resource=" << resource->GetName() << " failed with reason: " << stbi_failure_reason());

            if (img)
            {
                if (channels == 4)
                {
                    format = GL_RGBA;
                }
                else if (channels == 3)
                {
                    format = GL_RGB;
                }
                else if (channels == 1)
                {
                    format = GL_ALPHA;
                }
                else
                {
                    format = GL_RGB;
					TRACE_LOG("Resource=" << resource->GetName() << " unknown internalformat. Channels = " << channels << " Width=" << width << " Height=" << height);
                    CHECK_ASSERT(false && "Unknown internalformat", __FILE__, __LINE__);
                }
            }
            return img;
        }
        else
        {
			allocated = false;
            const unsigned char* img = nullptr;

			if (resource->GetBytes())
            {
				CHECK_ASSERT(width * height * channels == resource->GetBytes(), __FILE__, __LINE__);
				img = (const unsigned char*)resource->GetData();
            }
            return img;
        }
    }

    void Texture::AllocateResources()
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        glGenTextures(1, &texture_);

        Graphics::this_->SetTexture(0, this);

		bool allocated = false;
		const unsigned char* img = Texture::GetImageData(fromKnownImgFormat_, pResource_, allocated, format_, width_, height_, channels_);

        if (flags_ & (int)TextureFlag::INVERT_Y)
        {
            unsigned char* inverted = (unsigned char*)malloc(channels_ * width_ * height_);
            memcpy(inverted, img, channels_ * width_ * height_);
            int i, j;
            for ( j = 0; j * 2 < height_; ++j )
            {
                int index1 = j * width_ * channels_;
                int index2 = (height_ - 1 - j) * width_ * channels_;
                for ( i = width_ * channels_; i > 0; --i )
                {
                    unsigned char temp = inverted[index1];
                    inverted[index1] = inverted[index2];
                    inverted[index2] = temp;
                    ++index1;
                    ++index2;
                }
            }

			if (allocated)
				free((void*)img); // same as stbi_image_free

            img = inverted;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)wrapMode_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)wrapMode_);

        if (img != nullptr)
        {
            if (!Graphics::this_->IsTextureSizeCorrect(width_, height_))
            {
                unsigned new_width = width_;
                unsigned new_height = height_;
                GetPowerOfTwoValues(new_width, new_height);
                CHECK_ASSERT(new_width != width_ || new_height != height_, __FILE__, __LINE__);
                unsigned char* resampled = (unsigned char*)malloc(channels_ * new_width * new_height);
                up_scale_image(img, width_, height_, channels_, resampled, new_width, new_height);
                free((void*)img); // same as stbi_image_free
                img = resampled;
                width_ = new_width;
                height_ = new_height;
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

        if (fromKnownImgFormat_)
            stbi_image_free((void*)img);

        mipmapLevels_ = 0;
        if (flags_ & (int)TextureFlag::GENERATE_MIPMAPS)
        {
            {
                // calculate mipmap levels based on texture size
                unsigned maxSize = std::max(width_, height_);
                while (maxSize)
                {
                    maxSize >>= 1;
                    ++mipmapLevels_;
                }
            }

            glGenerateMipmap(GL_TEXTURE_2D);
        }

        switch (filterMode_)
        {
            case TextureFilterMode::NEAREST:
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    break;
                }
            case TextureFilterMode::BILINEAR:
                {
                    if (mipmapLevels_ < 2)
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    else
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    break;
                }
            case TextureFilterMode::TRILINEAR:
                {
                    if (mipmapLevels_ < 2)
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    else
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    break;
                }
            default:
                CHECK_ASSERT(false, __FILE__, __LINE__);
                break;
        }

        pResource_->Invalidate(false);

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    void Texture::ReleaseResources()
    {
        glDeleteTextures(1, &texture_);
        //pResource_->Invalidate(); //Maybe the user has released the resource: force the reload (next time) from the resource
    }

    PTexture Texture::CreateFrom(const pugi::xml_node& node)
    {
        std::string flags = node.attribute("flags").as_string();
		std::string resourceName = node.attribute("resource").as_string();
		auto resource = App::this_->GetResource(resourceName);
		CHECK_ASSERT(resource, __FILE__, __LINE__);
        auto texture = std::make_shared<Texture>(resource);
        texture->SetFlags(flags);
        return texture;
    }

    void Texture::Save(pugi::xml_node& node)
    {
        node.append_attribute("flags") = flags_.to_string().c_str();
		node.append_attribute("resource") = pResource_->GetName().c_str();
    }

    void Texture::SetFlags(const TextureFlags& flags)
    {
        if (flags != flags_)
        {
            flags_ = flags;
            Invalidate();
        }
    }

    void Texture::SetWrapMode(TextureWrapMode mode)
    {
        if (wrapMode_ != mode)
        {
            wrapMode_ = mode;
            Invalidate();
        }
    }

    void Texture::SetFilterMode(TextureFilterMode mode)
    {
        if (filterMode_ != mode)
        {
            filterMode_ = mode;
            Invalidate();
        }
    }

	int Texture::SaveAsPNG(PResource resource, const Path& outputDir)
    {
		CHECK_CONDITION(resource->IsReady(), __FILE__, __LINE__);
		bool allocated = false;
		int format, width, height, channels;
		const unsigned char* img = Texture::GetImageData(true, resource, allocated, format, width, height, channels);
		Path oPath;
		oPath.SetPath(outputDir.GetPath());
		oPath.SetName(Path(resource->GetName()).GetName());
		oPath.SetExtension("png");
		int result = stbi_write_png(oPath.GetFullAbsoluteFilePath().c_str(), width, height, channels, img, 0);
		if (allocated) free((void*)img);
		return result;
    }
}