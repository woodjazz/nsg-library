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
    auto window = Window::Create();
	auto resource = Resource::GetOrCreate<ResourceFile>("data/bscene.xml");
	AppData data(resource);
	auto scene = data.scenes_.at(0);
    auto camera = scene->GetChild<Camera>("Camera", false);
    auto control = std::make_shared<CameraControl>(camera);
	auto spot1Light = scene->GetLights()[1];
	spot1Light->SetShadowColor(Color(COLOR_GREEN,1));
	//spot1Light->SetDistance(60.f);
	auto spot2Light = scene->GetLights()[2];
	spot2Light->SetShadowColor(Color(COLOR_RED,1));
	//spot2Light->SetOnlyShadow(true);
	//spot2Light->SetDistance(60.f);
	//spot2Light->SetSpotCutOff(75.f);
	auto dir1 = spot1Light->GetLookAtDirection();
	auto dir2 = spot2Light->GetLookAtDirection();

	Engine engine;
#if 0
	auto updateSlot = engine.SigUpdate()->Connect([&](float deltaTime)
	{
		auto pos1 = spot1Light->GetGlobalPosition();
		auto pos2 = spot2Light->GetGlobalPosition();
		//spot1Light->SetGlobalPosition(pos1 - Vector3(deltaTime) * dir1);
		//spot2Light->SetGlobalPosition(pos2 - Vector3(deltaTime) * dir2);

		static float angle = 0;
		angle += glm::pi<float>() / 15.0f * deltaTime;
		spot1Light->SetOrientation(glm::angleAxis(angle, Vertex3(0, 1, 0)));

		spot1Light->SetOrientation(glm::angleAxis(angle, Vertex3(0, 1, 0)));
		spot2Light->SetOrientation(glm::angleAxis(angle, Vertex3(-1, 1, 0)));


		//auto pos1 = point2Light->GetGlobalPosition();
		//point2Light->SetGlobalPosition(pos1 + Vector3(0, deltaTime, 0));
	});
#else
	//auto pos = spotLight->GetGlobalPosition();
	//spotLight->SetGlobalPosition(pos - Vector3(2) * dir);

#endif
	//window->ShowMap(spot1Light->GetShadowMap());
	return engine.Run();
}
