/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

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

    App app;

    auto window = app.GetOrCreateWindow("window", 100, 100, 150, 30);

    auto atlas = std::make_shared<FontAtlas>(window->GetWidth(), window->GetHeight());
	auto textCenter = atlas->GetOrCreateMesh("C Hello World!!!", CENTER_ALIGNMENT, MIDDLE_ALIGNMENT);
	auto textLeftTop = atlas->GetOrCreateMesh("LT Hello World!!!", LEFT_ALIGNMENT, TOP_ALIGNMENT);
	auto textRightTop = atlas->GetOrCreateMesh("RT Hello World!!!", RIGHT_ALIGNMENT, TOP_ALIGNMENT);
	auto textLeftBottom = atlas->GetOrCreateMesh("LB Hello World!!!", LEFT_ALIGNMENT, BOTTOM_ALIGNMENT);
	auto textRightBottom = atlas->GetOrCreateMesh("RB Hello World!!!", RIGHT_ALIGNMENT, BOTTOM_ALIGNMENT);

    auto scene = std::make_shared<Scene>("scene");
    auto nodeCenter = scene->GetOrCreateChild<SceneNode>("nodeCenter");
	auto nodeLeftTop = scene->GetOrCreateChild<SceneNode>("nodeLeftTop");
	auto nodeRightTop = scene->GetOrCreateChild<SceneNode>("nodeRightTop");
	auto nodeLeftBottom = scene->GetOrCreateChild<SceneNode>("nodeLeftBottom");
	auto nodeRightBottom = scene->GetOrCreateChild<SceneNode>("nodeRightBottom");
	
    auto material = app.GetOrCreateMaterial("material");
    material->SetColor(Color(1, 1, 1, 1));
	material->SetTexture0(atlas->GetTexture());
    auto technique = std::make_shared<Technique>();
    auto pass = std::make_shared<Pass>();
    technique->Add(pass);
    material->SetTechnique(technique);
    pass->EnableDepthTest(false);
    pass->EnableStencilTest(false);
	auto program = app.GetOrCreateProgram("text");
	program->SetFlags((int)ProgramFlag::TEXT);
	pass->SetProgram(program);
	
	nodeCenter->Set(material);
	nodeLeftTop->Set(material);
	nodeRightTop->Set(material);
	nodeLeftBottom->Set(material);
	nodeRightBottom->Set(material);

	nodeCenter->Set(textCenter);
	nodeLeftTop->Set(textLeftTop);
	nodeRightTop->Set(textRightTop);
	nodeLeftBottom->Set(textLeftBottom);
	nodeRightBottom->Set(textRightBottom);

	auto resizeSlot = window->signalViewChanged_->Connect([&](int width, int height)
	{
		atlas->SetViewSize(width, height);
	});

    auto renderSlot = window->signalRender_->Connect([&]()
    {
		nodeCenter->Render();
		nodeLeftTop->Render();
		nodeRightTop->Render();
		nodeLeftBottom->Render();
		nodeRightBottom->Render();
	});

    return app.Run();
}

