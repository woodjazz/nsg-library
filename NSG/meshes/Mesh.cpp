/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 N�stor Silveira Gorski

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
#include "Context.h"
#include "BufferManager.h"
#include "VertexArrayObj.h"
#include "InstanceBuffer.h"
#include "AppStatistics.h"
#include "Util.h"
#include "Camera.h"
#include "InstanceData.h"
#include "pugixml.hpp"

namespace NSG
{
    Mesh::Mesh(const std::string& name, bool dynamic)
        : bufferVertexData_(nullptr),
          bufferIndexData_(nullptr),
          boundingSphereRadius_(0),
          isStatic_(!dynamic),
          name_(name),
          graphics_(*Graphics::this_),
          areTangentsCalculated_(false),
          serializable_(true)
    {
    }

    Mesh::~Mesh()
    {
        Context::RemoveObject(this);
    }

    bool Mesh::IsValid()
    {
        if (resource_)
            return resource_->IsLoaded();
        else
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
        pVBuffer_ = Context::this_->bufferManager_->GetStaticVertexBuffer(Buffer::MAX_BUFFER_SIZE, bytesNeeded, vertexsData_);
        bufferVertexData_ = pVBuffer_->GetLastAllocation();

        if (!indexes_.empty())
        {
            GLintptr indexBase = bufferVertexData_->offset_ / sizeof(VertexData);
            Indexes tmpIndexes(indexes_);
            if (indexBase)
                std::for_each(tmpIndexes.begin(), tmpIndexes.end(), [&](IndexType & x)
            {
                x += indexBase;
                CHECK_ASSERT(x < MAX_INDEX_VALUE, __FILE__, __LINE__);
            });

            GLsizeiptr bytesNeeded = sizeof(IndexType) * tmpIndexes.size();
            pIBuffer_ = Context::this_->bufferManager_->GetStaticIndexBuffer(Buffer::MAX_BUFFER_SIZE, bytesNeeded, tmpIndexes);
            bufferIndexData_ = pIBuffer_->GetLastAllocation();
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
        bb_.Define(Vertex3(0));
        boundingSphereRadius_ = 0;
        pVBuffer_ = nullptr;
        pIBuffer_ = nullptr;
        bufferVertexData_ = nullptr;
        bufferIndexData_ = nullptr;
        vertexsData_.clear();
        indexes_.clear();
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
                    ss << obj.uv_;
                    vertexData.append_attribute("uv") = ss.str().c_str();
                }

                {
                    std::stringstream ss;
                    ss << obj.color_;
                    vertexData.append_attribute("color") = ss.str().c_str();
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
                obj.uv_ = GetVertex2(vertexNode.attribute("uv").as_string());
                obj.color_ =  GetVertex4(vertexNode.attribute("color").as_string());
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

            float deltaU1 = v1.uv_.x - v0.uv_.x;
            float deltaV1 = v1.uv_.y - v0.uv_.y;
            float deltaU2 = v2.uv_.x - v0.uv_.x;
            float deltaV2 = v2.uv_.y - v0.uv_.y;

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
}