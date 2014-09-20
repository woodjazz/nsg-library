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
#include "ModelBehavior.h"
#include "CameraBehavior.h"
#include "LightBehavior.h"
using namespace NSG;

struct Sample : App
{
    PScene scene_;
    PCamera camera_;
    PSceneNode earth_;
    PLight light_;

    Sample()
    {
        //AppConfiguration::this_->width_ = 30;
        //AppConfiguration::this_->height_ = 20;
        AppConfiguration::this_->showStatistics_ = true;
    }

    void Start(int argc, char* argv[]) override
    {
		scene_ = GetCurrentScene();
        scene_->SetAmbientColor(Color(0.7f, 0.7f, 0.7f, 1));

		camera_ = scene_->CreateCamera("camera");
        camera_->AddBehavior(PBehavior(new CameraBehavior));
        camera_->Activate();

		earth_ = scene_->CreateSceneNodeFrom(PResourceFile(new ResourceFile("data/duck.xml")), "VisualSceneNode");
		earth_->AddBehavior(PBehavior(new ModelBehavior));

		light_ = scene_->CreateLight("light");
		light_->AddBehavior(PBehavior(new LightBehavior));
    }
};

NSG_MAIN(Sample);
