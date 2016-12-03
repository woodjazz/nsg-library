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
#pragma once
#include <vector>
#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Types.h"
#include "Node.h"
#include "Object.h"
#include "Resource.h"
#include "Buffer.h"
#include "BoundingBox.h"
#include "WeakFactory.h"
#include <string>
#include <set>

namespace NSG
{
	class Mesh : public std::enable_shared_from_this<Mesh>, public Object, public WeakFactory<std::string, Mesh>
    {
    public:
        virtual ~Mesh();
		void SetDynamic(bool dynamic);
		bool IsStatic() const { return isStatic_; }
        virtual GLenum GetWireFrameDrawMode() const = 0;
        virtual GLenum GetSolidDrawMode() const = 0;
        virtual size_t GetNumberOfTriangles() const = 0;
		const BoundingBox& GetBB() const;
		float GetBoundingSphereRadius() const;
        VertexBuffer* GetVertexBuffer() const { return pVBuffer_.get(); }
		IndexBuffer* GetIndexBuffer(bool solid) const { return solid ? pIBuffer_.get() : pIWireBuffer_.get(); }
        const VertexsData& GetConstVertexsData() const { return vertexsData_; }
        const Indexes& GetConstIndexes() const { return indexes_; }
		const VertexsData& GetVertexsData() const { return vertexsData_; }
		const Indexes& GetIndexes(bool solid) const { return solid ? indexes_ : indexesWireframe_; }
        void Save(pugi::xml_node& node);
        void SetSerializable(bool serializable) { serializable_ = serializable; }
        bool IsSerializable() const { return serializable_; }
        const std::string& GetName() const { return name_; }
        void SetName(const std::string& name) { name_ = name; }
        void AddSceneNode(SceneNode* node);
        void RemoveSceneNode(SceneNode* node);
        std::set<SceneNode*>& GetSceneNodes() { return sceneNodes_; }
		const std::set<SceneNode*>& GetConstSceneNodes() const { return sceneNodes_; }
        void AddQuad(const VertexData& v0, const VertexData& v1, const VertexData& v2, const VertexData& v3, bool calcFaceNormal);
        void AddTriangle(const VertexData& v0, const VertexData& v1, const VertexData& v2, bool averageFaceNormal);
		void AddTriangle(const VertexData& v0, const VertexData& v1, const VertexData& v2, const Vector3& normal);
		void AverageNormals(size_t indexBase, bool isQuad);
        const VertexData& GetTriangleVertex(size_t triangleIdx, size_t vertexIndex) const;
		static void SaveMeshes(pugi::xml_node& node);
		void SetMeshData(const VertexsData& vertexsData, const Indexes& indexes);
		virtual PhysicsShape GetShapeType() const { return SH_CONVEX_TRIMESH; } // assume rigidbody is dynamic by default 
        void SetUVName(int index, const std::string& name);
		const std::string& GetUVName(int index) const;
        int GetUVIndex(const std::string& name) const;
        bool HasDeformBones() const { return hasDeformBones_; }
	protected:
		void Load(const pugi::xml_node& node) override;
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        void CalculateTangents();
        Mesh(const std::string& name, bool dynamic = false);
    protected:
        VertexsData vertexsData_;
        Indexes indexes_; //for solid object
        Indexes indexesWireframe_; //for wireframe
        PVertexBuffer pVBuffer_;
        PIndexBuffer pIBuffer_; // for solid objects
		PIndexBuffer pIWireBuffer_; // for wireframe
        BoundingBox bb_;
        float boundingSphereRadius_;
        bool isStatic_;
        bool areTangentsCalculated_;
        bool serializable_;
        PSkeleton skeleton_;
        std::set<SceneNode*> sceneNodes_;
        static const int MAX_UVS = 2;
        std::string uvNames_[MAX_UVS];
        bool hasDeformBones_;
    };
}
