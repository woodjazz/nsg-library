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
    auto resource = Resource::GetOrCreate<ResourceFile>("data/bscroller.xml");
    auto scenes = resource->Load();
    auto scene = scenes.at(0);
    auto camera = scene->GetChild<Camera>("Camera", false);
    auto frustum = camera->GetFrustum();
    auto player = camera->GetChild<SceneNode>("Player", false);
    auto window = Window::Create();
    float deltaTime = 0;
    auto controller = std::make_shared<PlayerControl>();
    auto& orthoProjection = camera->GetOrthoProjection();
    auto cameraHalfWidth = (orthoProjection.right_ - orthoProjection.left_) / 2;
    auto cameraHalfHeight = (orthoProjection.top_ - orthoProjection.bottom_) / 2;
    auto& bbPlayer = player->GetWorldBoundingBox();
    auto playerHalfWidth = bbPlayer.Size().x * 0.5f;
    auto playerHalfHeight = bbPlayer.Size().y * 0.5f;
    auto horizontalLimit = cameraHalfWidth - playerHalfWidth;
    auto verticalLimit = cameraHalfHeight - playerHalfHeight;
    bool playerDestroyed = false;
    auto playerRigidBody = player->GetRigidBody();
    playerRigidBody->HandleCollisions(true);

    auto static slotCollision = player->signalCollision_->Connect([&](const ContactPoint & contactInfo)
    {
        playerDestroyed = true;
    });

    auto leftSlot = controller->signalLeft_->Connect([&]()
    {
        if (!playerDestroyed)
        {
            auto pos = player->GetPosition();
            pos.x -= deltaTime;
            if (pos.x > -horizontalLimit)
                player->SetPosition(pos);
        }
    });

    auto rightSlot = controller->signalRight_->Connect([&]()
    {
        if (!playerDestroyed)
        {
            auto pos = player->GetPosition();
            pos.x += deltaTime;
            if (pos.x < horizontalLimit)
                player->SetPosition(pos);
        }
    });

    auto forwardSlot = controller->signalForward_->Connect([&]()
    {
        if (!playerDestroyed)
        {
            auto pos = player->GetPosition();
            pos.y += deltaTime;
            if (pos.y < verticalLimit)
                player->SetPosition(pos);
        }
    });

    auto backwardSlot = controller->signalBackward_->Connect([&]()
    {
        if (!playerDestroyed)
        {
            auto pos = player->GetPosition();
            pos.y -= deltaTime;
            if (pos.y > -verticalLimit)
                player->SetPosition(pos);
        }
    });

    float alpha = 1;
    float alphaAdd = 0.1f;

    auto updateSlot = window->signalUpdate_->Connect([&](float dt)
    {
        if (!playerDestroyed)
        {
            deltaTime = dt;
            scene->Update(dt);
            auto pos = camera->GetPosition();
            pos.x += dt;
            camera->SetPosition(pos);
        }
        else
        {
            auto material = player->GetMaterial();
            alpha += alphaAdd;
            if (alpha > 1 || alpha < 0)
                alphaAdd *= -1;
            material->SetColor(Color(1, 1, 1, alpha));
        }

    });

    auto renderSlot = window->signalRender_->Connect([&]()
    {
        scene->Render();
    });

    return Window::RunApp();
}
