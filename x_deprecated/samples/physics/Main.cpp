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
    PSceneNode earth_;
    PSceneNode floor_;
    PLight light_;
    PMusic music_;
	PRigidBody rb_;
	PCameraControl control_;
    SignalStart::PSlot slotStart_;
    SignalCollision::PSlot slotCollision_;

    Sample()
    {
        AppConfiguration::this_->showStatistics_ = true;

		slotStart_ = slotStart_ = signalStart_->Connect([&](int argc, char* argv[])
		{
			scene_ = GetOrCreateScene("scene000");
			SetCurrentScene(scene_);
			scene_->SetAmbientColor(Color(0.1f, 0.1f, 0.1f, 1));

			camera_ = scene_->GetOrCreateChild<Camera>("camera");
			camera_->Activate();
			camera_->SetPosition(Vertex3(0, 2, 35));
			control_ = PCameraControl(new CameraControl(camera_));

			light_ = camera_->GetOrCreateChild<Light>("light");

			earth_ = scene_->GetOrCreateChild<SceneNode>("earth");
			{
				PSphereMesh pSphereMesh(CreateSphereMesh(3, 24));
				earth_->Set(pSphereMesh);

				PTexture pEarthTexture(GetOrCreateTextureFile("data/Earthmap720x360_grid.jpg"));
				PMaterial pMaterial(GetOrCreateMaterial("earth"));
				PProgram program(GetOrCreateProgram("EarthBehavior"));
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

				earth_->SetPosition(Vertex3(0, 10, 0));

				rb_ = earth_->GetOrCreateRigidBody();
				rb_->SetMass(1);
				rb_->SetShape(SH_SPHERE, false);
				rb_->HandleCollisions(true);

				slotCollision_ = earth_->signalCollision_->Connect([&](const ContactPoint& contactInfo)
				{
					rb_->SetLinearVelocity(Vertex3(0, 100, 0));
				});
			}

			{
				floor_ = scene_->GetOrCreateChild<SceneNode>("floor");
				PMesh mesh = CreateBoxMesh(20, 1, 20);
				floor_->Set(mesh);
				PRigidBody rb = floor_->GetOrCreateRigidBody();
				rb->SetMass(0);
				rb->SetShape(SH_BOX, true);
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
		});

    }

};

NSG_MAIN(Sample);
