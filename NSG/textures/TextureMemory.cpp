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
#include "Graphics.h"
#include "pugixml.hpp"
#include <vector>

namespace NSG
{
    TextureMemory::TextureMemory(GLint format, GLsizei width, GLsizei height, const char* pixels)
    : Texture(Flag::NONE)
    {
        width_ = width;
        height_ = height;
        format_ = format;

        switch (format_)
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

    TextureMemory::~TextureMemory()
    {
        Context::RemoveObject(this);
    }

    const unsigned char* TextureMemory::GetImageData()
    {
       const unsigned char* img = nullptr;

        if (pResource_->GetBytes())
        {
            CHECK_ASSERT(width_ * height_ * channels_ == pResource_->GetBytes(), __FILE__, __LINE__);
            img = (const unsigned char*)pResource_->GetData();
        }

        return img;
    }

    void TextureMemory::Save(pugi::xml_node& node)
    {
        node.append_attribute("type") = "TextureMemory";
        node.append_attribute("flags") = flags_.to_string().c_str();

        {
            std::stringstream ss;
            ss << format_;
            node.append_attribute("format") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << width_;
			node.append_attribute("width") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << height_;
			node.append_attribute("height") = ss.str().c_str();
        }

        CHECK_ASSERT(!pResource_->GetBytes(), __FILE__, __LINE__);
    }
}