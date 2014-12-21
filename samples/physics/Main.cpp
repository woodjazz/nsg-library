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
    auto scene = std::make_shared<Scene>("scene000");

    scene->SetAmbientColor(Color(0.1f, 0.1f, 0.1f, 1));
    auto camera = scene->GetOrCreateChild<Camera>("camera");
    camera->SetPosition(Vertex3(0, 2, 35));
    camera->SetAspectRatio(window->GetWidth(), window->GetHeight());
    auto resizeSlot = window->signalViewChanged_->Connect([&](int width, int height)
    {
        camera->SetAspectRatio(width, height);
    });

    auto control = std::make_shared<CameraControl>(camera);
    auto light = camera->GetOrCreateChild<Light>("light");
    {
        auto earth = scene->GetOrCreateChild<SceneNode>("earth");
        auto pSphereMesh(app.CreateSphereMesh(3, 24));
		earth->SetMesh(pSphereMesh);

        auto pEarthTexture(app.GetOrCreateTextureFile("data/Earthmap720x360_grid.jpg"));
        auto pMaterial(app.GetOrCreateMaterial("earth"));
        auto program(app.GetOrCreateProgram("EarthBehavior"));
        program->SetFlags((int)ProgramFlag::PER_PIXEL_LIGHTING);
        PTechnique technique(new Technique);
        PPass pass(new Pass);
        technique->Add(pass);
        pass->SetProgram(program);
        pMaterial->SetTechnique(technique);
        pMaterial->SetDiffuseMap(pEarthTexture);
        pMaterial->SetDiffuseColor(Color(0.8f, 0.8f, 0.8f, 1));
        pMaterial->SetSpecularColor(Color(1.0f, 0.0f, 0.0f, 1));
        pMaterial->SetShininess(10);
        earth->SetMaterial(pMaterial);
        earth->SetPosition(Vertex3(0, 10, 0));
        static auto rb = earth->GetOrCreateRigidBody();
        rb->SetMass(1);
        rb->SetShape(SH_SPHERE, false);
        rb->HandleCollisions(true);
        auto static slotCollision = earth->signalCollision_->Connect([&](const ContactPoint & contactInfo)
        {
            rb->SetLinearVelocity(Vertex3(0, 100, 0));
        });
    }

    {
        auto floor = scene->GetOrCreateChild<SceneNode>("floor");
        auto mesh = app.CreateBoxMesh(20, 1, 20);
		floor->SetMesh(mesh);
        auto rb = floor->GetOrCreateRigidBody();
        rb->SetMass(0);
        rb->SetShape(SH_BOX, true);
        auto texture(app.GetOrCreateTextureFile("data/wall.jpg"));
        auto material(app.GetOrCreateMaterial("floor"));
        material->SetDiffuseMap(texture);
        auto program(app.GetOrCreateProgram("program1"));
        program->SetFlags((int)ProgramFlag::PER_PIXEL_LIGHTING);
        PTechnique technique(new Technique);
        PPass pass(new Pass);
        technique->Add(pass);
        pass->SetProgram(program);
        material->SetTechnique(technique);
        floor->SetMaterial(material);
    }

    auto updateSlot = window->signalUpdate_->Connect([&](float deltaTime)
    {
        scene->Update(deltaTime);
    });

    auto renderSlot = window->signalRender_->Connect([&]()
    {
        scene->Render(camera.get());
    });

    return app.Run();
}