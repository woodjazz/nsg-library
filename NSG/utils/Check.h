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
#include "Log.h"
#include "GLES2Includes.h"
#include <stdlib.h>
#include <sstream>

#if defined(SDL) && !defined(EMSCRIPTEN)
#include "SDL.h"
#undef main
#define SHOW_ASSERT_POPUP_ERROR(msg) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Assert failed", msg, nullptr)
#else
#define SHOW_ASSERT_POPUP_ERROR(msg) ((void)0)
#endif

#if _MSC_VER
#include <intrin.h>
#define FORCE_BREAKPOINT() __debugbreak()
#else
#define FORCE_BREAKPOINT() assert(false)
#endif

#if (defined(IS_DEBUG) || defined(DEBUG) || defined (_DEBUG)) && !defined(NDEBUG)
#define CHECK_ASSERT(f, file, line) if (!(f)) {\
        ::std::stringstream stream;\
        stream << "Assert has failed in file " << file << " line " << line;\
        TRACE_LOG((#f) << stream.str());\
        SHOW_ASSERT_POPUP_ERROR(stream.str().c_str());\
        FORCE_BREAKPOINT();\
        exit(1);\
    }

#define CHECK_GL_STATUS(file, line) {\
        GLenum status = glGetError();\
        if(status != GL_NO_ERROR)\
        {\
            ::std::stringstream stream;\
            stream << "GL has failed with status = 0x" << std::hex << status << " in file " << file << " line " << std::dec << line;\
            TRACE_LOG(stream.str());\
            SHOW_ASSERT_POPUP_ERROR(stream.str().c_str());\
            FORCE_BREAKPOINT();\
            exit(1);\
        }\
    }
#else
#define CHECK_ASSERT(f, file, line) ((void)0)
#define CHECK_GL_STATUS(file, line) ((void)0)
#endif

#define CHECK_CONDITION(f, file, line) if (!(f)) {\
        ::std::stringstream stream;\
        stream << "Assert has failed in file " << file << " line " << line;\
        TRACE_LOG((#f) << stream.str());\
        SHOW_ASSERT_POPUP_ERROR(stream.str().c_str());\
        FORCE_BREAKPOINT();\
        exit(1);\
    }
