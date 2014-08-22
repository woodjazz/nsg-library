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
#include "AppStatistics.h"
#include <assert.h>

#if IS_TARGET_MOBILE
#define glMapBufferRange glMapBufferRangeEXT
#define glFlushMappedBufferRange glFlushMappedBufferRangeEXT
#define glUnmapBuffer glUnmapBufferOES
#define GL_MAP_WRITE_BIT GL_MAP_WRITE_BIT_EXT
#define GL_MAP_FLUSH_EXPLICIT_BIT GL_MAP_FLUSH_EXPLICIT_BIT_EXT
#define GL_MAP_UNSYNCHRONIZED_BIT GL_MAP_UNSYNCHRONIZED_BIT_EXT
#endif

namespace NSG
{
    Buffer::Buffer(GLsizeiptr bufferSize, GLsizeiptr bytesNeeded, GLenum type, GLenum usage)
        : type_(type),
          usage_(usage),
          bufferSize_(bufferSize),
          dynamic_(usage != GL_STATIC_DRAW)
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        glGenBuffers(1, &id_);

        dataCollection_.push_back(Data(0, bytesNeeded));

        if (type_ == GL_ARRAY_BUFFER)
            AppStatistics::this_->AddVertexBuffer(dynamic_);
        else
            AppStatistics::this_->AddIndexBuffer(dynamic_);
    }

    Buffer::~Buffer()
    {
        if (type_ == GL_ARRAY_BUFFER)
            AppStatistics::this_->RemoveVertexBuffer(dynamic_);
        else
            AppStatistics::this_->RemoveIndexBuffer(dynamic_);

        glDeleteBuffers(1, &id_);
    }

    void Buffer::Bind()
    {
        glBindBuffer(type_, id_);
    }

    bool Buffer::AllocateSpaceFor(GLsizeiptr bytesNeeded)
    {
        GLsizeiptr totalBytes = GetTotalBytes();

        if (totalBytes + bytesNeeded >= bufferSize_)
            return false;

        dataCollection_.push_back(Data {totalBytes, bytesNeeded});

        return true;
    }

    Buffer::Data* Buffer::GetLastAllocation()
    {
        CHECK_ASSERT(dataCollection_.size() < MAX_OBJECTS_PER_BUFFER, __FILE__, __LINE__);

        Data& obj = dataCollection_[dataCollection_.size() - 1];
        return &obj;
    }

    GLsizeiptr Buffer::GetTotalBytes() const
    {
        GLsizeiptr totalBytes = 0;
        auto it = dataCollection_.begin();
        while (it != dataCollection_.end())
        {
            const Data& obj = *(it++);
            totalBytes += obj.bytes_;
        }

        return totalBytes;
    }

    void Buffer::SetBufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data)
    {
        if (Graphics::this_->HasMapBufferRange())
        {
            void* old_data = glMapBufferRange(type_, offset, size,
                                              GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

            CHECK_ASSERT(old_data, __FILE__, __LINE__);

            memcpy(old_data, data, size);

            glFlushMappedBufferRange(type_, offset, size);

            CHECK_CONDITION(glUnmapBuffer(type_), __FILE__, __LINE__);
        }
        else
        {
            glBufferSubData(type_, offset, size, data);
        }
    }

}
