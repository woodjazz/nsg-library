/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "RenderingContext.h"
#include "ResourceFile.h"
#include "Texture.h"
#include "Path.h"
#include "Util.h"
#include "StringConverter.h"
#include "pugixml.hpp"
#include <algorithm>
#include <cerrno>

namespace NSG
{
    Texture::Texture(const std::string& name)
        : Object(name),
          flags_((int)TextureFlag::NONE),
          texture_(0),
          width_(0),
          height_(0),
          format_(RenderingContext::GetTexelFormatType()),
          type_(RenderingContext::GetTexelDataType()),
          channels_(0),
          serializable_(false),
          wrapMode_(TextureWrapMode::CLAMP_TO_EDGE),
          mipmapLevels_(0),
          filterMode_(TextureFilterMode::BILINEAR),
          blendType_(TextureBlend::NONE),
          mapType_(TextureType::COL),
          useAlpha_(false),
          uvTransform_(1, 1, 0, 0)
    {
    }

    Texture::Texture(PResource resource, const TextureFlags& flags)
        : Object(resource->GetName() + "Texture"),
          image_(std::make_shared<Image>(resource)),
          flags_(flags),
          texture_(0),
          pResource_(resource),
          width_(0),
          height_(0),
          format_(RenderingContext::GetTexelFormatType()),
          type_(RenderingContext::GetTexelDataType()),
          channels_(0),
          serializable_(true),
          wrapMode_(TextureWrapMode::CLAMP_TO_EDGE),
          mipmapLevels_(0),
          filterMode_(TextureFilterMode::BILINEAR),
          blendType_(TextureBlend::NONE),
          mapType_(TextureType::COL),
          useAlpha_(false),
          uvTransform_(1, 1, 0, 0)
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

    void Texture::SetSerializable(bool serializable)
    {
        serializable_ = serializable;
    }

    bool Texture::IsSerializable() const
    {
        return serializable_;
    }

    void Texture::FlipY()
    {
        if (flags_ & (int)TextureFlag::INVERT_Y)
            flags_ &= ~(int)TextureFlag::INVERT_Y;
        else
            flags_ |= (int)TextureFlag::INVERT_Y;
        Invalidate();
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
        CHECK_GL_STATUS();
        glGenTextures(1, &texture_);
        RenderingContext::GetPtr()->SetTexture(0, this);

		auto maxSize = RenderingContext::GetPtr()->GetMaxTextureSize();
		width_ = Clamp(width_, 0, maxSize);
		height_ = Clamp(height_, 0, maxSize);
		if (!RenderingContext::GetPtr()->IsTextureSizeCorrect(width_, height_))
			GetPowerOfTwoValues(width_, height_);

        if (image_)
        {
            channels_ = image_->GetChannels();
            width_ = image_->GetWidth();
            height_ = image_->GetHeight();
            format_ = image_->ConvertFormat2GL();
            if (flags_ & (int)TextureFlag::INVERT_Y)
            {
                if (!image_->FlipVertical())
                    LOGE("Cannot flip vertically image = %s", image_->GetName().c_str());
            }
        }
        else
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
                    CHECK_ASSERT(false && "Unknown format!");
                    break;
            }
        }

        if (GetTarget() == GL_TEXTURE_CUBE_MAP)
        {
            auto value = std::max(width_, height_);
            width_ = height_ = value;
        }

        CHECK_ASSERT(RenderingContext::GetPtr()->IsTextureSizeCorrect(width_, height_));
        CHECK_ASSERT(RenderingContext::GetPtr()->GetMaxTextureSize() >= width_ && RenderingContext::GetPtr()->GetMaxTextureSize() >= height_);

        switch (wrapMode_)
        {
            case TextureWrapMode::CLAMP_TO_EDGE:
                glTexParameteri(GetTarget(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GetTarget(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                break;
            case TextureWrapMode::MIRRORED_REPEAT:
                glTexParameteri(GetTarget(), GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                glTexParameteri(GetTarget(), GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                break;
            case TextureWrapMode::REPEAT:
                glTexParameteri(GetTarget(), GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GetTarget(), GL_TEXTURE_WRAP_T, GL_REPEAT);
                break;
            default:
                CHECK_ASSERT(false);
                break;
        }

        Define();

        mipmapLevels_ = 0;
        if (flags_ & (int)TextureFlag::GENERATE_MIPMAPS)
        {
            if (!image_ || !image_->IsCompressed())
            {
                // calculate mipmap levels based on texture size
                unsigned maxSize = std::max(width_, height_);
                while (maxSize)
                {
                    maxSize >>= 1;
                    ++mipmapLevels_;
                }
                glGenerateMipmap(GetTarget());
            }
        }

        switch (filterMode_)
        {
            case TextureFilterMode::NEAREST:
                {
                    glTexParameteri(GetTarget(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GetTarget(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    break;
                }
            case TextureFilterMode::BILINEAR:
                {
                    if (mipmapLevels_ < 2)
                        glTexParameteri(GetTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    else
                        glTexParameteri(GetTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                    glTexParameteri(GetTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    break;
                }
            case TextureFilterMode::TRILINEAR:
                {
                    if (mipmapLevels_ < 2)
                        glTexParameteri(GetTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    else
                        glTexParameteri(GetTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    glTexParameteri(GetTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    break;
                }
            default:
                CHECK_ASSERT(false);
                break;
        }

       // if (image_)
         //   image_->Invalidate(); // free mem

        CHECK_GL_STATUS();
    }

    void Texture::ReleaseResources()
    {
        glDeleteTextures(1, &texture_);
        texture_ = 0;
    }

    std::string Texture::TranslateFlags() const
    {
        std::string ss;

        if ((int)TextureFlag::GENERATE_MIPMAPS & flags_)
            ss = " GENERATE_MIPMAPS";

        if ((int)TextureFlag::INVERT_Y & flags_)
            ss = " INVERT_Y" + ss;

        return ss;
    }

    void Texture::Save(pugi::xml_node& node)
    {
        node.append_attribute("flags") = flags_.to_string().c_str();
        node.append_attribute("flagNames") = TranslateFlags().c_str();
        node.append_attribute("resource") = pResource_->GetName().c_str();
        node.append_attribute("uvName") = uvName_.c_str();
        node.append_attribute("wrapMode") = ToString(wrapMode_);
        node.append_attribute("filterMode") = ToString(filterMode_);
        node.append_attribute("blendType") = ToString(blendType_);
        node.append_attribute("mapType") = ToString(mapType_);
        node.append_attribute("useAlpha").set_value(useAlpha_);
        node.append_attribute("uvTransform").set_value(ToString(uvTransform_).c_str());
    }

    void Texture::SetSize(GLsizei width, GLsizei height)
    {
        CHECK_ASSERT(!image_ && "SetSize only can be applied for non images!!!");
        CHECK_ASSERT(width >= 0 && height >= 0);

        if (GetTarget() == GL_TEXTURE_CUBE_MAP)
        {
            auto value = std::max(width, height);
            width = height = value;
        }

        if (width_ != width || height_ != height)
        {
            width_ = width;
            height_ = height;
            Invalidate();
        }
    }

    void Texture::SetFormat(GLint format)
    {
        if (format != format_)
        {
            CHECK_ASSERT(format == GL_ALPHA || format == GL_RGB 
                || format == GL_RGBA || format == GL_DEPTH_COMPONENT);
            format_ = format;
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

    void Texture::SetUVTransform(const Vector4& uvTransform)
    {
        uvTransform_ = uvTransform;
    }
}
