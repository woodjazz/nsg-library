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
#pragma once
#include "Types.h"

class btDynamicsWorld;
class btDefaultCollisionConfiguration;
struct btDbvtBroadphase;
class btGhostPairCallback;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace NSG
{
    class PhysicsWorld
    {
    public:
        PhysicsWorld();
        ~PhysicsWorld();
        void StepSimulation(float timeStep);
		void Substep(float tick);
        std::shared_ptr<btDiscreteDynamicsWorld> GetWorld() const { return dynamicsWorld_; }
    private:
        static void SubstepCallback(btDynamicsWorld* dyn, float tick);
        btDefaultCollisionConfiguration* collisionConfiguration_;
        btDbvtBroadphase* pairCache_;
        btGhostPairCallback* ghostPairCallback_;
        btCollisionDispatcher* dispatcher_;
        btSequentialImpulseConstraintSolver* constraintSolver_;
		std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld_;
    };
}