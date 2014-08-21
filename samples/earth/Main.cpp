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
#include "EarthBehavior.h"
#include "CameraBehavior.h"
#include "LightBehavior.h"
using namespace NSG;

struct Sample : App
{
    AppStatistics statistics_;
    PScene scene_;
    PCamera camera_;
    PSceneNode earth_;
    PLight light_;
    PMusic music_;

    Sample()
    {
        AppConfiguration::this_->width_ = 640;
        AppConfiguration::this_->height_ = 480;
    }

    void Start()
    {
        scene_ = PScene(new Scene);
        scene_->SetAmbientColor(Color(0.1f, 0.1f, 0.1f, 1));

        camera_ = PCamera(new Camera);
        camera_->SetBehavior(PBehavior(new CameraBehavior));
        camera_->Activate();

        scene_->Add(camera_);

        earth_ = PSceneNode(new SceneNode);
        earth_->SetBehavior(PBehavior(new EarthBehavior));

        scene_->Add(earth_);

        light_ = PLight(new Light);
        light_->SetBehavior(PBehavior(new LightBehavior));

        scene_->Add(light_);

        music_ = PMusic(new Music("data/nice_music.ogg"));
        music_->Play();

        scene_->Start();

    }

    void Update()
    {
        scene_->Update();
    }

    void RenderFrame()
    {
        scene_->Render();
    }

    void RenderGUIWindow()
    {
        IMGUIWindow(&statistics_, 25, 25);
    }
};

NSG_MAIN(Sample);
