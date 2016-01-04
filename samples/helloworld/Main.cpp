/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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

    auto font = std::make_shared<FontAtlas>();

	auto xmlResource = Resource::GetOrCreate<ResourceFile>("data/AnonymousPro32.xml");
	font->Set(xmlResource);

	auto atlasResource = Resource::GetOrCreate<ResourceFile>("data/AnonymousPro32.png");
	auto atlasTexture = std::make_shared<Texture2D>(atlasResource);
	font->SetTexture(atlasTexture);


    auto scene = std::make_shared<Scene>("scene");
    auto camera = scene->CreateChild<Camera>();
    camera->SetPosition(Vector3(0, 0, 2));
    camera->EnableOrtho();
    camera->UnRegisterWindow();
	scene->SetWindow(window.get());
    auto control = std::make_shared<CameraControl>(camera);

    auto nodeCenter = scene->GetOrCreateChild<SceneNode>("nodeCenter");
    auto nodeLeftTop = scene->GetOrCreateChild<SceneNode>("nodeLeftTop");
    auto nodeRightTop = scene->GetOrCreateChild<SceneNode>("nodeRightTop");
    auto nodeLeftBottom = scene->GetOrCreateChild<SceneNode>("nodeLeftBottom");
    auto nodeRightBottom = scene->GetOrCreateChild<SceneNode>("nodeRightBottom");

	nodeCenter->SetMesh(font->GetOrCreateMesh("C Hello World!!!", CENTER_ALIGNMENT, MIDDLE_ALIGNMENT));
	nodeLeftTop->SetMesh(font->GetOrCreateMesh("LT Hello World!!!", LEFT_ALIGNMENT, TOP_ALIGNMENT));
	nodeRightTop->SetMesh(font->GetOrCreateMesh("RT Hello World!!!", RIGHT_ALIGNMENT, TOP_ALIGNMENT));
	nodeLeftBottom->SetMesh(font->GetOrCreateMesh("LB Hello World!!!", LEFT_ALIGNMENT, BOTTOM_ALIGNMENT));
	nodeRightBottom->SetMesh(font->GetOrCreateMesh("RB Hello World!!!", RIGHT_ALIGNMENT, BOTTOM_ALIGNMENT));

	auto material = Material::Create();
	material->SetTextMap(font->GetTexture());

    auto focusMaterial = material->Clone();
	focusMaterial->SetDiffuseColor(ColorRGB(0, 0, 1));

    auto activeMaterial = material->Clone();
	activeMaterial->SetDiffuseColor(ColorRGB(1, 0, 0));

    nodeCenter->SetMaterial(material);
    nodeLeftTop->SetMaterial(material);
    nodeRightTop->SetMaterial(material);
    nodeLeftBottom->SetMaterial(material);
    nodeRightBottom->SetMaterial(material);

    //control->AutoZoom();

    SceneNode* lastNode = nullptr;
    auto slotMouseDown = scene->SigNodeMouseDown()->Connect([&](SceneNode * node, int button, float x, float y)
    {
        lastNode = node;
        node->SetMaterial(activeMaterial);
    });

    auto slotMouseUp = window->SigMouseUp()->Connect([&](int button, float x, float y)
    {
		if (lastNode)
		{
			lastNode->SetMaterial(material);
			lastNode = nullptr;
		}
    });

	auto drawGUISlot = window->SigDrawIMGUI()->Connect([&]()
	{
		static bool show_test_window = true;
		ImGui::ShowTestWindow(&show_test_window);
	});

	window->SetScene(scene.get());
	return Engine::Create()->Run();
}

