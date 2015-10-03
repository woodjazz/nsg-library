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

int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;
	auto window = Window::Create();
    scene = std::make_shared<Scene>();
	scene->SetWindow(window.get());
	window->SetScene(scene.get());
    auto camera = scene->CreateChild<Camera>();
	camera->SetPosition(Vector3(0, 0, 10));
	Editor editor;
	editor.SetWindow(window);
	editor.SetScene(scene);
	//editor.SetCamera(camera);
	camera->SetWindow(window.get());
	{
		auto mesh = Mesh::Create<SphereMesh>();
		auto material = Material::Create();
		auto resource = Resource::GetOrCreate<ResourceFile>("data/map.jpg");
		auto texture = std::make_shared<Texture2D>(resource, (int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y);
		material->SetTexture(texture);
		auto node = scene->CreateChild<SceneNode>();
		node->SetMaterial(material);
		node->SetMesh(mesh);
		node->SetPosition(Vector3(1, 0, 0));
		editor.SetNode(node);
	}

	{
		auto mesh = Mesh::Create<BoxMesh>();
		auto material = Material::Create();
		auto node = scene->CreateChild<SceneNode>();
		node->SetMaterial(material);
		node->SetMesh(mesh);
		node->SetPosition(Vector3(-1, 0, 0));
		editor.SetNode(node);
	}

    auto light = scene->CreateChild<Light>("light");
    light->SetType(LightType::DIRECTIONAL);
	light->SetPosition(Vector3(0, 3, 0));

	return Engine::Create()->Run();
}

