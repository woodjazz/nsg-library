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
#include "LevelOver.h"

LevelOver::LevelOver(PWindow window)
	: Level(window)
{
	scene_ = std::make_shared<Scene>();
    auto font = std::make_shared<FontAtlas>();
    auto loadingNode = scene_->CreateChild<SceneNode>();
    auto loadingMaterial = Material::Create();
    font->Set(Resource::GetOrCreate<ResourceFile>("data/AnonymousPro32.xml"));
    auto atlasTexture = std::make_shared<Texture2D>(Resource::GetOrCreate<ResourceFile>("data/AnonymousPro32.png"));
    font->SetTexture(atlasTexture);
    auto camera = scene_->CreateChild<Camera>();
    camera->SetPosition(Vector3(0, 0, 1));
    camera->EnableOrtho();
    camera->SetWindow(nullptr);
    loadingNode->SetMesh(font->GetOrCreateMesh("Over", CENTER_ALIGNMENT, MIDDLE_ALIGNMENT));
    loadingMaterial->SetTextMap(atlasTexture);
    loadingNode->SetMaterial(loadingMaterial);
	window->SetScene(scene_.get());

    slotKey_ = window->SigKey()->Connect([&](int key, int action, int modifier)
    {
        Level::Load(0, window_);
    });

}

LevelOver::~LevelOver()
{
	
}
