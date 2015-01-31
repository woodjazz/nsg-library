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
#include "Mesh.h"
#include "Log.h"
#include "Check.h"
#include "App.h"
#include "Graphics.h"
#include "VertexArrayObj.h"
#include "InstanceBuffer.h"
#include "Util.h"
#include "Camera.h"
#include "InstanceData.h"
#include "pugixml.hpp"
#include <sstream>

namespace NSG
{
    Mesh::Mesh(const std::string& name, bool dynamic)
        : boundingSphereRadius_(0),
          isStatic_(!dynamic),
          name_(name),
          graphics_(*Graphics::this_),
          areTangentsCalculated_(false),
          serializable_(true)
    {
		if (name_.empty())
			name_ = GetUniqueName("Mesh");
    }

    Mesh::~Mesh()
    {
		Invalidate();
    }

	void Mesh::SetDynamic(bool dynamic)
	{
		isStatic_ = !dynamic;
		Invalidate();
	}

    bool Mesh::IsValid()
    {
        return !vertexsData_.empty();
    }

    void Mesh::AllocateResources()
    {
        if (!areTangentsCalculated_)
            CalculateTangents();

        CHECK_GL_STATUS(__FILE__, __LINE__);

        CHECK_ASSERT(pVBuffer_ == nullptr, __FILE__, __LINE__);
        CHECK_ASSERT(pIBuffer_ == nullptr, __FILE__, __LINE__);

        CHECK_ASSERT(!vertexsData_.empty(), __FILE__, __LINE__);
        CHECK_ASSERT(GetSolidDrawMode() != GL_TRIANGLES || indexes_.size() % 3 == 0, __FILE__, __LINE__);

        GLsizeiptr bytesNeeded = sizeof(VertexData) * vertexsData_.size();

        if (isStatic_)
            pVBuffer_ = PVertexBuffer(new VertexBuffer(bytesNeeded, bytesNeeded, vertexsData_, GL_STATIC_DRAW));
        else
            pVBuffer_ = PVertexBuffer(new VertexBuffer(bytesNeeded, bytesNeeded, vertexsData_, GL_DYNAMIC_DRAW));

        if (!indexes_.empty())
        {
            GLsizeiptr bytesNeeded = sizeof(IndexType) * indexes_.size();
            if (isStatic_)
                pIBuffer_ = PIndexBuffer(new IndexBuffer(bytesNeeded, bytesNeeded, indexes_, GL_STATIC_DRAW));
            else
                pIBuffer_ = PIndexBuffer(new IndexBuffer(bytesNeeded, bytesNeeded, indexes_, GL_DYNAMIC_DRAW));
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
        pVBuffer_ = nullptr;
        pIBuffer_ = nullptr;
        areTangentsCalculated_ = false;
    }

    void Mesh::Save(pugi::xml_node& node)
    {
        if(!serializable_)
            return;
        
        pugi::xml_node child = node.append_child("Mesh");

        {
            std::stringstream ss;
            ss << name_;
            child.append_attribute("name") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << GetWireFrameDrawMode();
            child.append_attribute("wireFrameDrawMode") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << GetSolidDrawMode();
            child.append_attribute("solidDrawMode") = ss.str().c_str();
        }
        {
            pugi::xml_node vertexes = child.append_child("Vertexes");

            for (auto& obj : vertexsData_)
            {
                pugi::xml_node vertexData = vertexes.append_child("VertexData");

                {
                    std::stringstream ss;
                    ss << obj.position_;
                    vertexData.append_attribute("position") = ss.str().c_str();
                }

                {
                    std::stringstream ss;
                    ss << obj.normal_;
                    vertexData.append_attribute("normal") = ss.str().c_str();
                }

                {
                    std::stringstream ss;
					ss << obj.uv_[0];
                    vertexData.append_attribute("uv0") = ss.str().c_str();
                }

				{
					std::stringstream ss;
					ss << obj.uv_[1];
					vertexData.append_attribute("uv1") = ss.str().c_str();
				}

                {
                    std::stringstream ss;
                    ss << obj.color_;
                    vertexData.append_attribute("color") = ss.str().c_str();
                }

				/*
				// Do not export tangents since they are calculated
                {
                    std::stringstream ss;
                    ss << obj.tangent_;
                    vertexData.append_attribute("tangent") = ss.str().c_str();
                }
				*/

                {
                    std::stringstream ss;
                    ss << obj.bonesID_;
                    vertexData.append_attribute("bonesID") = ss.str().c_str();
                }

                {
                    std::stringstream ss;
                    ss << obj.bonesWeight_;
                    vertexData.append_attribute("bonesWeight") = ss.str().c_str();
                }
                

            }

            if (indexes_.size())
            {
                CHECK_ASSERT(indexes_.size() % 3 == 0, __FILE__, __LINE__);
                pugi::xml_node indexesNode = child.append_child("Indexes");
                std::stringstream ss;
                for (auto& obj : indexes_)
                    ss << obj << " ";
                indexesNode.append_child(pugi::node_pcdata).set_value(ss.str().c_str());
            }
        }
    }

    void Mesh::Load(const pugi::xml_node& node)
    {
        vertexsData_.clear();
        indexes_.clear();

        name_ = node.attribute("name").as_string();

        pugi::xml_node vertexesNode = node.child("Vertexes");
        if (vertexesNode)
        {
            pugi::xml_node vertexNode = vertexesNode.child("VertexData");
            while (vertexNode)
            {
                VertexData obj;
                obj.position_ = GetVertex3(vertexNode.attribute("position").as_string());
                obj.normal_ = GetVertex3(vertexNode.attribute("normal").as_string());
				obj.uv_[0] = GetVertex2(vertexNode.attribute("uv0").as_string());
				obj.uv_[1] = GetVertex2(vertexNode.attribute("uv1").as_string());
                obj.color_ =  GetVertex4(vertexNode.attribute("color").as_string());
				// Do not import tangents since they are calculated
                //obj.tangent_ =  GetVertex3(vertexNode.attribute("tangent").as_string());
                obj.bonesID_ =  GetVertex4(vertexNode.attribute("bonesID").as_string());
                obj.bonesWeight_ =  GetVertex4(vertexNode.attribute("bonesWeight").as_string());
                vertexsData_.push_back(obj);
                vertexNode = vertexNode.next_sibling("VertexData");
            }

            pugi::xml_node indexesNode = node.child("Indexes");
            if (indexesNode)
            {
                std::string data = indexesNode.child_value();
                std::stringstream ss;
                ss << data;
                int index;
                for (;;)
                {
                    ss >> index;
                    if (ss.eof()) break;
                    indexes_.push_back(index);
                }
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

	void Mesh::SetBlendData(const std::vector<std::vector<unsigned>>& blendIndices, const std::vector<std::vector<float>>& blendWeights)
	{
		CHECK_ASSERT(blendIndices.size() == blendWeights.size(), __FILE__, __LINE__);
		CHECK_ASSERT(vertexsData_.size() == blendWeights.size(), __FILE__, __LINE__);
		unsigned n = vertexsData_.size();
		for (unsigned i = 0; i < n; i++)
		{
			{
				Vector4 bonesID;
				unsigned nBones = blendIndices[i].size();
				CHECK_ASSERT(nBones > 0 && nBones < 5, __FILE__, __LINE__);
				for (unsigned j = 0; j < nBones; j++)
					bonesID[j] = float(blendIndices[i][j]);
				vertexsData_[i].bonesID_ = bonesID;
			}

			{
				Vector4 bonesWeight;
				unsigned nBones = blendWeights[i].size();
				CHECK_ASSERT(nBones > 0 && nBones < 5, __FILE__, __LINE__);
				for (unsigned j = 0; j < nBones; j++)
					bonesWeight[j] = blendWeights[i][j];
				vertexsData_[i].bonesWeight_ = bonesWeight;
			}
		}
	}

	void Mesh::SetBlendData(unsigned vertex, const Vector4& bonesID, Vector4& bonesWeight)
	{
		CHECK_ASSERT(vertexsData_.size() > vertex, __FILE__, __LINE__);
		vertexsData_[vertex].bonesID_ = bonesID;
		vertexsData_[vertex].bonesWeight_ = bonesWeight;
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
		int vidx = vertexsData_.size();
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

        if(calcFaceNormal)
			AverageNormals(vidx, true);
        Invalidate();
    }

    void Mesh::AddTriangle(const VertexData& v0, const VertexData& v1, const VertexData& v2, bool calcFaceNormal)
    {
		int vidx = vertexsData_.size();
        vertexsData_.push_back(v0);
        vertexsData_.push_back(v1);
        vertexsData_.push_back(v2);
		indexes_.push_back(vidx);
		indexes_.push_back(vidx + 1);
		indexes_.push_back(vidx + 2);

        if(calcFaceNormal)
			AverageNormals(vidx, false);

        Invalidate();
    }

    void Mesh::AverageNormals(int vIndexBase, bool isQuad)
    {
		int n = isQuad ? 4 : 3;
		Vector3 normal(0);

		for (int i = 0; i<n; i++)
			normal += vertexsData_[vIndexBase + i].normal_;
		normal /= n;

        for(int i=0; i<n; i++)
			vertexsData_[vIndexBase + i].normal_ = normal;
    }
}