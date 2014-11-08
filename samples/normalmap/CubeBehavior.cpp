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
#include "CubeBehavior.h"

CubeBehavior::CubeBehavior()
    : x_angle_(0),
      y_angle_(0)
{

}

CubeBehavior::~CubeBehavior()
{

}

void CubeBehavior::Start()
{
    PBoxMesh pMesh(app_.CreateBoxMesh(1,1,1, 2,2,2));
	sceneNode_->Set(pMesh);
	sceneNode_->SetScale(Vector3(2, 2, 2));

	//PTexture wallTexture(app_.GetOrCreateTextureFile("data/wall.jpg"));
	//PTexture wallNormalMapTexture(app_.GetOrCreateTextureFile("data/wallnormalmap.jpg"));
	pMaterial_ = PMaterial(app_.GetOrCreateMaterial("wall"));
	PProgram program(app_.CreateProgram());
	//program->SetFlags((int)ProgramFlag::PER_PIXEL_LIGHTING | (int)ProgramFlag::NORMALMAP);
    PTechnique technique(new Technique);
    PPass pass(new Pass);

    technique->Add(pass);
	pass->SetProgram(program);
    pMaterial_->SetTechnique(technique);


	PTexture diffuseTexture(app_.GetOrCreateTextureFile("data/wall_COLOR.png"));
	PTexture normalTexture(app_.GetOrCreateTextureFile("data/wall_NRM.png"));
	PTexture specularTexture(app_.GetOrCreateTextureFile("data/wall_SPEC.png"));
	PTexture occTexture(app_.GetOrCreateTextureFile("data/wall_OCC.png"));
	dispTexture_ = PTexture(app_.GetOrCreateTextureFile("data/wall_DISP.png"));

	pMaterial_->SetDiffuseMap(diffuseTexture);
	pMaterial_->SetNormalMap(normalTexture);
	pMaterial_->SetSpecularMap(specularTexture);
	pMaterial_->SetAOMap(occTexture);
	pMaterial_->SetDisplacementMap(dispTexture_);


//    pMaterial->SetTexture0(wallTexture);
//    pMaterial->SetTexture1(wallNormalMapTexture);
    pMaterial_->SetShininess(10);

	sceneNode_->Set(pMaterial_);

	sceneNode_->SetPosition(Vertex3(0, 0, 0));
}

void CubeBehavior::Update()
{
    float deltaTime = App::this_->GetDeltaTime();

    x_angle_ += glm::pi<float>() / 10.0f * deltaTime;
    y_angle_ += glm::pi<float>() / 10.0f * deltaTime;

	sceneNode_->SetOrientation(glm::angleAxis(y_angle_, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle_, Vertex3(0, 1, 0)));
}

void CubeBehavior::IMGUI()
{
	IMGUIBeginHorizontal(100, 20);
	{
		bool on = pMaterial_->GetDisplacementMap() != nullptr;
		if (IMGUICheckButton(on, on ? "Off Parallax" : "On Parallax", 25, 100))
			pMaterial_->SetDisplacementMap(dispTexture_);
		else
			pMaterial_->SetDisplacementMap(nullptr);
	}

	{
		float parallaxScale = pMaterial_->GetParallaxScale();
		parallaxScale = IMGUIHSlider(parallaxScale, 75, 100, 10);
		pMaterial_->SetParallaxScale(parallaxScale);
	}
	IMGUIEndArea();
}


