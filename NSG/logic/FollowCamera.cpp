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
        : camera_(camera)
    {
        slotUpdate_ = Engine::SigUpdate()->Connect([this](float deltaTime)
        {
            OnUpdate(deltaTime);
        });
    }

    FollowCamera::~FollowCamera()
    {

    }

    void FollowCamera::OnUpdate(float deltaTime)
    {
        if(!trackNode_)
            return;
        
        auto trackPos = trackNode_->GetGlobalPosition();
        camera_->SetGlobalLookAt(trackPos);
        auto pos = camera_->GetGlobalPosition();
        auto targetPos = trackPos + offset_;
        auto direction = trackPos - pos;
        auto distance = glm::length(direction);
        auto world = camera_->GetScene()->GetPhysicsWorld();
        const float Radius = 1;
        PhysicsRaycastResult result = world->SphereCast(pos, direction, Radius, distance);
        if(result.rigidBody_)
        {
            auto trackSceneNode = std::dynamic_pointer_cast<SceneNode>(trackNode_);
            if(trackSceneNode && result.rigidBody_ != trackSceneNode->GetRigidBody().get())
            {
                LOGI("%s", result.rigidBody_->GetName().c_str());
                auto sliding = GetDisplacementToAvoidObstruction(direction, result.position_, result.normal_);
                camera_->SetGlobalPosition(pos + sliding * deltaTime);
            }
        }
        else
            camera_->SetGlobalPosition(Lerp(pos, targetPos, deltaTime));
    }

    Vector3 FollowCamera::GetDisplacementToAvoidObstruction(const Vector3& dir2Target, const Vector3& hitPoint, const Vector3& hitNormal)
    {
        Vector3 reflection = glm::reflect(dir2Target, hitNormal);
    
        Vector3 parallelComponent = glm::dot(reflection, hitNormal) * hitNormal;
    
        Vector3 sliding = reflection - parallelComponent;
    
        return sliding;
    }


    void FollowCamera::Track(PNode node)
    {
    	trackNode_ = node;
    }

    void FollowCamera::SetOffset(const Vector3& offset)
   	{
   		offset_ = offset;
   	}
}