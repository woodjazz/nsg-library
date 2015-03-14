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

    auto window = app.GetOrCreateWindow("window", 100, 400, 10, 10);

    PSceneNode node1;

    auto scene = std::make_shared<Scene>("scene000");
    auto camera = scene->GetOrCreateChild<Camera>("camera");
	camera->SetPosition(Vertex3(0, 0, 10));

    #if 1
    camera->AddBlurFilter();
    //camera->AddBlendFilter();
    auto slotUpdate = window->signalUpdate_->Connect([&](float deltaTime)
    {
        static float y_angle = 0;
        y_angle += glm::pi<float>() / 10.0f * deltaTime;
        node1->SetOrientation(glm::angleAxis(y_angle, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle, Vertex3(0, 1, 0)));
    });
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
    auto userFilter = camera->AddUserFilter(resourceMem);
    auto slotUpdate = window->signalUpdate_->Connect([&](float deltaTime)
    {
        static float y_angle = 0;
        y_angle += glm::pi<float>() / 10.0f * deltaTime;
        node1->SetOrientation(glm::angleAxis(y_angle, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle, Vertex3(0, 1, 0)));
        static float move = -1;
        move += deltaTime * TWO_PI * 0.25f;  // 1/4 of a wave cycle per second
        userFilter->GetMaterial()->SetShininess(move);
    });

    #endif

    auto control = std::make_shared<CameraControl>(camera);

    scene->GetOrCreateChild<Light>("light");

    {
        auto mesh = app.CreateBoxMesh();
        auto material = app.GetOrCreateMaterial("material1", (int)ProgramFlag::PER_PIXEL_LIGHTING);
        material->SetDiffuseMap(std::make_shared<Texture>(app.GetOrCreateResourceFile("data/wall.jpg")));

        node1 = scene->GetOrCreateChild<SceneNode>("node1");
        node1->SetPosition(Vertex3(3, -2, 0));
        node1->SetMesh(mesh);
        node1->SetMaterial(material);
    }

    {
        auto mesh = app.CreateSphereMesh();
        auto material = app.GetOrCreateMaterial("material2", (int)ProgramFlag::PER_PIXEL_LIGHTING);
        material->SetDiffuseMap(std::make_shared<Texture>(app.GetOrCreateResourceFile("data/stone.jpg")));

        auto node = scene->GetOrCreateChild<SceneNode>("node2");
        node->SetPosition(Vertex3(-3, 2, 0));
        node->SetMesh(mesh);
        node->SetMaterial(material);
    }

    auto renderSlot = window->signalRender_->Connect([&]()
    {
        scene->Render();
    });

    return app.Run();
};
