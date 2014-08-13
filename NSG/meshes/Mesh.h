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
#pragma once
#include <vector>
#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Types.h"
#include "Node.h"
#include "GPUObject.h"
#include "Resource.h"
#include "Buffer.h"
#include "BoundingBox.h"

namespace NSG
{
	class Mesh : public GPUObject
	{
	public:
		Mesh(GLenum usage);
		~Mesh();
		virtual GLenum GetWireFrameDrawMode() const = 0;
		virtual GLenum GetSolidDrawMode() const = 0;
		virtual bool IsValid() override;
		virtual void AllocateResources() override;
		virtual void ReleaseResources() override;
		const BoundingBox& GetBB() const { return bb_; }
		VertexBuffer* GetVertexBuffer() const { return pVBuffer_.get(); }
		IndexBuffer* GetIndexBuffer() const { return pIBuffer_.get(); }
		const VertexsData& GetVertexsData() const { return vertexsData_; }
		const Indexes& GetIndexes() const { return indexes_; }
		Buffer::Data* GetBufferVertexData() const { return bufferVertexData_; }
		Buffer::Data* GetBufferIndexData() const { return bufferIndexData_; }
	protected:
		VertexsData vertexsData_;
		Indexes indexes_;
		PIndexBuffer pIBuffer_;
		PVertexBuffer pVBuffer_;
		GLenum usage_;
        PResource resource_;
        Buffer::Data* bufferVertexData_;
        Buffer::Data* bufferIndexData_;
        BoundingBox bb_;
	};
}
