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
#include "AppConfiguration.h"
#if SDL
#include "SDL.h"
#endif

namespace NSG
{
    #if EMSCRIPTEN
      static const int WindowPosX = 0;
      static const int WindowPosY = 0;
      static const int WindowWidth = 640;
      static const int WindowHeight = 480;
    #else
      #if SDL
      static const int WindowPosX = SDL_WINDOWPOS_CENTERED;
      static const int WindowPosY = SDL_WINDOWPOS_CENTERED;
      #else
      static const int WindowPosX = 0;
      static const int WindowPosY = 0;
      #endif
	  static const int WindowWidth = 1024;
	  static const int WindowHeight = 768;
    #endif

    AppConfiguration::AppConfiguration()
        : fps_(60),
          x_(WindowPosX),
          y_(WindowPosY),
          width_(WindowWidth),
          height_(WindowHeight),
          vertical_sync_(true),
          showStatistics_(false),
          pauseMusicOnBackground_(true),
          orgName_("woodjazz"),
          appName_("nsg")
    {
    };
}
