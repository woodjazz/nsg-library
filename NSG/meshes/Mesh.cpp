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

namespace NSG
{
    Mesh::Mesh(GLenum usage)
        : pVBuffer_(nullptr),
          pIBuffer_(nullptr),
          usage_(usage),
          bb_(Vertex3(0))
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
        SetUniformsNeedUpdate();

        CHECK_GL_STATUS(__FILE__, __LINE__);

        CHECK_ASSERT(pVBuffer_ == nullptr, __FILE__, __LINE__);
        CHECK_ASSERT(pIBuffer_ == nullptr, __FILE__, __LINE__);

        CHECK_ASSERT(!vertexsData_.empty(), __FILE__, __LINE__);
        CHECK_ASSERT(GetSolidDrawMode() != GL_TRIANGLES || indexes_.size() % 3 == 0, __FILE__, __LINE__);

        GLsizeiptr bytesNeeded = sizeof(VertexData) * vertexsData_.size();
        //pVBuffer_ = Context::this_->bufferManager_->GetStaticVertexBuffer(Buffer::MAX_BUFFER_SIZE, bytesNeeded, vertexsData_);
        pVBuffer_ = Context::this_->bufferManager_->GetStaticVertexBuffer(bytesNeeded, bytesNeeded, vertexsData_);
        bufferVertexData_ = pVBuffer_->GetLastAllocation();
        CHECK_ASSERT(bufferVertexData_->maxSize_, __FILE__, __LINE__);

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
            //pIBuffer_ = Context::this_->bufferManager_->GetStaticIndexBuffer(Buffer::MAX_BUFFER_SIZE, bytesNeeded, tmpIndexes);
            pIBuffer_ = Context::this_->bufferManager_->GetStaticIndexBuffer(bytesNeeded, bytesNeeded, tmpIndexes);
            bufferIndexData_ = pIBuffer_->GetLastAllocation();
            CHECK_ASSERT(bufferIndexData_->maxSize_, __FILE__, __LINE__);
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
}