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
    static Indexes emptyIndexes;
    IndexBuffer::IndexBuffer(GLenum usage)
        : Buffer(GL_ELEMENT_ARRAY_BUFFER, usage),
          indexes_(emptyIndexes)
    {
    }

    IndexBuffer::IndexBuffer(const Indexes& indexes, GLenum usage)
        : Buffer(GL_ELEMENT_ARRAY_BUFFER, usage),
          indexes_(indexes)
    {
    }

    IndexBuffer::~IndexBuffer()
    {
    }

    void IndexBuffer::AllocateResources()
    {
        Buffer::AllocateResources();

        CHECK_GL_STATUS();
        context_->SetIndexBuffer(this);
        auto bytesNeeded = sizeof(IndexType) * indexes_.size();
        glBufferData(type_, bytesNeeded, &indexes_[0], usage_);
        //SetBufferSubData(0, bytesNeeded, &indexes_[0]);
        CHECK_GL_STATUS();
    }

    void IndexBuffer::ReleaseResources()
    {
        if (context_->GetIndexBuffer() == this)
            context_->SetIndexBuffer(nullptr);

        Buffer::ReleaseResources();
    }

    void IndexBuffer::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void IndexBuffer::UpdateData()
    {
        if(IsReady())
        {
            CHECK_GL_STATUS();
            auto bytesNeeded = sizeof(IndexType) * indexes_.size();
            glBufferData(type_, bytesNeeded, &indexes_[0], usage_);
            CHECK_GL_STATUS();
        }
    }

    void IndexBuffer::SetData(GLsizeiptr size, const GLvoid * data)
    {
        if(IsReady())
        {
            CHECK_GL_STATUS();
            context_->SetIndexBuffer(this, true);
            glBufferData(type_, size, data, usage_);
            CHECK_GL_STATUS();
        }
    }

}
