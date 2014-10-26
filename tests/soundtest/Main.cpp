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

#include "NSG.h"
using namespace NSG;

struct Test : public App
{
	PMusic music_;
    PSound sound_;
    PSound soundExplo_;
    float time_;

	void Start(int argc, char* argv[]) override
    {
        time_ = 0;
        #if defined(__APPLE__)
            music_ = PMusic(new Music(GetOrCreateResourceFile("../data/nice_music.ogg")));
            sound_ = PSound(new Sound(GetOrCreateResourceFile("../data/PlayerFist.wav")));
            soundExplo_ = PSound(new Sound(GetOrCreateResourceFile("../data/BigExplosion.wav")));
        #else
    		music_ = PMusic(new Music(GetOrCreateResourceFile("data/nice_music.ogg")));
    		sound_ = PSound(new Sound(GetOrCreateResourceFile("data/PlayerFist.wav")));
    		soundExplo_ = PSound(new Sound(GetOrCreateResourceFile("data/BigExplosion.wav")));
        #endif
    }

    void Update() override
    {
        if (sound_->IsReady() && music_->IsReady() && soundExplo_->IsReady())
        {
            if (time_ == 0)
            {
                CHECK_ASSERT(!music_->IsPlaying(), __FILE__, __LINE__);
                music_->Play(false);
                CHECK_ASSERT(music_->IsPlaying(), __FILE__, __LINE__);
            }
            else if (time_ > 8)
            {
                music_->Resume();
                CHECK_ASSERT(!music_->IsPaused(), __FILE__, __LINE__);
            }
            else if (time_ > 5)
            {
                music_->Pause();
                CHECK_ASSERT(music_->IsPaused(), __FILE__, __LINE__);
                if(!sound_->IsPlaying())
				    sound_->Play();
            }

            time_ += GetDeltaTime();

            if(!soundExplo_->IsPlaying())
           	    soundExplo_->Play();
        }
    }

    bool ShallExit() const override
    {
        bool shallExit = time_ > 0 && !music_->IsPlaying();
        return shallExit;
    }
};


NSG_MAIN(Test);
