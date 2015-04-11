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
int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;
    auto resource = Resource::GetOrCreate<ResourceFile>("data/bscene.xml");
    AppData data(resource);
	CHECK_CONDITION(data.scenes_.size() == 1, __FILE__, __LINE__);
	auto scene = data.scenes_.at(0);
    auto object = scene->GetChild<SceneNode>("Bone", true);
    CHECK_CONDITION(object, __FILE__, __LINE__);
    auto plane = scene->GetChild<SceneNode>("Plane", false);
    CHECK_CONDITION(plane, __FILE__, __LINE__);
    auto camera = scene->GetChild<Camera>("Camera", false);
    CHECK_CONDITION(camera, __FILE__, __LINE__);
    auto lamp = scene->GetChild<Light>("Lamp", false);
    CHECK_CONDITION(lamp, __FILE__, __LINE__);
    auto ball = scene->GetChild<SceneNode>("Earth", false);
    CHECK_CONDITION(ball, __FILE__, __LINE__);
    auto ramp1 = scene->GetChild<SceneNode>("Ramp1", false);
    CHECK_CONDITION(ramp1, __FILE__, __LINE__);
    auto ramp2 = scene->GetChild<SceneNode>("Ramp2", false);
    CHECK_CONDITION(ramp2, __FILE__, __LINE__);
	auto animations = scene->GetAnimations();
	CHECK_CONDITION(animations.size() == 1, __FILE__, __LINE__);
	{
		auto animations = scene->GetAnimationsFor(plane);
		CHECK_CONDITION(animations.size() == 0, __FILE__, __LINE__);
	}
	{
		auto animations = scene->GetAnimationsFor(object);
		CHECK_CONDITION(animations.size() == 1, __FILE__, __LINE__);
	}

}
