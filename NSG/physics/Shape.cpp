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
#include "Shape.h"
#include "Check.h"
#include "Util.h"
#include "Mesh.h"
#include "BoundingBox.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "hull.h"
#include "pugixml.hpp"
#include <algorithm>

namespace NSG
{
    Shape::Shape(const std::string& name)
        : Object(name + "Shape"),
          type_(SH_UNKNOWN),
          margin_(.06f),
          scale_(1)
    {
    }

    Shape::~Shape()
    {
    }

    bool Shape::IsValid()
    {
        auto mesh = mesh_.lock();
        return type_ != SH_UNKNOWN && mesh && mesh->IsReady();
    }

    void Shape::AllocateResources()
    {
        const BoundingBox& meshBB = mesh_.lock()->GetBB();
        Vector3 halfSize(meshBB.Size() / 2.0f);

        switch (type_)
        {
            case SH_SPHERE:
                shape_ = std::make_shared<btSphereShape>(std::max(halfSize.x, std::max(halfSize.y, halfSize.z)));
                break;

            case SH_BOX:
                shape_ = std::make_shared<btBoxShape>(ToBtVector3(halfSize));
                break;

            case SH_CONE:
                shape_ = std::make_shared<btConeShapeZ>(std::max(halfSize.x, halfSize.y), 2.f * halfSize.z);
                break;

            case SH_CYLINDER:
                shape_ = std::make_shared<btCylinderShapeZ>(ToBtVector3(halfSize));
                break;

            case SH_CAPSULE:
                {
                    float c_radius = std::max(halfSize.x, halfSize.y);
                    shape_ = std::make_shared<btCapsuleShapeZ>(c_radius - 0.05f, (halfSize.z - c_radius - 0.05f) * 2);
                    break;
                }

            case SH_CONVEX_TRIMESH:
                shape_ = GetConvexHullTriangleMesh();
                break;

            case SH_TRIMESH:
                {
                    CreateTriangleMesh();
                    shape_ = std::make_shared<btBvhTriangleMeshShape>(triMesh_.get(), true);
                    break;
                }

            case SH_UNKNOWN:
            default:
                CHECK_ASSERT(false, __FILE__, __LINE__);
                break;
        }

        CHECK_ASSERT(shape_, __FILE__, __LINE__);
        shape_->setLocalScaling(ToBtVector3(scale_));
        shape_->setMargin(margin_);
    }

    void Shape::ReleaseResources()
    {
        shape_ = nullptr;
        triMesh_ = nullptr;
    }

    void Shape::SetType(PhysicsShape type)
    {
        if (type_ != type)
        {
            type_ = type;
            Invalidate();
        }
    }

    void Shape::SetScale(const Vector3& scale)
    {
        if (scale != scale_)
        {
            scale_ = scale;
            if (shape_)
                shape_->setLocalScaling(ToBtVector3(scale));
        }
    }

    void Shape::SetMesh(PMesh mesh)
    {
        if (mesh_.lock() != mesh)
        {
            mesh_ = mesh;
            Invalidate();
            if (mesh)
            {
                slotReleased_ = mesh->signalReleased_->Connect([this]()
                {
                    Invalidate();
                });
            }
            else
                slotReleased_ = nullptr;
        }
    }

    void Shape::SetMargin(float margin)
    {
        if (margin != margin_)
        {
            margin_ = margin;
            if (shape_)
                shape_->setMargin(margin_);
        }
    }

    void Shape::CreateTriangleMesh()
    {
        auto mesh = mesh_.lock();
        auto vertexData = mesh->GetVertexsData();
        auto indices = mesh->GetIndexes(true);
        triMesh_ = std::make_shared<btTriangleMesh>();
        auto index_count = indices.size();
        CHECK_ASSERT(index_count % 3 == 0, __FILE__, __LINE__);
        for (auto i = 0; i < index_count; i += 3)
        {
            auto i0 = indices[i];
            auto i1 = indices[i + 1];
            auto i2 = indices[i + 2];

            triMesh_->addTriangle(
                btVector3(vertexData[i0].position_.x, vertexData[i0].position_.y, vertexData[i0].position_.z),
                btVector3(vertexData[i1].position_.x, vertexData[i1].position_.y, vertexData[i1].position_.z),
                btVector3(vertexData[i2].position_.x, vertexData[i2].position_.y, vertexData[i2].position_.z));
        }

        CHECK_ASSERT(triMesh_->getNumTriangles() > 0, __FILE__, __LINE__);
    }

    std::shared_ptr<btConvexHullShape> Shape::GetConvexHullTriangleMesh() const
    {
        auto mesh = mesh_.lock();
        auto vertexData = mesh->GetVertexsData();

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
            std::unique_ptr<Vector3> vertexData(new Vector3[vertexCount]);
            memcpy(vertexData.get(), result.mOutputVertices, vertexCount * sizeof(Vector3));
            auto shape = std::make_shared<btConvexHullShape>((btScalar*)vertexData.get(), (int)vertexCount, (int)sizeof(Vector3));
            lib.ReleaseResult(result);
            return shape;
        }
        return nullptr;
    }

    void Shape::Load(const pugi::xml_node& node)
    {
        type_ = (PhysicsShape)node.attribute("type").as_int();
        margin_ = node.attribute("margin").as_float();
        scale_ = GetVertex3(node.attribute("scale").as_string());
        Invalidate();
    }

    void Shape::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Shape");
        child.append_attribute("type").set_value((int)type_);
        child.append_attribute("margin").set_value(margin_);
        {
            std::stringstream ss;
            ss << scale_;
            child.append_attribute("scale") = ss.str().c_str();
        }
    }
}

