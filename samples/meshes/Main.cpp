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

PApp app;
PScene scene;

void CreateObject(PMesh mesh, Color color, const Vector3& pos)
{
	{
		auto obj = scene->CreateChild<SceneNode>();
		obj->SetPosition(pos);
		auto material = app->GetOrCreateMaterial();
		material->SetColor(color);
		obj->SetMesh(mesh);
		obj->SetMaterial(material);
	}

	{
		auto obj = scene->CreateChild<SceneNode>();
		obj->SetPosition(pos + Vector3(5, 0, 0));
		auto material = app->GetOrCreateMaterial();
		material->SetColor(color);
		material->SetSolid(false);
		obj->SetMesh(mesh);
		obj->SetMaterial(material);
	}
}

int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;
	app = PApp(new App);
    auto window = app->GetOrCreateWindow("window", 100, 100, 10, 10);
    scene = std::make_shared<Scene>("scene");
    auto light = scene->CreateChild<Light>();
    light->SetType(LightType::DIRECTIONAL);
    auto camera = scene->CreateChild<Camera>();
    auto control = std::make_shared<CameraControl>(camera);

	CreateObject(app->CreatePlaneMesh(), Color(1, 0, 0, 1), Vector3(-20, 0, 0));
	CreateObject(app->CreateCircleMesh(), Color(0, 1, 0, 1), Vector3(-10, 0, 0));
	CreateObject(app->CreateBoxMesh(), Color(0, 0, 1, 1), Vector3(0, 0, 0));
	CreateObject(app->CreateEllipseMesh(), Color(1, 1, 0, 1), Vector3(10, 0, 0));
	CreateObject(app->CreateRectangleMesh(), Color(1, 0, 1, 1), Vector3(20, 0, 0));
	CreateObject(app->CreateRoundedRectangleMesh(), Color(1, 1, 1, 1), Vector3(30, 0, 0));
	CreateObject(app->CreateSphereMesh(), Color(0, 1, 1, 1), Vector3(40, 0, 0));
	
	control->AutoZoom();
	auto renderSlot = window->signalRender_->Connect([&]() { scene->Render();});
    return app->Run();
}

