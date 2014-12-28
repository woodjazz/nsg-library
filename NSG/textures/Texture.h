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
#pragma once
#include "Types.h"
#include "Object.h"

namespace NSG
{
    class Texture : public Object
    {
    public:
        Texture(PResourceFile resource);
        Texture(GLint format, GLsizei width, GLsizei height, const char* pixels);
        Texture(PResourceMemory resource);
        virtual ~Texture();
		GLuint GetID() const;
		GLsizei GetWidth() const;
		GLsizei GetHeight() const;
		GLint GetFormat() const;
		int GetChannels() const;
        void Save(pugi::xml_node& node);
        static PTexture CreateFrom(const pugi::xml_node& node);
        void SetSerializable(bool serializable);
        bool IsSerializable() const;
        void SetFlags(const TextureFlags& flags);
        TextureFlags GetFlags() const { return flags_; }
        void SetWrapMode(TextureWrapMode mode);
        void SetFilterMode(TextureFilterMode mode);
    private:
        virtual bool IsValid() override;
        virtual void AllocateResources() override;
        virtual void ReleaseResources() override;
        const unsigned char* GetImageData();
    protected:
        bool fromKnownImgFormat_; // true if it is a known file format (png, bmp, jpeg,...)
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
		friend class TextureFileManager;
    };
}