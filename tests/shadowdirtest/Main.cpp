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
PScene scene;

PSceneNode CreateObject(PMesh mesh, ColorRGB color, const Vector3& pos)
{
	auto obj = scene->CreateChild<SceneNode>();
	obj->SetGlobalPosition(pos);
	auto material = Material::GetOrCreate(mesh->GetName());
	material->SetDiffuseColor(color);
	//material->SetSpecularColor(ColorRGB(0));
	material->SetRenderPass(RenderPass::PERPIXEL);
	//material->SetRenderPass(RenderPass::UNLIT);
	obj->SetMesh(mesh);
	obj->SetMaterial(material);
	return obj;
}

static void SetFog(PScene scene)
{
	scene->SetHorizonColor(ColorRGB(0, 1, 0));
	scene->EnableFog(true);
	scene->SetFogDepth(150);
	scene->SetFogStart(15);
	//scene->SetFogHeight(0.1f);
}

#define TEST1
int NSG_MAIN(int argc, char* argv[])
{
	//return 0;

    using namespace NSG;
	auto window = Window::Create();
    scene = std::make_shared<Scene>("scene");
	//SetFog(scene);
    auto light = scene->CreateChild<Light>();
    light->SetType(LightType::DIRECTIONAL);
	light->SetShadowColor(Vector4(COLOR_GREEN,1.f));
	//light->SetBias(0.1000f);
    auto camera = scene->CreateChild<Camera>();
#ifdef TEST1
	light->SetGlobalLookAtPosition(Vector3(-10, -1, 0));
	camera->SetPosition(Vector3(-0.795f, 25.018f, 13.577f));
	//camera->SetLocalLookAtPosition(Vector3(0, 0, -10));
	camera->SetOrientation(Quaternion(0.859f, -0.510f, -0.025f, -0.0149f));
	//camera->SetShadowSplitLogFactor(1.f);
#else
	light->SetGlobalLookAtPosition(Vector3(0, -5, -10));
	camera->SetPosition(Vector3(0, 20, 20));
	camera->SetLocalLookAtPosition(Vector3(0, -1, -1));
#endif
	camera->SetFarClip(100);
    auto control = std::make_shared<CameraControl>(camera);
	auto plane = Mesh::Create<BoxMesh>();
	plane->Set(500, 0.1f, 500);
	auto floor = CreateObject(plane, COLOR_WHITE, Vector3(0));
	floor->GetMaterial()->CastShadow(false);
#ifdef TEST1
	for (int i = 0; i < 25; i++)
	{
		float z = -i * 2.5f;
		CreateObject(Mesh::Create<SphereMesh>(), COLOR_RED, Vector3(-1, 1, z));
		//CreateObject(Mesh::GetOrCreate<BoxMesh>("Box"), COLOR_BLUE, Vector3(1, 1, z));
	}

	for (int i = 25; i < 50; i++)
	{
		float z = -i * 2.5f;
		//auto sphere = CreateObject(Mesh::GetOrCreate<SphereMesh>("Sphere"), COLOR_RED, Vector3(-1, 1, z));
		//sphere->GetMaterial()->SetSpecularColor(COLOR_WHITE);
		CreateObject(Mesh::Create<BoxMesh>(), COLOR_BLUE, Vector3(1, 1, z));
	}
#else
	camera->SetShadowSplits(1);
	#if 0
	auto mesh = Mesh::Create<SphereMesh>();
	mesh->Set(10, 20);
	auto sphere = CreateObject(mesh, COLOR_RED, Vector3(-1,10, 0));
	sphere->GetMaterial()->SetSpecularColor(COLOR_WHITE);
	#endif
	CreateObject(Mesh::Create<BoxMesh>(), COLOR_BLUE, Vector3(0, 1, 0));
	
#endif

	auto slotKey = window->SigKey()->Connect([&](int key, int action, int modifier)
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

	

	auto engine = Engine::Create();
	auto slotUpdate = engine->SigUpdate()->Connect([&](float deltaTime)
	{
		//LOGI("%s", ToString(camera->GetView() * Vector4(0, 0, -50, 1)).c_str());
		//LOGI("%s", ToString(camera->GetPosition()).c_str());
		//LOGI("q=%s", ToString(camera->GetOrientation()).c_str());
	});
	window->SetScene(scene.get());
	return engine->Run();
}

