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
#include "Util.h"

namespace NSG
{
    FollowCamera::FollowCamera(PCamera camera)
        : camera_(camera),
        radius_(1)
    {
        slotUpdate_ = Engine::SigUpdate()->Connect([this](float deltaTime)
        {
            OnUpdate(deltaTime);
        });
    }

    FollowCamera::~FollowCamera()
    {

    }

    Vector3 FollowCamera::GetCamPos() const
    {
        auto pos = camera_->GetGlobalPosition();
        auto trackPos = trackNode_->GetGlobalPosition() + offset_;
        auto world = camera_->GetScene()->GetPhysicsWorld();
        auto distance = glm::length(targetOffset_);
        PhysicsRaycastResult result = world->SphereCast(trackPos + targetOffset_, -targetOffset_, radius_, distance);
        if (result.rigidBody_ != trackNode_->GetRigidBody().get() && result.distance_ < distance)
        {
            //LOGI("-2-->%s", result.rigidBody_->GetName().c_str());
            auto direction = trackPos - pos;
            auto distance = glm::length(direction);
            result = world->SphereCast(pos, direction, radius_, distance);
            if (result.rigidBody_ != trackNode_->GetRigidBody().get() && result.distance_ < distance )
            {
                //LOGI("-1-->%s", result.rigidBody_->GetName().c_str());
                return pos + GetDisplacementToAvoidObstruction(direction, result.normal_);
            }
            return pos;
        }
        return trackPos + targetOffset_;
    }

    void FollowCamera::OnUpdate(float deltaTime)
    {
        if (!trackNode_)
            return;
        auto pos = camera_->GetGlobalPosition();
        auto camPos = GetCamPos();
        camera_->SetGlobalPosition(Lerp(pos, camPos, deltaTime));
        camera_->SetGlobalLookAt(trackNode_->GetGlobalPosition());

    }

    Vector3 FollowCamera::GetDisplacementToAvoidObstruction(const Vector3& dir2Target, const Vector3& hitNormal) const
    {
        Vector3 reflection = glm::reflect(dir2Target, hitNormal);

        Vector3 parallelComponent = glm::dot(reflection, hitNormal) * hitNormal;

        Vector3 sliding = reflection - parallelComponent;

        return sliding;
    }


    void FollowCamera::Track(PSceneNode node, const Vector3& offset)
    {
        trackNode_ = node;
        offset_ = offset;
    }

    void FollowCamera::SetTargetOffset(const Vector3& offset)
    {
        targetOffset_ = offset;
    }

    void FollowCamera::SetRadius(float radius)
    {
        radius_ = radius;
    }
}