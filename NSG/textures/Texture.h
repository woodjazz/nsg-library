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
#pragma once
#include "Types.h"
#include "Object.h"

namespace NSG
{
    class Texture : public Object
    {
    public:
        virtual ~Texture();
        GLuint GetID() const;
        GLsizei GetWidth() const;
        GLsizei GetHeight() const;
        void Save(pugi::xml_node& node);
        void SetSerializable(bool serializable);
        bool IsSerializable() const;
        void SetFormat(GLint format);
        void SetFlags(const TextureFlags& flags);
        void FlipY();
        TextureFlags GetFlags() const { return flags_; }
        TextureWrapMode GetWrapMode() const { return wrapMode_;}
        void SetWrapMode(TextureWrapMode mode);
        void SetFilterMode(TextureFilterMode mode);
        PResource GetResource() const { return pResource_;}
        void SetSize(GLsizei width, GLsizei height);
        void SetName(const std::string& name) { name_ = name; }
        void SetUVName(const std::string& name) { uvName_ = name; }
        const std::string& GetUVName() const { return uvName_; }
        std::string TranslateFlags() const;
        void SetBlendType(TextureBlend blendType) { blendType_ = blendType; }
        TextureBlend GetBlendType() const { return blendType_;}
        void SetMapType(TextureType mapType) { mapType_ = mapType; }
        TextureType GetMapType() const { return mapType_; }
        int GetChannels() const { return channels_; }
        virtual GLenum GetTarget() const = 0;
        virtual void Define() = 0;
        void SetUseAlpha(bool useAlpha) { useAlpha_ = useAlpha; }
        bool GetUseAlpha() const { return useAlpha_; }
        const Vector4& GetUVTransform() const { return uvTransform_; };
        void SetUVTransform(const Vector4& uvTransform);
    protected:
        Texture(PResource resource, const TextureFlags& flags);
        Texture(const std::string& name);
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
        std::string uvName_;
        TextureBlend blendType_;
        TextureType mapType_;
        bool useAlpha_;
        Vector4 uvTransform_;
    };
}