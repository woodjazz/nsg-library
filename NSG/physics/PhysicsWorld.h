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
#pragma once
#include "Types.h"

class btDynamicsWorld;
class btDefaultCollisionConfiguration;
struct btDbvtBroadphase;
class btGhostPairCallback;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
#include "LinearMath/btIDebugDraw.h"

namespace NSG
{
    struct PhysicsRaycastResult
    {
        // Hit world position
        Vector3 position_;
        // Hit world normal
        Vector3 normal_;
        // Hit distance from ray origin
        float distance_;
        // Hit RigidBody
        RigidBody* rigidBody_;
    };

    class PhysicsWorld : public btIDebugDraw
    {
    public:
        PhysicsWorld(const Scene* scene);
        ~PhysicsWorld();
        void StepSimulation(float timeStep);
        std::shared_ptr<btDiscreteDynamicsWorld> GetWorld() const { return dynamicsWorld_; }
        void SetGravity(const Vector3& gravity);
        const Vector3& GetGravity() const { return gravity_; }
        void SetFps(int fps);
        int GetFps() const { return fps_; }
        void SetMaxSubSteps(int steps);
        int GetMaxSubSteps() const { return maxSubSteps_; }
        ///////////////////////////////////////////////////////////////////////////////////////
        // Bullet btIDebugDraw
        //bool isVisible(const btVector3& aabbMin, const btVector3& aabbMax) override;
        void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
        void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
        void reportErrorWarning(const char* warningString) override;
        void draw3dText(const btVector3& location, const char* textString) override;
        void setDebugMode(int debugMode) override;
        int	getDebugMode() const override;
        ///////////////////////////////////////////////////////////////////////////////////////
        void DrawDebug();
        PLinesMesh GetDebugLines() const { return lines_; }
        void ClearDebugLines();
        PhysicsRaycastResult SphereCast(const Vector3& origin, const Vector3& direction, float radius, float maxDistance, int collisionMask = (int)CollisionMask::ALL);
        PhysicsRaycastResult SphereCastBut(const RigidBody* rigidBody, const Vector3& origin, const Vector3& direction, float radius, float maxDistance, int collisionMask = (int)CollisionMask::ALL);
        PhysicsRaycastResult RayCast(const Vector3& origin, const Vector3& direction, float maxDistance, int collisionMask = (int)CollisionMask::ALL);

    private:
        void Substep(float tick);
        static void SubstepCallback(btDynamicsWorld* dyn, float tick);
        const Scene* scene_;
        btDefaultCollisionConfiguration* collisionConfiguration_;
        btDbvtBroadphase* pairCache_;
        btGhostPairCallback* ghostPairCallback_;
        btCollisionDispatcher* dispatcher_;
        btSequentialImpulseConstraintSolver* constraintSolver_;
        std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld_;
        Vector3 gravity_;
        int debugMode_;
        PLinesMesh lines_;
        int fps_;
        int maxSubSteps_;
    };
}