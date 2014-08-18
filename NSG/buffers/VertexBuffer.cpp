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
#include "VertexBuffer.h"
#include "Graphics.h"
#include "Check.h"

namespace NSG
{
    VertexBuffer::VertexBuffer(GLsizeiptr bufferSize, GLsizeiptr bytesNeeded, const VertexsData& vertexes, GLenum usage)
        : Buffer(bufferSize, bytesNeeded, GL_ARRAY_BUFFER, usage),
          vao_(0)
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        CHECK_CONDITION(Graphics::this_->SetVertexBuffer(this), __FILE__, __LINE__);

        glBufferData(type_, bufferSize, nullptr, usage_);

        std::vector<GLubyte> emptyData(bufferSize, 0);

        SetBufferSubData(0, bufferSize, &emptyData[0]); //created with initialized data to avoid warnings when profiling

        GLsizeiptr bytes2Set = vertexes.size() * sizeof(VertexData);
        CHECK_ASSERT(bytes2Set <= bytesNeeded, __FILE__, __LINE__);

        SetBufferSubData(0, bytes2Set, &vertexes[0]);

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    VertexBuffer::~VertexBuffer()
    {
        if (Graphics::this_->GetVertexBuffer() == this)
            Graphics::this_->SetVertexBuffer(nullptr);
    }

    void VertexBuffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    bool VertexBuffer::AllocateSpaceFor(GLsizeiptr maxSize, const VertexsData& vertexes)
    {
        if (Buffer::AllocateSpaceFor(maxSize))
        {
            const Data* obj = GetLastAllocation();

            CHECK_GL_STATUS(__FILE__, __LINE__);

            Graphics::this_->SetVertexBuffer(this);

            GLsizeiptr bytes2Set = vertexes.size() * sizeof(VertexData);

            SetBufferSubData(obj->offset_, bytes2Set, &vertexes[0]);

            CHECK_GL_STATUS(__FILE__, __LINE__);

            return true;
        }

        return false;
    }

    void VertexBuffer::UpdateData(Buffer::Data& obj, const VertexsData& vertexes)
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        GLsizeiptr bytes2Set = vertexes.size() * sizeof(VertexData);

        CHECK_ASSERT(bytes2Set <= obj.bytes_, __FILE__, __LINE__);

        Graphics::this_->SetVertexBuffer(this);

        SetBufferSubData(obj.offset_, bytes2Set, &vertexes[0]);

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }
}
