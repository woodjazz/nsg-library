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
#include "GPUObject.h"
#include "FlagSet.h"

namespace NSG
{
	class Texture : public GPUObject
	{
	public:
		enum Flag {NONE=0, GENERATE_MIPMAPS = 1};
		typedef FlagSet<Flag> Flags;
		Texture(Flags flags); 
		virtual ~Texture();
		GLuint GetID() const { return texture_; }
		GLsizei GetWidth() const { return width_; }
		GLsizei GetHeight() const { return height_; }
		GLint GetFormat() const { return format_; }
		int GetChannels() const { return channels_; }
		virtual bool IsValid() override;
		virtual void AllocateResources() override;
		virtual void ReleaseResources() override;
		virtual const unsigned char* GetImageData() = 0;
		virtual void FreeImageData(const unsigned char* img) {}
		virtual void Save(pugi::xml_node& node) = 0;
		static PTexture CreateFrom(const pugi::xml_node& node);
		void SetSerializable(bool serializable) { serializable_ = serializable; }
		bool IsSerializable() const { return serializable_; }
	protected:
		Flags flags_;
		GLuint texture_;
		PResource pResource_;
		GLsizei width_;
		GLsizei height_;
		GLint format_;
		GLenum type_;
		int channels_;
		bool serializable_;
	};
}