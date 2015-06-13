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

PSceneNode CreateObject(PMesh mesh, Color color, const Vector3& pos)
{
	auto obj = scene->CreateChild<SceneNode>();
	obj->SetGlobalPosition(pos);
	auto material = Material::GetOrCreate();
	material->SetDiffuseColor(color);
	material->SetRenderPass(RenderPass::PERPIXEL);
	obj->SetMesh(mesh);
	obj->SetMaterial(material);
	return obj;
}

int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;
	auto window = Window::Create();
    scene = std::make_shared<Scene>("scene");
    auto light = scene->CreateChild<Light>();
	light->SetGlobalLookAt(Vector3(0, -1, -5));
    light->SetType(LightType::DIRECTIONAL);
	light->SetShadowColor(COLOR_GREEN);
	light->SetBias(0.1f);
    auto camera = scene->CreateChild<Camera>();
	camera->SetPosition(Vector3(0, 0, 10));
	//camera->SetFarClip(25);
    auto control = std::make_shared<CameraControl>(camera);
	auto plane = Mesh::Create<BoxMesh>();
	plane->Set(20, 0.1f, 20);
	auto floor = CreateObject(plane, COLOR_WHITE, Vector3(0));
	floor->GetMaterial()->CastShadow(false);
	CreateObject(Mesh::Create<SphereMesh>(), COLOR_RED, Vector3(-1, 1, 0));
	CreateObject(Mesh::Create<BoxMesh>(), COLOR_BLUE, Vector3(1, 1, 0));
	//window->ShowMap(light->GetShadowMap());
	return Engine().Run();
}

