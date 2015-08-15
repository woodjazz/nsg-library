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
#include "WeakFactory.h"
#include "BoundingBox.h"
#include "btBulletDynamicsCommon.h"
using namespace std;

class btTriangleMesh;
class btCollisionShape;
namespace NSG
{
    struct ShapeKey : public std::string
    {
		ShapeKey(const std::string& key);
        ShapeKey(PMesh mesh, const Vector3& scale);
        ShapeKey(PhysicsShape type, const Vector3& scale);
		void GetData(PMesh& mesh, Vector3& scale, PhysicsShape& type) const;
    };

    class Shape : public Object, public WeakFactory<std::string, Shape>
    {
    public:
        Shape(const std::string& name);
        ~Shape();
        void SetMargin(float margin);
		void SetBB(const BoundingBox& bb);
        PhysicsShape GetType() const { return type_; }
        std::shared_ptr<btCollisionShape> GetCollisionShape() const { return shape_; }
        void Load(const pugi::xml_node& node) override;
        void Save(pugi::xml_node& node);
		static void SaveShapes(pugi::xml_node& node);
		const Vector3& GetScale() const { return scale_; }
		PMesh GetMesh() const { return mesh_.lock(); }
    private:
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        void CreateTriangleMesh();
        std::shared_ptr<btConvexHullShape> GetConvexHullTriangleMesh() const;

        PWeakMesh mesh_;
		BoundingBox bb_;
        std::shared_ptr<btCollisionShape> shape_;
        std::shared_ptr<btTriangleMesh> triMesh_;
        PhysicsShape type_;
        float margin_;
        Vector3 scale_;
        SignalEmpty::PSlot slotReleased_;
    };
}


