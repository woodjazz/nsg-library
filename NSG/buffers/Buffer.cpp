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
#include "Buffer.h"
#include "Check.h"
#include "Graphics.h"
#include <assert.h>

namespace NSG 
{
	const unsigned int MAX_BUFFER_SIZE = 2 * 1000 * 1000;
	const unsigned int VERTEXES_PER_TRIANGLE = 3;
	const unsigned int MAX_OBJEXTS_PER_BUFFER = MAX_BUFFER_SIZE / VERTEXES_PER_TRIANGLE;

	Buffer::Buffer(GLenum type, GLsizeiptr maxSize, GLsizeiptr size, const GLvoid* data, GLenum usage)
	: type_(type),
	usage_(usage)
	{
		dataCollection_.reserve(MAX_OBJEXTS_PER_BUFFER); // we do not want reallocation on this buffer

		CHECK_ASSERT(type == GL_ARRAY_BUFFER || type == GL_ELEMENT_ARRAY_BUFFER, __FILE__, __LINE__);

        CHECK_GL_STATUS(__FILE__, __LINE__);

		glGenBuffers(1, &id_);

		dataCollection_.push_back(Data(0, maxSize, size, data));

	}

	Buffer::~Buffer()
	{
		glDeleteBuffers(1, &id_);
	}

	bool Buffer::ReallocateSpaceFor(GLsizeiptr maxSize, GLsizeiptr size, const GLvoid* data)
	{
		GLsizeiptr totalBytes = GetTotalBytes();

		if (totalBytes + maxSize >= MAX_BUFFER_SIZE)
			return false;

		CHECK_ASSERT(dataCollection_.size() < MAX_OBJEXTS_PER_BUFFER, __FILE__, __LINE__); // we do not want reallocation on this buffer

		dataCollection_.push_back(Data{totalBytes, maxSize, size, data });

		return true;
	}

	void Buffer::RedoBuffer()
	{
		CHECK_GL_STATUS(__FILE__, __LINE__);

		glBufferData(type_, MAX_BUFFER_SIZE, nullptr, usage_);

		auto it = dataCollection_.begin();
		while (it != dataCollection_.end())
		{
			const Data& obj = *(it++);
			glBufferSubData(type_, obj.offset_, obj.size_, obj.data_);
		}

		CHECK_GL_STATUS(__FILE__, __LINE__);
	}

	Buffer::Data* Buffer::GetLastAllocation()
	{
		Data& obj = dataCollection_[dataCollection_.size()-1];
		return &obj;
	}

	GLsizeiptr Buffer::GetTotalBytes() const
	{
		GLsizeiptr totalBytes = 0;
		auto it = dataCollection_.begin();
		while (it != dataCollection_.end())
		{
			const Data& obj = *(it++);
			totalBytes += obj.maxSize_;
		}

		return totalBytes;
	}
}
