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
#include "Object.h"
#include "btBulletDynamicsCommon.h"
#include <vector>
using namespace std;

class btDynamicsWorld;
class btRigidBody;
class btTriangleMesh;
class btCollisionShape;

namespace NSG
{
    class RigidBody : public btMotionState, public Object
    {
    public:
        RigidBody(PSceneNode sceneNode);
        ~RigidBody();
        void SetMass(float mass);
        void SetShape(PhysicsShape phyShape, bool isStatic);
        void HandleCollisions(bool enable) {handleCollision_ = enable; }
        void SetLinearVelocity(const Vector3& lv, TransformSpace tspace = TS_PARENT);
        Vector3 GetLinearVelocity() const;
        void HandleManifold(btPersistentManifold* manifold, RigidBody* collider) const;
        void ReScale();
    private:
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        btTriangleMesh* GetTriangleMesh() const;
		btConvexHullShape* GetConvexHullTriangleMesh() const;
        void CreateShape();
        void getWorldTransform(btTransform& worldTrans) const override;
        void setWorldTransform(const btTransform& worldTrans) override;

        btRigidBody* body_;
		PWeakSceneNode sceneNode_;
        std::weak_ptr<btDynamicsWorld> owner_;
        btCollisionShape* shape_;
        float mass_;
        PhysicsShape phyShape_;
        bool isStatic_;
        bool handleCollision_;
    };
}


