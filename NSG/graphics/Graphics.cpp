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
#include "Graphics.h"
#include "GLES2Includes.h"
#include "Check.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Program.h"

namespace NSG
{
    static const unsigned MAX_TEXTURE_UNITS = 8;

    static const bool DEFAULT_STENCIL_ENABLE = false;
    static const GLuint DEFAULT_STENCIL_WRITEMASK = ~GLuint(0);
    static const GLenum DEFAULT_STENCIL_SFAIL = GL_KEEP;
    static const GLenum DEFAULT_STENCIL_DPFAIL = GL_KEEP;
    static const GLenum DEFAULT_STENCIL_DPPASS = GL_KEEP;
    static const GLenum DEFAULT_STENCIL_FUNC = GL_ALWAYS;
    static const GLint DEFAULT_STENCIL_REF = 0;
    static const GLuint DEFAULT_STENCIL_COMPAREMASK = ~GLuint(0);

    static const bool DEFAULT_COLOR_MASK = true;
    static const bool DEFAULT_DEPTH_MASK = true;
    static const GLuint DEFAULT_STENCIL_MASK = ~GLuint(0);

    static const BLEND_MODE DEFAULT_BLEND_MODE = BLEND_NONE;

    static const bool DEFAULT_DEPTH_TEST_ENABLE = false;

    static const bool DEFAULT_CULL_FACE_ENABLE = false;

    bool CheckExtension(const std::string& name)
    {
        std::string extensions = (const char*)glGetString(GL_EXTENSIONS);
        return extensions.find(name) != std::string::npos;
    }

    void ResetCachedState()
    {
        SetClearColor(Color(0, 0, 0, 1));
        SetClearDepth(1);
        SetClearStencil(0);
        SetFrameBuffer(0);
        SetStencilTest(DEFAULT_STENCIL_ENABLE, DEFAULT_STENCIL_WRITEMASK, DEFAULT_STENCIL_SFAIL,
                       DEFAULT_STENCIL_DPFAIL, DEFAULT_STENCIL_DPPASS, DEFAULT_STENCIL_FUNC, DEFAULT_STENCIL_REF, DEFAULT_STENCIL_COMPAREMASK);
        SetColorMask(DEFAULT_COLOR_MASK);
        SetDepthMask(DEFAULT_DEPTH_MASK);
        SetStencilMask(DEFAULT_STENCIL_MASK);
        SetBlendModeTest(DEFAULT_BLEND_MODE);
        SetDepthTest(DEFAULT_DEPTH_TEST_ENABLE);
        SetCullFace(DEFAULT_CULL_FACE_ENABLE);
        for (unsigned idx = 0; idx < MAX_TEXTURE_UNITS; idx++)
            SetTexture(idx, nullptr);

        SetVertexBuffer(nullptr);
        SetIndexBuffer(nullptr);
        VertexBuffer::UnBind();
        IndexBuffer::UnBind();

        SetProgram(nullptr);
    }

    static GLuint frameBuffer_ = 0;
#if IOS
    static bool iosHack_ = false;
    static GLint iosSystemFbo = 0;
#endif
    void SetFrameBuffer(GLuint value)
    {
#if IOS
        if (!iosHack_)
        {
            glGetIntegerv(GL_FRAMEBUFFER_BINDING, &iosSystemFbo);
            iosHack_ = true;
        }

        if (value == 0)
            value = iosSystemFbo;
#endif
        if (value != frameBuffer_)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, value);
            frameBuffer_ = value;
        }
    }

    void SetClearColor(const Color& color)
    {
        static Color color_(0, 0, 0, 1);

        if (color_ != color)
        {
            glClearColor(color.r, color.g, color.b, color.a);
            
            color_ = color;
        }
    }

    void SetClearDepth(GLclampf depth)
    {
        static GLclampf depth_ = 1;

        if (depth_ != depth)
        {
            glClearDepth(depth);
            
            depth_ = depth;
        }
    }

    void SetClearStencil(GLint clear)
    {
        static GLint clear_ = 0;

        if (clear_ != clear)
        {
            glClearStencil(clear);
            
            clear_ = clear;
        }
    }
    void ClearAllBuffers()
    {
        SetClearColor(Color(0, 0, 0, 1));
        SetColorMask(true);
        SetClearDepth(1);
        SetDepthMask(true);
        SetClearStencil(0);
        SetStencilMask(~GLuint(0));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void ClearBuffers(bool color, bool depth, bool stencil)
    {
        GLbitfield mask(0);
        if (color)
        {
            mask |= GL_COLOR_BUFFER_BIT;
            SetColorMask(true);
            SetClearColor(Color(0, 0, 0, 1));
        }

        if (depth)
        {
            mask |= GL_DEPTH_BUFFER_BIT;
            SetDepthMask(true);
            SetClearDepth(1);
        }

        if (stencil)
        {
            mask |= GL_STENCIL_BUFFER_BIT;
            SetStencilMask(~GLuint(0));
            SetClearStencil(0);
        }

        glClear(mask);
    }

    void ClearStencilBuffer(GLint value)
    {
        SetClearStencil(value);
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    void SetStencilTest(bool enable, GLuint writeMask, GLenum sfail, GLenum dpfail, GLenum dppass, GLenum func, GLint ref, GLuint compareMask)
    {
        static bool enable_ = DEFAULT_STENCIL_ENABLE;
        static GLuint writeMask_ = DEFAULT_STENCIL_WRITEMASK;
        static GLenum sfail_ = DEFAULT_STENCIL_SFAIL;
        static GLenum dpfail_ = DEFAULT_STENCIL_DPFAIL;
        static GLenum dppass_ = DEFAULT_STENCIL_DPPASS;
        static GLenum func_ = DEFAULT_STENCIL_FUNC;
        static GLint ref_ = DEFAULT_STENCIL_REF;
        static GLuint compareMask_ = DEFAULT_STENCIL_COMPAREMASK;

        if (enable != enable_)
        {
            if (enable)
                glEnable(GL_STENCIL_TEST);
            else
                glDisable(GL_STENCIL_TEST);

            enable_ = enable;
        }

        if (enable)
        {
            if (func != func_ || ref != ref_ || compareMask != compareMask_)
            {
                glStencilFunc(func, ref, compareMask);
                func_ = func;
                ref_ = ref;
                compareMask_ = compareMask;
            }
            if (writeMask != writeMask_)
            {
                glStencilMask(writeMask);
                writeMask_ = writeMask;
            }
            if (sfail != sfail_ || dpfail != dpfail_ || dppass != dppass_)
            {
                glStencilOp(sfail, dpfail, dppass);
                sfail_ = sfail;
                dpfail_ = dpfail;
                dppass_ = dppass;
            }
        }
    }

    void SetColorMask(bool enable)
    {
        static bool enable_ = DEFAULT_COLOR_MASK;

        if (enable != enable_)
        {
            if (enable)
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            else
                glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

            enable_ = enable;
        }
    }

    void SetDepthMask(bool enable)
    {
        static bool enable_ = DEFAULT_DEPTH_MASK;

        if (enable != enable_)
        {
            if (enable)
                glDepthMask(GL_TRUE);
            else
                glDepthMask(GL_FALSE);

            enable_ = enable;
        }
    }

    void SetStencilMask(GLuint mask)
    {
        static GLuint mask_ = DEFAULT_STENCIL_MASK;

        if (mask != mask_)
        {
            glStencilMask(mask);
 
            mask_ = mask;
        }
    }

    void SetBlendModeTest(BLEND_MODE blendMode)
    {
        static BLEND_MODE blendMode_ = DEFAULT_BLEND_MODE;

        if (blendMode != blendMode_)
        {
            switch (blendMode)
            {
            case BLEND_NONE:
                glDisable(GL_BLEND);
                break;

            case BLEND_ALPHA:
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;

            default:
                CHECK_ASSERT(false && "Undefined blend mode", __FILE__, __LINE__);
                break;
            }

            blendMode_ = blendMode;
        }
    }

    void SetDepthTest(bool enable)
    {
        static bool enable_ = DEFAULT_DEPTH_TEST_ENABLE;

        if (enable != enable_)
        {
            if (enable)
                glEnable(GL_DEPTH_TEST);
            else
                glDisable(GL_DEPTH_TEST);

            enable_ = enable;
        }
    }

    void SetCullFace(bool enable)
    {
        static bool enable_ = DEFAULT_CULL_FACE_ENABLE;

        if (enable != enable_)
        {
            if (enable)
            {
                glEnable(GL_CULL_FACE);
                //glCullFace(GL_FRONT);
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }

            enable_ = enable;
        }
    }

    void SetTexture(unsigned index, Texture* texture)
    {
        if (index >= MAX_TEXTURE_UNITS)
            return;

        static Texture* textures_[MAX_TEXTURE_UNITS];

        static bool init_ = false;

        if (!init_)
        {
            memset(&textures_[0], 0, sizeof(textures_));
            init_ = true;
        }

        static unsigned activeTexture_ = 0;

        if (textures_[index] != texture)
        {
            if (activeTexture_ != index)
            {
                glActiveTexture(GL_TEXTURE0 + index);
                activeTexture_ = index;
            }

            if (texture)
            {
                glBindTexture(GL_TEXTURE_2D, texture->GetID());
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, 0);
            }

            textures_[index] = texture;
        }
    }

    static VertexBuffer* vertexBuffer_ = nullptr;
    bool SetVertexBuffer(VertexBuffer* buffer)
    {
        if (buffer != vertexBuffer_)
        {
            vertexBuffer_ = buffer;

            if (buffer)
            {
                buffer->Bind();
                return true;
            }
        }

        return false;
    }

    VertexBuffer* GetVertexBuffer()
    {
        return vertexBuffer_;
    }

    static IndexBuffer* indexBuffer_ = nullptr;
    bool SetIndexBuffer(IndexBuffer* buffer)
    {
        if (buffer != indexBuffer_)
        {
            indexBuffer_ = buffer;
            if (buffer)
            {
                buffer->Bind();
                return true;
            }

        }

        return false;
    }

    IndexBuffer* GetIndexBuffer()
    {
        return indexBuffer_;
    }

    static Program* program_ = nullptr;
    bool SetProgram(Program* program)
    {
        if (program != program_)
        {
            program_ = program;
            if (program)
            {
                glUseProgram(program->GetId());
            }
            else
            {
                glUseProgram(0);
            }

            return true;
        }

        return false;
    }

    Program* GetProgram()
    {
        return program_;
    }

    void DiscardFramebuffer()
    {
        static bool init_ = false;
        static bool has_discard_framebuffer_ = false;

        if (!init_)
        {
            init_ = true;
            if (CheckExtension("EXT_discard_framebuffer"))
                has_discard_framebuffer_ = true;
        }

        if (has_discard_framebuffer_)
        {
            const GLenum attachments[3] = { GL_COLOR_ATTACHMENT0 , GL_DEPTH_ATTACHMENT , GL_STENCIL_ATTACHMENT };
            glDiscardFramebufferEXT( GL_FRAMEBUFFER , 3, attachments);
        }
    }
}