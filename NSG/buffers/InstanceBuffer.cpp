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
#include "InstanceBuffer.h"
#include "RenderingContext.h"
#include "RenderingCapabilities.h"
#include "Batch.h"
#include "SceneNode.h"
#include "Util.h"
#include "Check.h"

namespace NSG
{
    InstanceBuffer::InstanceBuffer()
        : Buffer(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
        maxInstances_(0)
    {
    }

    InstanceBuffer::~InstanceBuffer()
    {
    }

    void InstanceBuffer::AllocateResources()
    {
        Buffer::AllocateResources();
    }

    void InstanceBuffer::ReleaseResources()
    {
        if (context_->GetVertexBuffer() == this)
            context_->SetVertexBuffer(nullptr);
        Buffer::ReleaseResources();
    }

    void InstanceBuffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

	void InstanceBuffer::UpdateData(const std::vector<InstanceData>& data)
	{
        context_->SetVertexBuffer(this);
        if (maxInstances_ >= data.size())
        {
            auto size = data.size() * sizeof(InstanceData);
            auto bufferSize = maxInstances_ * sizeof(InstanceData);
            CHECK_ASSERT(size <= bufferSize);
            SetBufferSubData(0, data.size() * sizeof(InstanceData), &(data[0]));
        }
        else
        {
            maxInstances_ = data.size();
            auto bufferSize = maxInstances_ * sizeof(InstanceData);
            glBufferData(type_, bufferSize, &(data[0]), usage_);
        }
	}

	void InstanceBuffer::UpdateBatchBuffer(const Batch& batch)
	{
        if(IsReady())
        {
            CHECK_GL_STATUS();
            CHECK_ASSERT(RenderingCapabilities::GetPtr()->HasInstancedArrays());

            std::vector<InstanceData> instancesData;
            auto& nodes = batch.GetNodes();
            instancesData.reserve(nodes.size());
            for (auto& node : nodes)
            {
                InstanceData data;
                const Matrix4& m = node->GetGlobalModelMatrix();
                // for the model matrix be careful in the shader as we are using rows instead of columns
                // in order to save space (for the attributes) we just pass the first 3 rows of the matrix as the fourth row is always (0,0,0,1) and can be set in the shader
                data.modelMatrixRow0_ = m.Row(0);
                data.modelMatrixRow1_ = m.Row(1);
                data.modelMatrixRow2_ = m.Row(2);

                const Matrix3& normal = node->GetGlobalModelInvTranspMatrix();
                // for the normal matrix we are OK since we pass columns (we do not need to save space as the matrix is 3x3)
                data.normalMatrixCol0_ = normal.Column(0);
                data.normalMatrixCol1_ = normal.Column(1);
                data.normalMatrixCol2_ = normal.Column(2);
                instancesData.push_back(data);
            }

            UpdateData(instancesData);

            CHECK_GL_STATUS();
        }
	}

}

