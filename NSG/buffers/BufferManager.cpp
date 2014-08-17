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
#include "BufferManager.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Graphics.h"

namespace NSG
{
	BufferManager::BufferManager()
		: currentStaticVertexBuffer_(nullptr),
		currentStaticIndexBuffer_(nullptr),
		currentDynamicVertexBuffer_(nullptr),
		currentDynamicIndexBuffer_(nullptr)
	{
	}

	BufferManager::~BufferManager()
	{
	}

	void BufferManager::Invalidate()
	{
		currentStaticVertexBuffer_ = nullptr;
		currentStaticIndexBuffer_ = nullptr;
		currentDynamicVertexBuffer_ = nullptr;
		currentDynamicIndexBuffer_ = nullptr;

		Graphics::this_->SetVertexBuffer(nullptr);
		Graphics::this_->SetIndexBuffer(nullptr);
	}

	PVertexBuffer BufferManager::GetStaticVertexBuffer(GLsizeiptr bufferSize, GLsizeiptr bytesNeeded, const VertexsData& vertexes)
	{
		if (!currentStaticVertexBuffer_ || !currentStaticVertexBuffer_->AllocateSpaceFor(bytesNeeded, vertexes))
		{
			PVertexBuffer p(new VertexBuffer(bufferSize, bytesNeeded, vertexes, GL_STATIC_DRAW));

			if(bufferSize > bytesNeeded)
				currentStaticVertexBuffer_ = p;

			return p;
		}

		return currentStaticVertexBuffer_;
	}

	PIndexBuffer BufferManager::GetStaticIndexBuffer(GLsizeiptr bufferSize, GLsizeiptr bytesNeeded, const Indexes& indexes)
	{
		if (!currentStaticIndexBuffer_ || !currentStaticIndexBuffer_->AllocateSpaceFor(bytesNeeded, indexes))
		{
			PIndexBuffer p(new IndexBuffer(bufferSize, bytesNeeded, indexes, GL_STATIC_DRAW));

			if(bufferSize > bytesNeeded)
				currentStaticIndexBuffer_ = p;

			return p;
		}

		return currentStaticIndexBuffer_;
	}

	PVertexBuffer BufferManager::GetDynamicVertexBuffer(GLsizeiptr bufferSize, GLsizeiptr bytesNeeded, const VertexsData& vertexes)
	{
		if (!currentDynamicVertexBuffer_ || !currentDynamicVertexBuffer_->AllocateSpaceFor(bytesNeeded, vertexes))
		{
			PVertexBuffer p(new VertexBuffer(bufferSize, bytesNeeded, vertexes, GL_DYNAMIC_DRAW));

			if(bufferSize > bytesNeeded)
				currentDynamicVertexBuffer_ = p;

			return p;
		}

		return currentDynamicVertexBuffer_;
	}

	PIndexBuffer BufferManager::GetDynamicIndexBuffer(GLsizeiptr bufferSize, GLsizeiptr bytesNeeded, const Indexes& indexes)
	{
		if (!currentDynamicIndexBuffer_ || !currentDynamicIndexBuffer_->AllocateSpaceFor(bytesNeeded, indexes))
		{
			PIndexBuffer p(new IndexBuffer(bufferSize, bytesNeeded, indexes, GL_DYNAMIC_DRAW));

			if(bufferSize > bytesNeeded)
				currentDynamicIndexBuffer_ = p;

			return p;

		}

		return currentDynamicIndexBuffer_;
	}
}
