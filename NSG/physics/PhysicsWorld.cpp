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
#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "Scene.h"
#include "Camera.h"
#include "Ray.h"
#include "LinesMesh.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

namespace NSG
{
    static const int DEFAULT_FPS = 60;
    PhysicsWorld::PhysicsWorld(const Scene* scene)
        : scene_(scene),
          gravity_(0, -9.81f, 0),
          debugMode_(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawConstraints | btIDebugDraw::DBG_DrawConstraintLimits),
          lines_(std::make_shared<LinesMesh>("DebugPhysics")),
          fps_(DEFAULT_FPS),
          maxSubSteps_(0)
    {
        collisionConfiguration_ = new btDefaultCollisionConfiguration();
        pairCache_ = new btDbvtBroadphase();
        ghostPairCallback_ = new btGhostPairCallback;
        pairCache_->getOverlappingPairCache()->setInternalGhostPairCallback(ghostPairCallback_);
        dispatcher_ = new btCollisionDispatcher(collisionConfiguration_);
        constraintSolver_ = new btSequentialImpulseConstraintSolver();
        dynamicsWorld_ = std::make_shared<btDiscreteDynamicsWorld>(dispatcher_, pairCache_, constraintSolver_, collisionConfiguration_);
        SetGravity(gravity_);
        dynamicsWorld_->setWorldUserInfo(this);
        dynamicsWorld_->setInternalTickCallback(SubstepCallback, static_cast<void*>(this));
        dynamicsWorld_->setDebugDrawer(this);
    }

    PhysicsWorld::~PhysicsWorld()
    {
        for (int i = dynamicsWorld_->getNumConstraints() - 1; i >= 0; i--)
            dynamicsWorld_->removeConstraint(dynamicsWorld_->getConstraint(i));

        dynamicsWorld_ = nullptr;
        delete constraintSolver_;
        delete dispatcher_;
        delete ghostPairCallback_;
        delete pairCache_;
        delete collisionConfiguration_;
    }

    void PhysicsWorld::SetGravity(const Vector3& gravity)
    {
        if (gravity_ != gravity)
        {
            gravity_ = gravity;
            dynamicsWorld_->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
        }
    }

    void PhysicsWorld::StepSimulation(float timeStep)
    {
        float internalTimeStep = 1.0f / fps_;
        int maxSubSteps = (int)(timeStep * fps_) + 1;
        if (maxSubSteps_ < 0)
        {
            internalTimeStep = timeStep;
            maxSubSteps = 1;
        }
        else if (maxSubSteps_ > 0)
            maxSubSteps = std::min(maxSubSteps, maxSubSteps_);

        dynamicsWorld_->stepSimulation(timeStep, maxSubSteps, internalTimeStep);
    }

    void PhysicsWorld::SubstepCallback(btDynamicsWorld* dyn, float tick)
    {
        PhysicsWorld* world = static_cast<PhysicsWorld*>(dyn->getWorldUserInfo());
        world->Substep(tick);
    }

    void PhysicsWorld::Substep(float tick)
    {
        int nr = dispatcher_->getNumManifolds();

        for (int i = 0; i < nr; ++i)
        {
            btPersistentManifold* manifold = dispatcher_->getManifoldByIndexInternal(i);

            RigidBody* colA = static_cast<RigidBody*>(manifold->getBody0()->getUserPointer());
            RigidBody* colB = static_cast<RigidBody*>(manifold->getBody1()->getUserPointer());

            colA->HandleManifold(manifold, colB);
            colB->HandleManifold(manifold, colA);
        }
    }

    bool PhysicsWorld::isVisible(const btVector3& aabbMin, const btVector3& aabbMax)
    {
        return scene_->GetMainCamera()->IsVisible(BoundingBox(ToVector3(aabbMin), ToVector3(aabbMax)));
    }

    void PhysicsWorld::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
    {
        lines_->Add(ToVector3(from), ToVector3(to), Color(ToVector3(color), 1));
    }

    void PhysicsWorld::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
    {

    }

    void PhysicsWorld::reportErrorWarning(const char* warningString)
    {
        LOGW("Physics:%s", warningString);
    }

    void PhysicsWorld::draw3dText(const btVector3& location, const char* textString)
    {

    }

    void PhysicsWorld::setDebugMode(int debugMode)
    {
        debugMode_ = debugMode;
        setDebugMode(debugMode_);
    }

    int PhysicsWorld::getDebugMode() const
    {
        return debugMode_;
    }

    void PhysicsWorld::DrawDebug()
    {
        dynamicsWorld_->debugDrawWorld();
    }

    void PhysicsWorld::ClearDebugLines()
    {
        lines_->Clear();
    }

    void PhysicsWorld::SetFps(int fps)
    {
        fps_ = glm::clamp(fps, 1, 1000);
    }

    void PhysicsWorld::SetMaxSubSteps(int steps)
    {
        maxSubSteps_ = steps;
    }

    PhysicsRaycastResult PhysicsWorld::SphereCast(const Vector3& origin, const Vector3& direction, float radius, float maxDistance, int collisionMask)
    {
        PhysicsRaycastResult result {VECTOR3_ZERO, VECTOR3_ZERO, 0.f, nullptr};
        btSphereShape shape(radius);
        btCollisionWorld::ClosestConvexResultCallback convexCallback(ToBtVector3(origin), ToBtVector3(origin +
                maxDistance * direction));
        convexCallback.m_collisionFilterGroup = (short)0xffff;
        convexCallback.m_collisionFilterMask = collisionMask;
        dynamicsWorld_->convexSweepTest(&shape, btTransform(btQuaternion::getIdentity(), convexCallback.m_convexFromWorld), 
            btTransform(btQuaternion::getIdentity(), convexCallback.m_convexToWorld), convexCallback);
        if (convexCallback.hasHit())
        {
            result.rigidBody_ = static_cast<RigidBody*>(convexCallback.m_hitCollisionObject->getUserPointer());
            result.position_ = ToVector3(convexCallback.m_hitPointWorld);
            result.normal_ = ToVector3(convexCallback.m_hitNormalWorld);
            result.distance_ = glm::length(result.position_ - origin);
        }
        return result;
    }

    PhysicsRaycastResult PhysicsWorld::RayCast(const Vector3& origin, const Vector3& direction, float maxDistance, int collisionMask)
    {
        PhysicsRaycastResult result {VECTOR3_ZERO, VECTOR3_ZERO, 0.f, nullptr};
        btCollisionWorld::ClosestRayResultCallback rayCallback(ToBtVector3(origin), ToBtVector3(origin +
                maxDistance * direction));
        rayCallback.m_collisionFilterGroup = (short)0xffff;
        rayCallback.m_collisionFilterMask = collisionMask;
        dynamicsWorld_->rayTest(rayCallback.m_rayFromWorld, rayCallback.m_rayToWorld, rayCallback);
        if (rayCallback.hasHit())
        {
            result.position_ = ToVector3(rayCallback.m_hitPointWorld);
            result.normal_ = ToVector3(rayCallback.m_hitNormalWorld);
            result.distance_ = glm::length(result.position_ - origin);
            result.rigidBody_ = static_cast<RigidBody*>(rayCallback.m_collisionObject->getUserPointer());
        }
        return result;
    }
}