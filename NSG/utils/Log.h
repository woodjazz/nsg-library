/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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

#if (defined(DEBUG) || defined (_DEBUG)) && !defined(NDEBUG)

#include "Types.h"
#include <stdio.h>
#include <sstream>
#include <string>

#ifdef ANDROID

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "nsg-library", ## __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "nsg-library", ##__VA_ARGS__))
#define TRACE_LOG(msg) {\
        std::stringstream stream; \
        stream << msg; \
        std::string cmsg = stream.str(); \
        __android_log_print(ANDROID_LOG_INFO, "nsg-library", "%s", cmsg.c_str());\
    }

#define TRACE_PRINTF(format, ...) {\
        __android_log_print(ANDROID_LOG_INFO, "nsg-library", format, ##__VA_ARGS__);\
    }

#elif _MSC_VER

#include "windows.h"

#define TRACE_LOG(msg) {\
        std::stringstream stream; \
        stream << msg; \
        std::string cmsg = stream.str(); \
        printf("%s\n",cmsg.c_str());\
        fflush(stdout);\
        OutputDebugString(cmsg.c_str());\
        OutputDebugString("\n");\
    }

#define TRACE_PRINTF(format, ...) {\
        printf(format, __VA_ARGS__);\
        fflush(stdout);\
    }

#elif __APPLE__

#include <Foundation/NSString.h>
#define TRACE_LOG(msg) {\
        std::stringstream stream;\
        stream << msg;\
        std::string cmsg = stream.str();\
        NSLog(@"%@", [NSString stringWithUTF8String: cmsg.c_str()]);\
    }

#define TRACE_PRINTF(format, ...) {\
        printf(format, ##__VA_ARGS__);\
        fflush(stdout);\
    }

#else

#define TRACE_LOG(msg) {\
        std::stringstream stream; \
        stream << msg; \
        std::string cmsg = stream.str(); \
        printf("%s\n",cmsg.c_str());\
        fflush(stdout);\
    }

#define TRACE_PRINTF(format, ...) {\
        printf(format, ##__VA_ARGS__);\
        fflush(stdout);\
    }

#endif
#else
#define TRACE_LOG(msg) ((void)0);
#define TRACE_PRINTF(format, ...) ((void)0);
#endif

