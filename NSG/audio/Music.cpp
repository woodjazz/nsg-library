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
#include "Music.h"
#include "ResourceFile.h"
#include "Util.h"
#include "Check.h"
#ifdef SDL
#include "SDL.h"
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

	Music::Music(const std::string& name)
		: Object(name),
          isPlaying_(false)
    {
    }

    Music::~Music()
    {
        Stop();
#ifdef SDL
        Mix_FreeMusic(music_);
#endif
        CloseAudio();
    }

    bool Music::IsPlaying() const
    {
        return isPlaying_;
    }

    bool Music::IsValid()
    {
        return resource_ && resource_->IsReady();
    }

    void Music::Set(PResource resource)
    {
        if(resource_ != resource)
        {
            resource_ = resource;
            Invalidate();
        }
    }

    void Music::AllocateResources()
    {
#ifdef SDL
            SDL_RWops* assetHandle = SDL_RWFromConstMem(resource_->GetData(), int(resource_->GetBytes()));
#if EMSCRIPTEN
            music_ = Mix_LoadMUS_RW(assetHandle);
            SDL_FreeRW(assetHandle);
#else
            music_ = Mix_LoadMUS_RW(assetHandle, 1);
#endif
            Mix_HookMusicFinished(MusicDone);
            CHECK_CONDITION(music_, __FILE__, __LINE__);
#endif
    }

    void Music::ReleaseResources()
    {
        if (resource_)
            return resource_->Invalidate();
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