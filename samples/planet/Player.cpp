/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "Player.h"
#include "Explo.h"
Player::Player(PScene scene)
    : node_(scene->CreateChild<SceneNode>()),
      child_(node_->CreateChild<SceneNode>()),
      body_(child_->GetOrCreateRigidBody()),
      explo_(std::make_shared<Explo>(child_))
{
	slotCollision_ = child_->SigCollision()->Connect([this](const ContactPoint & contactInfo)
	{
		moveSlot_ = nullptr;
		explo_->Fire();
	});

    child_->SetPosition(Vector3(0, 0, 6));
    child_->SetScale(0.25f);
    child_->SetMesh(Mesh::CreateClass<TriangleMesh>());
    auto material(Material::Create());
    //material->SetFillMode(FillMode::WIREFRAME);
    material->SetDiffuseColor(COLOR_DODGER_BLUE);
    material->SetRenderPass(RenderPass::UNLIT);
    child_->SetMaterial(material);

    moveSlot_ = control_.SigMoved()->Connect([this](float x, float y)
    {
        if (x || y)
        {
            auto dt = Engine::GetPtr()->GetDeltaTime();
            child_->Roll(-x * dt);
            auto dir = y * dt * (child_->GetOrientation() * VECTOR3_UP);
            node_->Pitch(-dir.y);
            node_->Yaw(dir.x);
        }
    });

    child_->SetUserData(this);

	body_->SetKinematic(true);
	body_->HandleCollisions(true);
	auto shape = Shape::GetOrCreate(ShapeKey(PhysicsShape::SH_SPHERE, VECTOR3_ONE));
	body_->AddShape(shape);
	shape->SetBB(child_->GetWorldBoundingBox());
}

Player::~Player()
{
}


