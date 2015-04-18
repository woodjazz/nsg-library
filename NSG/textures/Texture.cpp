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
#include "Texture.h"
#include "Resource.h"
#include "Image.h"
#include "Check.h"
#include "Graphics.h"
#include "ResourceFile.h"
#include "ResourceXMLNode.h"
#include "Texture.h"
#include "Path.h"
#include "Util.h"
#include "pugixml.hpp"
#include <algorithm>
#include <cerrno>

namespace NSG
{
    Texture::Texture(const std::string& name, GLint format)
        : Object(name),
          flags_((int)TextureFlag::NONE),
          texture_(0),
          width_(0),
          height_(0),
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
    }

    Texture::Texture(PResource resource, const TextureFlags& flags)
        : Object(resource->GetName() + "Texture"),
          image_(std::make_shared<Image>(resource)),
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
        if (image_)
            return image_->IsReady();
        else
            return width_ > 0 && height_ > 0;
    }

    void Texture::AllocateResources()
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);
        glGenTextures(1, &texture_);
        Graphics::this_->SetTexture(0, this);

        if(image_)
        {
            width_ = image_->GetWidth();
            height_ = image_->GetHeight();
            format_ = image_->ConvertFormat2GL();
        }

        if (flags_ & (int)TextureFlag::INVERT_Y)
        {
            if(!image_->FlipVertical())
            {
                TRACE_PRINTF("Cannot flip vertically image = %s!!!\n", image_->GetName().c_str());
            }
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)wrapMode_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)wrapMode_);

        if (!Graphics::this_->IsTextureSizeCorrect(width_, height_))
        {
            if(image_)
            {
                CHECK_CONDITION(!image_->IsCompressed() && "Resize not supported for compressed images!!!", __FILE__, __LINE__);
                image_->MakePowerOf2Size();
                width_ = image_->GetWidth();
                height_ = image_->GetHeight();
            }
            else
                GetPowerOfTwoValues(width_, height_);
        }

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
                         nullptr);
        }

        mipmapLevels_ = 0;
        if (flags_ & (int)TextureFlag::GENERATE_MIPMAPS)
        {
            if(!image_ || !image_->IsCompressed())
            {
                // calculate mipmap levels based on texture size
                unsigned maxSize = std::max(width_, height_);
                while (maxSize)
                {
                    maxSize >>= 1;
                    ++mipmapLevels_;
                }
                glGenerateMipmap(GL_TEXTURE_2D);
            }
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

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    void Texture::ReleaseResources()
    {
        glDeleteTextures(1, &texture_);
        texture_ = 0;
        if (image_)
            return image_->Invalidate();
    }

    PTexture Texture::CreateFrom(PResource resource, const pugi::xml_node& node)
    {
        std::string flags = node.attribute("flags").as_string();
        std::string resourceName = node.attribute("resource").as_string();
        auto res = Resource::Get(resourceName);
        PTexture texture;
        if (!res)
        {
            auto newRes = Resource::Create<ResourceXMLNode>(resourceName);
            texture = std::make_shared<Texture>(newRes);
            newRes->Set(resource, nullptr, "Resources", resourceName);
        }
        else
            texture = std::make_shared<Texture>(res);
        texture->SetName(resourceName);
        texture->SetFlags(flags);
        return texture;
    }

    void Texture::Save(pugi::xml_node& node)
    {
        node.append_attribute("flags") = flags_.to_string().c_str();
        node.append_attribute("resource") = pResource_->GetName().c_str();
    }

    void Texture::SetSize(GLsizei width, GLsizei height)
    {
        CHECK_ASSERT(width >= 0 && height >= 0, __FILE__, __LINE__);
        if (width_ != width || height_ != height)
        {
            width_ = width;
            height_ = height;
            Invalidate();
        }
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