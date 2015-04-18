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

static int Test01()
{
	auto window = Window::Create("hiddenWindow", (int)WindowFlag::HIDDEN);
    auto scene = std::make_shared<Scene>();
    auto camera = scene->CreateChild<Camera>();
    camera->SetPosition(Vector3(0, 0, 5));
    auto resource = Resource::GetOrCreate<ResourceFile>("data/stonediffuse.dds");
    auto texture = std::make_shared<Texture>(resource, (int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y);
    auto mesh = Mesh::Create<SphereMesh>();
    auto material = Material::Create();
	material->SetProgramFlags(0, (int)ProgramFlag::UNLIT);
    material->SetDiffuseMap(texture);
    auto node = scene->CreateChild<SceneNode>();
    node->SetMaterial(material);
    node->SetMesh(mesh);
    auto renderSlot = window->signalRender_->Connect([&]()
    {
        scene->Render();
		window = nullptr;
    });
    return Window::RunApp();
}

static int Test02()
{
	auto window = Window::Create("hiddenWindow", (int)WindowFlag::HIDDEN);
    auto scene = std::make_shared<Scene>();
    auto camera = scene->CreateChild<Camera>();
    camera->SetPosition(Vector3(0, 0, 5));
    auto resource = Resource::GetOrCreate<ResourceFile>("data/tex_etc1.ktx");
    auto texture = std::make_shared<Texture>(resource, (int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y);
    auto mesh = Mesh::Create<SphereMesh>();
    auto material = Material::Create();
    material->SetProgramFlags(0, (int)ProgramFlag::UNLIT);
    material->SetDiffuseMap(texture);
    auto node = scene->CreateChild<SceneNode>();
    node->SetMaterial(material);
    node->SetMesh(mesh);
    auto renderSlot = window->signalRender_->Connect([&]()
    {
        scene->Render();
        window = nullptr;
    });
    return Window::RunApp();
}

static int Test03()
{
    auto window = Window::Create("hiddenWindow", (int)WindowFlag::HIDDEN);
    auto scene = std::make_shared<Scene>();
    auto camera = scene->CreateChild<Camera>();
    camera->SetPosition(Vector3(0, 0, 5));
    auto resource = Resource::GetOrCreate<ResourceFile>("data/tex_pvr.pvr");
    auto texture = std::make_shared<Texture>(resource, (int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y);
    auto mesh = Mesh::Create<SphereMesh>();
    auto material = Material::Create();
    material->SetProgramFlags(0, (int)ProgramFlag::UNLIT);
    material->SetDiffuseMap(texture);
    auto node = scene->CreateChild<SceneNode>();
    node->SetMaterial(material);
    node->SetMesh(mesh);
    auto renderSlot = window->signalRender_->Connect([&]()
    {
        scene->Render();
        window = nullptr;
    });
    return Window::RunApp();
}

void Tests()
{
	Test01();
    #if 0
    Test02();
    #endif
	Test03();
}