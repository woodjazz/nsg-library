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

PSceneNode CreateObject(PMesh mesh, ColorRGB color, const Vector3& pos, const Vector3& scale, FillMode mode = FillMode::SOLID)
{
	auto obj = scene->CreateChild<SceneNode>();
	obj->SetGlobalPosition(pos);
	obj->SetGlobalScale(scale);
	auto material = Material::Create();
	material->SetDiffuseColor(color);
	material->SetSpecularColor(ColorRGB(0));
	material->SetRenderPass(RenderPass::PERPIXEL);
	material->SetFillMode(mode);
	obj->SetMesh(mesh);
	obj->SetMaterial(material);
	auto rb = obj->GetOrCreateRigidBody();
	auto shape = Shape::Create(ShapeKey{ mesh, scale });
	rb->AddShape(shape);
	return obj;
}

int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;
	auto window = Window::Create("0", 0, 0, 10, 10, (int)WindowFlag::HIDDEN);
    scene = std::make_shared<Scene>("scene");
    auto camera = scene->CreateChild<Camera>();
	camera->SetPosition(Vector3(0, 20, 20));
	camera->SetLocalLookAtPosition(Vector3(0, -2, -1));
	auto control = std::make_shared<CameraControl>(camera);

	PSceneNode floor;
	{
		auto mesh = Mesh::Create<BoxMesh>();
		mesh->Set(5, 0.1f, 5);
		floor = CreateObject(mesh, COLOR_WHITE, Vector3(0), Vector3(2), FillMode::WIREFRAME);
	}

	PSceneNode sphere;
	Vector3 spherePos(0, 30, 0);
	{
		auto mesh = Mesh::Create<SphereMesh>();
		mesh->Set(2);
		sphere = CreateObject(mesh, COLOR_WHITE, spherePos, Vector3(1), FillMode::WIREFRAME);
		auto rb = sphere->GetRigidBody();
		rb->SetMass(1);
		rb->HandleCollisions(true);
	}

    auto static slotCollision = sphere->SigCollision()->Connect([&](const ContactPoint & contactInfo)
    {
		static int step = 0;
        auto pos = sphere->GetGlobalPosition();
		if (Abs(pos.y - 2.1f) < 0.001f)
		{
			CHECK_CONDITION(step == 0, __FILE__, __LINE__);
			sphere->SetGlobalScale(Vector3(2));
			sphere->SetGlobalPosition(spherePos);
			sphere->GetRigidBody()->SyncWithNode();
			++step;
		}
		else if (Abs(pos.y - 4.1f) < 0.001f)
		{
			CHECK_CONDITION(step == 1, __FILE__, __LINE__);
			sphere->SetGlobalScale(Vector3(3));
			sphere->SetGlobalPosition(spherePos);
			sphere->GetRigidBody()->SyncWithNode();
			++step;
		}
		else if (Abs(pos.y - 6.1f) < 0.001f)
		{
			CHECK_CONDITION(step == 2, __FILE__, __LINE__);
			window = nullptr; // exit
		}
    });

	window->SetScene(scene.get());
	return Engine::Create()->Run();
}

