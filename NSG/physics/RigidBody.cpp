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
#include "RigidBody.h"
#include "App.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Mesh.h"
#include "BoundingBox.h"
#include "Check.h"
#include "PhysicsWorld.h"
#include "pugixml.hpp"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "hull.h"
#include <algorithm>

namespace NSG
{
    static btTransform ToTransform(const Vector3& pos, const Quaternion& rot)
    {
        btTransform trans;
        trans.setIdentity();
        trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
        trans.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
        return trans;
    }

    static btVector3 ToBtVector3(const Vector3& obj)
    {
        return btVector3(obj.x, obj.y, obj.z);
    }

    static Vector3 ToVector3(const btVector3& obj)
    {
        return Vector3(obj.x(), obj.y(), obj.z());
    }

    static btQuaternion ToBtQuaternion(const Quaternion& q)
    {
        return btQuaternion(q.x, q.y, q.z, q.w);
    }

    static Quaternion ToQuaternion(const btQuaternion& q)
    {
        return Quaternion(q.w(), q.x(), q.y(), q.z());
    }

    RigidBody::RigidBody(PSceneNode sceneNode)
        : body_(nullptr),
          sceneNode_(sceneNode),
          owner_(sceneNode->GetScene()->GetPhysicsWorld()->GetWorld()),
          shape_(nullptr),
          triMesh_(nullptr),
          mass_(0), //static by default
          phyShape_(SH_UNKNOWN),
          handleCollision_(false),
          restitution_(0),
          friction_(0.5f),
          linearDamp_(0),
          angularDamp_(0),
          margin_(.06f),
          collisionGroup_((int)CollisionMask::ALL),
          collisionMask_((int)CollisionMask::ALL),
          inWorld_(false),
		  gravity_(sceneNode->GetScene()->GetPhysicsWorld()->GetGravity())
    {
        //Since a non-visible scenenode will neither be rendered nor be validated then
        //better not to invalidate a Rigidbody (Rigidbody has to be alive because can make (in any moment) an object visible again)
        AllowInvalidate(false); //this will force not to invalidate the rigidbody(for example when the application is minimized)
        CHECK_ASSERT(sceneNode, __FILE__, __LINE__);
    }

    RigidBody::~RigidBody()
    {
        Invalidate(true);
    }

    bool RigidBody::IsValid()
    {
        auto sceneNode = sceneNode_.lock();
        return phyShape_ != SH_UNKNOWN && sceneNode && sceneNode->GetMesh();
    }

    void RigidBody::AllocateResources()
    {
        CreateShape();

        btVector3 inertia(0, 0, 0);
        if (mass_ > 0)
            shape_->calculateLocalInertia(mass_, inertia);
        btRigidBody::btRigidBodyConstructionInfo info(mass_, (IsStatic() ? nullptr : this), shape_, inertia);
        info.m_restitution = restitution_;
        info.m_friction = friction_;
        info.m_linearDamping = linearDamp_;
        info.m_angularDamping = angularDamp_;
        body_ = new btRigidBody(info);

        PSceneNode sceneNode(sceneNode_.lock());
        body_->setUserPointer(this);
        body_->setWorldTransform(ToTransform(sceneNode->GetGlobalPosition(), sceneNode->GetGlobalOrientation()));
        AddToWorld();
		body_->setFlags(BT_DISABLE_WORLD_GRAVITY);
		body_->setGravity(ToBtVector3(gravity_));
    }

    void RigidBody::ReleaseResources()
    {
        if (body_)
        {
            body_->setUserPointer(nullptr);
            body_->setMotionState(nullptr);
            RemoveFromWorld();
            delete shape_;
            delete triMesh_;
            delete body_;

            triMesh_ = nullptr;
            shape_ = nullptr;
            body_ = nullptr;
        }
    }

    void RigidBody::SyncWithNode()
    {
        PSceneNode sceneNode(sceneNode_.lock());

        if (IsReady())
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
			if (IsReady())
				body_->setGravity(ToBtVector3(gravity_));
		}
	}

    void RigidBody::SetMass(float mass)
    {
        if (mass_ != mass)
        {
            mass_ = mass;
            Invalidate(true);
            IsReady(); //rigidbody needs to be active even when it is not visible
        }
    }

    void RigidBody::SetShape(PhysicsShape phyShape)
    {
        if (phyShape_ != phyShape)
        {
            phyShape_ = phyShape;
            Invalidate(true);
            IsReady(); //rigidbody needs to be active even when it is not visible
        }
    }

    void RigidBody::SetRestitution(float restitution)
    {
        if (restitution != restitution_)
        {
            restitution_ = restitution;
            Invalidate(true);
            IsReady(); //rigidbody needs to be active even when it is not visible
        }
    }

    void RigidBody::SetFriction(float friction)
    {
        if (friction != friction_)
        {
            friction_ = friction;
            Invalidate(true);
            IsReady(); //rigidbody needs to be active even when it is not visible
        }
    }

    void RigidBody::SetLinearDamp(float linearDamp)
    {
        if (linearDamp != linearDamp_)
        {
            linearDamp_ = linearDamp;
            Invalidate(true);
            IsReady(); //rigidbody needs to be active even when it is not visible
        }
    }

    void RigidBody::SetAngularDamp(float angularDamp)
    {
        if (angularDamp != angularDamp_)
        {
            angularDamp_ = angularDamp;
            Invalidate(true);
            IsReady(); //rigidbody needs to be active even when it is not visible
        }
    }

    void RigidBody::SetMargin(float margin)
    {
        if (margin != margin_)
        {
            margin_ = margin;
            Invalidate(true);
            IsReady(); //rigidbody needs to be active even when it is not visible
        }

    }

    void RigidBody::CreateTriangleMesh()
    {
        CHECK_ASSERT(triMesh_ == nullptr, __FILE__, __LINE__);
        PSceneNode sceneNode(sceneNode_.lock());
        PMesh pMesh = sceneNode->GetMesh();
        const VertexsData& vertexData = pMesh->GetVertexsData();
        const Indexes& indices = pMesh->GetIndexes(true);
        triMesh_ = new btTriangleMesh();
        size_t index_count = indices.size();
        CHECK_ASSERT(index_count % 3 == 0, __FILE__, __LINE__);
        for (size_t i = 0; i < index_count; i += 3)
        {
            int i0 = indices[i];
            int i1 = indices[i + 1];
            int i2 = indices[i + 2];

            triMesh_->addTriangle(
                btVector3(vertexData[i0].position_.x, vertexData[i0].position_.y, vertexData[i0].position_.z),
                btVector3(vertexData[i1].position_.x, vertexData[i1].position_.y, vertexData[i1].position_.z),
                btVector3(vertexData[i2].position_.x, vertexData[i2].position_.y, vertexData[i2].position_.z));
        }

        CHECK_ASSERT(triMesh_->getNumTriangles() > 0, __FILE__, __LINE__);
    }

    btConvexHullShape* RigidBody::GetConvexHullTriangleMesh() const
    {
        PSceneNode sceneNode(sceneNode_.lock());
        PMesh pMesh = sceneNode->GetMesh();
        const VertexsData& vertexData = pMesh->GetVertexsData();

        btConvexHullShape* shape = nullptr;

        if (vertexData.size())
        {
            // Build the convex hull from the raw geometry
            StanHull::HullDesc desc;
            desc.SetHullFlag(StanHull::QF_TRIANGLES);
            //desc.SetHullFlag(StanHull::QF_REVERSE_ORDER);
            desc.mVcount = (unsigned int)vertexData.size();
            desc.mVertices = &(vertexData[0].position_[0]);
            desc.mVertexStride = sizeof(VertexData);
            desc.mSkinWidth = 0.0f;

            StanHull::HullLibrary lib;
            StanHull::HullResult result;
            lib.CreateConvexHull(desc, result);

            CHECK_ASSERT(result.mNumIndices % 3 == 0, __FILE__, __LINE__);

            unsigned vertexCount = result.mNumOutputVertices;
            Vector3* vertexData = new Vector3[vertexCount];
            memcpy(vertexData, result.mOutputVertices, vertexCount * sizeof(Vector3));
            shape = new btConvexHullShape((btScalar*)vertexData, vertexCount, sizeof(Vector3));
            delete[] vertexData;
            lib.ReleaseResult(result);
        }

        return shape;
    }


    void RigidBody::CreateShape()
    {
        PSceneNode sceneNode(sceneNode_.lock());
        Vector3 globalScale = sceneNode->GetGlobalScale();
        const BoundingBox& meshBB = sceneNode->GetMesh()->GetBB();
        Vector3 halfSize(meshBB.Size() / 2.0f);

        switch (phyShape_)
        {
            case SH_SPHERE:
                shape_ = new btSphereShape(std::max(halfSize.x, std::max(halfSize.y, halfSize.z)));
                break;

            case SH_BOX:
                shape_ = new btBoxShape(ToBtVector3(halfSize));
                break;

            case SH_CONE:
                shape_ = new btConeShapeZ(std::max(halfSize.x, halfSize.y), 2.f * halfSize.z);
                break;

            case SH_CYLINDER:
                shape_ = new btCylinderShapeZ(ToBtVector3(halfSize));
                break;

            case SH_CAPSULE:
                {
                    float c_radius = std::max(halfSize.x, halfSize.y);
                    shape_ = new btCapsuleShapeZ(c_radius - 0.05f, (halfSize.z - c_radius - 0.05f) * 2);
                    break;
                }

            case SH_CONVEX_TRIMESH:
                shape_ = GetConvexHullTriangleMesh();
                break;

            case SH_TRIMESH:
                {
                    CHECK_ASSERT(IsStatic(), __FILE__, __LINE__);
                    CreateTriangleMesh();
                    shape_ = new btBvhTriangleMeshShape(triMesh_, true);
                    break;
                }

            case SH_UNKNOWN:
            default:
                CHECK_ASSERT(false, __FILE__, __LINE__);
                break;
        }

        CHECK_ASSERT(shape_, __FILE__, __LINE__);
        shape_->setLocalScaling(ToBtVector3(globalScale));
        shape_->setMargin(margin_);
    }

    void RigidBody::getWorldTransform(btTransform& worldTrans) const
    {
        PSceneNode sceneNode(sceneNode_.lock());
        worldTrans = ToTransform(sceneNode->GetGlobalPosition(), sceneNode->GetGlobalOrientation());
    }

    void RigidBody::setWorldTransform(const btTransform& worldTrans)
    {
        PSceneNode sceneNode(sceneNode_.lock());
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
        if (!IsStatic() && IsReady())
        {
            if (lv != VECTOR3_ZERO)
                Activate();
            body_->setLinearVelocity(ToBtVector3(lv));
        }
    }

    void RigidBody::SetAngularVelocity(const Vector3& av)
    {
        if (!IsStatic() && IsReady())
        {
            if (av != VECTOR3_ZERO)
                Activate();
            body_->setAngularVelocity(ToBtVector3(av));
        }
    }

    Vector3 RigidBody::GetLinearVelocity() const
    {
        return ToVector3(body_->getLinearVelocity());
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

    void RigidBody::ReScale()
    {
        delete shape_;
        CreateShape();
        btVector3 inertia;
        shape_->calculateLocalInertia(mass_, inertia);
        body_->setMassProps(mass_, inertia);
        body_->updateInertiaTensor();
        body_->setCollisionShape(shape_);
    }

    void RigidBody::Load(const pugi::xml_node& node)
    {
        mass_ = node.attribute("mass").as_float();
        phyShape_ = (PhysicsShape)node.attribute("phyShape").as_int();
        handleCollision_ = node.attribute("handleCollision").as_bool();
        restitution_ = node.attribute("restitution").as_float();
        friction_ = node.attribute("friction").as_float();
        linearDamp_ = node.attribute("linearDamp").as_float();
        angularDamp_ = node.attribute("angularDamp").as_float();
        margin_ = node.attribute("margin").as_float();
        Invalidate(true);
        IsReady(); //rigidbody needs to be active even when it is not visible
    }

    void RigidBody::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("RigidBody");

        child.append_attribute("mass").set_value(mass_);
        child.append_attribute("phyShape").set_value((int)phyShape_);
        child.append_attribute("handleCollision").set_value(handleCollision_);
        child.append_attribute("restitution").set_value(restitution_);
        child.append_attribute("friction").set_value(friction_);
        child.append_attribute("linearDamp").set_value(linearDamp_);
        child.append_attribute("angularDamp").set_value(angularDamp_);
        child.append_attribute("margin").set_value(margin_);
    }

    void RigidBody::SetCollisionMask(int group, int mask)
    {
        if (collisionGroup_ != group || collisionMask_ != mask)
        {
            collisionGroup_ = group;
            collisionMask_ = mask;
            Invalidate(true);
            IsReady(); //rigidbody needs to be active even when it is not visible
        }
    }

    void RigidBody::Activate()
    {
        if (!IsStatic() && IsReady())
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
    }

    void RigidBody::AddToWorld()
    {
        if (!inWorld_)
        {
            auto world = owner_.lock();
            CHECK_ASSERT(world, __FILE__, __LINE__);
            CHECK_ASSERT(body_, __FILE__, __LINE__);
            world->addRigidBody(body_, (int)collisionGroup_, (int)collisionMask_);
            inWorld_ = true;
        }
    }

    void RigidBody::RemoveFromWorld()
    {
        if (inWorld_)
        {
            auto world = owner_.lock();
            if (world)
            {
                CHECK_ASSERT(body_, __FILE__, __LINE__);
                world->removeRigidBody(body_);
                inWorld_ = false;
            }
        }
    }

}

