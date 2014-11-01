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
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include <hull.h>
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

    RigidBody::RigidBody()
        : body_(nullptr),
          sceneNode_(nullptr),
          shape_(nullptr),
          mass_(-1),
          phyShape_(SH_UNKNOWN),
          isStatic_(false),
          handleCollision_(false)
    {
        owner_ = App::this_->GetCurrentScene()->GetPhysicsWorld()->GetWorld();
    }

    RigidBody::~RigidBody()
    {
        Context::RemoveObject(this);
    }

    bool RigidBody::IsValid()
    {
        return mass_ != -1 && phyShape_ != SH_UNKNOWN && sceneNode_ && sceneNode_->IsReady();
    }

    void RigidBody::AllocateResources()
    {
        CreateShape();

        if (!isStatic_)
        {
            btVector3 inertia;
            shape_->calculateLocalInertia(mass_, inertia);
            body_ = new btRigidBody(mass_, this, shape_, inertia);
        }
        else
        {
            body_ = new btRigidBody(0.f, 0, shape_, btVector3(0, 0, 0));
            body_->setDamping(0.f, 0.f);
            body_->setAngularFactor(0.f);
        }

        body_->setUserPointer(this);
        body_->setWorldTransform(ToTransform(sceneNode_->GetGlobalPosition(), sceneNode_->GetGlobalOrientation()));
        owner_->addRigidBody(body_);
    }

    void RigidBody::ReleaseResources()
    {
        body_->setUserPointer(nullptr);
        body_->setMotionState(nullptr);
        owner_->removeRigidBody(body_);
        delete shape_;
        delete body_;
    }

    void RigidBody::SetSceneNode(SceneNode* sceneNode)
    {
        if (sceneNode_ != sceneNode)
        {
            sceneNode_ = sceneNode;
            Invalidate();
        }
    }

    void RigidBody::SetMass(float mass)
    {
        if (mass_ != mass)
        {
            mass_ = mass;
            Invalidate();
        }
    }

    void RigidBody::SetShape(PhysicsShape phyShape, bool isStatic)
    {
        if (phyShape_ != phyShape || isStatic_ != isStatic)
        {
            phyShape_ = phyShape;
            isStatic_ = isStatic;
            Invalidate();
        }
    }

    btTriangleMesh* RigidBody::GetTriangleMesh() const
    {
        PMesh pMesh = sceneNode_->GetMesh();
        const VertexsData& vertexData = pMesh->GetVertexsData();
        const Indexes& indices = pMesh->GetIndexes();
        btTriangleMesh* triMesh = new btTriangleMesh();
        unsigned index_count = indices.size();
        CHECK_ASSERT(index_count % 3 == 0, __FILE__, __LINE__);
        for (size_t i = 0; i < index_count; i += 3)
        {
            int i0 = indices[i];
            int i1 = indices[i + 1];
            int i2 = indices[i + 2];

            triMesh->addTriangle(
                btVector3(vertexData[i0].position_.x, vertexData[i0].position_.y, vertexData[i0].position_.z),
                btVector3(vertexData[i1].position_.x, vertexData[i1].position_.y, vertexData[i1].position_.z),
                btVector3(vertexData[i2].position_.x, vertexData[i2].position_.y, vertexData[i2].position_.z));
        }

        CHECK_ASSERT(triMesh->getNumTriangles() > 0, __FILE__, __LINE__);

        return triMesh;
    }

	btConvexHullShape* RigidBody::GetConvexHullTriangleMesh() const
    {
        PMesh pMesh = sceneNode_->GetMesh();
        const VertexsData& vertexData = pMesh->GetVertexsData();

		btConvexHullShape* shape = nullptr;

		if (vertexData.size())
        {
            // Build the convex hull from the raw geometry
            StanHull::HullDesc desc;
			desc.SetHullFlag(StanHull::QF_TRIANGLES);
			//desc.SetHullFlag(StanHull::QF_REVERSE_ORDER);
            desc.mVcount = vertexData.size();
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
        Vector3 globalScale = sceneNode_->GetGlobalScale();
        const BoundingBox& meshBB = sceneNode_->GetMesh()->GetBB();
        Vector3 halfSize(meshBB.Size() / 2.0f);

        switch (phyShape_)
        {
            case SH_SPHERE:
                shape_ = new btSphereShape(std::max(halfSize.x, std::max(halfSize.y, halfSize.z)));
                shape_->setLocalScaling(ToBtVector3(globalScale));
                break;

            case SH_BOX:
                shape_ = new btBoxShape(btVector3(halfSize.x, halfSize.y, halfSize.z));
                shape_->setLocalScaling(ToBtVector3(globalScale));
                break;

            case SH_CONE:
                shape_ = new btConeShapeZ(std::max(halfSize.x, halfSize.y), 2.f * halfSize.z);
                shape_->setLocalScaling(ToBtVector3(globalScale));
                break;

            case SH_CYLINDER:
                shape_ = new btCylinderShapeZ(btVector3(halfSize.x, halfSize.y, halfSize.z));
                shape_->setLocalScaling(ToBtVector3(globalScale));
                break;

            case SH_CONVEX_TRIMESH:
                shape_ = GetConvexHullTriangleMesh();
				shape_->setLocalScaling(ToBtVector3(globalScale));// *halfSize * 2.f));
                break;

            case SH_TRIMESH:
                CHECK_ASSERT(isStatic_, __FILE__, __LINE__);
                shape_ = new btBvhTriangleMeshShape(GetTriangleMesh(), true);
				shape_->setLocalScaling(ToBtVector3(globalScale));// *halfSize * 2.f));
                break;

            case SH_UNKNOWN:
            default:
                CHECK_ASSERT(false, __FILE__, __LINE__);
                break;
        }

        CHECK_ASSERT(shape_, __FILE__, __LINE__);
        //const float MARGIN = 0.1f;
        //shape_->setMargin(MARGIN);
    }

    void RigidBody::getWorldTransform(btTransform& worldTrans) const
    {
        worldTrans.setIdentity();
        const Quaternion& rot = sceneNode_->GetGlobalOrientation();
        const Vector3& loc = sceneNode_->GetGlobalPosition();
        worldTrans.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
        worldTrans.setOrigin(btVector3(loc.x, loc.y, loc.z));
    }

    void RigidBody::setWorldTransform(const btTransform& worldTrans)
    {
        btQuaternion r = worldTrans.getRotation();
        Quaternion rot(r.w(), r.x(), r.y(), r.z());
        const btVector3& pos = worldTrans.getOrigin();
        Vector3 loc(pos.getX(), pos.getY(), pos.getZ());
        sceneNode_->SetGlobalOrientation(rot);
        sceneNode_->SetGlobalPosition(loc);
    }

    void RigidBody::SetLinearVelocity(const Vector3& lv, TransformSpace tspace)
    {
        // only dynamic bodies
        if (!isStatic_)
        {
            body_->activate();
            Vector3 vel;
            switch (tspace)
            {
                case TS_LOCAL:
                    {
                        btTransform trans = body_->getCenterOfMassTransform();
                        btQuaternion quat = trans.getRotation();
                        vel = Quaternion(quat.w(), quat.x(), quat.y(), quat.z())  * lv;
                    }
                    break;
                case TS_WORLD:
                case TS_PARENT:
                default:
                    vel = lv;
                    break;
            }

            btVector3 lf = body_->getLinearFactor();
            body_->setLinearVelocity(btVector3(vel.x, vel.y, vel.z) * lf);
        }
    }

    Vector3 RigidBody::GetLinearVelocity() const
    {
        const btVector3& nvel = body_->getLinearVelocity();
        return Vector3(nvel.getX(), nvel.getY(), nvel.getZ());
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

                cinf.collider_ = collider->sceneNode_;
                cinf.appliedImpulse_ = pt.m_appliedImpulse;
                cinf.appliedImpulseLateral1_ = pt.m_appliedImpulseLateral1;
                cinf.appliedImpulseLateral2_ = pt.m_appliedImpulseLateral2;
                cinf.contactMotion1_ = pt.m_contactMotion1;
                cinf.contactMotion2_ = pt.m_contactMotion2;
                cinf.contactCFM1_ = pt.m_contactCFM1;
                cinf.contactCFM2_ = pt.m_contactCFM2;
                cinf.lateralFrictionDir1_ = Vector3(pt.m_lateralFrictionDir1.getX(), pt.m_lateralFrictionDir1.getY(), pt.m_lateralFrictionDir1.getZ());
                cinf.lateralFrictionDir2_ = Vector3(pt.m_lateralFrictionDir2.getX(), pt.m_lateralFrictionDir2.getY(), pt.m_lateralFrictionDir2.getZ());
                sceneNode_->OnCollision(cinf);
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
}