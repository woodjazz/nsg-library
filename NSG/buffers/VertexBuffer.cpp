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
#include "VertexBuffer.h"
#include "RenderingContext.h"
#include "Check.h"

namespace NSG
{
    static VertexsData emptyVertexes;
    VertexBuffer::VertexBuffer(GLenum usage)
        : Buffer(0, GL_ARRAY_BUFFER, usage),
          vertexes_(emptyVertexes),
          bytesNeeded_(0)
    {

    }

    VertexBuffer::VertexBuffer(GLsizeiptr bufferSize, GLsizeiptr bytesNeeded, const VertexsData& vertexes, GLenum usage)
        : Buffer(bufferSize, GL_ARRAY_BUFFER, usage),
          vertexes_(vertexes),
          bytesNeeded_(bytesNeeded)
    {
        CHECK_ASSERT(bufferSize >= bytesNeeded);
    }

    VertexBuffer::~VertexBuffer()
    {
    }

    void VertexBuffer::AllocateResources()
    {
        Buffer::AllocateResources();

        context_->SetVertexBuffer(this);

        glBufferData(type_, bufferSize_, nullptr, usage_);

        std::vector<GLubyte> emptyData(bufferSize_, 0);

        SetBufferSubData(0, bufferSize_, &emptyData[0]); //created with initialized data to avoid warnings when profiling

        GLsizeiptr bytes2Set = vertexes_.size() * sizeof(VertexData);
        CHECK_ASSERT(bytes2Set <= bytesNeeded_);

        SetBufferSubData(0, bytes2Set, &vertexes_[0]);
    }

    void VertexBuffer::ReleaseResources()
    {
        if (context_->GetVertexBuffer() == this)
            context_->SetVertexBuffer(nullptr);

        Buffer::ReleaseResources();
    }


    void VertexBuffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::UpdateData()
    {
        if(IsReady())
        {
            CHECK_GL_STATUS();
            context_->SetVertexBuffer(this, true);
            GLsizeiptr bytes2Set = vertexes_.size() * sizeof(VertexData);
            if (bytes2Set > bufferSize_)
            {
                //rebuild buffer
                glBufferData(type_, bytes2Set, &vertexes_[0], usage_);
                bufferSize_ = bytes2Set;
            }
            else
                SetBufferSubData(0, bytes2Set, &vertexes_[0]);
            CHECK_GL_STATUS();
        }
    }

    void VertexBuffer::SetData(GLsizeiptr size, const GLvoid* data)
    {
        CHECK_GL_STATUS();
        context_->SetVertexBuffer(this, true);
        glBufferData(type_, size, data, usage_);
        CHECK_GL_STATUS();
    }
}
