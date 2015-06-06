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
	auto resource = Resource::GetOrCreate<ResourceFile>("data/bscene.xml.lz4");
	AppData data(resource);
	auto scene = data.scenes_.at(0);
    scene->SetAmbientColor(Color(0.0f));

    auto object = scene->GetChild<SceneNode>("Bone", true);
    auto plane = scene->GetChild<SceneNode>("Plane", false);
    plane->GetMaterial()->SetShininess(10);

    auto camera = scene->GetChild<Camera>("Camera", false);
	camera->SetFarClip(20);
    auto control = std::make_shared<CameraControl>(camera);
    auto sun = scene->GetChild<Light>("Sun", false);
	sun->EnableShadows(true);
    auto ball = scene->GetChild<SceneNode>("Earth", false);
    auto ramp1 = scene->GetChild<SceneNode>("Ramp1", false);
    auto ramp2 = scene->GetChild<SceneNode>("Ramp2", false);

    auto ballRigidBody = ball->GetOrCreateRigidBody();

    {
        auto animations = scene->GetAnimationsFor(object);
        auto animation = animations[0];
        animation->Play(true);
    }

	ballRigidBody->HandleCollisions(true);
	auto static slotCollision = ball->SigCollision()->Connect([&](const ContactPoint & contactInfo)
	{
		ballRigidBody->SetLinearVelocity(4.f * contactInfo.normalB_);
	});

    auto window = Window::Create();
    
    control->slotMouseDown_ = window->SigMouseDown()->Connect([&](int button, float x, float y)
    {
        if (button == NSG_BUTTON_LEFT)
        {
            control->OnMouseDown(button, x, y);
        }
        else
        {
            Ray ray = camera->GetScreenRay(x, y);
            RayNodeResult closest;
			if (scene->GetClosestRayNodeIntersection(ray, closest))
            {
                Vertex3 pos = ray.GetPoint(closest.distance_);
                pos.y = 7;
                ball->SetGlobalPosition(pos);
                ballRigidBody->SetLinearVelocity(Vector3(0));
                ballRigidBody->SyncWithNode();
            }
        }

    });
	return Engine().Run();
}
