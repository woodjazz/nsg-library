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
#include "InstanceBuffer.h"
#include "Graphics.h"
#include "Batch.h"
#include "SceneNode.h"
#include "Util.h"
#include "Check.h"

namespace NSG
{
    InstanceBuffer::InstanceBuffer()
        : Buffer(0, 0, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
		maxInstances_(0),
		graphics_(Graphics::GetPtr())
    {
    }

    InstanceBuffer::~InstanceBuffer()
    {
		if (graphics_->GetVertexBuffer() == this)
            graphics_->SetVertexBuffer(nullptr);
    }

    void InstanceBuffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

	void InstanceBuffer::UpdateData(const std::vector<InstanceData>& data)
	{
		graphics_->SetVertexBuffer(this);

		if (maxInstances_ >= data.size())
		{
			SetBufferSubData(0, data.size() * sizeof(InstanceData), &(data[0]));
		}
		else
		{
			maxInstances_ = data.size();
			bufferSize_ = maxInstances_ * sizeof(InstanceData);
			glBufferData(type_, bufferSize_, &(data[0]), usage_);
		}
	}

	void InstanceBuffer::UpdateBatchBuffer(const Batch& batch)
	{
		CHECK_GL_STATUS();

		CHECK_ASSERT(graphics_->HasInstancedArrays());

		std::vector<InstanceData> instancesData;
		auto& nodes = batch.GetNodes();
		instancesData.reserve(nodes.size());
		for (auto& node : nodes)
		{
			InstanceData data;
			const Matrix4& m = node->GetGlobalModelMatrix();
			// for the model matrix be careful in the shader as we are using rows instead of columns
			// in order to save space (for the attributes) we just pass the first 3 rows of the matrix as the fourth row is always (0,0,0,1) and can be set in the shader
			data.modelMatrixRow0_ = Row(m, 0);
			data.modelMatrixRow1_ = Row(m, 1);
			data.modelMatrixRow2_ = Row(m, 2);

			const Matrix3& normal = node->GetGlobalModelInvTranspMatrix();
			// for the normal matrix we are OK since we pass columns (we do not need to save space as the matrix is 3x3)
			data.normalMatrixCol0_ = Column(normal, 0);
			data.normalMatrixCol1_ = Column(normal, 1);
			data.normalMatrixCol2_ = Column(normal, 2);
			instancesData.push_back(data);
		}

		UpdateData(instancesData);

		CHECK_GL_STATUS();
	}

}

