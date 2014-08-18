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
#include "AppStatistics.h"

namespace NSG
{
    Mesh::Mesh(bool dynamic)
        : pVBuffer_(nullptr),
          pIBuffer_(nullptr),
          bb_(Vertex3(0)),
          isStatic_(!dynamic)
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
            bb_.Merge(vertex.position_);
    }

    void Mesh::ReleaseResources()
    {
        pVBuffer_ = nullptr;
        pIBuffer_ = nullptr;
        bufferVertexData_ = nullptr;
        bufferIndexData_ = nullptr;
        //vertexsData_.clear();
        //indexes_.clear();
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
        SetBuffersAndAttributes(program);

        GLenum mode = solid ? GetSolidDrawMode() : GetWireFrameDrawMode();

        if (!indexes_.empty())
        {
            const GLvoid* offset = reinterpret_cast<const GLvoid*>(bufferIndexData_->offset_);

            glDrawElements(mode, indexes_.size(), GL_UNSIGNED_SHORT, offset);

            if (AppStatistics::this_)
            {
                CHECK_ASSERT(GetSolidDrawMode() == GL_TRIANGLES && indexes_.size() % 3 == 0, __FILE__, __LINE__);
                AppStatistics::this_->NewTriangles(indexes_.size() / 3);
            }
        }
        else
        {
            GLint first = bufferVertexData_->offset_ / sizeof(VertexData);

            glDrawArrays(mode, first, vertexsData_.size());

            if (AppStatistics::this_ && solid)
            {
                CHECK_ASSERT(GetSolidDrawMode() != GL_TRIANGLES || vertexsData_.size() % 3 == 0, __FILE__, __LINE__);
                AppStatistics::this_->NewTriangles(vertexsData_.size() / 3);
            }
        }

        Graphics::this_->SetVertexArrayObj(nullptr);

    }
}