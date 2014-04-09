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
#include "GLES2Buffer.h"
#include "Check.h"
#include <assert.h>

namespace NSG 
{
	GLES2Buffer::GLES2Buffer(GLenum type, GLsizeiptr size, const GLvoid* data, GLenum usage)
	: type_(type)
	{
		CHECK_ASSERT(type == GL_ARRAY_BUFFER || type == GL_ELEMENT_ARRAY_BUFFER, __FILE__, __LINE__);

        assert(glGetError() == GL_NO_ERROR);

		glGenBuffers(1, &id_);

		glBindBuffer(type, id_);

		glBufferData(type, size, data, usage);

        CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);
	}

	GLES2Buffer::~GLES2Buffer()
	{
		glDeleteBuffers(1, &id_);
	}

	BindBuffer::BindBuffer(GLES2Buffer& obj) 
	: obj_(obj)
	{
		obj.Bind();
	}

	BindBuffer::~BindBuffer()
	{
		obj_.UnBind();
	}

}
