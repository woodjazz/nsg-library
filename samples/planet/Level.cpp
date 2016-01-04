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
#include "Level.h"
#include "Level0.h"
#include "Level1.h"
#include "LevelOver.h"
#include "LevelResources.h"

std::shared_ptr<Level> Level::currentLevel_;
PScene Level::scene_;

Level::Level(PWindow window)
    : window_(window),
      levelIndex_(0)
{
}

Level::~Level()
{
}

void Level::Load(int idx, PWindow window)
{
    Level::currentLevel_ = nullptr;
    std::shared_ptr<Level> level;

    switch (idx)
    {
        case -1:
            level = std::make_shared<LevelResources>(window, std::vector<const char*>
            {
                "data/AnonymousPro32.xml",
                "data/AnonymousPro32.png",
                "data/earthcolor.jpg",
                "data/earthspecular.jpg",
                "data/earthnormal.jpg",
                "data/clouds.jpg",
                "data/explo.png",
                "data/explo.wav"
            });
            break;
        case 0:
            level = std::make_shared<Level0>(window);
            break;
        case 1:
            level = std::make_shared<Level1>(window);
            break;
        case 2:
            level = std::make_shared<LevelOver>(window);
            break;
        default:
            CHECK_ASSERT(false);
            break;
    }

    //Renderer::GetPtr()->EnableDebugPhysics(true);
    level->SetIndex(idx);

    Level::currentLevel_ = level;
}

void Level::AddObject(PGameObject object)
{
    objects_[object.get()] = object;
}

void Level::RemoveObject(GameObject* object)
{
    objects_.erase(object);
}

float Level::GetFlyDistance()
{
    auto grid = scene_->GetChild<SceneNode>("Grid", true);
    auto bb = grid->GetWorldBoundingBox();
    return Length(grid->GetGlobalScale() * 0.6f);
}


