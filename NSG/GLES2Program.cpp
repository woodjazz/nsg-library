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
#include <malloc.h>
#include <assert.h>

#include "GLES2Program.h"

namespace NSG 
{
	GLES2Program::GLES2Program(const char* vShader, const char* fShader)
	: pVShader_(new GLES2VShader(vShader)),
		pFShader_(new GLES2FShader(fShader))
	{
		Initialize();
	}

		GLES2Program::GLES2Program(PGLES2VShader pVShader, PGLES2FShader pFShader)
	: pVShader_(std::move(pVShader)),
		pFShader_(std::move(pFShader))
	{

		Initialize();
	}

	void GLES2Program::Initialize()
	{
		// Creates the program name/index.
		id_ = glCreateProgram();

		// Will attach the fragment and vertex shaders to the program object.
		glAttachShader(id_, pVShader_->GetId());
		glAttachShader(id_, pFShader_->GetId());

		// Will link the program into OpenGL core.
		glLinkProgram(id_);

		GLint link_status;
		glGetProgramiv(id_, GL_LINK_STATUS, &link_status);

		if (link_status != GL_TRUE) 
		{
			GLint logLength;

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

				// Shows the message in console.
				printf("Error in Program Creation:\n%s\n",log);

				// Frees the allocated memory.
				free(log);

				assert(false);
			}
		}
	}

	GLES2Program::~GLES2Program()
	{
		glDeleteProgram(id_);
	}

	GLuint GLES2Program::GetAttributeLocation(const std::string& name) 
	{
		return glGetAttribLocation(id_, name.c_str());
	}

	GLuint GLES2Program::GetUniformLocation(const std::string& name)
	{
		return glGetUniformLocation(id_, name.c_str());
	}
}