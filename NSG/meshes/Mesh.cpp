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
        : pVBuffer_(nullptr),
          pIBuffer_(nullptr),
          bb_(Vertex3(0)),
          isStatic_(!dynamic),
          boundingSphereRadius_(0),
          name_(name),
          graphics_(*Graphics::this_)
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
    }

    void Mesh::SetBuffersAndAttributes(Program* program)
    {
        if (Graphics::this_->HasVertexArrayObject())
        {
            auto it = vaoMap_.find(program);
            if (it != vaoMap_.end())
            {
                Graphics::this_->SetVertexArrayObj(it->second.get());
            }
            else
            {
                VertexArrayObj* vao = new VertexArrayObj(program, this);
                CHECK_CONDITION(vaoMap_.insert(VAOMap::value_type(program, PVertexArrayObj(vao))).second, __FILE__, __LINE__);
                Graphics::this_->SetVertexArrayObj(vao);
            }
        }
        else
        {
            Graphics::this_->SetVertexBuffer(pVBuffer_.get());
            Graphics::this_->SetAttributes(this, program);
            Graphics::this_->SetInstanceAttrPointers(program);
            Graphics::this_->SetIndexBuffer(pIBuffer_.get());
        }
    }

    void Mesh::RedoVAO(Program* program)
    {
        if (Graphics::this_->HasVertexArrayObject())
        {
            auto it = vaoMap_.find(program);
            if (it != vaoMap_.end())
            {
                it->second->Redo();
            }
        }
    }

    void Mesh::Draw(bool solid, Program* program)
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        graphics_.UpdateBatchBuffer();

        SetBuffersAndAttributes(program);
        
        GLenum mode = solid ? GetSolidDrawMode() : GetWireFrameDrawMode();

        if (!indexes_.empty())
        {
            const GLvoid* offset = reinterpret_cast<const GLvoid*>(bufferIndexData_->offset_);

            glDrawElements(mode, indexes_.size(), GL_UNSIGNED_SHORT, offset);

            if (solid)
            {
                AppStatistics::this_->NewTriangles(GetNumberOfTriangles());
            }
        }
        else
        {
            GLint first = bufferVertexData_->offset_ / sizeof(VertexData);

            glDrawArrays(mode, first, vertexsData_.size());

            if (solid)
            {
                AppStatistics::this_->NewTriangles(GetNumberOfTriangles());
            }
        }

        Graphics::this_->SetVertexArrayObj(nullptr);
        
        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    void Mesh::Draw(bool solid, Program* program, Batch& batch)
    {
        graphics_.UpdateBatchBuffer(batch);

        SetBuffersAndAttributes(program);

        GLenum mode = solid ? GetSolidDrawMode() : GetWireFrameDrawMode();

        unsigned instances = batch.nodes_.size();

        if (!indexes_.empty())
        {
            const GLvoid* offset = reinterpret_cast<const GLvoid*>(bufferIndexData_->offset_);

            glDrawElementsInstanced(mode, indexes_.size(), GL_UNSIGNED_SHORT, offset, instances);

            if (solid)
            {
                AppStatistics::this_->NewTriangles(GetNumberOfTriangles() * instances);
            }
        }
        else
        {
            GLint first = bufferVertexData_->offset_ / sizeof(VertexData);

            glDrawArraysInstanced(mode, first, vertexsData_.size(), instances);

            if (solid)
            {
                AppStatistics::this_->NewTriangles(GetNumberOfTriangles() * instances);
            }
        }

        Graphics::this_->SetVertexArrayObj(nullptr);
    }


    void Mesh::Save(pugi::xml_node& node)
    {
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
}