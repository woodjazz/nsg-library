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
#include "Render2TextureBehavior.h"

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
    PSphereMesh mesh(new SphereMesh(3, 32));
    pSceneNode_->Set(mesh);

	PTexture pEarthTexture(new TextureFile("data/Earthmap720x360_grid.jpg"));
	PMaterial pMaterial(new Material ());
	PProgram perVertex(new ProgramPerVertex);
	pMaterial->SetProgram(perVertex);
	pMaterial->SetTexture0(pEarthTexture);
	pMaterial->SetDiffuseColor(Color(1.0f,1.0f,1.0f,1));
	pMaterial->SetSpecularColor(Color(1.0f,0.0f,0.0f,1));
	pMaterial->SetShininess(0.3f);

    pSceneNode_->Set(pMaterial);

    pSceneNode_->SetPosition(Vertex3(5, 0, 0));
    
    Render2TextureBehavior::this_->Add(PPass(new Pass), pSceneNode_, pMaterial, mesh);
}

void EarthBehavior::Update()
{
    float deltaTime = App::this_->GetDeltaTime();

	x_angle_ += glm::pi<float>()/10.0f * deltaTime;
	y_angle_ += glm::pi<float>()/10.0f * deltaTime;

	pSceneNode_->SetOrientation(glm::angleAxis(y_angle_, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle_, Vertex3(0, 1, 0)));

}



