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
    PCamera camera_;

    PSceneNode box_;
    PMaterial boxMaterial_;
    std::deque<Vertex3> boxControlPoints_;
    PBoxMesh boxMesh_;

    PSceneNode sphere_;
    PMaterial sphereMaterial_;
    PSphereMesh sphereMesh_;

    PTechnique technique_;
    PShowTexture showTexture_;

    SignalStart::PSlot slotStart_;
    SignalUpdate::PSlot slotUpdate_;

    Sample()
    {
        AppConfiguration::this_->showStatistics_ = true;

        slotStart_ = signalStart_->Connect([&](int argc, char* argv[])
        {
            scene_ = GetOrCreateScene("scene000");
            SetCurrentScene(scene_);

            camera_ = scene_->GetOrCreateChild<Camera>(GetUniqueName());
            camera_->SetPosition(Vertex3(0, 0, 10));
            camera_->Activate();

            box_ = scene_->GetOrCreateChild<SceneNode>(GetUniqueName());
            box_->SetScale(Vertex3(3, 3, 3));
            {
                boxControlPoints_.push_back(Vertex3(-5.0f, 0.0f, 0.0f));
                boxControlPoints_.push_back(Vertex3(0.0f, 0.0f, 5.0f));
                boxControlPoints_.push_back(Vertex3(5.0f, 0.0f, 0.0f));
                boxControlPoints_.push_back(Vertex3(0.0f, 0.0f, -5.0f));

                boxMesh_ = CreateBoxMesh(1, 1, 1, 2, 2, 2);
                boxMaterial_ = GetOrCreateMaterial(GetUniqueName());
                boxMaterial_->SetTexture0(GetOrCreateTextureFile("data/cube.png"));
            }

            sphere_ = scene_->GetOrCreateChild<SceneNode>(GetUniqueName());
            {
                sphereMesh_ = CreateSphereMesh(3, 32);
                sphereMaterial_ = GetOrCreateMaterial(GetUniqueName());
                PTexture texture(GetOrCreateTextureFile("data/Earth.jpg"));
                sphereMaterial_->SetTexture0(texture);
            }


            showTexture_ = PShowTexture(new ShowTexture);

            technique_ = PTechnique(new Technique);

            PPass normalPass(new Pass);
            PProgram normalProgram(GetOrCreateProgram(GetUniqueName()));
            normalProgram->SetFlags((int)ProgramFlag::UNLIT | (int)ProgramFlag::DIFFUSEMAP);
            normalPass->SetProgram(normalProgram);

            PPass depthPass(new Pass);
            depthPass->EnableColorBuffer(false);
            PProgram depthProgram(GetOrCreateProgram(GetUniqueName()));
            depthProgram->SetFlags((int)ProgramFlag::STENCIL);
            depthPass->SetProgram(depthProgram);

            PFilter boxFilter;

            {
                //box passes
                PPass2Texture pass2Texture(new Pass2Texture(1024, 1024));
                technique_->Add(pass2Texture);

                boxFilter = PFilter(new Filter(GetUniqueName(), pass2Texture->GetTexture(), 1024, 1024));

                static const char*
                fShader = STRINGIFY(void main()
                {
                    vec2 texcoord = v_texcoord0;
                    texcoord.x += sin(texcoord.y * 4.0 * 2.0 * 3.14159 + u_material.shininess) / 100.0;
                    gl_FragColor = texture2D(u_texture0, texcoord);
                });

                PResourceMemory resource(new ResourceMemory(fShader));
                boxFilter->GetProgram()->SetFragmentShader(resource);

                pass2Texture->Add(depthPass, sphere_.get(), nullptr, sphereMesh_);
                pass2Texture->Add(normalPass, box_.get(), boxMaterial_, boxMesh_);

                PPassFilter filterPass(new PassFilter(boxFilter));
                technique_->Add(filterPass);
            }

            #if 1
            PFilter sphereBlendFilter;
            {
                //sphere passes

                PPass2Texture pass2Texture(new Pass2Texture(1024, 1024));
                technique_->Add(pass2Texture);
                pass2Texture->Add(depthPass, box_.get(), nullptr, boxMesh_);
                pass2Texture->Add(normalPass, sphere_.get(), sphereMaterial_, sphereMesh_);

                PFilter blurFilter(new FilterBlur(pass2Texture->GetTexture(), 16, 16));

                PPassFilter filterPass(new PassFilter(blurFilter));
                technique_->Add(filterPass);


                sphereBlendFilter = PFilter(new FilterBlend(blurFilter->GetTexture(), pass2Texture->GetTexture(), 1024, 1024));

                PPassFilter passBlend(new PassFilter(sphereBlendFilter));
                technique_->Add(passBlend);

                //showTexture_->SetNormal(pass2Texture->GetTexture());
            }

            PFilter blendFilter(new FilterBlend(sphereBlendFilter->GetTexture(), boxFilter->GetTexture(), 1024, 1024));
            PPassFilter passBlend(new PassFilter(blendFilter));
            technique_->Add(passBlend);
            #endif

            showTexture_->SetNormal(blendFilter->GetTexture());
            //showTexture_->SetNormal(sphereBlendFilter->GetTexture());
            //showTexture_->SetNormal(boxBehavior_->filteredTexture_);
            //showTexture_->SetNormal(boxBehavior_->renderedTexture_);
            //showTexture_->SetNormal(sphereBehavior_->renderedTexture_);
            //showTexture_->SetNormal(sphereBehavior_->filteredTexture_);
        });

        slotUpdate_ = signalUpdate_->Connect([&](float deltaTime)
        {
            {
                static float delta1 = 0;

                Vertex3 position = glm::catmullRom(
                                       boxControlPoints_[0],
                                       boxControlPoints_[1],
                                       boxControlPoints_[2],
                                       boxControlPoints_[3],
                                       delta1);

                box_->SetPosition(position);

                delta1 += deltaTime * 0.3f;

                if (delta1 > 1)
                {
                    delta1 = 0;
                    Vertex3 p = boxControlPoints_.front();
                    boxControlPoints_.pop_front();
                    boxControlPoints_.push_back(p);
                }

                static float move = -1;
                move += deltaTime * TWO_PI * 0.25f;  // 1/4 of a wave cycle per second
                boxMaterial_->SetShininess(move);
            }

            {
                static float y_angle = 0;
                y_angle += glm::pi<float>() / 10.0f * deltaTime;
                sphere_->SetOrientation(glm::angleAxis(y_angle, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle, Vertex3(0, 1, 0)));
            }

        });

    }

    void RenderFrame() override
    {
        technique_->Render();
        showTexture_->Show();
    }
};

NSG_MAIN(Sample);
