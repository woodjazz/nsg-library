/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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
#include "Log.h"

namespace NSG
{
    static VertexsData emptyVertexes;
    VertexBuffer::VertexBuffer(GLenum usage)
        : Buffer(GL_ARRAY_BUFFER, usage),
          vertexes_(emptyVertexes)
    {

    }

    VertexBuffer::VertexBuffer(const VertexsData& vertexes, GLenum usage)
        : Buffer(GL_ARRAY_BUFFER, usage),
          vertexes_(vertexes)
    {
    }

    VertexBuffer::~VertexBuffer()
    {
    }

    void VertexBuffer::AllocateResources()
    {
        CHECK_GL_STATUS();
        auto ctx = RenderingContext::GetSharedPtr();
        CHECK_ASSERT(ctx);
        Buffer::AllocateResources();
        ctx->SetVertexBuffer(this);
        if (vertexes_.size())
        {
            auto bytesNeeded = vertexes_.size() * sizeof(VertexData);
            glBufferData(type_, bytesNeeded, &vertexes_[0], usage_);
        }
        CHECK_GL_STATUS();
    }

    void VertexBuffer::ReleaseResources()
    {
        auto ctx = RenderingContext::GetSharedPtr();
        if (ctx)
        {
            if (ctx->GetVertexBuffer() == this)
                ctx->SetVertexBuffer(nullptr);
            Buffer::ReleaseResources();
        }
    }


    void VertexBuffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::UpdateData()
    {
        if (IsReady())
        {
            CHECK_GL_STATUS();
            auto bytesNeeded = vertexes_.size() * sizeof(VertexData);
            glBufferData(type_, bytesNeeded, &vertexes_[0], usage_);
            CHECK_GL_STATUS();
        }
    }

    void VertexBuffer::SetData(GLsizeiptr size, const GLvoid* data)
    {
        auto ctx = RenderingContext::GetSharedPtr();
        CHECK_GL_STATUS();
        ctx->SetVertexBuffer(this, true);
        glBufferData(type_, size, data, usage_);
        CHECK_GL_STATUS();
    }
}
