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
#include "Constants.h"
#include "ICollision.h"
#include "PhysicsWorld.h"
#include "btBulletDynamicsCommon.h"

class btDynamicsWorld;
class btPairCachingGhostObject;
namespace NSG
{
    class Character : public ICollision, public btActionInterface, public Object
    {
    public:
        Character(PSceneNode sceneNode);
        ~Character();
        void AddShape(PShape shape, const Vector3& position = VECTOR3_ZERO, const Quaternion& rotation = QUATERNION_IDENTITY);
        void HandleCollisions(bool enable) {handleCollision_ = enable; }
        void SetRestitution(float restitution);
        void SetFriction(float friction);
        void SetStepHeight(float stepHeight); // [0..1] (a factor of shapeHeight_)
        void SetGravity(float gravity);
        float GetGravity() const { return gravity_; }
        bool IsOnGround() const;
        bool IsFalling() const { return verticalMove_ < 0; }
        bool IsJumping() const { return verticalMove_ > 0; }
        void SetJumpSpeed(float speed);
        void SetForwardSpeed(float speed);
        void SetAngularSpeed(float speed); // angle in degrees
		void Rotate(float angle); // angle in degrees
        BoundingBox GetColliderBoundingBox() const override;
        void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTime) override;
        void debugDraw(btIDebugDraw* debugDrawer) override;
        void ReScale();
        void Load(const pugi::xml_node& node) override;
        void Save(pugi::xml_node& node);
        SceneNode* GetSceneNode() const override;
        bool HandleCollision() const override { return handleCollision_; }
        ICollision* StepForwardCollides() const;
        void EnableFly(bool enable) { flying_ = enable; }
    private:
        Vector3 GetUnderFeetPoint(Vector3 origin) const; //point off contact with ground
        float GetGroundHeightFrom(const Vector3& pos) const;
        void ApplyGravity(float deltaTime);
        void StepForward(float deltaTime);
        PhysicsRaycastResult Obstruction(const Vector3& origin, const Vector3& targetPos, float radius) const;
        void SyncNode();
        void ReDoShape(const Vector3& newScale);
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        PWeakSceneNode sceneNode_;
        PWeakPhysicsWorld world_;
        std::shared_ptr<btPairCachingGhostObject> ghost_;
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
        float restitution_;
        float friction_;
        float stepHeight_;
        float verticalMove_;
        float jumpSpeed_;
        float forwardSpeed_;
        float angularSpeed_;
        float gravity_;
        int collisionGroup_;
        int collisionMask_;
        SignalEmpty::PSlot slotBeginFrame_;
        bool handleCollision_;
        float shapeHalfWidth_;
        float shapeHeight_;
        float shapeHalfHeight_;
        float shapeSphereRadius_;
        Vector3 forwardAxis_;
        Vector3 upAxis_;
        Vector3 nodeColliderOffset_;
        bool flying_;
		Vector3 stepForwardSourcePos_;
		Vector3 stepForwardTargetPos_;
		Vector3 stepForwardFinalPos_;
		Vector3 stepForward_;
		Vector3 minStepForward_;
		Vector3 bodyCenter_;
		Vector3 forwardDirection_;
		
    };
}