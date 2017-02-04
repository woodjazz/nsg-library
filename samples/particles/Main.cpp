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

int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;
	auto window = Window::Create();
    auto scene = std::make_shared<Scene>();
    auto camera = scene->CreateChild<Camera>();
    camera->SetWindow(window);
	camera->SetPosition(Vertex3(0, 8, 15));
    auto control = std::make_shared<CameraControl>(camera);
    
    auto planeMesh = Mesh::Create<BoxMesh>();
    planeMesh->Set(20, 0.1f, 20);
    auto floorObj = scene->CreateChild<SceneNode>();
    floorObj->SetMesh(planeMesh);
    floorObj->SetMaterial(Material::Create());
	floorObj->GetMaterial()->SetRenderPass(RenderPass::UNLIT);
    floorObj->GetMaterial()->SetDiffuseColor(Color(1, 0, 0));
    auto rb = floorObj->GetOrCreateRigidBody();
	auto shape = Shape::GetOrCreate(ShapeKey(planeMesh, Vector3(1)));
	rb->AddShape(shape);
	auto resource = Resource::GetOrCreateClass<ResourceFile>("data/spark.png");
	auto texture = std::make_shared<Texture2D>(resource, (int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y);
	texture->SetUseAlpha(true);
	auto ps = scene->GetOrCreateChild<ParticleSystem>("ps");
	ps->GetParticleMaterial()->SetTexture(texture);
	//ps->GetParticleMaterial()->EnableTransparent(true);
	//ps->GetParticleMaterial()->SetDiffuseColor(COLOR_GREEN);
	//ps->GetParticleMaterial()->SetAlpha(0);
	auto meshEmitter(Mesh::CreateClass<BoxMesh>());
    ps->SetMesh(meshEmitter);
    ps->SetPosition(Vertex3(0, 10, 0));
	ps->SetMaterial(Material::Create());
	ps->GetMaterial()->SetFillMode(FillMode::WIREFRAME);
    window->SetScene(scene);
	return Engine::Create()->Run();
}

