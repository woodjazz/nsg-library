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
    auto resource = Resource::GetOrCreate<ResourceFile>("data/scene.xml");
    LoaderXML loader("loader");
    auto slotLoaded = loader.Load(resource)->Connect([&]()
    {
        auto scene = loader.GetScene(0);
        scene->SetAmbientColor(ColorRGB(0.0f));

        auto armature = scene->GetChild<SceneNode>("Armature", true);
        auto plane = scene->GetChild<SceneNode>("Plane", false);
        plane->GetMaterial()->SetShininess(10);

        plane->GetMaterial()->CastShadow(true);

        auto camera = scene->GetChild<Camera>("Camera", false);
        //camera->SetNearClip(0.1f);
        auto control = std::make_shared<CameraControl>(camera);
        auto sun = scene->GetChild<Light>("Sun", false);
        sun->EnableShadows(true);
        auto ball = scene->GetChild<SceneNode>("Earth", false);
        auto ramp1 = scene->GetChild<SceneNode>("Ramp1", false);
        auto ramp2 = scene->GetChild<SceneNode>("Ramp2", false);

        static auto ballRigidBody = ball->GetOrCreateRigidBody();

        {
            auto controller = armature->GetOrCreateAnimationController();
            controller->Play("ArmatureAction.000", true);
        }

        ballRigidBody->HandleCollisions(true);
        auto static slotCollision = ball->SigCollision()->Connect([&](const ContactPoint & contactInfo)
        {
            ballRigidBody->SetLinearVelocity(4.f * contactInfo.normalB_);
        });

        auto sunLight = scene->GetChild<Light>("Sun", true);
        //sunLight->SetShadowColor(Color(1, 0, 0, 1));
        sunLight->SetBias(.666f);
        //window->ShowMap(sunLight->GetShadowMap(3));
        window->SetScene(scene.get());
    });
    return Engine::Create()->Run();
}
