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


static SignalKey::PSlot ShowShadowMaps(PWindow window, PLight light)
{
	SignalKey::PSlot slotKey = window->SigKey()->Connect([&](int key, int action, int modifier)
	{
		if (NSG_KEY_0 == key && action)
			window->ShowMap(light->GetShadowMap(0));
		else if (NSG_KEY_1 == key && action)
			window->ShowMap(light->GetShadowMap(1));
		else if (NSG_KEY_2 == key && action)
			window->ShowMap(light->GetShadowMap(2));
		else if (NSG_KEY_3 == key && action)
			window->ShowMap(light->GetShadowMap(3));
		else
			window->ShowMap(nullptr);
	});

	return slotKey;
}

static PLight SetupScene(PScene scene)
{
	auto materialFloor = Material::Create();
	materialFloor->SetRenderPass(RenderPass::PERPIXEL);
	materialFloor->SetDiffuseColor(COLOR_BLUE);
	materialFloor->CastShadow(false);
	auto boxMesh(Mesh::Create<BoxMesh>());
	auto sphereMesh(Mesh::Create<SphereMesh>());

	auto nodeFloor = scene->CreateChild<SceneNode>("floor");
	nodeFloor->SetMesh(boxMesh);
	nodeFloor->SetMaterial(materialFloor);
	nodeFloor->SetScale(Vertex3(100, 2, 100));

	auto materialBox = Material::Create();
	materialBox->SetRenderPass(RenderPass::PERPIXEL);
	materialBox->SetDiffuseColor(COLOR_RED);

	auto nodeBox = scene->CreateChild<SceneNode>("Box");
	nodeBox->SetMesh(sphereMesh);
	nodeBox->SetMaterial(materialBox);
	nodeBox->SetPosition(Vertex3(0, 4, 0));

	auto camera = scene->CreateChild<Camera>("Camera");
	camera->SetPosition(Vertex3(0, 9, 5));
	camera->SetGlobalLookAtPosition(Vector3(0, -1, -1));
	camera->SetFarClip(200);
	//camera->SetMaxShadowSplits(1);
	static PCameraControl control;
	control = std::make_shared<CameraControl>(camera);
	auto light = scene->CreateChild<Light>("Sun");
	light->SetType(LightType::DIRECTIONAL);
	//light->SetType(LightType::SPOT);
	//light->SetType(LightType::POINT);
	//light->SetSpotCutOff(90);
	light->SetPosition(Vertex3(10, 6, -10));
	light->SetGlobalLookAtDirection(Vector3(0, -1, 0));
	light->SetShadowColor(Color(0, 1, 0, 1));

	return light;
}

static void Test01()
{
	auto scene = std::make_shared<Scene>();
	auto light = SetupScene(scene);
	auto window = Window::Create("0", 0, 0, 10, 10, (int)WindowFlag::HIDDEN);
	window->SetScene(scene.get());

	auto engine = Engine::Create();
//	engine->Run();
	engine->RenderFrame();
	auto shadowCam = light->GetShadowCamera(0);
	CHECK_ASSERT(shadowCam->GetGlobalPosition() == Vector3(0, 5, 0), __FILE__, __LINE__);
	CHECK_ASSERT(shadowCam->IsOrtho(), __FILE__, __LINE__);
	CHECK_ASSERT(shadowCam->GetZNear() == 0, __FILE__, __LINE__);
	CHECK_ASSERT(shadowCam->GetZFar() == 7, __FILE__, __LINE__);
}

static void Test02()
{
	auto scene = std::make_shared<Scene>();
	auto light = SetupScene(scene);
	light->SetGlobalLookAtDirection(Vector3(0, -1, 0.05f));
	auto window = Window::Create();
	window->SetScene(scene.get());
	auto box = scene->GetChild<SceneNode>("Box", false);
	auto camera = scene->GetChild<Camera>("Camera", false);
	
	SignalKey::PSlot slotKey = window->SigKey()->Connect([&](int key, int action, int modifier)
	{
		if (NSG_KEY_UP == key && action)
		{
			auto pos = box->GetGlobalPosition();
			pos.y += 0.5f;
			box->SetGlobalPosition(pos);
		}
		else if (NSG_KEY_DOWN == key && action)
		{
			auto pos = box->GetGlobalPosition();
			pos.y -= 0.5f;
			box->SetGlobalPosition(pos);
		}
		LOGI("Box pos = %s", ToString(box->GetGlobalPosition()).c_str());
		auto nSplits = camera->GetMaxShadowSplits();
		for (int i = 0; i < nSplits; i++)
		{
			LOGI("SPLIT=%d", i);
			auto shadowCam = light->GetShadowCamera(i);
			LOGI("ShadowCam pos = %s", ToString(shadowCam->GetGlobalPosition()).c_str());
			LOGI("ShadowCam zNear = %f", shadowCam->GetZNear());
			LOGI("ShadowCam zFar = %f", shadowCam->GetZFar());
		}
	});

	auto slot = ShowShadowMaps(window, light);
	auto engine = Engine::Create();
	engine->Run();
}


void Test()
{
	Test01();
	//Test02();
}
