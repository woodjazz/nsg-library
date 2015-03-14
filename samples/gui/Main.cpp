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

    auto window = app.GetOrCreateWindow("window", 100, 500, 10, 10);

    auto xml = app.GetOrCreateResourceFile("data/GuiAnonymousPro32.xml");
    auto atlas = std::make_shared<FontAtlas>(xml);

    auto scene = std::make_shared<Scene>("scene");

	auto blurCamera = scene->CreateChild<Camera>("BlurCamera");
	blurCamera->EnableOrtho();
#if 0
	blurCamera->AddBlurFilter();
#else
	static const char* fShader = STRINGIFY(void main()
	{
		vec2 texcoord = v_texcoord0;
		const float FACTOR = 8.0 * 3.14159;
		texcoord.x += sin(texcoord.y * FACTOR + u_material.shininess) / 100.0;
		gl_FragColor = texture2D(u_texture0, texcoord);
	});

	auto resourceMem = app.GetOrCreateResourceMemory("ResourceMemory");
	resourceMem->SetData(fShader, strlen(fShader));
	auto userFilter = blurCamera->AddUserFilter(resourceMem);
	//blurCamera->AddBlendFilter();
	auto slotUpdate = window->signalUpdate_->Connect([&](float deltaTime)
	{
		static float move = -1;
		move += deltaTime * TWO_PI * 0.25f;  // 1/4 of a wave cycle per second
		userFilter->GetMaterial()->SetShininess(move);
	});

#endif

	blurCamera->SetLayer(RenderLayer::GUI_EFFECT_LAYER0);

    auto camera = scene->CreateChild<Camera>("DefaultCamera");
    auto control = std::make_shared<CameraControl>(camera);

	auto box = scene->CreateChild<SceneNode>();
	box->SetPosition(Vertex3(0, 0, -5));
	box->SetMesh(app.CreateBoxMesh());
	box->SetMaterial(app.GetOrCreateMaterial());
	box->GetMaterial()->SetSolid(false);
	box->GetMaterial()->SetColor(Color(1, 0, 1, 1));

    auto button1 = scene->CreateChild<Button>();
	button1->SetScale(0.25f);
	button1->SetAtlas(atlas);
    button1->SetText("Hello");
	button1->GetTextMaterial()->SetColor(Color(1, 0.2f, 1, 1));

	button1->SetScreenAlignment(HorizontalAlignment::LEFT_ALIGNMENT, VerticalAlignment::BOTTOM_ALIGNMENT);
	button1->SetBottomScreenMargin(0.1f);
	button1->SetLeftScreenMargin(0.1f);

	auto slotButtonClicked = button1->signalClicked_->Connect([&](int button)
    {
		button1->GetMaterial()->SetColor(Color(1, 0, 0, 1));
    });

	auto slotButtonPush = button1->signalPush_->Connect([&](int button)
	{
		button1->SetText("(Click)");
	});

	auto slotButtonPop = button1->signalPop_->Connect([&](int button)
	{
		button1->SetText("(Hello)");
	});

	auto slotMouseEnter = button1->signalMouseEnter_->Connect([&]()
	{
		button1->GetMaterial()->SetColor(Color(0,1,0,1));
		button1->Control::SetLayer(RenderLayer::GUI_EFFECT_LAYER0);
	});

	auto slotMouseLeave = button1->signalMouseLeave_->Connect([&]()
	{
		button1->GetMaterial()->SetColor(Color(1, 1, 1, 1));
		button1->Control::SetLayer(RenderLayer::GUI_LAYER0);
	});

    auto renderSlot = window->signalRender_->Connect([&]()
    {
		scene->Render();
    });

    return app.Run();
}

