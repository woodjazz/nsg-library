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

void CreateObject(PMesh mesh, ColorRGB color, const Vector3& pos)
{
	{
		auto obj = scene->CreateChild<SceneNode>();
		obj->SetPosition(pos);
		auto material = Material::Create();
		material->SetRenderPass(RenderPass::UNLIT);
		material->SetDiffuseColor(color);
		obj->SetMesh(mesh);
		obj->SetMaterial(material);
	}

	{
		auto obj = scene->CreateChild<SceneNode>();
		obj->SetPosition(pos + Vector3(5, 0, 0));
		auto material = Material::Create();
		material->SetRenderPass(RenderPass::UNLIT);
		material->SetDiffuseColor(color);
		material->SetFillMode(FillMode::WIREFRAME);
		obj->SetMesh(mesh);
		obj->SetMaterial(material);
	}
}

int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;
	auto window = Window::Create();
    scene = std::make_shared<Scene>("scene");
    auto camera = scene->CreateChild<Camera>();
    auto control = std::make_shared<CameraControl>(camera);

	CreateObject(Mesh::Create<PlaneMesh>(), ColorRGB(1, 0, 0), Vector3(-20, 0, 0));
	CreateObject(Mesh::Create<CircleMesh>(), ColorRGB(0, 1, 0), Vector3(-10, 0, 0));
	CreateObject(Mesh::Create<BoxMesh>(), ColorRGB(0, 0, 1), Vector3(0, 0, 0));
	CreateObject(Mesh::Create<EllipseMesh>(), ColorRGB(1, 1, 0), Vector3(10, 0, 0));
	CreateObject(Mesh::Create<RectangleMesh>(), ColorRGB(1, 0, 1), Vector3(20, 0, 0));
	CreateObject(Mesh::Create<RoundedRectangleMesh>(), ColorRGB(1, 1, 1), Vector3(30, 0, 0));
	CreateObject(Mesh::Create<SphereMesh>(), ColorRGB(0, 1, 1), Vector3(40, 0, 0));
	control->AutoZoom();
	window->SetScene(scene.get());
	auto r = Engine::Create()->Run();
	scene = nullptr;
	return r;
}

