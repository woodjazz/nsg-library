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

#include <memory>
#include <string>
#include "GLES2Includes.h"
#include "GLES2VShader.h"
#include "GLES2FShader.h"
#include "Resource.h"
#include "GLES2GPUObject.h"

namespace NSG 
{
	class UseProgram;
	class GLES2Program : public GLES2GPUObject
	{
	public:

		GLES2Program(PResource pRVShader, PResource pRFShader);
		GLES2Program(const char* vShader, const char* fShader);
		~GLES2Program();
		void Initialize();
		GLuint GetAttributeLocation(const std::string& name);
		GLuint GetUniformLocation(const std::string& name);
		virtual bool IsValid();
		virtual void AllocateResources();
		virtual void ReleaseResources();
	private:
		operator const GLuint() const { return id_; }
		GLuint GetId() const { return id_; }
		void Use() { glUseProgram(id_); }
		GLuint id_;
		PGLES2VShader pVShader_;
		PGLES2FShader pFShader_;
		PResource pRVShader_;
		PResource pRFShader_;
		const char* vShader_;
		const char* fShader_;
		friend class UseProgram;
	};

	typedef std::shared_ptr<GLES2Program> PGLES2Program;

	class UseProgram
	{
	public:
		UseProgram(GLES2Program& obj);
		~UseProgram();
	private:
		GLES2Program& obj_;
	};

}
