/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "Character.h"
#include "Shape.h"
#include "SceneNode.h"
#include "Scene.h"
#include "BoundingBox.h"
#include "Util.h"
#include "Check.h"
#include "Engine.h"
#include "Renderer.h"
#include "DebugRenderer.h"
#include "StringConverter.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "pugixml.hpp"

namespace NSG
{
    static const float DEFAULT_FRICTION = 0.5f;

    Character::Character(PSceneNode sceneNode)
        : Object(sceneNode->GetName() + "Character"),
          sceneNode_(sceneNode),
          world_(sceneNode->GetScene()->GetPhysicsWorld()),
          ghost_(new btPairCachingGhostObject),
          compoundShape_(new btCompoundShape),
          restitution_(0),
          friction_(DEFAULT_FRICTION),
          stepHeight_(0.6f),
          verticalMove_(0),
          jumpSpeed_(0),
          forwardSpeed_(0),
          angularSpeed_(0),
          gravity_(9.8f),
          collisionGroup_((int)CollisionMask::ALL),
          collisionMask_((int)CollisionMask::ALL),
          handleCollision_(false),
          shapeHalfWidth_(0),
          shapeHeight_(0),
          shapeHalfHeight_(0),
          shapeSphereRadius_(0),
          forwardAxis_(-VECTOR3_FORWARD),
          upAxis_(VECTOR3_UP),
          flying_(false)
    {
        ghost_->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

        slotBeginFrame_ = Engine::SigBeginFrame()->Connect([this]()
        {
            TryReady();
        });
    }

    Character::~Character()
    {
    }

    float Character::GetGroundHeightFrom(const Vector3& pos) const
    {
        auto targetPos = pos - VECTOR3_UP * MAX_WORLD_SIZE;
        auto direction = targetPos - pos;
        auto distance = Length(direction);
        PhysicsRaycastResult result = world_.lock()->SphereCastBut(this, pos, Normalize(direction), shapeHalfWidth_, distance);
        if (result.HasCollided())
            return result.position_.y;
        return -MAX_WORLD_SIZE;
    }

    Vector3 Character::GetUnderFeetPoint(Vector3 origin) const
    {
        origin.y -= shapeHalfHeight_ + nodeColliderOffset_.y;
        return origin;
    }

    void Character::ApplyGravity(float deltaTime)
    {
        if(flying_)
            return;
        verticalMove_ += jumpSpeed_ * deltaTime - gravity_ * deltaTime * deltaTime;
        auto worldTrans = ghost_->getWorldTransform();
        auto feetPoint = GetUnderFeetPoint(ToVector3(worldTrans.getOrigin()));
        auto targetPos = feetPoint + verticalMove_ * VECTOR3_UP;
        auto result = Obstruction(feetPoint, targetPos, shapeHalfWidth_);
        if (result.HasCollided())
        {
            auto onGroundPos = result.position_.y + shapeHalfHeight_ + nodeColliderOffset_.y;
            targetPos.y = onGroundPos;
            verticalMove_ = 0;
            worldTrans.setOrigin(ToBtVector3(targetPos));
            ghost_->setWorldTransform(worldTrans);
            SyncNode();
        }
        else if(verticalMove_)
        {
            worldTrans.setOrigin(ToBtVector3(targetPos));
            ghost_->setWorldTransform(worldTrans);
            SyncNode();
        }
    }

    void Character::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTime)
    {
        ApplyGravity(deltaTime);
        StepForward(deltaTime);
    }

    ICollision* Character::StepForwardCollides() const
    {
		float stepUp = stepHeight_ * shapeHeight_;
		if (flying_)
			stepUp = 0;
		auto result = Obstruction(stepForwardSourcePos_, stepForwardTargetPos_, std::min(stepUp, shapeSphereRadius_));
        return result.collider_;
    }

    void Character::StepForward(float deltaTime)
    {
        auto worldTrans =  ghost_->getWorldTransform();
        auto rot = ToQuaternion(worldTrans.getRotation());
        auto position = ToVector3(worldTrans.getOrigin());
        auto feetPoint = GetUnderFeetPoint(position);
		forwardDirection_ = rot * forwardAxis_;
		stepForward_ = forwardSpeed_ * deltaTime * forwardDirection_;
		minStepForward_ = 1.0f * shapeSphereRadius_ * forwardDirection_;
        float stepUp = stepHeight_ * shapeHeight_;
        if(flying_)
            stepUp = 0;
        auto verticalStep = VECTOR3_UP * stepUp;
		bodyCenter_ = feetPoint + VECTOR3_UP * shapeHalfHeight_;
		auto groundHeight = GetGroundHeightFrom(bodyCenter_);
		stepForwardSourcePos_ = bodyCenter_ + verticalStep;
		stepForwardFinalPos_ = stepForwardSourcePos_ + stepForward_;
		if (Length(minStepForward_) < Length(stepForward_))
			stepForwardTargetPos_ = stepForwardFinalPos_;
		else
			stepForwardTargetPos_ = stepForwardSourcePos_ + minStepForward_;
		auto result = Obstruction(stepForwardSourcePos_, stepForwardTargetPos_, std::min(stepUp, shapeSphereRadius_));
        if (result.HasCollided())
        {
            // collided forward ( cannot step up)
			auto dir2Target = stepForwardFinalPos_ - stepForwardSourcePos_;
            slidingDirection_ = GetSlidingVector(dir2Target, result.normal_);
            slidingDirection_.y = 0;
			if (Length(slidingDirection_) > EPSILON)
			{
				auto result = Obstruction(stepForwardSourcePos_, stepForwardSourcePos_ + slidingDirection_, 0.25f * shapeSphereRadius_);
				if (!result.HasCollided())
					stepForwardFinalPos_ = position + slidingDirection_; //slide
				else
					stepForwardFinalPos_ = position; //keep current position
			}
			else
				stepForwardFinalPos_ = position; //keep current position
        }
        else if (IsOnGround())
        {
			auto groundHeight2 = GetGroundHeightFrom(stepForwardFinalPos_);
            position.y = groundHeight2 + shapeHalfHeight_ + nodeColliderOffset_.y;
            if (std::abs(groundHeight - groundHeight2) < stepUp)
				stepForwardFinalPos_ = position + stepForward_; // ground is at the same height after stepping
            else if (groundHeight + stepUp > groundHeight2)
				stepForwardFinalPos_ = position + stepForward_; // stepping down
            else
				stepForwardFinalPos_ = position + stepForward_ + verticalStep; //stepping up
        }
        else
        {
			stepForwardFinalPos_ = position + stepForward_;
        }
		worldTrans.setOrigin(ToBtVector3(stepForwardFinalPos_));
        auto orn = worldTrans.getBasis();
        auto incRot = AngleAxis(Radians(angularSpeed_ * deltaTime), upAxis_);
        orn *= btMatrix3x3(ToBtQuaternion(incRot));
        worldTrans.setBasis(orn);
        ghost_->setWorldTransform(worldTrans);
        SyncNode();
    }

    bool Character::IsOnGround() const
    {
        return verticalMove_ == 0 && !flying_;
    }

    void Character::debugDraw(btIDebugDraw* debugDrawer)
    {
		auto debugRenderer = Renderer::GetPtr()->GetDebugRenderer();
		debugRenderer->AddLine(stepForwardSourcePos_, stepForwardTargetPos_, Color(COLOR_RED, 1));
        auto worldTrans =  ghost_->getWorldTransform();
        auto position = ToVector3(worldTrans.getOrigin());
        debugRenderer->AddLine(position, position + 10.f * Normalize(slidingDirection_), Color(COLOR_BLUE, 1));
    }

    PhysicsRaycastResult Character::Obstruction(const Vector3& origin, const Vector3& targetPos, float radius) const
    {
        auto direction = targetPos - origin;
        auto distance = Length(direction);
        PhysicsRaycastResult result = world_.lock()->SphereCastBut(this, origin, Normalize(direction), radius, distance);
        return result;
    }

    void Character::SyncNode()
    {
        auto& worldTrans = ghost_->getWorldTransform();
        auto sceneNode(sceneNode_.lock());
        sceneNode->SetGlobalOrientation(ToQuaternion(worldTrans.getRotation()));
        auto pos = ToVector3(worldTrans.getOrigin());
        sceneNode->SetGlobalPosition(pos);
    }

    void Character::AddShape(PShape shape, const Vector3& position, const Quaternion& rotation)
    {
        auto key = shape->GetName();

        auto slotShapeReleased = shape->SigReleased()->Connect([this]()
        {
            Invalidate();
        });

        shapes_.push_back(ShapeData{ shape, position, rotation, slotShapeReleased});
        Invalidate();
    }

    void Character::SetRestitution(float restitution)
    {
        if (restitution != restitution_)
        {
            restitution_ = restitution;
            if (ghost_)
                ghost_->setRestitution(restitution);
        }
    }

    void Character::SetFriction(float friction)
    {
        if (friction != friction_)
        {
            friction_ = friction;
            if (ghost_)
                ghost_->setFriction(friction);
        }
    }

    void Character::SetStepHeight(float stepHeight)
    {
        if (stepHeight_ != stepHeight)
        {
            stepHeight_ = stepHeight;
            Invalidate();
        }
    }

    void Character::SetJumpSpeed(float speed)
    {
        jumpSpeed_ = speed;
    }

    void Character::SetGravity(float gravity)
    {
        if (gravity_ != gravity)
        {
            gravity_ = gravity;
        }
    }

    void Character::SetForwardSpeed(float speed)
    {
        forwardSpeed_ = speed;
    }

    void Character::SetAngularSpeed(float speed)
    {
        angularSpeed_ = speed;
    }

	void Character::Rotate(float angle)
	{
		auto worldTrans = ghost_->getWorldTransform();
		auto orn = worldTrans.getBasis();
		auto incRot = AngleAxis(Radians(angle), upAxis_);
		orn *= btMatrix3x3(ToBtQuaternion(incRot));
		worldTrans.setBasis(orn);
		ghost_->setWorldTransform(worldTrans);
		SyncNode();
	}

    bool Character::IsValid()
    {
        if (world_.lock() && sceneNode_.lock())
        {
            for (auto& shape : shapes_)
                if (!shape.shape->IsReady())
                    return false;
            return !shapes_.empty();
        }
        return false;
    }

    void Character::AllocateResources()
    {
        for (auto data : shapes_)
        {
            btTransform offset;
            offset.setOrigin(ToBtVector3(data.position));
            offset.setRotation(ToBtQuaternion(data.rotation));
            compoundShape_->addChildShape(offset, data.shape->GetCollisionShape().get());
        }
        ghost_->setUserPointer(this);
        ghost_->setCollisionShape(compoundShape_.get());
        ghost_->setFriction(friction_);
        ghost_->setRestitution(restitution_);

        auto sceneNode = sceneNode_.lock();
        auto position = sceneNode->GetGlobalPosition();
        auto orientation = sceneNode->GetGlobalOrientation();
        ghost_->setWorldTransform(ToTransform(position, orientation));
        forwardAxis_ = orientation * -VECTOR3_FORWARD;
        upAxis_ = orientation * VECTOR3_UP;
        auto bb = GetColliderBoundingBox();
        auto size = bb.Size();
        nodeColliderOffset_ = position - bb.Center();
        shapeHalfWidth_ = 0.5f * std::max(size.x, size.z);
        shapeHeight_ = size.y;
        shapeHalfHeight_ = 0.5f * size.y;
        shapeSphereRadius_ = std::max(shapeHalfWidth_, shapeHalfHeight_);
        auto owner = world_.lock()->GetWorld();
        owner->addCollisionObject(ghost_.get(), (int)collisionGroup_, (int)collisionMask_);
        owner->addAction(this);
    }

    void Character::ReleaseResources()
    {
        while (compoundShape_->getNumChildShapes())
            compoundShape_->removeChildShapeByIndex(0);

        auto owner = world_.lock()->GetWorld();
        owner->removeAction(this);
        owner->removeCollisionObject(ghost_.get());
        ghost_->setUserPointer(nullptr);
    }

    BoundingBox Character::GetColliderBoundingBox() const
    {
        if (compoundShape_ && ghost_)
        {
            btVector3 aabbMin;
            btVector3 aabbMax;
            compoundShape_->getAabb(ghost_->getWorldTransform(), aabbMin, aabbMax);
            return BoundingBox(ToVector3(aabbMin), ToVector3(aabbMax));
        }
        return BoundingBox();
    }

    void Character::ReScale()
    {
        auto sceneNode(sceneNode_.lock());
        ReDoShape(sceneNode->GetGlobalScale());
    }

    void Character::ReDoShape(const Vector3& newScale)
    {
        Shapes shapes = shapes_;
        shapes_.clear();
        for (auto it : shapes)
        {
            auto shape = it.shape;
            auto position = it.position;
            auto rotation = it.rotation;
            PMesh mesh;
            Vector3 scale;
            PhysicsShape type;
            ShapeKey(shape->GetName()).GetData(mesh, scale, type);
            if (mesh)
                shape = Shape::GetOrCreate(ShapeKey{mesh, newScale});
            else
                shape = Shape::GetOrCreate(ShapeKey{type, newScale});
            AddShape(shape, position, rotation);
        }
    }

    void Character::Load(const pugi::xml_node& node)
    {
        handleCollision_ = true;// node.attribute("handleCollision").as_bool();
        auto restitutionAtt = node.attribute("restitution");
        if (restitutionAtt)
            restitution_ = restitutionAtt.as_float();
        auto frictionAtt = node.attribute("friction");
        if (frictionAtt)
            friction_ = frictionAtt.as_float();
        collisionGroup_ = node.attribute("collisionGroup").as_int();
        collisionMask_ = node.attribute("collisionMask").as_int();
        stepHeight_ = node.attribute("stepHeight").as_float();
        auto gravityAtt = node.attribute("gravity");
        if (gravityAtt)
            gravity_ = gravityAtt.as_float();

        auto shapesNode = node.child("Shapes");
        if (shapesNode)
        {
            auto shapeNode = shapesNode.child("Shape");
            while (shapeNode)
            {
                auto shapeName = shapeNode.attribute("name").as_string();
                auto shape = Shape::GetOrCreate(shapeName);
                Vertex3 position(VECTOR3_ZERO);
                auto posAtt = shapeNode.attribute("position");
                if (posAtt)
                    position = ToVertex3(posAtt.as_string());
                Quaternion orientation(QUATERNION_IDENTITY);
                auto rotAtt = shapeNode.attribute("orientation");
                if (rotAtt)
                    orientation = ToQuaternion(rotAtt.as_string());
                AddShape(shape, position, orientation);
                shapeNode = shapeNode.next_sibling("Shape");
            }
        }

        Invalidate();
    }

    void Character::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Character");
        child.append_attribute("handleCollision").set_value(handleCollision_);
        child.append_attribute("restitution").set_value(restitution_);
        child.append_attribute("friction").set_value(friction_);
        child.append_attribute("collisionGroup").set_value(collisionGroup_);
        child.append_attribute("collisionMask").set_value(collisionMask_);
        child.append_attribute("stepHeight").set_value(stepHeight_);
        child.append_attribute("gravity").set_value(gravity_);

        if (!shapes_.empty())
        {
            auto shapesNode = child.append_child("Shapes");
            for (auto& shape : shapes_)
            {
                auto shapeNode = shapesNode.append_child("Shape");
                shapeNode.append_attribute("name").set_value(shape.shape->GetName().c_str());
                shapeNode.append_attribute("position").set_value(ToString(shape.position).c_str());
                shapeNode.append_attribute("orientation").set_value(ToString(shape.rotation).c_str());
            }
        }
    }

    SceneNode* Character::GetSceneNode() const
    {
        return sceneNode_.lock().get();
    }
}