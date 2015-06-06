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

#include "Types.h"
#include <stdio.h>
#include <sstream>
#include <string>

#if _WIN32
#include <windows.h>
#endif

#ifdef ANDROID

#include <android/log.h>

#if (defined(DEBUG) || defined (_DEBUG)) && !defined(NDEBUG)

#define LOGI(format, ...) {\
        __android_log_print(ANDROID_LOG_INFO, "nsg-library", "*Info*" format "\n", ##__VA_ARGS__);\
    }

#else //(defined(DEBUG) || defined (_DEBUG)) && !defined(NDEBUG)

#define LOGI(format, ...) ((void)0);

#endif


#define LOGW(format, ...) {\
        __android_log_print(ANDROID_LOG_WARN, "nsg-library", "*Warning*" format "\n", ##__VA_ARGS__);\
    }

#define LOGE(format, ...) {\
        __android_log_print(ANDROID_LOG_ERROR, "nsg-library", "*Error*" format "\n", ##__VA_ARGS__);\
    }

#elif EMSCRIPTEN

#include <emscripten.h>

#if (defined(DEBUG) || defined (_DEBUG)) && !defined(NDEBUG)

#define LOGI(format, ...) {\
        emscripten_log(EM_LOG_CONSOLE, format, ##__VA_ARGS__);\
        printf("*Info*" format "\n", ##__VA_ARGS__);\
        fflush(stdout);\
    }

#else //(defined(DEBUG) || defined (_DEBUG)) && !defined(NDEBUG)

#define LOGI(format, ...) ((void)0);

#endif

#define LOGW(format, ...) {\
        emscripten_log(EM_LOG_WARN, format, ##__VA_ARGS__);\
        printf("*Warning*" format "\n", ##__VA_ARGS__);\
        fflush(stdout);\
    }

#define LOGE(format, ...) {\
        emscripten_log(EM_LOG_ERROR, format, ##__VA_ARGS__);\
        printf("*Error*" format "\n", ##__VA_ARGS__);\
        fflush(stdout);\
    }

#else //(defined(DEBUG) || defined (_DEBUG)) && !defined(NDEBUG)

#if (defined(DEBUG) || defined (_DEBUG)) && !defined(NDEBUG)

#define LOGI(format, ...) {\
        printf("*Info*" format "\n", ##__VA_ARGS__);\
        fflush(stdout);\
    }
#else

#define LOGI(format, ...) ((void)0);

#endif

#define LOGW(format, ...) {\
        printf("*Warning*" format "\n", ##__VA_ARGS__);\
        fflush(stdout);\
    }

#define LOGE(format, ...) {\
        printf("*Error*" format "\n", ##__VA_ARGS__);\
        fflush(stdout);\
    }

#endif


