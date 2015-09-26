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
#include "FollowCamera.h"
#include "Camera.h"
#include "Engine.h"
#include "Ray.h"
#include "Scene.h"
#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "Character.h"
#include "PointOnSphere.h"
#include "Util.h"

namespace NSG
{
    FollowCamera::FollowCamera(PCamera camera)
        : camera_(camera),
          track_(nullptr),
          distance_(1),
          angle_(Radians(45.f)),
          world_(camera_->GetScene()->GetPhysicsWorld())
    {
        slotUpdate_ = Engine::SigUpdate()->Connect([this](float deltaTime)
        {
            if (track_ && world_.lock())
                OnUpdate(deltaTime);
        });
    }

    FollowCamera::~FollowCamera()
    {

    }

    void FollowCamera::SetAngle(float angle)
    {
        angle_ = Radians(angle);
    }

    bool FollowCamera::Obstruction(const Vector3& origin, const Vector3& targetPos, float radius) const
    {
        auto direction = targetPos - origin;
        auto distance = Length(direction);
        PhysicsRaycastResult result = world_.lock()->SphereCastBut(track_, origin, Normalize(direction), radius, distance);
        return result.collider_ != nullptr;
    }

    Vector3 FollowCamera::GetBestTargetPoint(const Vector3& center, float radius) const
    {
        auto camPos = camera_->GetGlobalPosition();
        auto point = center + Normalize(camPos - center) * distance_;
        PointOnSphere sphere(center, point);
        auto theta = sphere.GetTheta();
        for (float incTheta = 0.f; incTheta < TWO_PI; incTheta += PI15)
        {
            sphere.SetAngles(theta + incTheta, angle_);
            auto target = sphere.GetPoint();
            if (!Obstruction(center, target, radius))
                return target;
        }
        if (angle_ + PI10 < PI90)
        {
            for (float incTheta = 0.f; incTheta < TWO_PI; incTheta += PI15)
            {
                sphere.SetAngles(theta + incTheta, PI90);
                auto target = sphere.GetPoint();
                if (!Obstruction(center, target, radius))
                    return target;
            }
        }
        if (angle_ > PI10)
        {
            sphere.SetAngles(theta, 0);
            auto target = sphere.GetPoint();
            if (!Obstruction(center, target, radius))
                return target;
        }
        return camPos;
    }

    void FollowCamera::OnUpdate(float deltaTime)
    {
        auto pos = camera_->GetGlobalPosition();
        auto bb = track_->GetColliderBoundingBox();
        auto center = bb.Center();
        auto size = bb.Size();
        auto radius = std::min(std::min(size.x, size.y), size.z) * .4f;
        auto target = GetBestTargetPoint(center, radius);
        auto direction = target - pos;
        auto distance = Length(direction);
        PhysicsRaycastResult result = world_.lock()->SphereCastBut(track_, pos, Normalize(direction), radius, distance);
        if (result.collider_)
            target = pos + GetSlidingVector(direction, result.normal_);
        camera_->SetGlobalPosition(Lerp(pos, target, deltaTime));
        camera_->SetGlobalLookAtPosition(center);
    }

    void FollowCamera::Track(ICollision* track, float distance)
    {
        track_ = track;
        distance_ = distance;
    }
}