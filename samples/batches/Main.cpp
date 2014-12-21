/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 N�stor Silveira Gorski

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

    auto window = app.GetOrCreateWindow("window", 100, 100, 50, 30);

    const int COLS = 11;
    const int ROWS = 11;
    PSceneNode earth[COLS][ROWS];

    auto scene = std::make_shared<Scene>("scene000");
    auto sphereMesh(app.CreateSphereMesh(3, 24));

    auto pEarthTexture1(app.GetOrCreateTextureFile("data/Earthmap720x360_grid.jpg"));
    auto pEarthTexture2(app.GetOrCreateTextureFile("data/jup0vss1.jpg"));
    auto pMaterial1(app.GetOrCreateMaterial("earth1"));
    auto pMaterial2(app.GetOrCreateMaterial("earth2"));
    auto program(app.GetOrCreateProgram("program0"));
    program->SetFlags((int)ProgramFlag::PER_PIXEL_LIGHTING);
    PTechnique technique(new Technique);
    PPass pass(new Pass);
    technique->Add(pass);
    pass->SetProgram(program);
    pMaterial1->SetTechnique(technique);
    pMaterial1->SetDiffuseMap(pEarthTexture1);
    pMaterial1->SetDiffuseColor(Color(0.8f, 0.8f, 0.8f, 1));
    pMaterial1->SetSpecularColor(Color(1.0f, 0.0f, 0.0f, 1));
    pMaterial1->SetShininess(10);

    pMaterial2->SetTechnique(technique);
    pMaterial2->SetDiffuseMap(pEarthTexture2);
    pMaterial2->SetDiffuseColor(Color(0.8f, 0.8f, 0.8f, 1));
    pMaterial2->SetSpecularColor(Color(1.0f, 0.0f, 0.0f, 1));
    pMaterial2->SetShininess(100);

    auto camera = scene->GetOrCreateChild<Camera>("camera");
	camera->SetAspectRatio(window->GetWidth(), window->GetHeight());

    auto resizeSlot = window->signalViewChanged_->Connect([&](int width, int height)
    {
        camera->SetAspectRatio(width, height);
    });

    auto control = std::make_shared<CameraControl>(camera);

    const float STEP = 8.0f;
    Vector3 position(0, 0, -10);
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            earth[c][r] = scene->GetOrCreateChild<SceneNode>(GetUniqueName());
            earth[c][r]->SetPosition(position);
			earth[c][r]->SetMesh(sphereMesh);
            earth[c][r]->SetMaterial(pMaterial1);
            std::swap(pMaterial1, pMaterial2);
            position.x += STEP;
        }
        position.x = 0;
        position.y -= STEP;
    }

    Vertex3 camPos(COLS / 2 * STEP, -(ROWS) / 2 * STEP, 75);
    Vertex3 lighPos(Vertex2(camPos), -5);

    auto light = scene->GetOrCreateChild<Light>("light");
    light->SetPosition(lighPos);

    camera->SetPosition(camPos);
    Vector3 lookAtPoint(Vertex2(camPos), 0);
    camera->SetGlobalLookAt(lookAtPoint);

	control->AutoZoom();

    auto updateSlot = window->signalUpdate_->Connect([&](float deltaTime)
    {
        static float x_angle(0);
        static float y_angle(0);

        for (int r = 0; r < ROWS; r++)
            for (int c = 0; c < COLS; c++)
                earth[c][r]->SetOrientation(glm::angleAxis(y_angle, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle, Vertex3(0, 1, 0)));

        x_angle += glm::pi<float>() / 10.0f * deltaTime;
        y_angle += glm::pi<float>() / 10.0f * deltaTime;

        control->OnUpdate(deltaTime);
    });

	auto renderSlot = window->signalRender_->Connect([&]()
	{
		scene->Render(camera.get());
	});

    auto slotMouseMoved = window->signalMouseMoved_->Connect([&](float x, float y)
    {
        control->OnMousemoved(x, y);
    });

    auto slotMouseDown = window->signalMouseDown_->Connect([&](int button, float x, float y)
    {
        control->OnMouseDown(button, x, y);
    });

    auto slotMouseUp = window->signalMouseUp_->Connect([&](int button, float x, float y)
    {
        control->OnMouseUp(button, x, y);
    });

    auto slotMouseWheel = window->signalMouseWheel_->Connect([&](float x, float y)
    {
        control->OnMousewheel(x, y);
    });

    auto slotMultiGesture = window->signalMultiGesture_->Connect([&](int timestamp, float x, float y, float dTheta, float dDist, int numFingers)
    {
        control->OnMultiGesture(timestamp, x, y, dTheta, dDist, numFingers);
    });

    auto slotKey = window->signalKey_->Connect([&](int key, int action, int modifier)
    {
        control->OnKey(key, action, modifier);
    });

    return app.Run();
};