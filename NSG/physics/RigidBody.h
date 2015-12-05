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
#include "Object.h"
#include "ICollision.h"
#include "Constants.h"
#include "btBulletDynamicsCommon.h"
#include <map>
using namespace std;

class btDynamicsWorld;
class btRigidBody;
namespace NSG
{
	class RigidBody : public ICollision, public btMotionState, public Object
    {
    public:
        RigidBody(PSceneNode sceneNode);
        ~RigidBody();
		void SetGravity(const Vector3& gravity);
        void SetKinematic(bool enable);
        bool IsKinematic() const { return kinematic_; }
        void SetMass(float mass);
        void AddShape(PShape shape, const Vector3& position = VECTOR3_ZERO, const Quaternion& rotation = QUATERNION_IDENTITY);
        void HandleCollisions(bool enable) {handleCollision_ = enable; }
        void SetLinearVelocity(const Vector3& lv);
        Vector3 GetLinearVelocity() const;
        void SetAngularVelocity(const Vector3& av);
        void ReScale();
        bool IsStatic() const;
        void SyncWithNode();
		void SetRestitution(float restitution);
		void SetFriction(float friction);
        void SetRollingFriction(float friction);
		void SetLinearDamp(float linearDamp);
		void SetAngularDamp(float angularDamp);
        void Load(const pugi::xml_node& node) override;
        void Save(pugi::xml_node& node);
		void SetCollisionMask(int group, int mask);
        void Activate();
        void ResetForces();
		void Reset();
        void ReAddToWorld();
        void AddToWorld();
        void RemoveFromWorld();
        void SetTrigger(bool enable);
        void SetLinearFactor(const Vector3& factor);
        const Vector3& GetLinearFactor() const { return linearFactor_; }
        void SetAngularFactor(const Vector3& factor);
        const Vector3& GetAngularFactor() const { return angularFactor_; }
        void ApplyForce(const Vector3& force);
        void ApplyImpulse(const Vector3& impulse);
        BoundingBox GetColliderBoundingBox() const override;
		SceneNode* GetSceneNode() const override;
		bool HandleCollision() const override { return handleCollision_; }
	private:
		void ReDoShape(const Vector3& newScale);
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        void getWorldTransform(btTransform& worldTrans) const override;
        void setWorldTransform(const btTransform& worldTrans) override;
        void UpdateInertia();
        void SetMaterialPhysicsSlot();
        void SetMaterialPhysics();

        std::shared_ptr<btRigidBody> body_;
		PWeakSceneNode sceneNode_;
        std::weak_ptr<btDynamicsWorld> owner_;
        float mass_;
		std::shared_ptr<btCompoundShape> compoundShape_;
		struct ShapeData
		{
			PShape shape;
			Vector3 position;
			Quaternion rotation;
            SignalEmpty::PSlot slotShapeReleased;
		};
		typedef std::vector<ShapeData> Shapes;
		Shapes shapes_;
        bool handleCollision_;
		float restitution_;
		float friction_;
        float rollingFriction_;
		float linearDamp_;
		float angularDamp_;
		int collisionGroup_;
		int collisionMask_;
		bool inWorld_;
        bool trigger_;
		Vector3 gravity_;
        Vector3 linearVelocity_;
        Vector3 angularVelocity_;
		bool kinematic_;
		Vector3 linearFactor_;
		Vector3 angularFactor_;
        SignalEmpty::PSlot slotMaterialSet_;
        SignalEmpty::PSlot slotMaterialPhysicsSet_;
        SignalEmpty::PSlot slotBeginFrame_;
    };
}


