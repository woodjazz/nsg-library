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
    PLight pointLight_;
    PLight dirLight_;
    PLight spotLight_;
    PMesh sphereMesh_;
    PMesh cubeMesh_;
    PTexture diffuseMap_;
    PTexture normalMap_;
    PMaterial material_;
    PProgram program_;
	PMesh mesh_;

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
        camera->AddBehavior(PCameraControl(new CameraControl));
		camera->Activate();

		pointLight_ = scene_->CreatePointLight("pointlight");
		pointLight_->SetPosition(Vertex3(-15, 0, 0));

		dirLight_ = scene_->CreateDirectionalLight("dirlight");
		dirLight_->SetLookAt(Vertex3(0, -1, 0));

		spotLight_ = scene_->CreateSpotLight("spotlight");
		spotLight_->SetPosition(Vertex3(0, 0, 15));
		spotLight_->SetLookAt(Vertex3(-1, 0, 0));
		spotLight_->SetSpotLight(25);

        mesh_ = sphereMesh_ = CreateSphereMesh();
        cubeMesh_ = CreateBoxMesh();

		diffuseMap_ = GetOrCreateTextureFile("data/wall.jpg");
		normalMap_ = GetOrCreateTextureFile("data/wallnormalmap.jpg");
        material_ = CreateMaterial();
		program_ = CreateProgram();
        PTechnique technique(new Technique);
        PPass pass(new Pass);
        technique->Add(pass);
		pass->SetProgram(program_);
        material_->SetTechnique(technique);

        PSceneNode node = scene_->CreateSceneNode("node");
		node->SetPosition(Vertex3(0, 0, 0));
        node->Set(material_);
        node->Set(mesh_);
    }

    void RenderGUIWindow() override
    {
        using namespace IMGUI;

		const float Y_PERCENTAGE = 20;
    }

};

NSG_MAIN(Sample);
