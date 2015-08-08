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
#include "Shape.h"
#include "Check.h"
#include "Util.h"
#include "Mesh.h"
#include "BoundingBox.h"
#include "ResourceXMLNode.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "hull.h"
#include "pugixml.hpp"
#include <algorithm>

namespace NSG
{
	template<> std::map<std::string, PWeakShape> WeakFactory<std::string, Shape>::objsMap_ = std::map<std::string, PWeakShape>{};
    
    ShapeKey::ShapeKey(const std::string& key)
        : std::string(key)
    {

    }

    ShapeKey::ShapeKey(PMesh mesh, const Vector3& scale)
        : std::string(ToString(mesh->GetName().size()) + " " + mesh->GetName() + " " + ToString(scale) + " " + ToString(mesh->GetShapeType()))
    {
        LOGI("ShapeKey=%s", c_str());
    }

    ShapeKey::ShapeKey(PhysicsShape type, const Vector3& scale)
        : std::string(ToString(scale) + " " + ToString(type))
    {
    }

    void ShapeKey::GetData(PMesh& mesh, Vector3& scale, PhysicsShape& type) const
    {
        CHECK_ASSERT(size() > 0, __FILE__, __LINE__);
        bool hasMesh = !empty() && at(0) != '[';
        if (hasMesh)
        {
            int size;
            sscanf(c_str(), "%d", &size);
            string meshName;
            meshName.resize(size);
            char typeStr[100];
            std::string formatStr = "%d %" + ToString(size) + "c [%f, %f, %f] %s";
            sscanf(c_str(), formatStr.c_str(), &size, meshName.data(), &scale.x, &scale.y, &scale.z, typeStr);
            mesh = Mesh::Get(meshName);
            CHECK_ASSERT(mesh && "Mesh not found!!!", __FILE__, __LINE__);
            type = ToPhysicsShape(typeStr);
        }
        else
        {
            char typeStr[100];
            sscanf(c_str(), "[%f, %f, %f] %s", &scale.x, &scale.y, &scale.z, typeStr);
            type = ToPhysicsShape(typeStr);
        }
    }

	Shape::Shape(const std::string& name)
        : Object(name),
          type_(SH_EMPTY),
          margin_(.06f),
          scale_(1)
    {
		PMesh mesh;
        ShapeKey(name).GetData(mesh, scale_, type_);
		mesh_ = mesh;
    }

    Shape::~Shape()
    {
    }

    bool Shape::IsValid()
    {
		if (type_ == PhysicsShape::SH_EMPTY)
			return true;

		auto mesh = mesh_.lock();
		if(mesh)
			bb_ = mesh->GetBB(); 

        return (!xmlResource_ || xmlResource_->IsReady()) && bb_.IsDefined();
    }

    void Shape::AllocateResources()
    {
        Vector3 halfSize(bb_.Size() * 0.5f);

        switch (type_)
        {
            case SH_SPHERE:
                {
                    if (!IsScaleUniform(scale_))
                    {
                        btVector3 position(0.f, 0.f, 0.f);
                        btScalar radi = std::max(halfSize.x, std::max(halfSize.y, halfSize.z));
                        //only way to have non-uniform scaling
                        shape_ = std::make_shared<btMultiSphereShape>(&position, &radi, 1);
                    }
                    else
                        shape_ = std::make_shared<btSphereShape>(std::max(halfSize.x, std::max(halfSize.y, halfSize.z)));
                    break;
                }

            case SH_BOX:
                shape_ = std::make_shared<btBoxShape>(ToBtVector3(halfSize));
                break;

            case SH_CONE:
            {
                auto c_radius = std::max(halfSize.x, halfSize.y);
                shape_ = std::make_shared<btConeShapeZ>(c_radius, 2*halfSize.z);
                break;
            }

            case SH_CYLINDER:
                shape_ = std::make_shared<btCylinderShapeZ>(ToBtVector3(halfSize));
                break;

            case SH_CAPSULE:
                {
                    auto c_radius = std::max(halfSize.x, halfSize.y);
                    shape_ = std::make_shared<btCapsuleShapeZ>(c_radius - 0.05f, (halfSize.z - 0.05f) * 2 - c_radius);
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

            case SH_EMPTY:
                shape_ = std::make_shared<btEmptyShape>();
                break;

            default:
                CHECK_ASSERT(false, __FILE__, __LINE__);
                break;
        }

        CHECK_ASSERT(shape_, __FILE__, __LINE__);
        shape_->setLocalScaling(ToBtVector3(scale_));
        shape_->setMargin(margin_);
        shape_->setUserPointer(this);
    }

    void Shape::ReleaseResources()
    {
        shape_->setUserPointer(nullptr);
        shape_ = nullptr;
        triMesh_ = nullptr;
    }

    void Shape::SetBB(const BoundingBox& bb)
    {
        if (!(bb == bb_))
        {
            bb_ = bb;
            Invalidate();
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
        CHECK_CONDITION(mesh->IsReady(), __FILE__, __LINE__);
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
        CHECK_CONDITION(mesh->IsReady(), __FILE__, __LINE__);
        auto& vertexData = mesh->GetVertexsData();

        if (vertexData.size())
        {
            // Build the convex hull from the raw geometry
            StanHull::HullDesc desc;
            desc.SetHullFlag(StanHull::QF_TRIANGLES);
            //desc.SetHullFlag(StanHull::QF_SKIN_WIDTH);
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
            std::unique_ptr<Vector3> data(new Vector3[vertexCount]);
            memcpy(data.get(), result.mOutputVertices, vertexCount * sizeof(Vector3));
            auto shape = std::make_shared<btConvexHullShape>((btScalar*)data.get(), (int)vertexCount, (int)sizeof(Vector3));
            lib.ReleaseResult(result);
            return shape;
        }
        return nullptr;
    }

    void Shape::LoadFrom(PResource resource, const pugi::xml_node& node)
    {
        CHECK_ASSERT(name_ == node.attribute("name").as_string(), __FILE__, __LINE__);
		CHECK_ASSERT(type_ == ToPhysicsShape(node.attribute("type").as_string()), __FILE__, __LINE__);
		CHECK_ASSERT(scale_ == ToVertex3(node.attribute("scale").as_string()), __FILE__, __LINE__);
		margin_ = node.attribute("margin").as_float();
        bb_ = ToBoundigBox(node.attribute("bb").as_string());
        CHECK_ASSERT(bb_.IsDefined(), __FILE__, __LINE__);
		CHECK_ASSERT(!node.attribute("meshName") || mesh_.lock()->GetName() == node.attribute("meshName").as_string(), __FILE__, __LINE__);
        //CHECK_ASSERT((meshNameAtt ? name_ == ShapeKey(mesh_.lock(), scale_) : name_ == ShapeKey(type_, scale_)) && "shape key has changed!!!", __FILE__, __LINE__);
        Invalidate();
    }

    void Shape::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Shape");
        child.append_attribute("name").set_value(name_.c_str());
        auto mesh = mesh_.lock();
        if (mesh)
            child.append_attribute("meshName").set_value(mesh->GetName().c_str());
        child.append_attribute("bb").set_value(ToString(bb_).c_str());
        child.append_attribute("type").set_value(ToString(type_));
        child.append_attribute("margin").set_value(margin_);
        child.append_attribute("scale").set_value(ToString(scale_).c_str());
    }

    std::vector<PShape> Shape::LoadShapes(PResource resource, const pugi::xml_node& node)
    {
        std::vector<PShape> result;
        pugi::xml_node objs = node.child("Shapes");
        if (objs)
        {
            pugi::xml_node child = objs.child("Shape");
            while (child)
            {
                std::string name = child.attribute("name").as_string();
                auto shape(Shape::GetOrCreate(name));
                auto xmlResource = Resource::CreateClass<ResourceXMLNode>(GetUniqueName(name));
                xmlResource->Set(resource, shape, "Shapes", name);
                xmlResource->IsReady(); //force load resources
                shape->Set(xmlResource);
                result.push_back(shape);
                child = child.next_sibling("Shape");
            }
        }
        return result;
    }

    void Shape::SaveShapes(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Shapes");
        auto objs = Shape::GetObjs();
        for (auto& obj : objs)
            obj->Save(child);
    }

    void Shape::Set(PResourceXMLNode xmlResource)
    {
        if (xmlResource != xmlResource_)
        {
            xmlResource_ = xmlResource;
            Invalidate();
        }
    }
}

