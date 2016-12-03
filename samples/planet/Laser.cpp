/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "Laser.h"
#include "Explo.h"
#include "Level.h"
Laser::Laser(PScene scene)
    : node_(scene->CreateChild<SceneNode>()),
      child_(node_->CreateChild<SceneNode>()),
      body_(child_->GetOrCreateRigidBody()),
      collisionGroup_((int)CollisionMask::PLAYER),
      collisionMask_((int)CollisionMask::ALL & ~(int)CollisionMask::PLAYER),
      totalTime_(0),
      destroyed_(false)
{
    const Vector3 Scale(0.03f, 0.15f, 0.03f);
    child_->SetScale(Scale);
    child_->SetPosition(Vector3(0, 0, 3));
    auto mesh = Mesh::GetOrCreateClass<CylinderMesh>("LaserMesh");
    child_->SetMesh(mesh);
    auto material(Material::GetOrCreate("LaserMaterial"));
    material->SetDiffuseColor(Color::Yellow);
    material->SetEmitIntensity(10);
    material->SetRenderPass(RenderPass::UNLIT);
    child_->SetMaterial(material);
    child_->SetUserData(this);
    auto blurMaterial = Material::GetOrCreate("LaserBlurMaterial");
    blurMaterial->SetRenderPass(RenderPass::BLUR);
    blurMaterial->FlipYTextureCoords(true);
    auto child1 = child_->CreateChild<SceneNode>();
    child1->SetMesh(mesh);
    child1->SetMaterial(material);
    child1->SetScale(1.3f);
    child1->SetFilter(blurMaterial);

    body_->SetKinematic(true);
    body_->HandleCollisions(true);
    auto shape = Shape::GetOrCreate(ShapeKey(mesh, Vector3(Scale.x, Scale.y, Scale.z)));
    shape->SetBB(child_->GetWorldBoundingBox());
    body_->AddShape(shape, Vector3(0, Scale.y * 0.5f, 0));// , AngleAxis(PI90, VECTOR3_RIGHT));
    body_->SetCollisionMask(collisionGroup_, collisionMask_);

    updateSlot_ = Engine::SigUpdate()->Connect([this](float dt)
    {
        totalTime_ += dt;
        if (destroyed_ || totalTime_ > 1)
            Destroyed();
        else
        {
            auto dir = 2 * dt * (child_->GetOrientation() * Vector3::Up);
            node_->Pitch(-dir.y);
            node_->Yaw(dir.x);
        }
    });

    slotCollision_ = child_->SigCollision()->Connect([this](const ContactPoint & contactInfo)
    {
        destroyed_ = true;
    });

}

Laser::~Laser()
{
    node_->SetParent(nullptr);
}

void Laser::SetOrientation(const Quaternion& q0, const Quaternion& q1)
{
    node_->SetOrientation(q0);
    child_->SetOrientation(q1);
}

void Laser::SetPosition(const Vector3& position)
{
    child_->SetPosition(position);
}

void Laser::Destroyed()
{
    auto level = Level::GetCurrent();
    level->RemoveObject(this);
}

