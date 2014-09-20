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
	PProgram pProgram(new Program);
    PTechnique technique(new Technique);
    PPass pass(new Pass);
    technique->Add(pass);
    pass->SetProgram(pProgram);
    pMaterial->SetTechnique(technique);

	sceneNode_->Set(pMaterial);
    
    PMesh pMesh(app_.CreateSphereMesh(0.1f, 16));
	sceneNode_->Set(pMesh);

    camControlPoints_.push_back(Vertex3(-4.0f, 0.0f, 5.0f)); 
    camControlPoints_.push_back(Vertex3(0.0f, 0.0f, 5.0f));
    camControlPoints_.push_back(Vertex3(4.0f, 0.0f, 5.0f));
    camControlPoints_.push_back(Vertex3(0.0f, 0.0f, 5.0f)); 
}

void LightBehavior::Update()
{
#if 1
    float deltaTime = App::this_->GetDeltaTime();

    static float delta1 = 0;

    Vertex3 position = glm::catmullRom(
        camControlPoints_[0],
        camControlPoints_[1],
        camControlPoints_[2],
        camControlPoints_[3],
        delta1);

	sceneNode_->SetPosition(position);

    delta1 += deltaTime * 0.1f;

    if(delta1 > 1)
    {
        delta1 = 0;
        Vertex3 p = camControlPoints_.front();
        camControlPoints_.pop_front();
        camControlPoints_.push_back(p);
    }
#endif
}


