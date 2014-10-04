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
    static const int COLS = 11;
    static const int ROWS = 11;
    PSceneNode earth_[COLS][ROWS];
    PLight light_;

    Sample()
    {
        //AppConfiguration::this_->width_ = 30;
        //AppConfiguration::this_->height_ = 20;
        AppConfiguration::this_->showStatistics_ = true;
    }

    void Start(int argc, char* argv[]) override
    {
        PSphereMesh pSphereMesh(CreateSphereMesh(3, 24));

        PTexture pEarthTexture1(new TextureFile("data/Earthmap720x360_grid.jpg"));
        PTexture pEarthTexture2(new TextureFile("data/jup0vss1.jpg"));
        PMaterial pMaterial1(CreateMaterial("earth1"));
		PMaterial pMaterial2(CreateMaterial("earth2"));
		PProgram perVertex(new Program("", Program::PER_PIXEL_LIGHTING));
        PTechnique technique(new Technique);
        PPass pass(new Pass);
        technique->Add(pass);
        pass->SetProgram(perVertex);
        pMaterial1->SetTechnique(technique);
        pMaterial1->SetTexture0(pEarthTexture1);
        pMaterial1->SetDiffuseColor(Color(0.8f, 0.8f, 0.8f, 1));
        pMaterial1->SetSpecularColor(Color(1.0f, 0.0f, 0.0f, 1));
        pMaterial1->SetShininess(10);

        pMaterial2->SetTechnique(technique);
        pMaterial2->SetTexture0(pEarthTexture2);
        pMaterial2->SetDiffuseColor(Color(0.8f, 0.8f, 0.8f, 1));
        pMaterial2->SetSpecularColor(Color(1.0f, 0.0f, 0.0f, 1));
        pMaterial2->SetShininess(100);

		scene_ = GetCurrentScene();

        camera_ = scene_->CreateCamera("camera");
        camera_->Activate();

        camera_->AddBehavior(PCameraControl(new CameraControl));

        static const float STEP = 8.0f;
        Vector3 position(0, 0, -10);
        for (int r = 0; r < ROWS; r++)
        {
            for (int c = 0; c < COLS; c++)
            {
                earth_[c][r] = scene_->CreateSceneNode("");
				earth_[c][r]->AddBehavior(PBehavior(new EarthBehavior));
                earth_[c][r]->SetPosition(position);
                earth_[c][r]->Set(pSphereMesh);
                earth_[c][r]->Set(pMaterial1);
                std::swap(pMaterial1, pMaterial2);

                position.x += STEP;
            }
            position.x = 0;
            position.y -= STEP;
        }

        Vertex3 camPos(COLS/2*STEP, -(ROWS)/2*STEP, 75);
        Vertex3 lighPos(Vertex2(camPos), -5);
        
        light_ = scene_->CreateLight("light");
        light_->SetPosition(lighPos);
        
        camera_->SetPosition(camPos);
        Vector3 lookAtPoint(Vertex2(camPos), 0);
        camera_->SetLookAt(lookAtPoint);
    }
};

NSG_MAIN(Sample);
