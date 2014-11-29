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
#include <deque>
using namespace NSG;

struct Sample : App
{
    PScene scene_;
    PCamera camera_;
    PSceneNode earth_;
    PLight light_;
    PMusic music_;
    std::deque<Vertex3> camControlPoints_;
    SignalStart::PSlot slotStart_;
    SignalUpdate::PSlot slotUpdate_;

    Sample()
    {
        AppConfiguration::this_->showStatistics_ = true;

        slotStart_ = signalStart_->Connect([&](int argc, char* argv[])
        {
            scene_ = GetOrCreateScene("scene000");
            SetCurrentScene(scene_);
            scene_->SetAmbientColor(Color(0.1f, 0.1f, 0.1f, 1));

            camera_ = scene_->GetOrCreateChild<Camera>("camera");
            {
                camControlPoints_.push_back(Vertex3(-10.0f, 0.0f, 0.0f));
                camControlPoints_.push_back(Vertex3(0.0f, 0.0f, 10.0f));
                camControlPoints_.push_back(Vertex3(10.0f, 0.0f, 0.0f));
                camControlPoints_.push_back(Vertex3(0.0f, 0.0f, -10.0f));

                camera_->SetPosition(Vertex3(0, 0, 10));
                camera_->SetGlobalLookAt(Vertex3(0));
            }
            camera_->Activate();

            earth_ = scene_->GetOrCreateChild<SceneNode>("node 1");
            {
                PSphereMesh pSphereMesh(CreateSphereMesh(3, 24));
                earth_->Set(pSphereMesh);
                //earth_->SetEnabled(false);

                PTexture pEarthTexture(GetOrCreateTextureFile("data/Earthmap720x360_grid.jpg"));
                PMaterial pMaterial(GetOrCreateMaterial("earth"));
                PProgram program(GetOrCreateProgram("program"));
                program->SetFlags((int)ProgramFlag::PER_PIXEL_LIGHTING);
                PTechnique technique(new Technique);
                PPass pass(new Pass);

                technique->Add(pass);
                pass->SetProgram(program);
                pMaterial->SetTechnique(technique);
                pMaterial->SetDiffuseMap(pEarthTexture);
                pMaterial->SetDiffuseColor(Color(0.8f, 0.8f, 0.8f, 1));
                pMaterial->SetSpecularColor(Color(1.0f, 0.0f, 0.0f, 1));
                pMaterial->SetShininess(10);

                earth_->Set(pMaterial);

                earth_->SetPosition(Vertex3(5, 0, 0));
            }

            light_ = scene_->GetOrCreateChild<Light>("light");
            {
                PMaterial pMaterial(GetOrCreateMaterial("light"));
                pMaterial->SetColor(Color(1, 0, 0, 1));
                PProgram pProgram = GetOrCreateProgram("program");
                PTechnique technique(new Technique);
                PPass pass(new Pass);
                technique->Add(pass);
                pass->SetProgram(pProgram);
                pMaterial->SetTechnique(technique);

                light_->Set(pMaterial);

                PMesh pMesh(CreateSphereMesh(0.2f, 16));
                light_->Set(pMesh);

                light_->SetPosition(Vertex3(-10.0, 0.0, 5.0));
            }

            PResourceFile resource(ResourceFileManager::this_->GetOrCreate("data/nice_music.ogg"));
            music_ = PMusic(new Music(resource));
            music_->Play();
        });

        slotUpdate_ = signalUpdate_->Connect([&](float deltaTime)
        {
            {
                static float delta1 = 0;

                Vertex3 position = glm::catmullRom(
                                       camControlPoints_[0],
                                       camControlPoints_[1],
                                       camControlPoints_[2],
                                       camControlPoints_[3],
                                       delta1);

                camera_->SetPosition(position);
                camera_->SetGlobalLookAt(Vertex3(0));

                delta1 += deltaTime * 0.1f;

                if (delta1 > 1)
                {
                    delta1 = 0;
                    Vertex3 p = camControlPoints_.front();
                    camControlPoints_.pop_front();
                    camControlPoints_.push_back(p);
                }
            }

            {
                static float x_angle = 0;
                static float y_angle = 0;
                x_angle += glm::pi<float>() / 10.0f * deltaTime;
                y_angle += glm::pi<float>() / 10.0f * deltaTime;

                earth_->SetOrientation(glm::angleAxis(y_angle, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle, Vertex3(0, 1, 0)));
            }
        });
    }

};

NSG_MAIN(Sample);
