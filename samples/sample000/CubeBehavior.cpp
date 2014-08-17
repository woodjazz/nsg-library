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
#include "Render2TextureBehavior.h"
#define ENABLED 1
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
#if ENABLED    
    PTechnique technique(new Technique);
    pSceneNode_->Set(technique);

    PPass pass(new Pass);
    technique->Add(pass);

    Render2TextureBehavior::this_->AddPass(pass);

	PBoxMesh pMesh(new BoxMesh(1,1,1, 2,2,2));

    pass->Add(pSceneNode_, pMesh);

	PMaterial pMaterial(new Material ());

    PResource pVResource(new ResourceFile("data/shaders/DiffuseSpecularReflection.vert"));
	PResource pFResource(new ResourceFile("data/shaders/Simple.frag"));
	PProgram pDiffuseProgram(new Program(pVResource, pFResource));
    pMaterial->SetProgram(pDiffuseProgram);

    PTexture pTexture(new TextureFile("data/cube_example.png"));
	pMaterial->SetTexture0(pTexture);
    pass->Set(pMaterial);

    pSceneNode_->SetPosition(Vertex3(-5, 0, 0));
    pSceneNode_->SetScale(Vertex3(3,3,3));
#endif    
}

void CubeBehavior::Update()
{
#if ENABLED    
    float deltaTime = App::this_->GetDeltaTime();

	x_angle_ += glm::pi<float>()/10.0f * deltaTime;
	y_angle_ += glm::pi<float>()/10.0f * deltaTime;

	pSceneNode_->SetOrientation(glm::angleAxis(y_angle_, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle_, Vertex3(0, 1, 0)));
#endif    

}

void CubeBehavior::Render()
{
#if ENABLED    
	//pSceneNode_->Render();
#endif    
}

