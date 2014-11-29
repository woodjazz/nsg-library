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
#include "Music.h"
#include "ResourceFile.h"
#include "Util.h"
#include "Check.h"
#ifdef SDL
#include "SDL_mixer.h"
#endif

namespace NSG
{
	template<> Music* Singleton<Music>::this_ = nullptr;

#ifdef SDL
    static Mix_Music* music_ = nullptr;
    static void MusicDone()
    {
        Music::this_->Stop();
    }
#endif

	Music::Music(PResource resource)
		: resource_(resource),
          isReady_(false),
          isPlaying_(false)
    {
    }

    Music::~Music()
    {
        Stop();
#ifdef SDL
        Mix_FreeMusic(music_);
#endif
    }

    bool Music::IsPlaying() const
    {
        return isPlaying_;
    }

    bool Music::IsReady()
    {
        if (resource_->IsReady() && !isReady_)
        {
#ifdef SDL
            SDL_RWops* assetHandle = SDL_RWFromConstMem(resource_->GetData(), resource_->GetBytes());

#if EMSCRIPTEN
            music_ = Mix_LoadMUS_RW(assetHandle);
            SDL_FreeRW(assetHandle);
#else
            music_ = Mix_LoadMUS_RW(assetHandle, 1);
#endif

            Mix_HookMusicFinished(MusicDone);

            if (!music_)
            {
				TRACE_LOG("Unable to read " << resource_->GetPath().GetFilePath() << " !!!");
            }

            isReady_ = music_ != nullptr;
#endif
        }

        return isReady_;
    }

    bool Music::Play(bool loop)
    {
        if (IsReady())
        {

#ifdef SDL
            Stop();
            Mix_PlayMusic(music_, loop ? -1 : 0);
            isPlaying_ = true;
            return true;
#endif
        }

        return false;
    }

    void Music::Stop()
    {
#ifdef SDL
        if (IsPlaying())
        {
            if (Mix_PlayingMusic())
                Mix_HaltMusic();

            isPlaying_ = false;
        }
#endif
    }

    void Music::Pause()
    {
#ifdef SDL
        if (IsPlaying() && Mix_PlayingMusic())
            Mix_PauseMusic();
#endif
    }

    void Music::Resume()
    {
#ifdef SDL
        if (IsPlaying() && Mix_PausedMusic())
            Mix_ResumeMusic();
#endif
    }

    bool Music::IsPaused() const
    {
#ifdef SDL        
        return Mix_PausedMusic() ? true : false;
#else
        return false;        
#endif        
    }
}