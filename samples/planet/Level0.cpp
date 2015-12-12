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
#include "Level0.h"
#include "Planet.h"
#include "Sun.h"
#include "Enemy.h"
#include "Player.h"

Level0::Level0(PWindow window)
	: Level(window)
{
	scene_ = std::make_shared<Scene>("Level1");
	scene_->SetAmbientColor(ColorRGB(0.1f));
    AddObject(std::make_shared<Planet>(scene_));
    AddObject(std::make_shared<Sun>(scene_));
    player_ = std::make_shared<Player>(scene_);
	enemy_ = std::make_shared<Enemy>(scene_);
	Enemy::SetTotal(1);
	enemy_->SetPosition(-PI10, 0);
    camera_ = player_->GetCameraNode()->CreateChild<Camera>();
    camera_->SetPosition(Vertex3(0, 0, 10));
	window->SetScene(scene_.get());
}

Level0::~Level0()
{
	
}
