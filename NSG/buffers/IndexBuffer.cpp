/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "RenderingContext.h"
#include "Check.h"
#include <vector>
#include <algorithm>
#include <sstream>

namespace NSG
{
    IndexBuffer::IndexBuffer(GLenum usage)
        : Buffer(GL_ELEMENT_ARRAY_BUFFER, usage)
    {

    }

    IndexBuffer::IndexBuffer(GLsizeiptr bufferSize, GLsizeiptr bytesNeeded, const Indexes& indexes, GLenum usage)
        : Buffer(bufferSize, bytesNeeded, GL_ELEMENT_ARRAY_BUFFER, usage)
    {
        CHECK_GL_STATUS();

        CHECK_CONDITION(graphics_.lock()->SetIndexBuffer(this));

        glBufferData(type_, bufferSize, nullptr, usage_);

        std::vector<GLubyte> emptyData(bufferSize, 0);
        SetBufferSubData(0, bufferSize, &emptyData[0]); //created with initialized data to avoid warnings when profiling

        GLsizeiptr bytes2Set = indexes.size() * sizeof(IndexType);
        CHECK_ASSERT(bytes2Set <= bytesNeeded);

        SetBufferSubData(0, bytes2Set, &indexes[0]);

        CHECK_GL_STATUS();
    }

    IndexBuffer::~IndexBuffer()
    {
		auto context = graphics_.lock();
		if (context && context->GetIndexBuffer() == this)
			context->SetIndexBuffer(nullptr);
    }

    void IndexBuffer::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void IndexBuffer::UpdateData(const Indexes& indexes)
    {
        CHECK_GL_STATUS();
        graphics_.lock()->SetIndexBuffer(this, true);
        GLsizeiptr bytes2Set = indexes.size() * sizeof(IndexType);
        if (bytes2Set > bufferSize_)
        {
            //rebuild buffer
            glBufferData(type_, bytes2Set, &indexes[0], usage_);
            bufferSize_ = bytes2Set;
        }
        else
            SetBufferSubData(0, bytes2Set, &indexes[0]);
        CHECK_GL_STATUS();
    }

    void IndexBuffer::SetData(GLsizeiptr size, const GLvoid * data)
    {
        CHECK_GL_STATUS();
        graphics_.lock()->SetIndexBuffer(this, true);
        glBufferData(type_, size, data, usage_);
        bufferSize_ = size;
        CHECK_GL_STATUS();
    }

}
