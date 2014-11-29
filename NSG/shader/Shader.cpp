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

#include "Shader.h"
#include <stdlib.h>
#include <stdio.h>
#if !defined(__APPLE__)
#include <malloc.h>
#endif
#include <assert.h>
#include "Log.h"
#include "Check.h"

namespace NSG 
{

	Shader::Shader(GLenum type, const char* source)
	: source_(source)
	{
        CHECK_GL_STATUS(__FILE__, __LINE__);

		// Creates a Shader Object and returns its name/id.
		id_ = glCreateShader(type);

		// Uploads the source to the Shader Object.
		glShaderSource(id_, 1, &source, NULL);

		// Compiles the Shader Object.
		glCompileShader(id_);

		GLint compile_status = GL_FALSE;

		glGetShaderiv(id_, GL_COMPILE_STATUS, &compile_status);

		if(compile_status != GL_TRUE)
		{
			GLint logLength = 0;

			// Instead use GL_INFO_LOG_LENGTH we could use COMPILE_STATUS.
			// I prefer to take the info log length, because it'll be 0 if the
			// shader was successful compiled. If we use COMPILE_STATUS
			// we will need to take info log length in case of a fail anyway.
			glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &logLength);

        
			if (logLength > 0)
			{
				// Allocates the necessary memory to retrieve the message.
				GLchar* log = (GLchar*)malloc(logLength);

				// Get the info log message.
				glGetShaderInfoLog(id_, logLength, &logLength, log);

				TRACE_LOG("Shader source:" << source);
				
				// Shows the message in console.
				TRACE_LOG("Error in Shader Creation: " << log);

                assert(false);

				// Frees the allocated memory.
				free(log);
			}
		}

        CHECK_GL_STATUS(__FILE__, __LINE__);
	}

	Shader::~Shader()
	{
		glDeleteShader(id_);
	}
}
