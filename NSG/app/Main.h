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
#include "App.h"
#include "NSG/app/AppNaCl.h"
#include "MemoryTest.h"
#ifdef NACL
#define NSG_MAIN(ClassName)\
    namespace pp\
    {\
        Module* CreateModule()\
        {\
            return new NSG::NaCl::Graphics3DModule(new ClassName);\
        }\
    }
#elif defined(ANDROID) && !defined(SDL)
#define NSG_MAIN(ClassName)\
    struct android_app;\
    namespace NSG\
    {\
        extern void CreateModule(struct android_app* state, App* pApp);\
    }\
    extern "C" void android_main(struct android_app* state)\
    {\
        NSG::CreateModule(state, new ClassName);\
    }
#elif IOS || ANDROID
#define NSG_MAIN(ClassName)\
    namespace NSG\
    {\
        extern bool CreateModule(App* pApp);\
    }\
    extern "C" int SDL_main(int argc, char** argv); \
    int SDL_main(int argc, char** argv) \
    {\
        NSG::CreateModule(new ClassName);\
        return 0;\
    }
#else
#define NSG_MAIN(ClassName)\
    namespace NSG\
    {\
        extern bool CreateModule(App* pApp);\
    }\
    int main(int argc, char * argv[])\
    {\
        ClassName* objApp(new ClassName);\
        objApp->SetCommandLineParameters(argc, argv);\
        NSG::CreateModule(objApp);\
        return 0;\
    }
#endif


