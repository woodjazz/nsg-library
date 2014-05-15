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

#include <stdlib.h>
#include <stdio.h>
#if !defined(__APPLE__)
#include <malloc.h>
#endif
#include <assert.h>
#include <cstring>
#include <string>
#include "GLES2Program.h"
#include "Log.h"
#include "Check.h"
#include "Context.h"

static const char s_fragmentShaderHeader[] = {
#include "GLES2FragmentCompatibility.h"
};

static const char s_vertexShaderHeader[] = {
#include "GLES2VertexCompatibility.h"
};

namespace NSG 
{
	GLES2Program::GLES2Program(PResource pRVShader, PResource pRFShader)
	: pRVShader_(pRVShader),
	pRFShader_(pRFShader)
	{
		CHECK_ASSERT(pRVShader && pRFShader, __FILE__, __LINE__);
		Context::this_->Add(this);
	}

	GLES2Program::GLES2Program(const char* vShader, const char* fShader)
	: pRVShader_(new Resource(vShader, strlen(vShader))),
	pRFShader_(new Resource(fShader, strlen(fShader))),
    vShader_(vShader),
    fShader_(fShader)
	{
		CHECK_ASSERT(vShader && fShader, __FILE__, __LINE__);
		Context::this_->Add(this);
	}

	bool GLES2Program::IsValid()
	{
		return pRVShader_->IsLoaded() && pRFShader_->IsLoaded();
	}

	void GLES2Program::AllocateResources()
	{
		std::string vbuffer;
		size_t vHeaderSize = strlen(s_vertexShaderHeader);
		size_t fHeaderSize = strlen(s_fragmentShaderHeader);
		vbuffer.resize(vHeaderSize + pRVShader_->GetBytes());
		vbuffer = s_vertexShaderHeader;
		memcpy(&vbuffer[0] + vHeaderSize, pRVShader_->GetData(), pRVShader_->GetBytes());
		CHECK_GL_STATUS(__FILE__, __LINE__);
		pVShader_ = PGLES2VShader(new GLES2VShader(vbuffer.c_str()));
		CHECK_GL_STATUS(__FILE__, __LINE__);
		vbuffer = s_fragmentShaderHeader;
		vbuffer.resize(fHeaderSize + pRFShader_->GetBytes());
		memcpy(&vbuffer[0] + fHeaderSize, pRFShader_->GetData(), pRFShader_->GetBytes());
		CHECK_GL_STATUS(__FILE__, __LINE__);
		pFShader_ = PGLES2FShader(new GLES2FShader(vbuffer.c_str()));
		CHECK_GL_STATUS(__FILE__, __LINE__);
	    Initialize();
	}

	void GLES2Program::ReleaseResources()
	{
		glDetachShader(id_, pVShader_->GetId());
		glDetachShader(id_, pFShader_->GetId());
		pVShader_ = nullptr;
		pFShader_ = nullptr;
        glDeleteProgram(id_);
	}

	void GLES2Program::Initialize()
	{
        CHECK_GL_STATUS(__FILE__, __LINE__);

		// Creates the program name/index.
		id_ = glCreateProgram();

		// Will attach the fragment and vertex shaders to the program object.
		glAttachShader(id_, pVShader_->GetId());
		glAttachShader(id_, pFShader_->GetId());

		// Will link the program into OpenGL core.
		glLinkProgram(id_);

		GLint link_status = GL_FALSE;

		glGetProgramiv(id_, GL_LINK_STATUS, &link_status);

		if (link_status != GL_TRUE) 
		{
			GLint logLength = 0;

			// Instead use GL_INFO_LOG_LENGTH we could use COMPILE_STATUS.
			// I prefer to take the info log length, because it'll be 0 if the
			// shader was successful compiled. If we use COMPILE_STATUS
			// we will need to take info log length in case of a fail anyway.
			glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &logLength);

			if (logLength > 0)
			{
				// Allocates the necessary memory to retrieve the message.
				GLchar *log = (GLchar *)malloc(logLength);

				// Get the info log message.
				glGetProgramInfoLog(id_, logLength, &logLength, log);

				TRACE_LOG("Error in Program Creation: " << log);

                assert(false);

				// Frees the allocated memory.
				free(log);
			}
		}

        CHECK_GL_STATUS(__FILE__, __LINE__);
	}

	GLES2Program::~GLES2Program()
	{
		Context::this_->Remove(this);
	}

	GLuint GLES2Program::GetAttributeLocation(const std::string& name) 
	{
		return glGetAttribLocation(id_, name.c_str());
	}

	GLuint GLES2Program::GetUniformLocation(const std::string& name)
	{
		return glGetUniformLocation(id_, name.c_str());
	}

	UseProgram::UseProgram(GLES2Program& obj)
	: obj_(obj)
	{
		obj.Use();
	}

	UseProgram::~UseProgram()
	{
		glUseProgram(0);
	}

}
