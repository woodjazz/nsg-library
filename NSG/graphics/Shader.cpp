/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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
#include <stdio.h>
#include <stdlib.h>
#if !defined(IS_TARGET_APPLE)
#include <malloc.h>
#endif
#include "Check.h"
#include "Log.h"
#include <assert.h>

namespace NSG {

Shader::Shader(GLenum type, const char* source) : source_(source) {
    CHECK_GL_STATUS();
    id_ = glCreateShader(type);
    glShaderSource(id_, 1, &source, NULL);
    glCompileShader(id_);
    GLint compile_status = GL_FALSE;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE) {
        GLint logLength = 0;
        glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            std::string log;
            log.resize(logLength);
            glGetShaderInfoLog(id_, logLength, &logLength, &log[0]);
            LOGE("Shader creation failed: %s", log.c_str());
            // LOGE("%s", source);
            auto e = glGetError();
            glDeleteShader(id_);
            throw GLException(e);
        }
    }
    CHECK_GL_STATUS();
}

Shader::~Shader() { glDeleteShader(id_); }
}
