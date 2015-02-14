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
class btTriangleMesh;

namespace NSG
{
    class RigidBody : public btMotionState, public Object
    {
    public:
        RigidBody(PSceneNode sceneNode);
        ~RigidBody();
        void SetMass(float mass);
        void SetShape(PhysicsShape phyShape);
        void HandleCollisions(bool enable) {handleCollision_ = enable; }
        void SetLinearVelocity(const Vector3& lv);
        Vector3 GetLinearVelocity() const;
        void SetAngularVelocity(const Vector3& av);
        void HandleManifold(btPersistentManifold* manifold, RigidBody* collider) const;
        void ReScale();
        bool IsStatic() const;
        void SyncWithNode();
		void SetRestitution(float restitution);
		void SetFriction(float friction);
		void SetLinearDamp(float linearDamp);
		void SetAngularDamp(float angularDamp);
		void SetMargin(float margin);
        void Load(const pugi::xml_node& node);
        void Save(pugi::xml_node& node);
		void SetCollisionMask(int group, int mask);
        void Activate();
        void ResetForces();
		void Reset();
		void AddToWorld();
		void RemoveFromWorld();
    private:
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        void CreateTriangleMesh();
		btConvexHullShape* GetConvexHullTriangleMesh() const;
        void CreateShape();
        void getWorldTransform(btTransform& worldTrans) const override;
        void setWorldTransform(const btTransform& worldTrans) override;

        btRigidBody* body_;
		PWeakSceneNode sceneNode_;
        std::weak_ptr<btDynamicsWorld> owner_;
        btCollisionShape* shape_;
		btTriangleMesh* triMesh_;
        float mass_;
        PhysicsShape phyShape_;
        bool handleCollision_;
		float restitution_;
		float friction_;
		float linearDamp_;
		float angularDamp_;
		float margin_;
		int collisionGroup_;
		int collisionMask_;
		bool inWorld_;
    };
}


