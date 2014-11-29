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
    static const int COLS = 11;
    static const int ROWS = 11;
    PSceneNode earth_[COLS][ROWS];
    PLight light_;
	PCameraControl control_;

    Sample()
    {
        AppConfiguration::this_->showStatistics_ = true;

		static auto slot = signalStart_->Connect([&](int argc, char* argv[])
		{
			scene_ = GetOrCreateScene("scene000");
			SetCurrentScene(scene_);

			PSphereMesh pSphereMesh(CreateSphereMesh(3, 24));

			PTexture pEarthTexture1(GetOrCreateTextureFile("data/Earthmap720x360_grid.jpg"));
			PTexture pEarthTexture2(GetOrCreateTextureFile("data/jup0vss1.jpg"));
			PMaterial pMaterial1(GetOrCreateMaterial("earth1"));
			PMaterial pMaterial2(GetOrCreateMaterial("earth2"));
			PProgram program(GetOrCreateProgram("program0"));
			program->SetFlags((int)ProgramFlag::PER_PIXEL_LIGHTING);
			PTechnique technique(new Technique);
			PPass pass(new Pass);
			technique->Add(pass);
			pass->SetProgram(program);
			pMaterial1->SetTechnique(technique);
			pMaterial1->SetDiffuseMap(pEarthTexture1);
			pMaterial1->SetDiffuseColor(Color(0.8f, 0.8f, 0.8f, 1));
			pMaterial1->SetSpecularColor(Color(1.0f, 0.0f, 0.0f, 1));
			pMaterial1->SetShininess(10);

			pMaterial2->SetTechnique(technique);
			pMaterial2->SetDiffuseMap(pEarthTexture2);
			pMaterial2->SetDiffuseColor(Color(0.8f, 0.8f, 0.8f, 1));
			pMaterial2->SetSpecularColor(Color(1.0f, 0.0f, 0.0f, 1));
			pMaterial2->SetShininess(100);

			camera_ = scene_->GetOrCreateChild<Camera>("camera");
			camera_->Activate();

			control_ = PCameraControl(new CameraControl(camera_));
			
			static const float STEP = 8.0f;
			Vector3 position(0, 0, -10);
			for (int r = 0; r < ROWS; r++)
			{
				for (int c = 0; c < COLS; c++)
				{
					earth_[c][r] = scene_->GetOrCreateChild<SceneNode>(GetUniqueName());
					earth_[c][r]->SetPosition(position);
					earth_[c][r]->Set(pSphereMesh);
					earth_[c][r]->Set(pMaterial1);
					std::swap(pMaterial1, pMaterial2);

					position.x += STEP;
				}
				position.x = 0;
				position.y -= STEP;
			}

			Vertex3 camPos(COLS / 2 * STEP, -(ROWS) / 2 * STEP, 75);
			Vertex3 lighPos(Vertex2(camPos), -5);

			light_ = scene_->GetOrCreateChild<Light>("light");
			light_->SetPosition(lighPos);

			camera_->SetPosition(camPos);
			Vector3 lookAtPoint(Vertex2(camPos), 0);
			camera_->SetGlobalLookAt(lookAtPoint);
		});

		static auto updateSlot = signalUpdate_->Connect([&](float deltaTime)
		{
			static float x_angle(0);
			static float y_angle(0);

			for (int r = 0; r < ROWS; r++)
				for (int c = 0; c < COLS; c++)
					earth_[c][r]->SetOrientation(glm::angleAxis(y_angle, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle, Vertex3(0, 1, 0)));


			x_angle += glm::pi<float>() / 10.0f * deltaTime;
			y_angle += glm::pi<float>() / 10.0f * deltaTime;
		});
    }
};

NSG_MAIN(Sample);
