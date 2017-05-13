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
#include "PhysicsWorld.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "Camera.h"
#include "Color.h"
#include "DebugRenderer.h"
#include "ICollision.h"
#include "Log.h"
#include "Maths.h"
#include "Ray.h"
#include "Scene.h"
#include "btBulletDynamicsCommon.h"

namespace NSG {
static const int DEFAULT_FPS = 60;
PhysicsWorld::PhysicsWorld(const Scene* scene)
    : gravity_(0, -9.81f, 0),
      debugMode_(btIDebugDraw::DBG_DrawWireframe |
                 btIDebugDraw::DBG_DrawConstraints |
                 btIDebugDraw::DBG_DrawConstraintLimits),
      fps_(DEFAULT_FPS), maxSubSteps_(0),
      debugRenderer_(std::make_shared<DebugRenderer>()) {
    collisionConfiguration_ = new btDefaultCollisionConfiguration();
    pairCache_ = new btDbvtBroadphase();
    ghostPairCallback_ = new btGhostPairCallback;
    pairCache_->getOverlappingPairCache()->setInternalGhostPairCallback(
        ghostPairCallback_);
    dispatcher_ = new btCollisionDispatcher(collisionConfiguration_);
    constraintSolver_ = new btSequentialImpulseConstraintSolver();
    dynamicsWorld_ = std::make_shared<btDiscreteDynamicsWorld>(
        dispatcher_, pairCache_, constraintSolver_, collisionConfiguration_);
    SetGravity(gravity_);
    dynamicsWorld_->setWorldUserInfo(this);
    dynamicsWorld_->setInternalTickCallback(SubstepCallback,
                                            static_cast<void*>(this));
    dynamicsWorld_->setDebugDrawer(this);
}

PhysicsWorld::~PhysicsWorld() {
    for (int i = dynamicsWorld_->getNumConstraints() - 1; i >= 0; i--)
        dynamicsWorld_->removeConstraint(dynamicsWorld_->getConstraint(i));

    dynamicsWorld_ = nullptr;
    delete constraintSolver_;
    delete dispatcher_;
    delete ghostPairCallback_;
    delete pairCache_;
    delete collisionConfiguration_;
}

void PhysicsWorld::SetGravity(const Vector3& gravity) {
    if (gravity_ != gravity) {
        gravity_ = gravity;
        dynamicsWorld_->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
    }
}

void PhysicsWorld::StepSimulation(float timeStep) {
    float internalTimeStep = 1.0f / fps_;
    int maxSubSteps = (int)(timeStep * fps_) + 1;
    if (maxSubSteps_ < 0) {
        internalTimeStep = timeStep;
        maxSubSteps = 1;
    } else if (maxSubSteps_ > 0)
        maxSubSteps = std::min(maxSubSteps, maxSubSteps_);

    dynamicsWorld_->stepSimulation(timeStep, maxSubSteps, internalTimeStep);
}

void PhysicsWorld::SubstepCallback(btDynamicsWorld* dyn, float tick) {
    PhysicsWorld* world = static_cast<PhysicsWorld*>(dyn->getWorldUserInfo());
    world->Substep(tick);
}

void PhysicsWorld::Substep(float tick) {
    int nr = dispatcher_->getNumManifolds();

    for (int i = 0; i < nr; ++i) {
        btPersistentManifold* manifold =
            dispatcher_->getManifoldByIndexInternal(i);

        ICollision* colA =
            static_cast<ICollision*>(manifold->getBody0()->getUserPointer());
        ICollision* colB =
            static_cast<ICollision*>(manifold->getBody1()->getUserPointer());

        colA->HandleManifold(manifold, colB);
        colB->HandleManifold(manifold, colA);
    }
}
#if 0
    bool PhysicsWorld::isVisible(const btVector3& aabbMin, const btVector3& aabbMax)
    {
        return scene_->GetMainCamera()->IsVisible(BoundingBox(ToVector3(aabbMin), ToVector3(aabbMax)));
    }
#endif
void PhysicsWorld::drawLine(const btVector3& from, const btVector3& to,
                            const btVector3& color) {
    debugRenderer_->AddLine(ToVector3(from), ToVector3(to),
                            Color(ToVector3(color)));
}

void PhysicsWorld::drawContactPoint(const btVector3& PointOnB,
                                    const btVector3& normalOnB,
                                    btScalar distance, int lifeTime,
                                    const btVector3& color) {}

void PhysicsWorld::reportErrorWarning(const char* warningString) {
    LOGW("Physics:%s", warningString);
}

void PhysicsWorld::draw3dText(const btVector3& location,
                              const char* textString) {}

void PhysicsWorld::setDebugMode(int debugMode) { debugMode_ = debugMode; }

int PhysicsWorld::getDebugMode() const { return debugMode_; }

void PhysicsWorld::DrawDebug() { dynamicsWorld_->debugDrawWorld(); }

void PhysicsWorld::SetFps(int fps) { fps_ = Clamp(fps, 1, 1000); }

void PhysicsWorld::SetMaxSubSteps(int steps) { maxSubSteps_ = steps; }

PhysicsRaycastResult PhysicsWorld::SphereCast(const Vector3& origin,
                                              const Vector3& direction,
                                              float radius, float maxDistance,
                                              int collisionMask) {
    PhysicsRaycastResult result{Vector3::Zero, Vector3::Zero, 0.f, nullptr};
    btSphereShape shape(radius);
    btCollisionWorld::ClosestConvexResultCallback convexCallback(
        ToBtVector3(origin), ToBtVector3(origin + maxDistance * direction));
    convexCallback.m_collisionFilterGroup = (short)0xffff;
    convexCallback.m_collisionFilterMask = collisionMask;
    dynamicsWorld_->convexSweepTest(
        &shape, btTransform(btQuaternion::getIdentity(),
                            convexCallback.m_convexFromWorld),
        btTransform(btQuaternion::getIdentity(),
                    convexCallback.m_convexToWorld),
        convexCallback);
    if (convexCallback.hasHit()) {
        result.collider_ = static_cast<ICollision*>(
            convexCallback.m_hitCollisionObject->getUserPointer());
        result.position_ = ToVector3(convexCallback.m_hitPointWorld);
        result.normal_ = ToVector3(convexCallback.m_hitNormalWorld);
        result.distance_ = (result.position_ - origin).Length();
    }
    return result;
}

PhysicsRaycastResult
PhysicsWorld::SphereCastBut(const ICollision* collider, const Vector3& origin,
                            const Vector3& direction, float radius,
                            float maxDistance, int collisionMask) {
    PhysicsRaycastResult result{Vector3::Zero, Vector3::Zero, 0.f, nullptr};
    btSphereShape shape(radius);

    struct CallBackBut : btCollisionWorld::ClosestConvexResultCallback {
        const ICollision* collider_;
        CallBackBut(const ICollision* collider,
                    const btVector3& convexFromWorld,
                    const btVector3& convexToWorld)
            : btCollisionWorld::ClosestConvexResultCallback(convexFromWorld,
                                                            convexToWorld),
              collider_(collider) {}

        bool needsCollision(btBroadphaseProxy* proxy0) const override {
            if (btCollisionWorld::ClosestConvexResultCallback::needsCollision(
                    proxy0)) {
                btCollisionObject* obj =
                    static_cast<btCollisionObject*>(proxy0->m_clientObject);
                return !obj || obj->getUserPointer() != collider_;
            }
            return false;
        }
    };

    CallBackBut convexCallback(collider, ToBtVector3(origin),
                               ToBtVector3(origin + maxDistance * direction));
    convexCallback.m_collisionFilterGroup = (short)0xffff;
    convexCallback.m_collisionFilterMask = collisionMask;
    dynamicsWorld_->convexSweepTest(
        &shape, btTransform(btQuaternion::getIdentity(),
                            convexCallback.m_convexFromWorld),
        btTransform(btQuaternion::getIdentity(),
                    convexCallback.m_convexToWorld),
        convexCallback);
    if (convexCallback.hasHit()) {
        result.collider_ = static_cast<ICollision*>(
            convexCallback.m_hitCollisionObject->getUserPointer());
        result.position_ = ToVector3(convexCallback.m_hitPointWorld);
        result.normal_ = ToVector3(convexCallback.m_hitNormalWorld);
        result.distance_ = (result.position_ - origin).Length();
    }
    return result;
}

PhysicsRaycastResult PhysicsWorld::RayCast(const Vector3& origin,
                                           const Vector3& direction,
                                           float maxDistance,
                                           int collisionMask) {
    PhysicsRaycastResult result{Vector3::Zero, Vector3::Zero, 0.f, nullptr};
    btCollisionWorld::ClosestRayResultCallback rayCallback(
        ToBtVector3(origin), ToBtVector3(origin + maxDistance * direction));
    rayCallback.m_collisionFilterGroup = (short)0xffff;
    rayCallback.m_collisionFilterMask = collisionMask;
    dynamicsWorld_->rayTest(rayCallback.m_rayFromWorld,
                            rayCallback.m_rayToWorld, rayCallback);
    if (rayCallback.hasHit()) {
        result.position_ = ToVector3(rayCallback.m_hitPointWorld);
        result.normal_ = ToVector3(rayCallback.m_hitNormalWorld);
        result.distance_ = (result.position_ - origin).Length();
        result.collider_ = static_cast<ICollision*>(
            rayCallback.m_collisionObject->getUserPointer());
    }
    return result;
}
}
