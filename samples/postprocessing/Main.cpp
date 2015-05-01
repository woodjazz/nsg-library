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
	auto window = Window::Create();
    auto frameBuffer = window->GetFrameBuffer();
    const unsigned FrameBufferWidth = 50;
    const unsigned FrameBufferHeight = 50;
    frameBuffer->SetWindow(nullptr);
    frameBuffer->SetSize(FrameBufferWidth, FrameBufferHeight);
    auto scene = std::make_shared<Scene>();
	auto camera = scene->CreateChild<Camera>();
	camera->SetPosition(Vertex3(0, 0, 10));
	
    auto blurFilter = window->AddBlurFilter();
    {
        auto frameBuffer = blurFilter->GetFrameBuffer();
        frameBuffer->SetWindow(nullptr);
        frameBuffer->SetSize(FrameBufferWidth, FrameBufferHeight);
    }
    auto waveFilter = window->AddWaveFilter();
    {
        auto frameBuffer = waveFilter->GetFrameBuffer();
        frameBuffer->SetWindow(nullptr);
        frameBuffer->SetSize(FrameBufferWidth, FrameBufferHeight);
    }

    auto control = std::make_shared<CameraControl>(camera);
	scene->CreateChild<Light>();

    auto materialBox = Material::GetOrCreate("material1", (int)ProgramFlag::PER_PIXEL_LIGHTING);
	materialBox->SetDiffuseMap(std::make_shared<Texture>(Resource::GetOrCreate<ResourceFile>("data/wall.jpg")));
	auto boxNode = scene->CreateChild<SceneNode>();
    boxNode->SetPosition(Vertex3(3, -2, 0));
	boxNode->SetMesh(Mesh::Create<BoxMesh>());
    boxNode->SetMaterial(materialBox);

	auto materialSphere = Material::GetOrCreate("material2", (int)ProgramFlag::PER_PIXEL_LIGHTING);
	materialSphere->SetDiffuseMap(std::make_shared<Texture>(Resource::GetOrCreate<ResourceFile>("data/stone.jpg")));
    auto sphereNode = scene->CreateChild<SceneNode>();
    sphereNode->SetPosition(Vertex3(-3, 2, 0));
	sphereNode->SetMesh(Mesh::Create<SphereMesh>());
    sphereNode->SetMaterial(materialSphere);

	Engine engine;
	auto slotUpdate = engine.SigUpdate()->Connect([&](float deltaTime)
	{
		static float y_angle = 0;
		y_angle += glm::pi<float>() / 10.0f * deltaTime;
		boxNode->SetOrientation(glm::angleAxis(y_angle, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle, Vertex3(0, 1, 0)));

		static float move = -1;
		move += deltaTime * TWO_PI * 0.25f;  // 1/4 of a wave cycle per second
		WaveFilter dataWave = waveFilter->GetMaterial()->GetWaveFilter();
		dataWave.offset_ = move;
		waveFilter->GetMaterial()->SetFilterWave(dataWave);
		BlurFilter dataBlur = blurFilter->GetMaterial()->GetFilterBlur();
		dataBlur.sigma_ = move;
		blurFilter->GetMaterial()->SetFilterBlur(dataBlur);
	});

    return engine.Run();
};
