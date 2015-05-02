/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
    auto resource = Resource::GetOrCreate<ResourceFile>("data/bscroller.xml");
    AppData data(resource);
    auto soundMusic = Sound::Get("nice_music.ogg.004");
    Music music;
    music.Set(soundMusic->GetResource());
    music.Play();
	auto scene = data.scenes_.at(0);
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

    auto sound = Sound::Get("BigExplosion.wav.001");
    auto static slotCollision = player->SigCollision()->Connect([&](const ContactPoint & contactInfo)
    {
        if(!playerDestroyed)
        {
            sound->Play();
            playerDestroyed = true;
        }
    });

    auto moveSlot = controller->SigMoved()->Connect([&](float x, float y)
    {
        if (!playerDestroyed)
        {
            auto pos = player->GetPosition();
            pos.x += x * deltaTime;
            pos.y += y * deltaTime;
            if (pos.x > -horizontalLimit && pos.x < horizontalLimit && pos.y < verticalLimit && pos.y > -verticalLimit)
                player->SetPosition(pos);
        }
    });

    float alpha = 1;
    float alphaAdd = 0.1f;

	Engine engine;
	auto updateSlot = engine.SigUpdate()->Connect([&](float dt)
    {
        if (!playerDestroyed)
        {
            deltaTime = dt;
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

    return engine.Run();
}
