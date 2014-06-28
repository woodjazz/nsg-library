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
#include "AppStatistics.h"

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
		buffers_.clear();
		currentStaticVertexBuffer_ = nullptr;
		currentStaticIndexBuffer_ = nullptr;
		currentDynamicVertexBuffer_ = nullptr;
		currentDynamicIndexBuffer_ = nullptr;

		SetVertexBuffer(nullptr);
		SetIndexBuffer(nullptr);

		if(AppStatistics::this_)
		{
			AppStatistics::this_->ResetVertexBuffer();	
			AppStatistics::this_->ResetIndexBuffer();
		}
	}


	VertexBuffer* BufferManager::GetStaticVertexBuffer(GLsizeiptr maxSize, GLsizeiptr size, const GLvoid* data)
	{
		if (!currentStaticVertexBuffer_ || !currentStaticVertexBuffer_->ReallocateSpaceFor(maxSize, size, data))
		{
			currentStaticVertexBuffer_ = new VertexBuffer(maxSize, size, data, GL_STATIC_DRAW);
			buffers_.push_back(PBuffer(currentStaticVertexBuffer_));

			if(AppStatistics::this_)
				AppStatistics::this_->NewVertexBuffer();

		}

		return currentStaticVertexBuffer_;
	}

	IndexBuffer* BufferManager::GetStaticIndexBuffer(GLsizeiptr maxSize, GLsizeiptr size, const GLvoid* data, GLintptr indexBase)
	{
		if (!currentStaticIndexBuffer_ || !currentStaticIndexBuffer_->ReallocateSpaceFor(maxSize, size, data, indexBase))
		{
			currentStaticIndexBuffer_ = new IndexBuffer(maxSize, size, data, GL_STATIC_DRAW);
			buffers_.push_back(PBuffer(currentStaticIndexBuffer_));

			if(AppStatistics::this_)
				AppStatistics::this_->NewIndexBuffer();

		}

		return currentStaticIndexBuffer_;
	}


	VertexBuffer* BufferManager::GetDynamicVertexBuffer(GLsizeiptr maxSize, GLsizeiptr size, const GLvoid* data)
	{
		if (!currentDynamicVertexBuffer_ || !currentDynamicVertexBuffer_->ReallocateSpaceFor(maxSize, size, data))
		{
			currentDynamicVertexBuffer_ = new VertexBuffer(maxSize, size, data, GL_DYNAMIC_DRAW);
			buffers_.push_back(PBuffer(currentDynamicVertexBuffer_));

			if(AppStatistics::this_)
				AppStatistics::this_->NewVertexBuffer();

		}

		return currentDynamicVertexBuffer_;
	}

	IndexBuffer* BufferManager::GetDynamicIndexBuffer(GLsizeiptr maxSize, GLsizeiptr size, const GLvoid* data, GLintptr indexBase)
	{
		if (!currentDynamicIndexBuffer_ || !currentDynamicIndexBuffer_->ReallocateSpaceFor(maxSize, size, data, indexBase))
		{
			currentDynamicIndexBuffer_ = new IndexBuffer(maxSize, size, data, GL_DYNAMIC_DRAW);
			buffers_.push_back(PBuffer(currentDynamicIndexBuffer_));

			if(AppStatistics::this_)
				AppStatistics::this_->NewIndexBuffer();

		}

		return currentDynamicIndexBuffer_;
	}


}