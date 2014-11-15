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
using namespace NSG;

struct Sample : App
{
    PScene scene_;
    PCamera camera_;
    PSceneNode earth_;
    PSceneNode floor_;
    PLight light_;
    PMusic music_;

    Sample()
    {
        AppConfiguration::this_->showStatistics_ = true;
    }

    void Start(int argc, char* argv[]) override
    {
        scene_ = GetCurrentScene();
        scene_->SetAmbientColor(Color(0.1f, 0.1f, 0.1f, 1));

		camera_ = scene_->GetOrCreateChild<Camera>("camera");
        camera_->Activate();
        camera_->SetPosition(Vertex3(0, 2, 35));
        camera_->AddBehavior(PCameraControl(new CameraControl));

		light_ = camera_->GetOrCreateChild<Light>("light");

		earth_ = scene_->GetOrCreateChild<SceneNode>("earth");
        earth_->AddBehavior(PBehavior(new EarthBehavior));

        {
			floor_ = scene_->GetOrCreateChild<SceneNode>("floor");
            PMesh mesh = CreateBoxMesh(20, 1, 20);
            floor_->Set(mesh);
            PRigidBody rb = CreateRigidBody();
            rb->SetMass(0);
            rb->SetShape(SH_BOX, true);
            floor_->Set(rb);
            PTexture texture(GetOrCreateTextureFile("data/wall.jpg"));
            PMaterial material(GetOrCreateMaterial("floor"));
            material->SetDiffuseMap(texture);
            PProgram program(GetOrCreateProgram("program1"));
            program->SetFlags((int)ProgramFlag::PER_PIXEL_LIGHTING);
            PTechnique technique(new Technique);
            PPass pass(new Pass);
            technique->Add(pass);
            pass->SetProgram(program);
            material->SetTechnique(technique);
            floor_->Set(material);
        }
    }
};

NSG_MAIN(Sample);
