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

    Sample()
    {
        //AppConfiguration::this_->width_ = 30;
        //AppConfiguration::this_->height_ = 20;
        AppConfiguration::this_->showStatistics_ = true;
    }

    void Start(int argc, char* argv[]) override
    {
		scene_ = GetCurrentScene();
        scene_->SetAmbientColor(Color(0));

		PCamera camera = scene_->CreateCamera("camera");
		camera->Activate();

		PLight pointLight0 = scene_->CreatePointLight("pointlight0");
		pointLight0->SetPosition(Vertex3(-5, 15, 0));
        pointLight0->SetEnabled(false);

		PLight pointLight1 = scene_->CreatePointLight("pointlight1");
        pointLight1->SetEnabled(false);
		pointLight1->SetPosition(Vertex3(5, -15, 0));

		PLight dirLight0 = scene_->CreateDirectionalLight("dirlight0");
        dirLight0->SetEnabled(false);
		dirLight0->SetLookAt(Vertex3(-10, 0, 0));

		PLight dirLight1 = scene_->CreateDirectionalLight("dirlight1");
        dirLight1->SetEnabled(false);
		dirLight1->SetLookAt(Vertex3(1, 0, 0));

		PLight spotLight0 = scene_->CreateSpotLight("spotlight0");
		spotLight0->SetPosition(Vertex3(0, 0, 5));
		spotLight0->SetLookAt(Vertex3(-1, 0, -1));
		spotLight0->SetSpotLight(15);
		spotLight0->SetEnabled(true);

        PMesh mesh(CreateSphereMesh(3, 24));

        PTexture wallTexture(new TextureFile("data/wall.jpg"));
        PTexture wallNormalMapTexture(new TextureFile("data/wallnormalmap.jpg"));
        PMaterial material(CreateMaterial("wall"));
		PProgram program(new Program("", Program::PER_PIXEL_LIGHTING | Program::NORMALMAP));
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
};

NSG_MAIN(Sample);
