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
#pragma once
#include "Types.h"
#include "Object.h"

namespace NSG
{
    class Texture : public Object
    {
    public:
        Texture(PResource resource, const TextureFlags& flags = (int)TextureFlag::NONE);
        Texture(const std::string& name, GLint format);
        virtual ~Texture();
        GLuint GetID() const;
        GLsizei GetWidth() const;
        GLsizei GetHeight() const;
        void Save(pugi::xml_node& node);
        static PTexture CreateFrom(PResource resource, const pugi::xml_node& node);
        void SetSerializable(bool serializable);
        bool IsSerializable() const;
        void SetFlags(const TextureFlags& flags);
        TextureFlags GetFlags() const { return flags_; }
        void SetWrapMode(TextureWrapMode mode);
        void SetFilterMode(TextureFilterMode mode);
        PResource GetResource() const { return pResource_;}
        void SetSize(GLsizei width, GLsizei height);
        void SetName(const std::string& name) { name_ = name; }
    private:
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
    protected:
        PImage image_;
        TextureFlags flags_;
        GLuint texture_;
        PResource pResource_;
        GLsizei width_;
        GLsizei height_;
        GLint format_;       
        GLenum type_;
        int channels_;
        bool serializable_;
        TextureWrapMode wrapMode_;
        int mipmapLevels_;
        TextureFilterMode filterMode_;
    };
}