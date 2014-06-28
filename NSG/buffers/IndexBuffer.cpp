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
		RedoBuffer();
	}

	IndexBuffer::~IndexBuffer()
	{
	}

	void IndexBuffer::RedoBuffer()
	{
		SetIndexBuffer(this);
		Buffer::RedoBuffer();
	}

	void IndexBuffer::UnBind() 
	{ 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
	}

	bool IndexBuffer::ReallocateSpaceFor(GLsizeiptr maxSize, GLsizeiptr size, const GLvoid* data, GLintptr indexBase)
	{
		if (Buffer::ReallocateSpaceFor(maxSize, size, data))
		{
			const Data* obj = GetLastAllocation();
			const IndexType* old_indexes = static_cast<const IndexType*>(obj->data_);
			GLsizeiptr n = size / sizeof(IndexType);
			std::vector<IndexType> indexes;
			indexes.resize(n);
			std::copy_n(old_indexes, n, &indexes[0]);
			for (GLsizeiptr i = 0; i < n; i++)
			{
				indexes[i] += indexBase;
				CHECK_ASSERT(indexes[i] < MAX_INDEX_VALUE, __FILE__, __LINE__);
			}

			CHECK_GL_STATUS(__FILE__, __LINE__);

			SetIndexBuffer(this);

			glBufferSubData(type_, obj->offset_, obj->size_, &indexes[0]);

			CHECK_GL_STATUS(__FILE__, __LINE__);
			
			return true;
		}
		return false;
	}

	void IndexBuffer::UpdateData(Buffer::Data& obj, const Indexes& indexes, GLintptr indexBase)
	{
		CHECK_GL_STATUS(__FILE__, __LINE__);

		CHECK_ASSERT(obj.data_ == &indexes[0], __FILE__, __LINE__);

		GLsizeiptr bytes2Set = indexes.size() * sizeof(IndexType);

		CHECK_ASSERT(bytes2Set <= obj.maxSize_, __FILE__, __LINE__);

		obj.size_ = bytes2Set;

		std::vector<IndexType> tmpIndexes = indexes;
		GLsizeiptr n = tmpIndexes.size();
		for (GLsizeiptr i = 0; i < n; i++)
		{
			tmpIndexes[i] += indexBase;
			CHECK_ASSERT(tmpIndexes[i] < MAX_INDEX_VALUE, __FILE__, __LINE__);
		}

		SetVertexBuffer(this);

		glBufferSubData(type_, obj.offset_, obj.size_, &tmpIndexes[0]);

		CHECK_GL_STATUS(__FILE__, __LINE__);
		
	}
}
