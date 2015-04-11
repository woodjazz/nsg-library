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
#include "Audio.h"
#include "Check.h"
#include "Util.h"
#if EMSCRIPTEN
#include <emscripten.h>
#endif

#ifdef SDL
#include "SDL.h"
#include "SDL_mixer.h"
#endif

namespace NSG
{
    void OpenAudio()
    {
        #ifdef SDL
        {
            if (SDL_InitSubSystem(SDL_INIT_AUDIO))
                TRACE_LOG("SDL_INIT_AUDIO Error: " << SDL_GetError() << std::endl);

            int audio_rate = MIX_DEFAULT_FREQUENCY;
            Uint16 audio_format = MIX_DEFAULT_FORMAT;
            int audio_channels = 2;

            if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, 4096))
                TRACE_LOG("Unable to open audio!");
        }
        #endif
    }

    void CloseAudio()
    {
        #ifdef SDL
        {
            Mix_CloseAudio();
            SDL_QuitSubSystem(SDL_INIT_AUDIO);
        }
        #endif
    }
}