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
#include "b64/encode.h"
#include "b64/decode.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#include "jpgd.h"
#include <algorithm>

namespace NSG
{
    Texture::Texture(GLint format, GLsizei width, GLsizei height, const char* pixels)
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

        pResource_ = PResource(new ResourceMemory(pixels, width * height * channels_));
    }

    Texture::Texture(PResourceMemory resource, const TextureFlags& flags)
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
		pResource_->AllowInvalidate(false);
    }

    Texture::Texture(PResourceFile resource, const TextureFlags& flags)
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

    const unsigned char* Texture::GetImageData()
    {
        if (fromKnownImgFormat_)
        {
            const unsigned char* img = stbi_load_from_memory((const unsigned char*)pResource_->GetData(), pResource_->GetBytes(), &width_, &height_, &channels_, 0);

            if (!img)
                img = jpgd::decompress_jpeg_image_from_memory((const unsigned char*)pResource_->GetData(), pResource_->GetBytes(), &width_, &height_, &channels_, 4);

            if (!img)
                TRACE_LOG("Filename=" << pResource_->GetPath().GetFilePath() << " failed with reason: " << stbi_failure_reason());

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
                else if (channels_ == 1)
                {
                    format_ = GL_ALPHA;
                }
                else
                {
                    format_ = GL_RGB;
                    TRACE_LOG("Filename=" << pResource_->GetPath().GetFilePath() << " unknown internalformat. Channels = " << channels_ << " Width=" << width_ << " Height=" << height_);
                    CHECK_ASSERT(false && "Unknown internalformat", __FILE__, __LINE__);
                }
            }
            return img;
        }
        else
        {
            const unsigned char* img = nullptr;

            if (pResource_->GetBytes())
            {
                CHECK_ASSERT(width_ * height_ * channels_ == pResource_->GetBytes(), __FILE__, __LINE__);
                img = (const unsigned char*)pResource_->GetData();
            }
            return img;
        }
    }

    void Texture::AllocateResources()
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        glGenTextures(1, &texture_);

        Graphics::this_->SetTexture(0, this);

        const unsigned char* img = GetImageData();

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
		pugi::xml_attribute fileAtt = node.attribute("filename");
		PTexture texture;
		if (fileAtt)
		{
			std::string filename = node.attribute("filename").as_string();
			auto resource = std::make_shared<ResourceFile>(filename);
			texture = std::make_shared<Texture>(resource);
		}
		else
		{
			pugi::xml_node dataNode = node.child("data");
			size_t nBytes = dataNode.attribute("dataSize").as_uint();
			std::string value;
			if (nBytes)
			{
				const pugi::char_t* data = dataNode.child_value();
				value.resize(nBytes);
				memcpy(&value[0], data, nBytes);
			}

			std::string decoded_binary;
			decoded_binary.resize(nBytes);
			base64::base64_decodestate state;
			base64::base64_init_decodestate(&state);
			int decoded_length = base64::base64_decode_block(value.c_str(), nBytes, &decoded_binary[0], &state);
			decoded_binary.resize(decoded_length);
			auto resource = std::make_shared<ResourceMemory>(decoded_binary.c_str(), decoded_binary.size());
			texture = std::make_shared<Texture>(resource);
		}
        
        texture->SetFlags(flags);
        return texture;
    }

    void Texture::Save(pugi::xml_node& node)
    {
        Path path(pResource_->GetPath());
		if (path.IsEmpty())
		{
			const char* const data = pResource_->GetData();
			size_t nBytes = pResource_->GetBytes();

			base64::base64_encodestate state;
			base64::base64_init_encodestate(&state);

			std::string encoded_data;
			encoded_data.resize(2 * nBytes);

			int numchars = base64::base64_encode_block(data, nBytes, &encoded_data[0], &state);
			numchars += base64::base64_encode_blockend(&encoded_data[0] + numchars, &state);
			encoded_data.resize(numchars);

			pugi::xml_node dataNode = node.append_child("data");
			dataNode.append_attribute("dataSize").set_value((unsigned)encoded_data.size());
			dataNode.append_child(pugi::node_pcdata).set_value(encoded_data.c_str());
		}
		else
		{
			if (path.IsPathRelative())
				node.append_attribute("filename") = path.GetFilePath().c_str();
			else
				node.append_attribute("filename") = path.GetFilename().c_str();
		}
        node.append_attribute("flags") = flags_.to_string().c_str();
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

}