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

static void Test01()
{
	{
		auto resource = Resource::Create<ResourceFile>("data/frankie_home_col");
		auto image = std::make_shared<Image>(resource);
		for (int i = 0; i < 50; i++)
		{
			CHECK_CONDITION(image->IsReady());
			image->Invalidate();
		}
		Resource::Clear();
	}
}

static void Test02()
{
	{
		auto resource = Resource::Create<ResourceFile>("data/frankie_home_col");
		auto texture = std::make_shared<Texture2D>(resource);
		for (int i = 0; i < 1050; i++)
		{
			CHECK_CONDITION(texture->IsReady());
			texture->Invalidate();
		}
		Resource::Clear();
	}
}

static void Test03()
{
	auto window = Window::Create("window", 0, 0, 1, 1, (int)WindowFlag::HIDDEN);
	auto slotUpdate = Engine::SigUpdate()->Connect([&](float deltaTime)
	{
		Camera cam;
	});

	Engine::Create()->Run();
}


void Tests()
{
#if 0
	{
		auto window = Window::Create("window", 0, 0, 1, 1, (int)WindowFlag::HIDDEN);
		Test01();
		Test02();
	}
#endif
	//Test03();
}
