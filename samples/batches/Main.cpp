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

    const int COLS = 11;
    const int ROWS = 11;
    PSceneNode earth[COLS][ROWS];

    auto scene = std::make_shared<Scene>();
	auto sphereMesh(Mesh::CreateClass<SphereMesh>("Sphere"));
	sphereMesh->Set(3, 24);

    auto pEarthTexture1(std::make_shared<Texture2D>(Resource::GetOrCreateClass<ResourceFile>("data/Earthmap720x360_grid.jpg")));
	auto pEarthTexture2(std::make_shared<Texture2D>(Resource::GetOrCreateClass<ResourceFile>("data/jup0vss1.jpg")));
	auto pMaterial1(Material::Create("earth1"));
	pMaterial1->SetRenderPass(RenderPass::LIT);
	auto pMaterial2(Material::Create("earth2"));
	pMaterial2->SetRenderPass(RenderPass::LIT);
	pMaterial1->SetTexture(pEarthTexture1);
	pMaterial1->SetDiffuseColor(ColorRGB(0.8f, 0.8f, 0.8f));
	pMaterial1->SetSpecularColor(ColorRGB(1.0f, 0.0f, 0.0f));
    pMaterial1->SetShininess(10);
	pMaterial1->SetAmbientIntensity(0);

	pMaterial2->SetTexture(pEarthTexture2);
	pMaterial2->SetDiffuseColor(ColorRGB(0.8f, 0.8f, 0.8f));
	pMaterial2->SetSpecularColor(ColorRGB(1.0f, 0.0f, 0.0f));
    pMaterial2->SetShininess(100);
	pMaterial2->SetAmbientIntensity(0);

    auto camera = scene->CreateChild<Camera>();
    auto control = std::make_shared<CameraControl>(camera);

    const float STEP = 8.0f;
    Vector3 position(0, 0, -10);
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            earth[c][r] = scene->CreateChild<SceneNode>();
            earth[c][r]->SetMaterial(pMaterial1);
            earth[c][r]->SetPosition(position);
			earth[c][r]->SetMesh(sphereMesh);
            std::swap(pMaterial1, pMaterial2);
            position.x += STEP;
        }
        position.x = 0;
        position.y -= STEP;
    }

    Vertex3 camPos(COLS / 2 * STEP, -(ROWS) / 2 * STEP, 75);
    Vertex3 lighPos(Vertex2(camPos), 5);

    auto light = scene->CreateChild<Light>();
	//light->SetType(LightType::SPOT);
	//light->SetLocalLookAtPosition(VECTOR3_RIGHT);
    light->SetPosition(lighPos);

    camera->SetPosition(camPos);
    Vector3 lookAtPoint(Vertex2(camPos), 0);
    camera->SetGlobalLookAtPosition(lookAtPoint);

	control->AutoZoom();

	auto engine = Engine::Create();
    auto updateSlot = engine->SigUpdate()->Connect([&](float deltaTime)
    {
        static float x_angle(0);
        static float y_angle(0);

        for (int r = 0; r < ROWS; r++)
            for (int c = 0; c < COLS; c++)
                earth[c][r]->SetOrientation(AngleAxis(y_angle, Vertex3(0, 0, 1)) * AngleAxis(y_angle, Vertex3(0, 1, 0)));

        x_angle += PI / 10.0f * deltaTime;
        y_angle += PI / 10.0f * deltaTime;

		static float distance = 0;
		static float step = 15;
		light->SetDistance(distance);
		light->SetShadowClipEnd(distance);
		distance += step * deltaTime;
		if (distance > 50 || distance < 0)
			step *= -1;
    });

	window->SetScene(scene.get());
	return engine->Run();
};
