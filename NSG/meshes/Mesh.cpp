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
#include "Mesh.h"
#include "Log.h"
#include "Check.h"
#include "Graphics.h"
#include "VertexArrayObj.h"
#include "InstanceBuffer.h"
#include "Util.h"
#include "Camera.h"
#include "InstanceData.h"
#include "ModelMesh.h"
#include "ResourceXMLNode.h"
#include "Skeleton.h"
#include "pugixml.hpp"
#include <sstream>

namespace NSG
{
	template<> std::map<std::string, PWeakMesh> WeakFactory<std::string, Mesh>::objsMap_ = {};

    Mesh::Mesh(const std::string& name, bool dynamic)
        : Object(name),
          boundingSphereRadius_(0),
          isStatic_(!dynamic),
          areTangentsCalculated_(false),
          serializable_(true)
    {
        if (name_.empty())
            name_ = GetUniqueName("Mesh");
    }

    Mesh::~Mesh()
    {
    }

    void Mesh::SetDynamic(bool dynamic)
    {
        isStatic_ = !dynamic;
        Invalidate();
    }

    bool Mesh::IsValid()
    {
        return Graphics::this_ && !vertexsData_.empty();
    }

    void Mesh::AllocateResources()
    {
        if (!areTangentsCalculated_)
            CalculateTangents();

        CHECK_GL_STATUS(__FILE__, __LINE__);

		CHECK_ASSERT(!isStatic_ || pVBuffer_ == nullptr, __FILE__, __LINE__);
		CHECK_ASSERT(!isStatic_ || pIBuffer_ == nullptr, __FILE__, __LINE__);
		CHECK_ASSERT(!isStatic_ || pIWirefameBuffer_ == nullptr, __FILE__, __LINE__);

        CHECK_ASSERT(!vertexsData_.empty(), __FILE__, __LINE__);
        CHECK_ASSERT(GetSolidDrawMode() != GL_TRIANGLES || indexes_.size() % 3 == 0, __FILE__, __LINE__);

        GLsizeiptr bytesNeeded = sizeof(VertexData) * vertexsData_.size();

        if (isStatic_)
            pVBuffer_ = PVertexBuffer(new VertexBuffer(bytesNeeded, bytesNeeded, vertexsData_, GL_STATIC_DRAW));
		else if (!pVBuffer_)
			pVBuffer_ = PVertexBuffer(new VertexBuffer(bytesNeeded, bytesNeeded, vertexsData_, GL_DYNAMIC_DRAW));
		else
			pVBuffer_->UpdateData(vertexsData_);

        if (!indexes_.empty())
        {
            GLsizeiptr bytesNeeded = sizeof(IndexType) * indexes_.size();
            if (isStatic_)
                pIBuffer_ = std::make_shared<IndexBuffer>(bytesNeeded, bytesNeeded, indexes_, GL_STATIC_DRAW);
			else if (!pIBuffer_)
				pIBuffer_ = std::make_shared<IndexBuffer>(bytesNeeded, bytesNeeded, indexes_, GL_DYNAMIC_DRAW);
			else
				pIBuffer_->UpdateData(indexes_);
        }

        if (!indexesWireframe_.empty())
        {
            GLsizeiptr bytesNeeded = sizeof(IndexType) * indexesWireframe_.size();
            if (isStatic_)
                pIWirefameBuffer_ = std::make_shared<IndexBuffer>(bytesNeeded, bytesNeeded, indexesWireframe_, GL_STATIC_DRAW);
			else if (!pIWirefameBuffer_)
				pIWirefameBuffer_ = std::make_shared<IndexBuffer>(bytesNeeded, bytesNeeded, indexesWireframe_, GL_DYNAMIC_DRAW);
			else
				pIWirefameBuffer_->UpdateData(indexesWireframe_);
		}

        CHECK_GL_STATUS(__FILE__, __LINE__);

        for (auto& vertex : vertexsData_)
        {
            bb_.Merge(vertex.position_);
            boundingSphereRadius_ = std::max(boundingSphereRadius_, glm::length(vertex.position_));
        }
    }

    void Mesh::ReleaseResources()
    {
        bb_ = BoundingBox();
        boundingSphereRadius_ = 0;

        vertexsData_.clear();
        indexes_.clear();

		if (isStatic_)
		{
			pVBuffer_ = nullptr;
			pIBuffer_ = nullptr;
			pIWirefameBuffer_ = nullptr;
		}

        areTangentsCalculated_ = false;

        for (auto& node : sceneNodes_)
            node->OnDirty(); // due text meshes can change with window resize
    }

    void Mesh::Save(pugi::xml_node& node)
    {
        if (!serializable_)
            return;
        pugi::xml_node child = node.append_child("Mesh");
        child.append_attribute("name").set_value(name_.c_str());
        child.append_attribute("wireFrameDrawMode").set_value(GetWireFrameDrawMode());
        child.append_attribute("solidDrawMode").set_value(GetSolidDrawMode());
        for (int i = 0; i < MAX_UVS; i++)
        {
            std::string attName = "uv" + ToString(i) + "Name";
            child.append_attribute(attName.c_str()).set_value(uvNames_[i].c_str());
        }

        pugi::xml_node vertexes = child.append_child("Vertexes");
        for (auto& obj : vertexsData_)
        {
            pugi::xml_node vertexData = vertexes.append_child("VertexData");
            vertexData.append_attribute("position").set_value(ToString(obj.position_).c_str());
            vertexData.append_attribute("normal").set_value(ToString(obj.normal_).c_str());
            vertexData.append_attribute("uv0").set_value(ToString(obj.uv_[0]).c_str());
            vertexData.append_attribute("uv1").set_value(ToString(obj.uv_[1]).c_str());
            vertexData.append_attribute("color").set_value(ToString(obj.color_).c_str());
            // Do not export tangents since they are calculated
            vertexData.append_attribute("bonesID").set_value(ToString(obj.bonesID_).c_str());
            vertexData.append_attribute("bonesWeight").set_value(ToString(obj.bonesWeight_).c_str());
        }
        if (indexes_.size())
        {
            CHECK_ASSERT(indexes_.size() % 3 == 0, __FILE__, __LINE__);
            pugi::xml_node indexesNode = child.append_child("Indexes");
            std::string s;
            for (auto& obj : indexes_)
                s += ToString(obj) + " ";
            indexesNode.append_child(pugi::node_pcdata).set_value(s.c_str());
        }
    }

    void Mesh::Load(const pugi::xml_node& node)
    {
        vertexsData_.clear();
        indexes_.clear();

        name_ = node.attribute("name").as_string();

        for (int i = 0; i < MAX_UVS; i++)
        {
            std::string attName = "uv" + ToString(i) + "Name";
            uvNames_[i] = node.attribute(attName.c_str()).as_string();
        }

        pugi::xml_node vertexesNode = node.child("Vertexes");
        if (vertexesNode)
        {
            pugi::xml_node vertexNode = vertexesNode.child("VertexData");
            while (vertexNode)
            {
                VertexData obj;
                obj.position_ = ToVertex3(vertexNode.attribute("position").as_string());
                obj.normal_ = ToVertex3(vertexNode.attribute("normal").as_string());
                obj.uv_[0] = ToVertex2(vertexNode.attribute("uv0").as_string());
                obj.uv_[1] = ToVertex2(vertexNode.attribute("uv1").as_string());
                obj.color_ =  ToVertex4(vertexNode.attribute("color").as_string());
                // Do not import tangents since they are calculated
                //obj.tangent_ =  ToVertex3(vertexNode.attribute("tangent").as_string());
                obj.bonesID_ =  ToVertex4(vertexNode.attribute("bonesID").as_string());
                obj.bonesWeight_ =  ToVertex4(vertexNode.attribute("bonesWeight").as_string());
                vertexsData_.push_back(obj);
                vertexNode = vertexNode.next_sibling("VertexData");
            }

            pugi::xml_node indexesNode = node.child("Indexes");
            if (indexesNode)
            {
                std::string data = indexesNode.child_value();
                std::string token;
                for_each(data.begin(), data.end(), [&](char c)
                {
                    if (!isspace(c))
                        token += c;
                    else if (!token.empty())
                    {
                        indexes_.push_back(ToInt(token.c_str()));
                        token.clear();
                    }
                });
            }
        }
    }

    void Mesh::CalculateTangents()
    {
        for (unsigned int i = 0 ; i < indexes_.size() ; i += 3)
        {
            VertexData& v0 = vertexsData_[indexes_[i]];
            VertexData& v1 = vertexsData_[indexes_[i + 1]];
            VertexData& v2 = vertexsData_[indexes_[i + 2]];

            Vector3 edge1 = v1.position_ - v0.position_;
            Vector3 edge2 = v2.position_ - v0.position_;

            float deltaU1 = v1.uv_[0].x - v0.uv_[0].x;
            float deltaV1 = v1.uv_[0].y - v0.uv_[0].y;
            float deltaU2 = v2.uv_[0].x - v0.uv_[0].x;
            float deltaV2 = v2.uv_[0].y - v0.uv_[0].y;

            float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

            Vector3 tangent;
            Vector3 bitangent;

            tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
            tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
            tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

            bitangent.x = f * (-deltaU2 * edge1.x - deltaU1 * edge2.x);
            bitangent.y = f * (-deltaU2 * edge1.y - deltaU1 * edge2.y);
            bitangent.z = f * (-deltaU2 * edge1.z - deltaU1 * edge2.z);

            v0.tangent_ += tangent;
            v1.tangent_ += tangent;
            v2.tangent_ += tangent;
        }

        for (unsigned int i = 0 ; i < vertexsData_.size() ; i++)
        {
            glm::normalize(vertexsData_[i].tangent_);
        }
    }

    void Mesh::AddSceneNode(SceneNode* node)
    {
        sceneNodes_.insert(node);
    }

    void Mesh::RemoveSceneNode(SceneNode* node)
    {
        sceneNodes_.erase(node);
    }

    void Mesh::AddQuad(const VertexData& v0, const VertexData& v1, const VertexData& v2, const VertexData& v3, bool calcFaceNormal)
    {
		if (vertexsData_.size() + 4 <= std::numeric_limits<IndexType>::max())
		{
			IndexType vidx = (IndexType)vertexsData_.size();

			vertexsData_.push_back(v0);
			vertexsData_.push_back(v1);
			vertexsData_.push_back(v2);
			vertexsData_.push_back(v3);

			indexes_.push_back(vidx);
			indexes_.push_back(vidx + 1);
			indexes_.push_back(vidx + 2);
			indexes_.push_back(vidx);
			indexes_.push_back(vidx + 2);
			indexes_.push_back(vidx + 3);

			if (calcFaceNormal)
				AverageNormals(vidx, true);
			Invalidate();
		}
		else
		{
			LOGW("Too many vertices. Limit is %d", std::numeric_limits<IndexType>::max());
		}
    }

	void Mesh::AddTriangle(const VertexData& v0, const VertexData& v1, const VertexData& v2, bool averageFaceNormal)
    {
        auto idx0 = vertexsData_.size();
		if (idx0 + 2 <= std::numeric_limits<IndexType>::max())
		{
			IndexType vidx = (IndexType)idx0;
			vertexsData_.push_back(v0);
			vertexsData_.push_back(v1);
			vertexsData_.push_back(v2);
			indexes_.push_back(vidx);
			indexes_.push_back(vidx + 1);
			indexes_.push_back(vidx + 2);

			if (averageFaceNormal)
				AverageNormals(vidx, false);

			Invalidate();
		}
		else
		{
			LOGW("Too many vertices. Limit is %d", std::numeric_limits<IndexType>::max());
		}
	}

	void Mesh::AddTriangle(const VertexData& v0, const VertexData& v1, const VertexData& v2, const Vector3& normal)
	{
		auto idx0 = vertexsData_.size();
		if (idx0 + 2 <= std::numeric_limits<IndexType>::max())
		{
			IndexType vidx = (IndexType)idx0;
			vertexsData_.push_back(v0);
			vertexsData_.push_back(v1);
			vertexsData_.push_back(v2);
			indexes_.push_back(vidx);
			indexes_.push_back(vidx + 1);
			indexes_.push_back(vidx + 2);

			vertexsData_[vidx].normal_ = normal;
			vertexsData_[vidx + 1].normal_ = normal;
			vertexsData_[vidx + 2].normal_ = normal;

			Invalidate();
		}
		else
		{
			LOGW("Too many vertices. Limit is %d", std::numeric_limits<IndexType>::max());
		}
	}

    void Mesh::AverageNormals(size_t vIndexBase, bool isQuad)
    {
        size_t n = isQuad ? 4 : 3;
        Vector3 normal(0);

        for (size_t i = 0; i < n; i++)
            normal += vertexsData_[vIndexBase + i].normal_;
        normal /= n;

        for (size_t i = 0; i < n; i++)
            vertexsData_[vIndexBase + i].normal_ = normal;
    }

    const VertexData& Mesh::GetTriangleVertex(size_t triangleIdx, size_t vertexIndex) const
    {
        CHECK_ASSERT(vertexIndex < 3, __FILE__, __LINE__);
        if (GetSolidDrawMode() == GL_TRIANGLES)
        {
            CHECK_ASSERT(!indexes_.empty(), __FILE__, __LINE__);
            CHECK_ASSERT(triangleIdx < indexes_.size() / 3, __FILE__, __LINE__);
            return vertexsData_.at(indexes_.at(triangleIdx * 3 + vertexIndex));
        }
        else
        {
            CHECK_ASSERT(indexes_.empty(), __FILE__, __LINE__);
            CHECK_ASSERT(GetSolidDrawMode() == GL_TRIANGLE_FAN, __FILE__, __LINE__);
            if (vertexIndex == 0)
                return vertexsData_.at(0);
            return vertexsData_.at(triangleIdx + vertexIndex);
        }
    }

    void Mesh::SetSkeleton(PSkeleton skeleton)
    {
        if (skeleton)
        {
            LOGI("Setting skeleton for mesh %s", name_.c_str());
        }

        skeleton_ = skeleton;
    }

    std::vector<PMesh> Mesh::LoadMeshes(PResource resource, const pugi::xml_node& node)
    {
        std::vector<PMesh> result;
        pugi::xml_node meshes = node.child("Meshes");
        if (meshes)
        {
            pugi::xml_node child = meshes.child("Mesh");
            while (child)
            {
                std::string name = child.attribute("name").as_string();
                auto mesh = Mesh::GetOrCreateClass<ModelMesh>(name);
                auto xmlResource = Resource::CreateClass<ResourceXMLNode>(name);
                xmlResource->Set(resource, mesh, "Meshes", name);
                mesh->Set(xmlResource);
                result.push_back(mesh);
                child = child.next_sibling("Mesh");
            }
        }
        return result;
    }

    void Mesh::SaveMeshes(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Meshes");
        auto meshes = Mesh::GetObjs();
        for (auto& obj : meshes)
            obj->Save(child);
    }

    void Mesh::SetMeshData(const VertexsData& vertexsData, const Indexes& indexes)
    {
        if (vertexsData_ != vertexsData || indexes_ != indexes)
        {
            vertexsData_ = vertexsData;
            indexes_ = indexes;
            Invalidate();
        }
    }

    void Mesh::SetUVName(int index, const std::string& name)
    {
        CHECK_CONDITION(index >= 0 && index < MAX_UVS, __FILE__, __LINE__);
        uvNames_[index] = name;
    }

    size_t Mesh::GetMaxPlatformBones(size_t nBones) const
    {
        static const size_t MAX_BONES0 = 64;
        static const size_t MAX_BONES1 = 48;
        static const size_t MAX_BONES2 = 32;
        // set a maximum value per platform to avoid shader variations
        if(nBones <= MAX_BONES2)
            return MAX_BONES2;
        else if(nBones <= MAX_BONES1)
            return MAX_BONES1;
        else if(nBones <= MAX_BONES0)
            return MAX_BONES0;
        return nBones;
    }

	size_t Mesh::FillShaderDefines(std::string& defines) const
    {
        if (skeleton_)
        {
            const std::vector<PWeakNode>& bones = skeleton_->GetBones();
            auto nBones = bones.size();
            if (nBones)
            {
                defines += "MAX_BONES " + ToString(GetMaxPlatformBones(nBones)) + "\n";
                defines += "SKINNED\n";
                return nBones;
            }
        }
        return 0;
    }

	const std::string& Mesh::GetUVName(int index) const 
	{ 
		CHECK_ASSERT(index >= 0 && index < MAX_UVS, __FILE__, __LINE__);
		return uvNames_[index]; 
	}

    int Mesh::GetUVIndex(const std::string& name) const
    {
        for(int i=0; i<MAX_UVS; i++)
            if(uvNames_[i] == name)
                return i;
        LOGE("UV not found!!!");
        return 0;
    }

	const BoundingBox& Mesh::GetBB() const 
	{ 
        CHECK_CONDITION(((Mesh*)this)->Mesh::IsReady(), __FILE__, __LINE__);
		return bb_; 
	}

	float Mesh::GetBoundingSphereRadius() const 
	{ 
		CHECK_CONDITION(((Mesh*)this)->IsReady(), __FILE__, __LINE__);
		return boundingSphereRadius_; 
	}

}