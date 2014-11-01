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
using namespace NSG;

struct Sample : App
{
    PScene scene_;
    PLight light_;
    PNode node_;

    Sample()
    {
        AppConfiguration::this_->showStatistics_ = true;
    }

    void Start(int argc, char* argv[]) override
    {
        scene_ = GetCurrentScene();

        PResourceFile resource(GetOrCreateResourceFile("data/scene.xml"));
        scene_->Load(resource);
    }

    void OnSceneLoaded() override
    {
        PCamera camera;
        auto& cameras = scene_->GetCameras();
        if (cameras.size())
            camera = cameras[0];
        else
            camera = scene_->CreateCamera("");

        camera->SetPosition(Vector3(0, 70, 100));

        camera->Activate();
        camera->AddBehavior(PCameraControl(new CameraControl));
        light_ = scene_->CreateLight("");
        light_->SetType(Light::DIRECTIONAL);
        //light_->SetPosition(Vertex3(100, 0, 0));
        light_->SetLookAt(Vector3(1, 0, 0));
        light_->SetDiffuseColor(Color(1, 0, 0, 1));
        //camera->AddChild(light_);

        scene_->PlayAnimation("AnimationSet0", true);
        scene_->SetAnimationSpeed("AnimationSet0", 0.001f);

        node_ = Node::GetUniqueNodeFrom(scene_, "Body");
    }

    void OnKey(int key, int action, int modifier) override
    {
        App::OnKey(key, action, modifier);
        
        switch (key)
        {
        case NSG_KEY_W:
        {
            node_->Translate(Vector3(0, 0, 1));
            break;
        }

        case NSG_KEY_S:
        {
            node_->Translate(Vector3(0, 0, -1));
            break;
        }

        case NSG_KEY_LEFT:
        {
            node_->Translate(Vector3(-1, 0, 0));
            break;
        }

        case NSG_KEY_RIGHT:
        {
            node_->Translate(Vector3(1, 0, 0));
            break;
        }

        }
    }


};

NSG_MAIN(Sample);
