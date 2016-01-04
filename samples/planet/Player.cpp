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
#include "Player.h"
#include "Explo.h"
#include "Level.h"
#include "Laser.h"

static int s_lives = 3;

static std::string GetOverlayName(int life)
{
    return "Life" + ToString(life);
}

Player::Player(PScene scene)
    : node_(scene->CreateChild<SceneNode>()),
      child_(node_->CreateChild<SceneNode>()),
      body_(child_->GetOrCreateRigidBody()),
      explo_(std::make_shared<Explo>(child_)),
      collisionGroup_((int)CollisionMask::PLAYER),
      collisionMask_((int)CollisionMask::ALL & ~(int)CollisionMask::PLAYER),
      shot_(false),
      buttonADown_(false),
      lastShotTime_(0),
      totalSpawningTime_(0),
      spawning_(true)
{
    slotCollision_ = child_->SigCollision()->Connect([this](const ContactPoint & contactInfo)
    {
        child_->Hide(true, false);
        body_->HandleCollisions(false);
        explo_->Start();
    });

    auto radius = Level::GetFlyDistance();
    child_->SetPosition(Vector3(0, 0, radius));
    child_->SetScale(0.25f);
    auto mesh = Mesh::GetOrCreate<TriangleMesh>("PlayerMesh");
    child_->SetMesh(mesh);
    auto material(Material::Create());
    //material->SetFillMode(FillMode::WIREFRAME);
    material->SetDiffuseColor(COLOR_DODGER_BLUE);
    material->SetRenderPass(RenderPass::UNLIT);
    child_->SetMaterial(material);

    moveSlot_ = control_.SigMoved()->Connect([this](float x, float y)
    {
        if (!child_->IsHidden() && (x || y))
        {
            auto dt = Engine::GetPtr()->GetDeltaTime();
            child_->Roll(-x * dt);
            auto dir = y * dt * (child_->GetOrientation() * VECTOR3_UP);
            node_->Pitch(-dir.y);
            node_->Yaw(dir.x);
        }
    });

    moveLeftStickSlot_ = control_.SigLeftStickMoved()->Connect([this](float x, float y)
    {
        if (!child_->IsHidden() && (x || y))
        {
            auto angle = Angle(VECTOR3_UP, Normalize(Vector3(x, y, 0)));
            if (x > 0)
                angle *= -1;
            child_->SetOrientation(AngleAxis(angle, VECTOR3_FORWARD));
            auto dt = Engine::GetPtr()->GetDeltaTime();
            auto dir = dt * (child_->GetOrientation() * VECTOR3_UP);
            node_->Pitch(-dir.y);
            node_->Yaw(dir.x);
        }
    });

    moveRightStickSlot_ = control_.SigRightStickMoved()->Connect([this](float x, float y)
    {
        if (!child_->IsHidden() && (x || y))
        {
            auto angle = Angle(VECTOR3_UP, Normalize(Vector3(x, y, 0)));
            if (x > 0)
                angle *= -1;
            lastShotOrientation_ = AngleAxis(angle, VECTOR3_FORWARD);
            shot_ = true;
        }
        else
            shot_ = false;
    });

    buttonASlot_ =  control_.SigButtonA()->Connect([this](bool down)
    {
        buttonADown_ = down;
    });

    updateSlot_ = Engine::SigUpdate()->Connect([this](float deltaTime)
    {
        lastShotTime_ += deltaTime;
        totalSpawningTime_ += deltaTime;
        if (spawning_)
        {
            if (totalSpawningTime_ > 3)
            {
                child_->Hide(false, false);
                body_->HandleCollisions(true);
                spawning_ = false;
            }
            else
            {
                child_->Hide(!child_->IsHidden(), false);
            }
        }
        if (shot_ || buttonADown_)
        {
            if (lastShotTime_ > 0.2f)
            {
                if (buttonADown_ && !shot_)
                    lastShotOrientation_ = child_->GetOrientation();

                lastShotTime_ = 0;
                auto obj = std::make_shared<Laser>(node_->GetScene());
                obj->SetPosition(child_->GetPosition());
                obj->SetOrientation(node_->GetOrientation(), lastShotOrientation_);
                Level::GetCurrent()->AddObject(obj);
            }
        }
    });

    child_->SetUserData(this);

    body_->SetKinematic(true);
    body_->SetCollisionMask(collisionGroup_, collisionMask_);
    auto shape = Shape::GetOrCreate(ShapeKey(PhysicsShape::SH_SPHERE, VECTOR3_ONE));
    body_->AddShape(shape);
    shape->SetBB(child_->GetWorldBoundingBox());

    for (auto i = 0; i < s_lives; i++)
    {
        auto overlay = scene->CreateOverlay(GetOverlayName(i));
        overlay->SetMaterial(material);
        overlay->SetMesh(mesh);
        overlay->SetScale(0.1f);
        auto bb = overlay->GetWorldBoundingBox();
        auto size = bb.Size();
        overlay->SetPosition(Vector3(-1 + size.x / 2.f + i * size.x * 1.1f, 1 - size.y, 0));
    }
}

Player::~Player()
{
    node_->SetParent(nullptr);
}

void Player::Destroyed()
{
    CHECK_ASSERT(s_lives > 0);
    if (--s_lives == 0)
    {
        s_lives = 3;
        Level::Load(2, Window::GetMainWindow()->shared_from_this());
    }
    else
    {
        totalSpawningTime_ = 0;
        spawning_ = true;
        node_->GetScene()->RemoveOverlay(GetOverlayName(s_lives));
    }
}
