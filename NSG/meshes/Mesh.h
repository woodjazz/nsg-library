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
#include <string>
#include <set>

namespace NSG
{
	class Mesh : public Object
    {
    public:
        virtual ~Mesh();
		void SetDynamic(bool dynamic);
        virtual GLenum GetWireFrameDrawMode() const = 0;
        virtual GLenum GetSolidDrawMode() const = 0;
        virtual size_t GetNumberOfTriangles() const = 0;
        const BoundingBox& GetBB() const { return bb_; }
        float GetBoundingSphereRadius() const { return boundingSphereRadius_; }
        VertexBuffer* GetVertexBuffer() const { return pVBuffer_.get(); }
        IndexBuffer* GetIndexBuffer() const { return pIBuffer_.get(); }
        const VertexsData& GetVertexsData() const { return vertexsData_; }
        const Indexes& GetIndexes() const { return indexes_; }
        void Save(pugi::xml_node& node);
        virtual void Load(const pugi::xml_node& node);
        void SetSerializable(bool serializable) { serializable_ = serializable; }
        bool IsSerializable() const { return serializable_; }
        PSkeleton GetSkeleton() const { return skeleton_; }
        void SetSkeleton(PSkeleton skeleton) { skeleton_ = skeleton; }
        const std::string& GetName() const { return name_; }
        void SetName(const std::string& name) { name_ = name; }
		void SetBlendData(const std::vector<std::vector<unsigned>>& blendIndices, const std::vector<std::vector<float>>& blendWeights);
        void AddSceneNode(SceneNode* node);
        void RemoveSceneNode(SceneNode* node);
        std::set<SceneNode*>& GetSceneNodes() { return sceneNodes_; }
		const std::set<SceneNode*>& GetConstSceneNodes() const { return sceneNodes_; }
    protected:
        virtual bool IsValid() override;
        virtual void AllocateResources() override;
        virtual void ReleaseResources() override;
        void CalculateTangents();
        Mesh(const std::string& name, bool dynamic = false);
    protected:
        VertexsData vertexsData_;
        Indexes indexes_;
        PVertexBuffer pVBuffer_;
        PIndexBuffer pIBuffer_;
        PResource resource_;
        BoundingBox bb_;
        float boundingSphereRadius_;
        bool isStatic_;
        std::string name_;
        Graphics& graphics_;
        bool areTangentsCalculated_;
        bool serializable_;
        PSkeleton skeleton_;
        std::set<SceneNode*> sceneNodes_;
    };
}
