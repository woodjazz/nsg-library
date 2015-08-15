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
    auto window = Window::Create("window", 0, 0, 10, 10);
	if (!Graphics::GetPtr()->HasInstancedArrays())
		return 0;
    auto scene = std::make_shared<Scene>();
	window->SetScene(scene.get());
    auto camera = scene->CreateChild<Camera>();
    auto control = std::make_shared<CameraControl>(camera);
    auto mesh = Mesh::Create<SphereMesh>();
    auto material = Material::Create();
    auto node1 = scene->CreateChild<SceneNode>();
    auto node2 = scene->CreateChild<SceneNode>();
    node1->SetMaterial(material);
    node2->SetMaterial(material);
    node1->SetMesh(mesh);
    node2->SetMesh(mesh);
    node1->SetPosition(Vertex3(-10, 0, 0));
    node2->SetPosition(Vertex3(10, 0, 0));
    control->AutoZoom();
    CHECK_CONDITION(material->IsBatched(), __FILE__, __LINE__);
    node1->SetPosition(Vertex3(-20, 0, 0));
    CHECK_CONDITION(material->IsBatched(), __FILE__, __LINE__);
	control->AutoZoom();
	auto engine = Engine::Create();
    engine->RenderFrame();
	CHECK_CONDITION(material->IsBatched(), __FILE__, __LINE__);
    const float ANGLE = 10;
	node1->SetOrientation(glm::angleAxis(ANGLE, Vertex3(0, 0, 1)));
	engine->RenderFrame();
    CHECK_CONDITION(!material->IsBatched(), __FILE__, __LINE__);
	return 0;
}
