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
#include "Buffer.h"
#include "Util.h"
#include "Check.h"
#include "RenderingContext.h"
#include "RenderingCapabilities.h"
#include "Window.h"

namespace NSG
{
    Buffer::Buffer(GLenum type, GLenum usage)
        : Object(GetUniqueName("Buffer")),
          type_(type),
          usage_(usage),
          dynamic_(usage != GL_STATIC_DRAW)
    {
    }

    Buffer::~Buffer()
    {
    }

    bool Buffer::IsValid()
    {
        return Window::GetMainWindow() != nullptr;
    }


    void Buffer::AllocateResources()
    {
        context_ = RenderingContext::Create();
        CHECK_GL_STATUS();
        glGenBuffers(1, &id_);

    }

    void Buffer::ReleaseResources()
    {
        CHECK_GL_STATUS();
        glDeleteBuffers(1, &id_);
        context_ = nullptr;
    }

    void Buffer::Bind()
    {
        glBindBuffer(type_, id_);
    }

    void Buffer::SetBufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data)
    {
        #if !defined(ANDROID) && !defined(EMSCRIPTEN)
        if (RenderingCapabilities::GetPtr()->HasMapBufferRange())
        {
            void* old_data = glMapBufferRange(type_, offset, size,
                                              GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

            CHECK_ASSERT(old_data);

            memcpy(old_data, data, size);

            glFlushMappedBufferRange(type_, offset, size);

            CHECK_CONDITION(glUnmapBuffer(type_));
        }
        else
        #endif
        {
            glBufferSubData(type_, offset, size, data);
        }
    }

}
