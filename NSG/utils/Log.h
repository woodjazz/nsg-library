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
        fprintf(stdout, "*Info*" format "\n", ##__VA_ARGS__);\
        fflush(stdout);\
    }

#else //(defined(DEBUG) || defined (_DEBUG)) && !defined(NDEBUG)

#define LOGI(format, ...) ((void)0);

#endif

#define LOGW(format, ...) {\
        emscripten_log(EM_LOG_WARN, format, ##__VA_ARGS__);\
        fprintf(stderr, "*Warning*" format "\n", ##__VA_ARGS__);\
        fflush(stderr);\
    }

#define LOGE(format, ...) {\
        emscripten_log(EM_LOG_ERROR, format, ##__VA_ARGS__);\
        fprintf(stderr, "*Error*" format "\n", ##__VA_ARGS__);\
        fflush(stderr);\
    }

#else //(defined(DEBUG) || defined (_DEBUG)) && !defined(NDEBUG)

	#if _WIN32
		#define SHOWINDEBUGWINDOW(format, ...) {\
					int n = snprintf(nullptr, 0, format "\n", ##__VA_ARGS__);\
					fflush(stdout);\
					if(n > 0) {\
						std::vector<char> buffer(n + 1);\
						sprintf(&buffer[0], format "\n", ##__VA_ARGS__);\
						OutputDebugStringA(&buffer[0]);\
					}\
				}
	#else
		#define SHOWINDEBUGWINDOW(format, ...) ((void)0)
	#endif

	#if (defined(DEBUG) || defined (_DEBUG)) && !defined(NDEBUG)

		#define LOGI(format, ...) {\
					int n = fprintf(stdout, "*Info*" format "\n", ##__VA_ARGS__);\
					fflush(stdout);\
					SHOWINDEBUGWINDOW("*Info*" format, ##__VA_ARGS__);\
				}
	#else

		#define LOGI(format, ...) ((void)0);

	#endif

	#define LOGW(format, ...) {\
			fprintf(stderr, "*Warning*" format "\n", ##__VA_ARGS__);\
			fflush(stderr);\
			SHOWINDEBUGWINDOW("*Warning*" format, ##__VA_ARGS__);\
		}

	#define LOGE(format, ...) {\
			fprintf(stderr, "*Error*" format "\n", ##__VA_ARGS__);\
			fflush(stderr);\
			SHOWINDEBUGWINDOW("*Error*" format, ##__VA_ARGS__);\
		}

#endif


