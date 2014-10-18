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
#include "LightBehavior.h"

LightBehavior::LightBehavior()
{
}

LightBehavior::~LightBehavior()
{

}

void LightBehavior::Start()
{
    PMaterial pMaterial(app_.CreateMaterial("light"));
    pMaterial->SetColor(Color(1,0,0,1));
    PTechnique technique(new Technique);
    PPass pass(new Pass);
    technique->Add(pass);
    pass->SetProgram(app_.CreateProgram());
    pMaterial->SetTechnique(technique);

	sceneNode_->Set(pMaterial);

    PMesh pMesh(app_.CreateSphereMesh(0.2f, 16));
	sceneNode_->Set(pMesh);

	sceneNode_->SetPosition(Vertex3(-1.0, 0.0, 5.0));
}

void LightBehavior::Update()
{
}

void LightBehavior::Render()
{
    //pSceneNode_->Render();
}

