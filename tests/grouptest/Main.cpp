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
static void Test0()
{
	auto window = Window::Create("0", 100, 100, 150, 50, (int)WindowFlag::HIDDEN);
	auto resource = Resource::GetOrCreate<ResourceFile>("data/scene0.xml");
	LoaderXML data("loader");
	auto slot = data.Load(resource)->Connect([&]()
	{
		auto scene = data.GetScene(0);
		CHECK_CONDITION(scene->GetChildren().size() == 7);
		{
			auto object = scene->GetChild<SceneNode>("Cube", true);
			CHECK_CONDITION(object);
		}
		{
			auto object = scene->GetChild<SceneNode>("Sphere", true);
			CHECK_CONDITION(object);
		}
		{
			auto object = scene->GetChild<SceneNode>("Torus", true);
			CHECK_CONDITION(object);
		}
		{
			auto object = scene->GetChild<SceneNode>("Plane", true);
			CHECK_CONDITION(object);
		}
		{
			auto object = scene->GetChild<SceneNode>("InstancedGroup", true);
			CHECK_CONDITION(object);
			{
				auto object1 = object->GetChild<SceneNode>("Cube", true);
				CHECK_CONDITION(object1);
			}
			{
				auto object1 = object->GetChild<SceneNode>("Sphere", true);
				CHECK_CONDITION(object1);
			}
			{
				auto object1 = object->GetChild<SceneNode>("Torus", true);
				CHECK_CONDITION(object1);
			}
			{
				auto object1 = object->GetChild<SceneNode>("Plane", true);
				CHECK_CONDITION(!object1);
			}
		}
		window = nullptr;
	});

	Engine::Create()->Run();
}

static void Test1()
{
	auto window = Window::Create("0", 100, 100, 150, 50, (int)WindowFlag::HIDDEN);
	auto resource = Resource::GetOrCreate<ResourceFile>("data/scene1.xml");
	LoaderXML data("loader");
	auto slot = data.Load(resource)->Connect([&]()
	{
		auto scene = data.GetScene(0);
		CHECK_CONDITION(scene->GetChildren().size() == 11);
		{
			auto object = scene->GetChild<SceneNode>("Cube", true);
			CHECK_CONDITION(object);
		}
		{
			auto object = scene->GetChild<SceneNode>("Sphere", true);
			CHECK_CONDITION(object);
		}
		{
			auto object = scene->GetChild<SceneNode>("Torus", true);
			CHECK_CONDITION(object);
		}
		{
			auto object = scene->GetChild<SceneNode>("Plane", true);
			CHECK_CONDITION(object);
		}
		{
			auto object = scene->GetChild<SceneNode>("InstancedGroup001", true);
			CHECK_CONDITION(object);
			CHECK_CONDITION(object->GetChildren().size() == 2);
		}

		{
			auto object = scene->GetChild<SceneNode>("InstancedGroup", true);
			CHECK_CONDITION(object);
			CHECK_CONDITION(object->GetChildren().size() == 3);
			{
				auto object1 = object->GetChild<SceneNode>("Cube", true);
				CHECK_CONDITION(object1);
			}
			{
				auto object1 = object->GetChild<SceneNode>("Sphere", true);
				CHECK_CONDITION(object1);
			}
			{
				auto object1 = object->GetChild<SceneNode>("Torus", true);
				CHECK_CONDITION(object1);
			}
			{
				auto object1 = object->GetChild<SceneNode>("Plane", true);
				CHECK_CONDITION(!object1);
			}
		}
		window = nullptr;
	});

	Engine::Create()->Run();
}

int NSG_MAIN(int argc, char* argv[])
{
	Test0();
	Test1();
	return 0;
}
