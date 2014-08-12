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

#include "IndexBuffer.h"
#include "Types.h"
#include "Graphics.h"
#include "Check.h"
#include <vector>
#include <algorithm>

namespace NSG 
{
	IndexBuffer::IndexBuffer(GLsizeiptr bufferSize, GLsizeiptr bytesNeeded, const Indexes& indexes, GLenum usage)
	: Buffer(bufferSize, bytesNeeded, GL_ELEMENT_ARRAY_BUFFER, usage)
	{
		CHECK_GL_STATUS(__FILE__, __LINE__);

		bool ok = Graphics::this_->SetIndexBuffer(this);
		CHECK_ASSERT(ok, __FILE__, __LINE__);

		glBufferData(type_, bufferSize, nullptr, usage_);

		GLsizeiptr bytes2Set = indexes.size() * sizeof(IndexType);
		CHECK_ASSERT(bytes2Set <= bytesNeeded, __FILE__, __LINE__);

		glBufferSubData(type_, 0, bytes2Set, &indexes[0]);
		
		CHECK_GL_STATUS(__FILE__, __LINE__);
	}

	IndexBuffer::~IndexBuffer()
	{
		if(Graphics::this_->GetIndexBuffer() == this)
			Graphics::this_->SetIndexBuffer(nullptr);
	}

	void IndexBuffer::UnBind() 
	{ 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
	}

	bool IndexBuffer::AllocateSpaceFor(GLsizeiptr maxSize, const Indexes& indexes)
	{
		if (Buffer::AllocateSpaceFor(maxSize))
		{
			CHECK_GL_STATUS(__FILE__, __LINE__);

			Graphics::this_->SetIndexBuffer(this);

			const Data* obj = GetLastAllocation();

			GLsizeiptr bytes2Set = indexes.size() * sizeof(IndexType);

			glBufferSubData(type_, obj->offset_, bytes2Set, &indexes[0]);

			CHECK_GL_STATUS(__FILE__, __LINE__);
			
			return true;
		}
		return false;
	}

	void IndexBuffer::UpdateData(Buffer::Data& obj, const Indexes& indexes)
	{
		CHECK_GL_STATUS(__FILE__, __LINE__);

		GLsizeiptr bytes2Set = indexes.size() * sizeof(IndexType);

		CHECK_ASSERT(bytes2Set <= obj.maxSize_, __FILE__, __LINE__);

		Graphics::this_->SetIndexBuffer(this);

		glBufferSubData(type_, obj.offset_, bytes2Set, &indexes[0]);

		CHECK_GL_STATUS(__FILE__, __LINE__);
		
	}
}
