/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 N�stor Silveira Gorski

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

int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;

    App app;

    auto music = std::make_shared<Music>(app.GetOrCreateResourceFile("data/nice_music.ogg"));
	auto sound = std::make_shared<Sound>(app.GetOrCreateResourceFile("data/PlayerFist.wav"));
	auto soundExplo = std::make_shared<Sound>(app.GetOrCreateResourceFile("data/BigExplosion.wav"));

    CHECK_ASSERT(!music->IsPlaying(), __FILE__, __LINE__);
    music->Play(false);
    CHECK_ASSERT(music->IsPlaying(), __FILE__, __LINE__);
    std::this_thread::sleep_for(Seconds(1));
    music->Pause();
    sound->Play();
    CHECK_ASSERT(music->IsPaused(), __FILE__, __LINE__);
    std::this_thread::sleep_for(Seconds(1));
    music->Resume();
    CHECK_ASSERT(!music->IsPaused(), __FILE__, __LINE__);
    soundExplo->Play();
    std::this_thread::sleep_for(Seconds(3));

    return 0;
}
