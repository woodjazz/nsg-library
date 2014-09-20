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
#include "EarthBehavior.h"

EarthBehavior::EarthBehavior()
    : x_angle_(0),
      y_angle_(0)
{

}

EarthBehavior::~EarthBehavior()
{

}

void EarthBehavior::Start()
{
    PSphereMesh pSphereMesh(app_.CreateSphereMesh(3, 24));
	sceneNode_->Set(pSphereMesh);

    PTexture pEarthTexture(new TextureFile("data/Earthmap720x360_grid.jpg"));
    PMaterial pMaterial(app_.CreateMaterial("earth"));
	PProgram perVertex(new Program("", Program::PER_PIXEL_LIGHTING));
    PTechnique technique(new Technique);
    PPass pass(new Pass);

    technique->Add(pass);
    pass->SetProgram(perVertex);
    pMaterial->SetTechnique(technique);
    pMaterial->SetTexture0(pEarthTexture);
    pMaterial->SetDiffuseColor(Color(0.8f, 0.8f, 0.8f, 1));
    pMaterial->SetSpecularColor(Color(1.0f, 0.0f, 0.0f, 1));
    pMaterial->SetShininess(10);

	sceneNode_->Set(pMaterial);

	sceneNode_->SetPosition(Vertex3(5, 0, 0));
}

void EarthBehavior::Update()
{
    float deltaTime = App::this_->GetDeltaTime();

    x_angle_ += glm::pi<float>() / 10.0f * deltaTime;
    y_angle_ += glm::pi<float>() / 10.0f * deltaTime;

	sceneNode_->SetOrientation(glm::angleAxis(y_angle_, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle_, Vertex3(0, 1, 0)));

}


