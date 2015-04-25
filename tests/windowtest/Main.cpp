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

#include "NSG.h"
using namespace NSG;

static void Test0()
{
	auto window1 = Window::Create("0", 100, 100, 150, 50, (int)WindowFlag::HIDDEN);
	auto window2 = Window::Create("1", 200, 200, 150, 50, (int)WindowFlag::HIDDEN);
	auto window3 = Window::Create("2", 300, 300, 150, 50, (int)WindowFlag::HIDDEN);
	window1 = nullptr;
	CHECK_CONDITION(Engine().Run() == 0, __FILE__, __LINE__);
}

static void Test1()
{
	auto window1 = Window::Create("0", 100, 100, 150, 50, (int)WindowFlag::HIDDEN);
	auto window2 = Window::Create("1", 200, 200, 150, 50, (int)WindowFlag::HIDDEN);
	auto window3 = Window::Create("2", 300, 300, 150, 50, (int)WindowFlag::HIDDEN);
	window2 = nullptr;

	Engine engine;
    auto updateSlot = engine.signalUpdate_->Connect([&](float deltaTime)
    {
    	static float totalTime = 0;
    	totalTime += deltaTime;

		if (totalTime > 1)
		{
			window1 = nullptr;
		}
    });

	CHECK_CONDITION(engine.Run() == 0, __FILE__, __LINE__);
}

int NSG_MAIN(int argc, char* argv[])
{
	Test0();
	Test1();
	return 0;
}			
