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
#include "Sound.h"
#include "Audio.h"
#include "ResourceFile.h"
#include "Util.h"
#include "Path.h"
#include "Check.h"
#ifdef SDL
#include "SDL_mixer.h"
#endif

namespace NSG
{
	Sound::Sound(PResource resource)
        : resource_(resource),
          isReady_(false),
          sound_(nullptr),
          channel_(-1)
    {
        OpenAudio();
    }

    Sound::~Sound()
    {
        Stop();
#ifdef SDL        
        Mix_FreeChunk(sound_);
#endif        
        CloseAudio();
    }

    bool Sound::IsPlaying() const
    {
#ifdef SDL        
        return Mix_Playing(channel_) ? true : false;
#else
        return false;
#endif        
    }

    bool Sound::IsReady()
    {
        if (resource_->IsReady() && !isReady_)
        {
#ifdef SDL
            SDL_RWops* assetHandle = SDL_RWFromConstMem(resource_->GetData(), resource_->GetBytes());

            sound_ = Mix_LoadWAV_RW(assetHandle, 1);

            if (!sound_)
            {
				TRACE_LOG("Unable to read " << resource_->GetName() << " !!!");
            }

            isReady_ = sound_ != nullptr;
#endif
        }

        return isReady_;
    }

    bool Sound::Play(bool loop)
    {
        if (IsReady())
        {
#ifdef SDL
            Stop();
            channel_ = Mix_PlayChannel(-1, sound_, loop ? -1 : 0);
            return true;
#endif
        }

        return false;
    }

    void Sound::Stop()
    {
#ifdef SDL
        if (IsPlaying())
        {
            Mix_HaltChannel(channel_);
            channel_ = -1;
        }
#endif
    }

    void Sound::Pause()
    {
#ifdef SDL
        if (IsPlaying())
        {
            Mix_Pause(channel_);
        }
#endif
    }

    void Sound::Resume()
    {
#ifdef SDL
        if (Mix_Paused(channel_))
        {
            Mix_Resume(channel_);
        }
#endif
    }
}