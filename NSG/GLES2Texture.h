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

#include "GLES2Includes.h"
#include "GLES2Mesh.h"
#include "Node.h"
#include "Resource.h"
#include "Types.h"
#include "GLES2GPUObject.h"
#include <memory>
#include <string>
#include <vector>

namespace NSG
{
	class BindTexture;
	class GLES2Texture : public GLES2GPUObject
	{
	public:
		GLES2Texture(const char* filename);
		GLES2Texture(GLint format, GLsizei width, GLsizei height, const char* pixels); 
		~GLES2Texture();
		void Bind() { glBindTexture(GL_TEXTURE_2D, texture_); }
		GLuint GetID() const { return texture_; }
		void Show(PGLES2Texture pTexture);
		GLsizei GetWidth() const { return width_; }
		GLsizei GetHeight() const { return height_; }
		GLint GetFormat() const { return format_; }
		virtual bool IsValid();
		virtual void AllocateResources();
		virtual void ReleaseResources();

	private:
		void InitializeCommonSettings();

	protected:
		std::string filename_;
		GLuint texture_;
		PResource pResource_;
        PGLES2Material pMaterial_; // used to show the texture on the screen
		PGLES2Mesh pMesh_; // used to show the texture on the screen
		GLsizei width_;
		GLsizei height_;
		GLint format_;

		friend class BindTexture;
	};

	class BindTexture
	{
	public:
		BindTexture(GLES2Texture& obj);
		~BindTexture();
	private:
		GLES2Texture& obj_;
	};

}