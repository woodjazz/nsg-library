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
#include "Enemy.h"
#include "Explo.h"
Enemy::Enemy(PScene scene)
    : node_(scene->CreateChild<SceneNode>()),
      child_(node_->CreateChild<SceneNode>()),
      body_(child_->GetOrCreateRigidBody()),
      explo_(std::make_shared<Explo>(child_))
{
    //node_->Pitch(-PI10);
    //node_->Yaw(PI/4.f);
    child_->SetScale(0.25f);
	child_->SetPosition(Vector3(0, 0, 6));
    child_->SetMesh(Mesh::CreateClass<TriangleMesh>());
    auto material(Material::Create());
    material->SetDiffuseColor(COLOR_RED);
    //material->SetFillMode(FillMode::WIREFRAME);
    //material->SetDiffuseColor(COLOR_DODGER_BLUE);
    material->SetRenderPass(RenderPass::UNLIT);
    child_->SetMaterial(material);
    child_->SetUserData(this);

	body_->SetKinematic(true);
	body_->HandleCollisions(true);
	auto shape = Shape::GetOrCreate(ShapeKey(PhysicsShape::SH_SPHERE, VECTOR3_ONE));
	shape->SetBB(child_->GetWorldBoundingBox());
	body_->AddShape(shape);

	slotCollision_ = child_->SigCollision()->Connect([this](const ContactPoint & contactInfo)
	{
		explo_->Fire();
	});
}

Enemy::~Enemy()
{
}

void Enemy::SetPosition(float pitch, float yaw)
{
	node_->Pitch(pitch);
	node_->Yaw(yaw);
}



