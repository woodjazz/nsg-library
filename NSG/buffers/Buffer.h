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
#pragma once

#include "Types.h"
#include "Allocators.h"
#include "PODVector.h"

namespace NSG 
{
	class Buffer
	{
	public:

		struct Data
		{
			GLintptr offset_;
			GLsizeiptr maxSize_;
			GLsizeiptr size_;
			const GLvoid* data_;

			Data()
			{
				memset(this, 0, sizeof(*this));
			}

			Data(GLintptr offset, GLsizeiptr maxSize, GLsizeiptr size, const GLvoid* data)
				: offset_(offset), maxSize_(maxSize), size_(size), data_(data)
			{
			}
		};

		~Buffer();

		Data* GetLastAllocation();
		void Bind() { glBindBuffer(type_, id_); }
		virtual bool ReallocateSpaceFor(GLsizeiptr maxSize, GLsizeiptr size, const GLvoid* data);
	protected:
		Buffer(GLenum type, GLsizeiptr maxSize, GLsizeiptr size, const GLvoid *data, GLenum usage = GL_STATIC_DRAW);
		GLsizeiptr GetTotalBytes() const;
		GLenum type_;
		GLuint id_;
		GLenum usage_;
		static const size_t MAX_BUFFER_SIZE = 2 * 1000 * 1000;
	private:
		static const size_t VERTEXES_PER_TRIANGLE = 3;
		static const size_t MAX_OBJECTS_PER_BUFFER = MAX_BUFFER_SIZE / VERTEXES_PER_TRIANGLE;
		PODVector<Data, MAX_OBJECTS_PER_BUFFER> dataCollection_;
	};
}

