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
#include "GLIncludes.h"
#include <stdlib.h>
#include <sstream>
#include <cassert>

#define SHOW_ASSERT_POPUP_ERROR(msg) ((void)0)

#if _MSC_VER
#include <intrin.h>
#define snprintf _snprintf
#define FORCE_BREAKPOINT() __debugbreak()
#else
#define FORCE_BREAKPOINT() assert(false)
#endif

#if (defined(DEBUG) || defined (_DEBUG)) && !defined(NDEBUG)

#define CHECK_ASSERT(f) if (!(f)) {\
        char buffer[1024];\
        const char* format = "Assert " #f " has failed in file %s line %u\n";\
        snprintf(buffer, 1024, format, (const char*)__FILE__, (unsigned)__LINE__);\
        fprintf(stderr, "*Error*%s\n", buffer);\
        FORCE_BREAKPOINT();\
        exit(1);\
    }

#define CHECK_GL_STATUS() {\
        GLenum status = glGetError();\
        if(status != GL_NO_ERROR)\
        {\
            char buffer[1024];\
            const char* format = "GL has failed with status = 0x%x in file %s line %u\n";\
            snprintf(buffer, 1024, format, status, (const char*)__FILE__, (unsigned)__LINE__);\
            fprintf(stderr, "*Error*%s\n", buffer);\
            FORCE_BREAKPOINT();\
            exit(1);\
        }\
    }
#else
#define CHECK_ASSERT(f) ((void)0)
#define CHECK_GL_STATUS() ((void)0)
#endif

#define CHECK_CONDITION(f) if (!(f)) {\
        char buffer[1024];\
        const char* format = "Assert " #f " has failed in file %s line %u\n";\
        snprintf(buffer, 1024, format, (const char*)__FILE__, (unsigned)__LINE__);\
        fprintf(stderr, "*Error*%s\n", buffer);\
        FORCE_BREAKPOINT();\
        exit(1);\
    }

#define CHECK_CONDITION_ARGS(f, args) if (!(f)) {\
        char buffer[1024];\
        const char* format = "Assert " #f "(" #args ") has failed in file %s line %u\n";\
        snprintf(buffer, 1024, format, (const char*)__FILE__, (unsigned)__LINE__);\
        fprintf(stderr, "*Error*%s\n", buffer);\
        FORCE_BREAKPOINT();\
        exit(1);\
    }

