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
#include <deque>
int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;

    App app;

    auto window = app.GetOrCreateWindow("window", 100, 100, 10, 10);
    std::deque<Vertex3> camControlPoints;

    auto scene = std::make_shared<Scene>("scene000");
    scene->SetAmbientColor(Color(0.1f, 0.1f, 0.1f, 1));

    auto camera = scene->GetOrCreateChild<Camera>("camera");

	{
        camControlPoints.push_back(Vertex3(-10.0f, 0.0f, 0.0f));
        camControlPoints.push_back(Vertex3(0.0f, 0.0f, 10.0f));
        camControlPoints.push_back(Vertex3(10.0f, 0.0f, 0.0f));
        camControlPoints.push_back(Vertex3(0.0f, 0.0f, -10.0f));

        camera->SetPosition(Vertex3(0, 0, 10));
        camera->SetGlobalLookAt(Vertex3(0));
    }

    auto earth = scene->GetOrCreateChild<SceneNode>("node 1");
    {
        auto pSphereMesh(app.CreateSphereMesh(3, 24));
        earth->SetMesh(pSphereMesh);
        //earth->SetEnabled(false);

        auto earthResource = app.GetOrCreateResourceFile("data/Earthmap720x360_grid.jpg");
        auto pEarthTexture = std::make_shared<Texture>(earthResource, (int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y);
        auto pMaterial(app.GetOrCreateMaterial("earth", (int)ProgramFlag::PER_PIXEL_LIGHTING));
        pMaterial->SetDiffuseMap(pEarthTexture);
        pMaterial->SetDiffuseColor(Color(0.8f, 0.8f, 0.8f, 1));
        pMaterial->SetSpecularColor(Color(1.0f, 0.0f, 0.0f, 1));
        pMaterial->SetShininess(10);

        earth->SetMaterial(pMaterial);

        earth->SetPosition(Vertex3(5, 0, 0));
    }

    auto light = scene->GetOrCreateChild<Light>("light");
    {
		auto pMaterial(app.GetOrCreateMaterial("light", (int)ProgramFlag::PER_VERTEX_LIGHTING));
        pMaterial->SetColor(Color(1, 0, 0, 1));
        light->SetMaterial(pMaterial);

		auto pMesh(app.CreateSphereMesh(0.2f, 16));
        light->SetMesh(pMesh);

        light->SetPosition(Vertex3(-10.0, 0.0, 5.0));
    }

    auto resource = app.GetOrCreateResourceFile("data/nice_music.ogg");
    auto music = std::make_shared<Music>(resource);
    music->Play();

    auto slotUpdate = window->signalUpdate_->Connect([&](float deltaTime)
    {
        {
            static float delta1 = 0;

            Vertex3 position = glm::catmullRom(
                                   camControlPoints[0],
                                   camControlPoints[1],
                                   camControlPoints[2],
                                   camControlPoints[3],
                                   delta1);

            camera->SetPosition(position);
            camera->SetGlobalLookAt(Vertex3(0));

            delta1 += deltaTime * 0.1f;

            if (delta1 > 1)
            {
                delta1 = 0;
                Vertex3 p = camControlPoints.front();
                camControlPoints.pop_front();
                camControlPoints.push_back(p);
            }
        }

        {
            static float x_angle = 0;
            static float y_angle = 0;
            x_angle += glm::pi<float>() / 10.0f * deltaTime;
            y_angle += glm::pi<float>() / 10.0f * deltaTime;

            earth->SetOrientation(glm::angleAxis(y_angle, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle, Vertex3(0, 1, 0)));
        }
    });

	auto renderSlot = window->signalRender_->Connect([&]()
	{
		scene->Render();
	});

    return app.Run();
}

