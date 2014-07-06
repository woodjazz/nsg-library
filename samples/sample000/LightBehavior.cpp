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

static const char* vShader = STRINGIFY(
	uniform mat4 u_mvp;
	attribute vec4 a_position;
	void main()
	{
		gl_Position = u_mvp * a_position;
	}
);

static const char* fShader = STRINGIFY(
	void main()
	{
		gl_FragColor = vec4(1,1,0,1);
	}
);


LightBehavior::LightBehavior()
{
}
	
LightBehavior::~LightBehavior()
{

}

void LightBehavior::Start()
{
	PMaterial pMaterial(new Material());
	PProgram pProgram(new Program(vShader, fShader));
    pMaterial->SetProgram(pProgram);
    PTechnique technique(new Technique);
    pSceneNode_->Set(technique);
    PPass pass(new Pass);
    pass->Set(pMaterial);
    technique->Add(pass);

	PMesh pMesh(new SphereMesh(0.2f, 32, GL_STATIC_DRAW));
	pass->Add(pSceneNode_, pMesh);

    pSceneNode_->SetPosition(Vertex3(-1.0,  0.0,  5.0));
}

void LightBehavior::Update()
{
//    float deltaTime = App::this_->GetDeltaTime();
}

void LightBehavior::Render()
{
	//pSceneNode_->Render();
}

