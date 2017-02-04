/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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

static int Test0()
{
    auto prefPath = FileSystem::GetPreferencesPath();
    Path filePath;
    filePath.SetPath(prefPath);
    filePath.SetFileName("data.txt");
    LOGI("%s\n", filePath.GetFullAbsoluteFilePath().c_str());
    auto fsSlot = FileSystem::SigReady()->Connect([&]()
    {
		int counter = 0;
		{
			std::ifstream ifs(filePath.GetFullAbsoluteFilePath().c_str());
			if (ifs.is_open())
				ifs >> counter;
		}
		LOGI("Counter = %d\n", counter);
		{
			std::ofstream ofs(filePath.GetFullAbsoluteFilePath().c_str(), std::ios::out);
			ofs << counter + 1;
		}
		//FileSystem::Save();
    });

    bool saved = false;
    auto saveSlot = FileSystem::SigSaved()->Connect([&]()
    {
    	saved = true;
    });
    
	Engine::Create()->Run();
    CHECK_CONDITION(saved);
    return 0;
}

int NSG_MAIN(int argc, char* argv[])
{
    Test0();
    return 0;
}
