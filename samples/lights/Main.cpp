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
    PLight pointLight0_;
    PLight pointLight1_;
    PLight dirLight0_;
    PLight dirLight1_;
    PLight spotLight0_;

    Sample()
    {
        //AppConfiguration::this_->width_ = 30;
        //AppConfiguration::this_->height_ = 20;
        AppConfiguration::this_->showStatistics_ = false;
    }

    void Start(int argc, char* argv[]) override
    {
		scene_ = GetCurrentScene();
        scene_->SetAmbientColor(Color(0));

		PCamera camera = scene_->CreateCamera("camera");
		camera->Activate();

		pointLight0_ = scene_->CreatePointLight("pointlight0");
		pointLight0_->SetPosition(Vertex3(-5, 15, 0));
        pointLight0_->SetEnabled(false);

		pointLight1_ = scene_->CreatePointLight("pointlight1");
        pointLight1_->SetEnabled(false);
		pointLight1_->SetPosition(Vertex3(5, -15, 0));

		dirLight0_ = scene_->CreateDirectionalLight("dirlight0");
        dirLight0_->SetEnabled(false);
		dirLight0_->SetLookAt(Vertex3(-1, -1, 0));

		dirLight1_ = scene_->CreateDirectionalLight("dirlight1");
        dirLight1_->SetEnabled(false);
		dirLight1_->SetLookAt(Vertex3(1, 1, 0));

		spotLight0_ = scene_->CreateSpotLight("spotlight0");
		spotLight0_->SetPosition(Vertex3(0, 0, 5));
		spotLight0_->SetLookAt(Vertex3(0, 0, -1));
		spotLight0_->SetSpotLight(10);
		spotLight0_->SetEnabled(false);

        PMesh mesh(CreateSphereMesh(3, 24));

		PTexture wallTexture(GetOrCreateTextureFile("data/wall.jpg"));
		PTexture wallNormalMapTexture(GetOrCreateTextureFile("data/wallnormalmap.jpg"));
        PMaterial material(CreateMaterial("wall"));
		PProgram program(CreateProgram());
		program->SetFlags((int)ProgramFlag::PER_PIXEL_LIGHTING | (int)ProgramFlag::NORMALMAP);
        PTechnique technique(new Technique);
        PPass pass(new Pass);
        technique->Add(pass);
        pass->SetProgram(program);
        material->SetTechnique(technique);
        material->SetTexture0(wallTexture);
        material->SetTexture1(wallNormalMapTexture);
        material->SetAmbientColor(Color(0));
        material->SetShininess(10);

        PSceneNode node = scene_->CreateSceneNode("node");
		node->SetPosition(Vertex3(0, 0, -10));
        node->Set(material);
        node->Set(mesh);
    }

    void RenderGUIWindow() override
    {
        using namespace IMGUI;

		const float Y_PERCENTAGE = 20;
		pointLight0_->SetEnabled(IMGUICheckButton(pointLight0_->IsEnabled(), pointLight0_->IsEnabled() ? "Disable P0" : "Enable P0", 25, Y_PERCENTAGE));
		pointLight1_->SetEnabled(IMGUICheckButton(pointLight1_->IsEnabled(), pointLight1_->IsEnabled() ? "Disable P1" : "Enable P1", 25, Y_PERCENTAGE));
		dirLight0_->SetEnabled(IMGUICheckButton(dirLight0_->IsEnabled(), dirLight0_->IsEnabled() ? "Disable D0" : "Enable D0", 25, Y_PERCENTAGE));
		dirLight1_->SetEnabled(IMGUICheckButton(dirLight1_->IsEnabled(), dirLight1_->IsEnabled() ? "Disable D1" : "Enable D1", 25, Y_PERCENTAGE));
		IMGUIBeginHorizontal(100, Y_PERCENTAGE);
		{
			spotLight0_->SetEnabled(IMGUICheckButton(spotLight0_->IsEnabled(), spotLight0_->IsEnabled() ? "Disable S0" : "Enable S0", 25, 100));
			float cutOff = spotLight0_->GetSpotCutOff() / 100;
			cutOff = IMGUIHSlider(cutOff, 75, 100);
			spotLight0_->SetSpotLight(cutOff * 100);
		}
		IMGUIEndArea();
    }

};

NSG_MAIN(Sample);
