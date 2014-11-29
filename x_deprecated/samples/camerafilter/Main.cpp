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
    PSceneNode node1_;
    float y_angle_;
    PFilter userFilter_;
	PCameraControl control_;
	SignalStart::PSlot slotStart_;
	SignalUpdate::PSlot slotUpdate_;

    Sample()
    {
        AppConfiguration::this_->showStatistics_ = true;

		slotStart_ = signalStart_->Connect([&](int argc, char* argv[])
		{
			y_angle_ = 0;

			scene_ = GetOrCreateScene("scene000");
			SetCurrentScene(scene_);

			PCamera camera = scene_->GetOrCreateChild<Camera>("camera");
			camera->Activate();

#if 0
			camera->AddBlurFilter(16, 16);
			camera->AddBlendFilter(1024, 1024);
#else
			{
				static const char* fShader =
					STRINGIFY
					(void main()
				{
					vec2 texcoord = v_texcoord0;
					const float FACTOR = 8.0 * 3.14159;
					texcoord.x += sin(texcoord.y * FACTOR + u_material.shininess) / 100.0;
					gl_FragColor = texture2D(u_texture0, texcoord);
				}
				);
				userFilter_ = camera->AddUserFilter(PResourceMemory(new ResourceMemory(fShader)), 1024, 1024);
			}
#endif

			control_ = PCameraControl(new CameraControl(camera));
			camera->SetPosition(Vertex3(0, 0, 10));

			PProgram program(GetOrCreateProgram("program0"));
			program->SetFlags((int)ProgramFlag::PER_PIXEL_LIGHTING);
			PTechnique technique(new Technique);
			PPass pass(new Pass);
			pass->SetProgram(program);
			technique->Add(pass);

			scene_->GetOrCreateChild<Light>("light");

			{
				PMesh mesh = CreateBoxMesh();
				PMaterial material = GetOrCreateMaterial("material");
				material->SetDiffuseMap(GetOrCreateTextureFile("data/wall.jpg"));
				material->SetTechnique(technique);

				node1_ = scene_->GetOrCreateChild<SceneNode>("node1");
				node1_->SetPosition(Vertex3(3, -2, 0));
				node1_->Set(mesh);
				node1_->Set(material);
			}

		{
			PMesh mesh = CreateSphereMesh();
			PMaterial material = GetOrCreateMaterial("material");
			material->SetDiffuseMap(GetOrCreateTextureFile("data/stone.jpg"));
			material->SetTechnique(technique);

			PSceneNode node = scene_->GetOrCreateChild<SceneNode>("node2");
			node->SetPosition(Vertex3(-3, 2, 0));
			node->Set(mesh);
			node->Set(material);
		}

		});

		slotUpdate_ = signalUpdate_->Connect([&](float deltaTime)
		{
			y_angle_ += glm::pi<float>() / 10.0f * deltaTime;

			node1_->SetOrientation(glm::angleAxis(y_angle_, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle_, Vertex3(0, 1, 0)));

			if (userFilter_)
			{
				static float move = -1;
				move += deltaTime * TWO_PI * 0.25f;  // 1/4 of a wave cycle per second
				userFilter_->GetMaterial()->SetShininess(move);
			}
		});
    }
};

NSG_MAIN(Sample);
