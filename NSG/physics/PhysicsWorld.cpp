/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

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
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

namespace NSG
{
    PhysicsWorld::PhysicsWorld()
    {
        collisionConfiguration_ = new btDefaultCollisionConfiguration();
        pairCache_ = new btDbvtBroadphase();
        ghostPairCallback_ = new btGhostPairCallback;
        pairCache_->getOverlappingPairCache()->setInternalGhostPairCallback(ghostPairCallback_);
        dispatcher_ = new btCollisionDispatcher(collisionConfiguration_);
        constraintSolver_ = new btSequentialImpulseConstraintSolver();
        dynamicsWorld_ = new btDiscreteDynamicsWorld(dispatcher_, pairCache_, constraintSolver_, collisionConfiguration_);
        dynamicsWorld_->setGravity(btVector3(0, 20 * -9.81f, 0));
        dynamicsWorld_->setWorldUserInfo(this);
        dynamicsWorld_->setInternalTickCallback(SubstepCallback, static_cast<void*>(this));
    }

    PhysicsWorld::~PhysicsWorld()
    {
        for (int i = dynamicsWorld_->getNumConstraints() - 1; i >= 0; i--)
            dynamicsWorld_->removeConstraint(dynamicsWorld_->getConstraint(i));

        delete dynamicsWorld_;
        delete constraintSolver_;
        delete dispatcher_;
        delete ghostPairCallback_;
        delete pairCache_;
        delete collisionConfiguration_;
    }

    void PhysicsWorld::StepSimulation(float timeStep)
    {
        dynamicsWorld_->stepSimulation(timeStep);
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


}