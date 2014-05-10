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
	PGLES2BoxMesh pMesh(new GLES2BoxMesh(1,1,1, 2,2,2, GL_STATIC_DRAW));
    pSceneNode_->SetMesh(pMesh);

	PGLES2Material pMaterial(new GLES2Material ());

    PResource pVResource(new Resource("shaders/DiffuseSpecularReflection.vert"));
	PResource pFResource(new Resource("shaders/Simple.frag"));
	PGLES2Program pDiffuseProgram(new GLES2Program(pVResource, pFResource));
    pMaterial->SetProgram(pDiffuseProgram);

    PGLES2Texture pTexture(new GLES2Texture("cube_example.png"));
	pMaterial->SetTexture0(pTexture);
    pSceneNode_->SetMaterial(pMaterial);

    pSceneNode_->SetPosition(Vertex3(-5, 0, 0));
    pSceneNode_->SetScale(Vertex3(3,3,3));
}

void CubeBehavior::Update()
{
    float deltaTime = App::GetPtrInstance()->GetDeltaTime();

	x_angle_ += glm::pi<float>()/10.0f * deltaTime;
	y_angle_ += glm::pi<float>()/10.0f * deltaTime;

	pSceneNode_->SetOrientation(glm::angleAxis(y_angle_, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle_, Vertex3(0, 1, 0)));

}

void CubeBehavior::Render()
{
	//pSceneNode_->Render(true);
}

void CubeBehavior::Render2Select()
{
	pSceneNode_->Render2Select();
}
