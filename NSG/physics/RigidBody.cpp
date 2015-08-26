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
#include "RigidBody.h"
#include "Scene.h"
#include "Material.h"
#include "SceneNode.h"
#include "Mesh.h"
#include "Shape.h"
#include "BoundingBox.h"
#include "Check.h"
#include "PhysicsWorld.h"
#include "Graphics.h"
#include "Engine.h"
#include "pugixml.hpp"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "hull.h"
#include <algorithm>

namespace NSG
{
    static const float DEFAULT_FRICTION = 0.5f;

    RigidBody::RigidBody(PSceneNode sceneNode)
        : Object(sceneNode->GetName() + "RigidBody"),
          sceneNode_(sceneNode),
          owner_(sceneNode->GetScene()->GetPhysicsWorld()->GetWorld()),
          mass_(0), //static by default
          compoundShape_(new btCompoundShape),
          handleCollision_(false),
          restitution_(0),
          friction_(DEFAULT_FRICTION),
          linearDamp_(0),
          angularDamp_(0),
          collisionGroup_((int)CollisionMask::ALL),
          collisionMask_((int)CollisionMask::ALL),
          inWorld_(false),
          trigger_(false),
          gravity_(sceneNode->GetScene()->GetPhysicsWorld()->GetGravity()),
          kinematic_(false),
          linearFactor_(1),
          angularFactor_(1)
    {
        CHECK_ASSERT(sceneNode, __FILE__, __LINE__);

        slotMaterialSet_ = sceneNode->SigMaterialSet()->Connect([this]()
        {
            SetMaterialPhysics();
            SetMaterialPhysicsSlot();
        });

        SetMaterialPhysics();
        SetMaterialPhysicsSlot();

        slotBeginFrame_ = Engine::SigBeginFrame()->Connect([this]()
        {
            IsReady();
        });
    }

    RigidBody::~RigidBody()
    {
    }

    void RigidBody::SetMaterialPhysicsSlot()
    {
        auto node = sceneNode_.lock();
        auto material = node->GetMaterial();
        if (material)
        {
            slotMaterialPhysicsSet_ = material->SigPhysicsSet()->Connect([this]()
            {
                SetMaterialPhysics();
            });
        }
        else
            slotMaterialPhysicsSet_ = nullptr;
    }

    void RigidBody::SetMaterialPhysics()
    {
        auto node = sceneNode_.lock();
        auto material = node->GetMaterial();
        if (material)
        {
            SetFriction(material->GetFriction());
        }
        else
            SetFriction(DEFAULT_FRICTION);
    }

    bool RigidBody::IsValid()
    {
        for (auto& shape : shapes_)
            if (!shape.shape->IsReady())
                return false;
        return !shapes_.empty();
    }

    void RigidBody::AllocateResources()
    {
        for (auto data : shapes_)
        {
            btTransform offset;
            //offset.setOrigin(ToBtVector3(shapeData.shape->GetScale() * shapeData.position));
            offset.setOrigin(ToBtVector3(data.position));
            offset.setRotation(ToBtQuaternion(data.rotation));
            compoundShape_->addChildShape(offset, data.shape->GetCollisionShape().get());
        }

        btVector3 inertia(0, 0, 0);
        if (mass_ > 0)
            compoundShape_->calculateLocalInertia(mass_, inertia);
        btRigidBody::btRigidBodyConstructionInfo info(mass_, (IsStatic() ? nullptr : this), compoundShape_.get(), inertia);
        info.m_restitution = restitution_;
        info.m_friction = friction_;
        info.m_linearDamping = linearDamp_;
        info.m_angularDamping = angularDamp_;
        body_ = std::make_shared<btRigidBody>(info);

        auto sceneNode = sceneNode_.lock();
        body_->setUserPointer(this);
        body_->setWorldTransform(ToTransform(sceneNode->GetGlobalPosition(), sceneNode->GetGlobalOrientation()));
        AddToWorld();
        body_->setFlags(BT_DISABLE_WORLD_GRAVITY);
        body_->setGravity(ToBtVector3(gravity_));
        body_->setLinearVelocity(ToBtVector3(linearVelocity_));
        body_->setAngularVelocity(ToBtVector3(angularVelocity_));
        body_->setLinearFactor(ToBtVector3(linearFactor_));
        body_->setAngularFactor(ToBtVector3(angularFactor_));
    }

    void RigidBody::ReleaseResources()
    {
        while (compoundShape_->getNumChildShapes())
            compoundShape_->removeChildShapeByIndex(0);

        if (body_)
        {
            body_->setUserPointer(nullptr);
            body_->setMotionState(nullptr);
            RemoveFromWorld();
        }
    }

    void RigidBody::SyncWithNode()
    {
        auto sceneNode(sceneNode_.lock());

        if (body_)
        {
            Activate();
            btTransform& worldTrans = body_->getWorldTransform();
            worldTrans.setRotation(ToBtQuaternion(sceneNode->GetGlobalOrientation()));
            worldTrans.setOrigin(ToBtVector3(sceneNode->GetGlobalPosition()));

            btTransform interpTrans = body_->getInterpolationWorldTransform();
            interpTrans.setOrigin(worldTrans.getOrigin());
            interpTrans.setRotation(worldTrans.getRotation());
            body_->setInterpolationWorldTransform(interpTrans);
            body_->updateInertiaTensor();
        }
    }


    void RigidBody::SetGravity(const Vector3& gravity)
    {
        if (gravity != gravity_)
        {
            gravity_ = gravity;
            if (body_)
            {
                body_->setGravity(ToBtVector3(gravity_));
                Activate();
            }
        }
    }

    void RigidBody::SetMass(float mass)
    {
        if (mass_ != mass)
        {
            mass_ = mass;
            if (body_)
                ReAddToWorld();
        }
    }

    void RigidBody::SetKinematic(bool enable)
    {
        if (enable != kinematic_)
        {
            kinematic_ = enable;
            ReAddToWorld();
        }
    }

    void RigidBody::AddShape(PShape shape, const Vector3& position, const Quaternion& rotation)
    {
        auto key = shape->GetName();

        auto slotShapeReleased = shape->SigReleased()->Connect([this]()
        {
            Invalidate();
        });

        shapes_.push_back(ShapeData{ shape, position, rotation, slotShapeReleased});
        Invalidate();
    }

    void RigidBody::SetRestitution(float restitution)
    {
        if (restitution != restitution_)
        {
            restitution_ = restitution;
            if (body_)
                body_->setRestitution(restitution);
        }
    }

    void RigidBody::SetFriction(float friction)
    {
        if (friction != friction_)
        {
            friction_ = friction;
            if (body_)
                body_->setFriction(friction);
        }
    }

    void RigidBody::SetLinearDamp(float linearDamp)
    {
        if (linearDamp != linearDamp_)
        {
            linearDamp_ = linearDamp;
            if (body_)
                body_->setDamping(linearDamp_, angularDamp_);
        }
    }

    void RigidBody::SetAngularDamp(float angularDamp)
    {
        if (angularDamp != angularDamp_)
        {
            angularDamp_ = angularDamp;
            if (body_)
                body_->setDamping(linearDamp_, angularDamp_);
        }
    }

    void RigidBody::getWorldTransform(btTransform& worldTrans) const
    {
        auto sceneNode(sceneNode_.lock());
        worldTrans = ToTransform(sceneNode->GetGlobalPosition(), sceneNode->GetGlobalOrientation());
    }

    void RigidBody::setWorldTransform(const btTransform& worldTrans)
    {
        auto sceneNode(sceneNode_.lock());
        sceneNode->SetGlobalOrientation(ToQuaternion(worldTrans.getRotation()));
        sceneNode->SetGlobalPosition(ToVector3(worldTrans.getOrigin()));
    }

    bool RigidBody::IsStatic() const
    {
        return mass_ == 0;
    }

    void RigidBody::SetLinearVelocity(const Vector3& lv)
    {
        // only dynamic bodies
        if (!IsStatic())
        {
            if (linearVelocity_ != lv)
            {
                linearVelocity_ = lv;
                if (body_)
                {
                    if (lv != VECTOR3_ZERO)
                        Activate();
                    body_->setLinearVelocity(ToBtVector3(lv));
                }
            }
        }
    }

    void RigidBody::SetAngularVelocity(const Vector3& av)
    {
        if (!IsStatic())
        {
            if (angularVelocity_ != av)
            {
                angularVelocity_ = av;
                if (body_)
                {
                    if (av != VECTOR3_ZERO)
                        Activate();
                    body_->setAngularVelocity(ToBtVector3(av));
                }
            }
        }
    }

    Vector3 RigidBody::GetLinearVelocity() const
    {
        return body_ ? ToVector3(body_->getLinearVelocity()) : linearVelocity_;
    }

    void RigidBody::HandleManifold(btPersistentManifold* manifold, RigidBody* collider) const
    {
        if (!handleCollision_) return;

        int nrc = manifold->getNumContacts();
        if (nrc)
        {
            for (int j = 0; j < nrc; ++j)
            {
                ContactPoint cinf;
                btManifoldPoint& pt = manifold->getContactPoint(j);

                PSceneNode sceneNode(sceneNode_.lock());
                cinf.collider_ = collider->sceneNode_.lock().get();
                cinf.normalB_ = ToVector3(pt.m_normalWorldOnB);
                cinf.appliedImpulse_ = pt.m_appliedImpulse;
                cinf.appliedImpulseLateral1_ = pt.m_appliedImpulseLateral1;
                cinf.appliedImpulseLateral2_ = pt.m_appliedImpulseLateral2;
                cinf.contactMotion1_ = pt.m_contactMotion1;
                cinf.contactMotion2_ = pt.m_contactMotion2;
                cinf.contactCFM1_ = pt.m_contactCFM1;
                cinf.contactCFM2_ = pt.m_contactCFM2;
                cinf.lateralFrictionDir1_ = ToVector3(pt.m_lateralFrictionDir1);
                cinf.lateralFrictionDir2_ = ToVector3(pt.m_lateralFrictionDir2);
                sceneNode->OnCollision(cinf);
            }
        }
    }

    void RigidBody::ReDoShape(const Vector3& newScale)
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

    void RigidBody::ReScale()
    {
        auto sceneNode(sceneNode_.lock());
        ReDoShape(sceneNode->GetGlobalScale());
    }

    void RigidBody::UpdateInertia()
    {
        btVector3 inertia(0.0f, 0.0f, 0.0f);
        if (mass_ > 0)
            compoundShape_->calculateLocalInertia(mass_, inertia);
        body_->setMassProps(mass_, inertia);
        body_->updateInertiaTensor();
    }

    void RigidBody::Load(const pugi::xml_node& node)
    {
        mass_ = node.attribute("mass").as_float();
        handleCollision_ = node.attribute("handleCollision").as_bool();
        restitution_ = node.attribute("restitution").as_float();
        friction_ = node.attribute("friction").as_float();
        linearDamp_ = node.attribute("linearDamp").as_float();
        angularDamp_ = node.attribute("angularDamp").as_float();
        collisionGroup_ = node.attribute("collisionGroup").as_int();
        collisionMask_ = node.attribute("collisionMask").as_int();
        trigger_ = node.attribute("trigger").as_bool();
        auto gravityAtt = node.attribute("gravity");
        if (gravityAtt)
            gravity_ = ToVertex3(gravityAtt.as_string());
        linearVelocity_ = ToVertex3(node.attribute("linearVelocity").as_string());
        angularVelocity_ = ToVertex3(node.attribute("angularVelocity").as_string());
        linearFactor_ = ToVertex3(node.attribute("linearFactor").as_string());
        angularFactor_ = ToVertex3(node.attribute("angularFactor").as_string());
        kinematic_ = node.attribute("kinematic").as_bool();

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

    void RigidBody::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("RigidBody");
        child.append_attribute("mass").set_value(mass_);
        child.append_attribute("handleCollision").set_value(handleCollision_);
        child.append_attribute("restitution").set_value(restitution_);
        child.append_attribute("friction").set_value(friction_);
        child.append_attribute("linearDamp").set_value(linearDamp_);
        child.append_attribute("angularDamp").set_value(angularDamp_);
        child.append_attribute("collisionGroup").set_value(collisionGroup_);
        child.append_attribute("collisionMask").set_value(collisionMask_);
        child.append_attribute("trigger").set_value(trigger_);
        child.append_attribute("gravity").set_value(ToString(gravity_).c_str());
        child.append_attribute("linearVelocity").set_value(ToString(linearVelocity_).c_str());
        child.append_attribute("angularVelocity").set_value(ToString(angularVelocity_).c_str());
        child.append_attribute("linearFactor").set_value(ToString(linearFactor_).c_str());
        child.append_attribute("angularFactor").set_value(ToString(angularFactor_).c_str());
        child.append_attribute("kinematic").set_value(kinematic_);

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
    void RigidBody::SetCollisionMask(int group, int mask)
    {
        if (collisionGroup_ != group || collisionMask_ != mask)
        {
            collisionGroup_ = group;
            collisionMask_ = mask;
            ReAddToWorld();
        }
    }

    void RigidBody::Activate()
    {
        if (!IsStatic() && body_)
            body_->activate(true);
    }

    void RigidBody::ResetForces()
    {
        if (!IsStatic() && IsReady())
            body_->clearForces();
    }

    void RigidBody::Reset()
    {
        ResetForces();
        SetLinearVelocity(VECTOR3_ZERO);
        SetAngularVelocity(VECTOR3_ZERO);
        SetLinearFactor(VECTOR3_ONE);
        SetAngularFactor(VECTOR3_ONE);
    }

    void RigidBody::ReAddToWorld()
    {
        RemoveFromWorld();
        AddToWorld();
    }

    void RigidBody::AddToWorld()
    {
        if (!inWorld_ && body_)
        {
            auto world = owner_.lock();
            CHECK_ASSERT(world, __FILE__, __LINE__);
            CHECK_ASSERT(body_, __FILE__, __LINE__);
            int flags = body_->getCollisionFlags();

            if (trigger_)
                flags |= btCollisionObject::CF_NO_CONTACT_RESPONSE;
            else
                flags &= ~btCollisionObject::CF_NO_CONTACT_RESPONSE;

            if (kinematic_)
                flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
            else
                flags &= ~btCollisionObject::CF_KINEMATIC_OBJECT;

            body_->setCollisionFlags(flags);
            body_->forceActivationState(kinematic_ ? DISABLE_DEACTIVATION : ISLAND_SLEEPING);

            world->addRigidBody(body_.get(), (int)collisionGroup_, (int)collisionMask_);
            if (mass_ > 0.0f)
                Activate();
            else
            {
                SetLinearVelocity(VECTOR3_ZERO);
                SetAngularVelocity(VECTOR3_ZERO);
            }
            inWorld_ = true;
        }
    }

    void RigidBody::RemoveFromWorld()
    {
        if (inWorld_ && body_)
        {
            auto world = owner_.lock();
            if (world)
            {
                CHECK_ASSERT(body_, __FILE__, __LINE__);
                world->removeRigidBody(body_.get());
                inWorld_ = false;
            }
        }
    }

    void RigidBody::SetTrigger(bool enable)
    {
        if (trigger_ != enable)
        {
            trigger_ = enable;
            RemoveFromWorld();
            AddToWorld();
        }
    }

    void RigidBody::SetLinearFactor(const Vector3& factor)
    {
        if (linearFactor_ != factor)
        {
            linearFactor_ = factor;
            if (body_)
                body_->setLinearFactor(ToBtVector3(factor));
        }
    }

    void RigidBody::SetAngularFactor(const Vector3& factor)
    {
        if (angularFactor_ != factor)
        {
            angularFactor_ = factor;
            if (body_)
                body_->setAngularFactor(ToBtVector3(factor));
        }
    }

    void RigidBody::ApplyForce(const Vector3& force)
    {
        if (body_ && force != VECTOR3_ZERO)
        {
            Activate();
            body_->applyCentralForce(ToBtVector3(force));
        }
    }

    void RigidBody::ApplyImpulse(const Vector3& impulse)
    {
        if (body_ && impulse != VECTOR3_ZERO)
        {
            Activate();
            body_->applyCentralImpulse(ToBtVector3(impulse));
        }
    }

}

