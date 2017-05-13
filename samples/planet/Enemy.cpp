/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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
#include "Level.h"
static unsigned s_total = 0;
Enemy::Enemy(PScene scene)
    : node_(scene->CreateChild<SceneNode>()),
      child_(node_->CreateChild<SceneNode>()),
      body_(child_->GetOrCreateRigidBody()),
      explo_(std::make_shared<Explo>(child_)),
      collisionGroup_((int)CollisionMask::ENEMY),
      collisionMask_((int)CollisionMask::ALL & ~(int)CollisionMask::ENEMY) {
    auto radius = Level::GetFlyDistance();
    child_->SetScale(0.25f);
    child_->SetPosition(Vector3(0, 0, radius));
    child_->SetMesh(Mesh::GetOrCreate<TriangleMesh>("EnemyMesh"));
    auto material(Material::GetOrCreate("EnemyMaterial"));
    material->SetDiffuseColor(Color::Red);
    // material->SetFillMode(FillMode::WIREFRAME);
    // material->SetDiffuseColor(COLOR_DODGER_BLUE);
    material->SetRenderPass(RenderPass::UNLIT);
    child_->SetMaterial(material);
    child_->SetUserData(this);

#if 0
    auto waveMaterial = Material::GetOrCreate("WaveMaterial");
    waveMaterial->SetRenderPass(RenderPass::WAVE);
    waveMaterial->FlipYTextureCoords(true);
    child_->SetFilter(waveMaterial);
#endif

    body_->SetKinematic(true);
    body_->HandleCollisions(true);
    auto shape =
        Shape::GetOrCreate(ShapeKey(PhysicsShape::SH_SPHERE, Vector3::One));
    shape->SetBB(child_->GetWorldBoundingBox());
    body_->AddShape(shape);
    body_->SetCollisionMask(collisionGroup_, collisionMask_);

    slotCollision_ = child_->SigCollision()->Connect(
        [this](const ContactPoint& contactInfo) {
            if (contactInfo.collider_->GetRigidBody()->HandleCollision()) {
                body_->HandleCollisions(false);
                explo_->Start();
            }
        });

    updateSlot_ = Engine::SigUpdate()->Connect([this](float dt) {
        wave0_.x += dt;
        wave0_.y = sin(wave0_.x);
        child_->SetOrientation(Quaternion(wave0_.y, Vector3::Forward));
        auto dir = .5f * dt * (child_->GetOrientation() * Vector3::Up);
        node_->Pitch(-dir.y);
        node_->Yaw(dir.x);
    });
}

Enemy::~Enemy() { node_->SetParent(nullptr); }

void Enemy::SetPosition(float pitch, float yaw) {
    node_->Pitch(pitch);
    node_->Yaw(yaw);
}

void Enemy::SetTotal(unsigned total) { s_total = total; }

void Enemy::Destroyed() {
    CHECK_ASSERT(s_total > 0);
    auto level = Level::GetCurrent();
    level->RemoveObject(this);
    if (--s_total == 0) {
        Level::Load(level->GetIndex() + 1,
                    Window::GetMainWindow()->shared_from_this());
    }
}
