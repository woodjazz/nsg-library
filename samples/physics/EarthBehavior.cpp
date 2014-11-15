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
{

}

EarthBehavior::~EarthBehavior()
{

}

void EarthBehavior::Start()
{
    PSphereMesh pSphereMesh(app_.CreateSphereMesh(3, 24));
	sceneNode_->Set(pSphereMesh);

	PTexture pEarthTexture(app_.GetOrCreateTextureFile("data/Earthmap720x360_grid.jpg"));
    PMaterial pMaterial(app_.GetOrCreateMaterial("earth"));
	PProgram program(app_.GetOrCreateProgram("EarthBehavior"));
	program->SetFlags((int)ProgramFlag::PER_PIXEL_LIGHTING);
    PTechnique technique(new Technique);
    PPass pass(new Pass);

    technique->Add(pass);
	pass->SetProgram(program);
    pMaterial->SetTechnique(technique);
	pMaterial->SetDiffuseMap(pEarthTexture);
    pMaterial->SetDiffuseColor(Color(0.8f, 0.8f, 0.8f, 1));
    pMaterial->SetSpecularColor(Color(1.0f, 0.0f, 0.0f, 1));
    pMaterial->SetShininess(10);

	sceneNode_->Set(pMaterial);

	sceneNode_->SetPosition(Vertex3(0, 10, 0));

    rb_ = app_.CreateRigidBody();
	rb_->SetMass(1);
	rb_->SetShape(SH_SPHERE, false);
	sceneNode_->Set(rb_);
	rb_->HandleCollisions(true);
}

void EarthBehavior::OnCollision(const ContactPoint& contactInfo)
{
	rb_->SetLinearVelocity(Vertex3(0, 100, 0));
}


