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
#include "PointOnSphere.h"
#include "Util.h"

namespace NSG
{
    FollowCamera::FollowCamera(PCamera camera)
        : camera_(camera),
          phyRadius_(1.f),
          angle_(glm::radians(45.f)),
          world_(camera_->GetScene()->GetPhysicsWorld())
    {
        slotUpdate_ = Engine::SigUpdate()->Connect([this](float deltaTime)
        {
            OnUpdate(deltaTime);
        });
    }

    FollowCamera::~FollowCamera()
    {

    }

    void FollowCamera::SetAngle(float angle)
    {
        angle_ = glm::radians(angle);
    }

    bool FollowCamera::Obstruction(const Vector3& origin, const Vector3& targetPos) const
    {
        auto direction = origin - targetPos;
        auto distance = glm::length(direction);
        PhysicsRaycastResult result = world_->SphereCast(targetPos, glm::normalize(direction), phyRadius_, distance);
        if(result.rigidBody_ && result.rigidBody_ != track_->GetRigidBody().get() && result.distance_ < distance)
        {
            //LOGI("%s", result.rigidBody_->GetName().c_str());
            return true;
        }
        return false;
    }

    Vector3 FollowCamera::GetBestTargetPoint() const
    {
        auto camPos = camera_->GetGlobalPosition();
        auto center = track_->GetGlobalPosition();
        auto point = center + glm::normalize(camPos - center) * distance_;
        PointOnSphere sphere(center, point);
        auto theta = sphere.GetTheta();
        for(float incTheta =0.f; incTheta < 360.f; incTheta += 1.f)
        {
            sphere.SetAngles(theta + incTheta, angle_);
            auto target = sphere.GetPoint();
            if(!Obstruction(center, target))
                return target;
        }
        return camPos;
    }


    void FollowCamera::OnUpdate(float deltaTime)
    {
        if (!track_)
            return;
        auto pos = camera_->GetGlobalPosition();
        auto target = GetBestTargetPoint();
        auto center = track_->GetGlobalPosition();
#if 1
        auto direction = target - pos;
        auto distance = glm::length(direction);
        PhysicsRaycastResult result = world_->SphereCast(pos, glm::normalize(direction), phyRadius_, distance);
        if(result.rigidBody_ && result.rigidBody_ != track_->GetRigidBody().get() && result.distance_ < distance)
            target = pos + GetDisplacementToAvoidObstruction(direction, result.normal_);
#endif
        camera_->SetGlobalPosition(Lerp(pos, target, deltaTime));
        camera_->SetGlobalLookAt(center);

    }

    Vector3 FollowCamera::GetDisplacementToAvoidObstruction(const Vector3& dir2Target, const Vector3& hitNormal) const
    {
        Vector3 reflection = glm::reflect(dir2Target, hitNormal);
        Vector3 parallelComponent = glm::dot(reflection, hitNormal) * hitNormal;
        Vector3 sliding = reflection - parallelComponent;
        return sliding;
    }


    void FollowCamera::Track(PSceneNode track, float distance)
    {
        track_ = track;
        distance_ = distance;

    }

    void FollowCamera::SetCameraPhysicsRadius(float radius)
    {
        phyRadius_ = radius;
    }
}