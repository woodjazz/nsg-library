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
	IndexBuffer::IndexBuffer(GLsizeiptr maxSize, GLsizeiptr size, const GLvoid* data, GLenum usage)
	: Buffer(GL_ELEMENT_ARRAY_BUFFER, maxSize, size, data, usage)
	{
		CHECK_GL_STATUS(__FILE__, __LINE__);
		SetVertexBuffer(this);
		glBufferData(type_, MAX_BUFFER_SIZE, nullptr, usage_);
		glBufferSubData(type_, 0, maxSize, data);
		CHECK_GL_STATUS(__FILE__, __LINE__);
	}

	IndexBuffer::~IndexBuffer()
	{
		if(GetIndexBuffer() == this)
			SetIndexBuffer(nullptr);

	}

	void IndexBuffer::UnBind() 
	{ 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
	}

	bool IndexBuffer::ReallocateSpaceFor(GLsizeiptr maxSize, GLsizeiptr size, const GLvoid* data, GLintptr indexBase)
	{
		if (Buffer::ReallocateSpaceFor(maxSize, size, data))
		{
			const IndexType* old_indexes = static_cast<const IndexType*>(data);
			GLsizeiptr n = size / sizeof(IndexType);
			std::vector<IndexType> indexes;
			indexes.reserve(n);
			std::copy_n(old_indexes, n, std::back_inserter(indexes));
			std::for_each(indexes.begin(), indexes.end(), [&](IndexType& x) { x += indexBase; CHECK_ASSERT(x < MAX_INDEX_VALUE, __FILE__, __LINE__); });

			CHECK_GL_STATUS(__FILE__, __LINE__);

			SetIndexBuffer(this);

			const Data* obj = GetLastAllocation();
			glBufferSubData(type_, obj->offset_, size, &indexes[0]);

			CHECK_GL_STATUS(__FILE__, __LINE__);
			
			return true;
		}
		return false;
	}

	void IndexBuffer::UpdateData(Buffer::Data& obj, const Indexes& indexes, GLintptr indexBase)
	{
		CHECK_GL_STATUS(__FILE__, __LINE__);

		GLsizeiptr bytes2Set = indexes.size() * sizeof(IndexType);

		CHECK_ASSERT(bytes2Set <= obj.maxSize_, __FILE__, __LINE__);

		obj.size_ = bytes2Set;

		Indexes tmpIndexes(indexes);
		std::for_each(tmpIndexes.begin(), tmpIndexes.end(), [&](IndexType& x) { x += indexBase; CHECK_ASSERT(x < MAX_INDEX_VALUE, __FILE__, __LINE__); });

		SetVertexBuffer(this);

		glBufferSubData(type_, obj.offset_, bytes2Set, &tmpIndexes[0]);

		CHECK_GL_STATUS(__FILE__, __LINE__);
		
	}
}
