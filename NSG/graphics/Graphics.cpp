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
#include "VertexArrayObj.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Program.h"
#include "Material.h"
#include "Mesh.h"
#include "AppStatistics.h"
#include "Scene.h"
#include "Constants.h"

namespace NSG
{
    template <> Graphics* Singleton<Graphics>::this_ = nullptr;

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
    static GLenum DEFAULT_BLEND_SFACTOR = GL_ONE;
    static GLenum DEFAULT_BLEND_DFACTOR = GL_ZERO;

    static const bool DEFAULT_DEPTH_TEST_ENABLE = false;

    static const bool DEFAULT_CULL_FACE_ENABLE = false;

    Graphics::Graphics()
    {
        TRACE_LOG("GL_VENDOR = " << (const char*)glGetString(GL_VENDOR));
        TRACE_LOG("GL_RENDERER = " << (const char*)glGetString(GL_RENDERER));
        TRACE_LOG("GL_VERSION = " << (const char*)glGetString(GL_VERSION));
        TRACE_LOG("GL_SHADING_LANGUAGE_VERSION = " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
        TRACE_LOG("GL_EXTENSIONS = " << (const char*)glGetString(GL_EXTENSIONS));

        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &systemFbo_); // On IOS default FBO is not zero

        has_discard_framebuffer_ext_ = false;
        if (CheckExtension("EXT_discard_framebuffer"))
        {
            has_discard_framebuffer_ext_ = true;
            TRACE_LOG("Using extension: EXT_discard_framebuffer");
        }

        has_vertex_array_object_ext_ = false;
        if (CheckExtension("OES_vertex_array_object"))
        {
            has_vertex_array_object_ext_ = true;
            TRACE_LOG("Using extension: OES_vertex_array_object");
        }

        has_map_buffer_range_ext_ = false;
        if (CheckExtension("EXT_map_buffer_range"))
        {
            has_map_buffer_range_ext_ = true;
            TRACE_LOG("Using extension: EXT_map_buffer_range");
        }
        
        ResetCachedState();
    }

    Graphics::~Graphics()
    {
        Graphics::this_ = nullptr;
    }

    bool Graphics::CheckExtension(const std::string& name)
    {
        std::string extensions = (const char*)glGetString(GL_EXTENSIONS);
        return extensions.find(name) != std::string::npos;
    }

    void Graphics::ResetCachedState()
    {
        activeTexture_ = 0;
        enabledAttributes_ = 0;
        uniformsNeedUpdate_ = true;
        lastMesh_ = nullptr;
        lastMaterial_ = nullptr;
        lastNode_ = nullptr;
        activeMesh_ = nullptr;
        activeMaterial_ = nullptr;
        activeNode_ = nullptr;

        // Set up texture data read/write alignment
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        currentFbo_ = 0; //the default framebuffer (except for IOS)
        glGetIntegerv(GL_VIEWPORT, &viewport_[0]);
        uniformsNeedUpdate_ = true;
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
        EnableDepthTest(DEFAULT_DEPTH_TEST_ENABLE);
        EnableCullFace(DEFAULT_CULL_FACE_ENABLE);
        SetCullFace(CullFaceMode::DEFAULT);
        SetFrontFace(FrontFaceMode::DEFAULT);
        for (unsigned idx = 0; idx < MAX_TEXTURE_UNITS; idx++)
            SetTexture(idx, nullptr);

        SetVertexArrayObj(nullptr);
        SetVertexBuffer(nullptr);
        SetIndexBuffer(nullptr);

        SetProgram(nullptr);
    }

    void Graphics::SetFrameBuffer(GLuint value)
    {
        if (value != currentFbo_)
        {
            if (value == 0)
                glBindFramebuffer(GL_FRAMEBUFFER, systemFbo_);
            else
                glBindFramebuffer(GL_FRAMEBUFFER, value);

            currentFbo_ = value;
        }
    }

    void Graphics::SetClearColor(const Color& color)
    {
        static Color color_(0, 0, 0, 1);

        if (color_ != color)
        {
            glClearColor(color.r, color.g, color.b, color.a);

            color_ = color;
        }
    }

    void Graphics::SetClearDepth(GLclampf depth)
    {
        static GLclampf depth_ = 1;

        if (depth_ != depth)
        {
            glClearDepth(depth);

            depth_ = depth;
        }
    }

    void Graphics::SetClearStencil(GLint clear)
    {
        static GLint clear_ = 0;

        if (clear_ != clear)
        {
            glClearStencil(clear);

            clear_ = clear;
        }
    }

    void Graphics::ClearAllBuffers()
    {
        SetClearColor(Color(0, 0, 0, 1));
        SetColorMask(true);
        SetClearDepth(1);
        SetDepthMask(true);
        SetClearStencil(0);
        SetStencilMask(~GLuint(0));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void Graphics::ClearBuffers(bool color, bool depth, bool stencil)
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

    void Graphics::ClearStencilBuffer(GLint value)
    {
        SetClearStencil(value);
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    void Graphics::SetStencilTest(bool enable, GLuint writeMask, GLenum sfail, GLenum dpfail, GLenum dppass, GLenum func, GLint ref, GLuint compareMask)
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

    void Graphics::SetColorMask(bool enable)
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

    void Graphics::SetDepthMask(bool enable)
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

    void Graphics::SetStencilMask(GLuint mask)
    {
        static GLuint mask_ = DEFAULT_STENCIL_MASK;

        if (mask != mask_)
        {
            glStencilMask(mask);

            mask_ = mask;
        }
    }

    void Graphics::SetBlendModeTest(BLEND_MODE blendMode)
    {
        static BLEND_MODE blendMode_ = DEFAULT_BLEND_MODE;
        static GLenum blendSFactor_ = DEFAULT_BLEND_SFACTOR;
        static GLenum blendDFactor_ = DEFAULT_BLEND_DFACTOR;

        if (blendMode != blendMode_)
        {
            switch (blendMode)
            {
            case BLEND_NONE:
                glDisable(GL_BLEND);
                if (blendSFactor_ != GL_ONE || blendDFactor_ != GL_ZERO)
                {
                    glBlendFunc(GL_ONE, GL_ZERO);
                    blendSFactor_ = GL_ONE;
                    blendDFactor_ = GL_ZERO;
                }

                break;

            case BLEND_ALPHA:
                glEnable(GL_BLEND);
                if (blendSFactor_ != GL_SRC_ALPHA || blendDFactor_ != GL_ONE_MINUS_SRC_ALPHA)
                {
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    blendSFactor_ = GL_SRC_ALPHA;
                    blendDFactor_ = GL_ONE_MINUS_SRC_ALPHA;
                }
                break;

            default:
                CHECK_ASSERT(false && "Undefined blend mode", __FILE__, __LINE__);
                break;
            }

            blendMode_ = blendMode;
        }
    }

    void Graphics::EnableDepthTest(bool enable)
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

    void Graphics::EnableCullFace(bool enable)
    {
        static bool enable_ = DEFAULT_CULL_FACE_ENABLE;

        if (enable != enable_)
        {
            if (enable)
            {
                glEnable(GL_CULL_FACE);
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }

            enable_ = enable;
        }
    }

    void Graphics::SetCullFace(CullFaceMode mode)
    {
        if (mode != cullFaceMode_)
        {
            glCullFace((GLenum)mode);
            cullFaceMode_ = mode;
        }
    }

    void Graphics::SetFrontFace(FrontFaceMode mode)
    {
        if (mode != frontFaceMode_)
        {
            glFrontFace((GLenum)mode);
            frontFaceMode_ = mode;
        }
    }


    void Graphics::SetTexture(unsigned index, Texture* texture)
    {
        if (index >= MAX_TEXTURE_UNITS)
            return;

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

    void Graphics::SetViewport(const Recti& viewport)
    {
        if (viewport_ != viewport)
        {
            glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
            viewport_ = viewport;
        }
    }

    bool Graphics::SetVertexArrayObj(VertexArrayObj* obj)
    {
        if (obj != vertexArrayObj_)
        {
            vertexArrayObj_ = obj;

            if (obj)
            {
                obj->Bind();
            }
            else
            {
                VertexArrayObj::Unbind();
            }
            return true;
        }

        return false;
    }

    bool Graphics::SetVertexBuffer(VertexBuffer* buffer)
    {
        if (buffer != vertexBuffer_)
        {
            vertexBuffer_ = buffer;

            if (buffer)
            {
                buffer->Bind();
            }
            else
            {
                VertexBuffer::Unbind();
            }
            return true;
        }

        return false;
    }

    bool Graphics::SetIndexBuffer(IndexBuffer* buffer)
    {
        if (buffer != indexBuffer_)
        {
            indexBuffer_ = buffer;

            if (buffer)
            {
                buffer->Bind();
            }
            else
            {
                IndexBuffer::Unbind();
            }
            return true;

        }
        return false;
    }

    bool Graphics::SetProgram(Program* program)
    {
        if (program != program_)
        {
            program_ = program;
            if (program)
                glUseProgram(program->GetId());
            else
                glUseProgram(0);
            return true;
        }
        return false;
    }

    void Graphics::DiscardFramebuffer()
    {
#if IS_TARGET_MOBILE
        if (has_discard_framebuffer_ext_)
        {
            const GLenum attachments[3] = { GL_COLOR_ATTACHMENT0 , GL_DEPTH_ATTACHMENT , GL_STENCIL_ATTACHMENT };
            glDiscardFramebufferEXT( GL_FRAMEBUFFER , 3, attachments);
        }
#endif
    }

    void Graphics::BeginFrame()
    {
        if (AppStatistics::this_)
            AppStatistics::this_->NewFrame();
    }

    void Graphics::EndFrame()
    {
        DiscardFramebuffer();

        if (!uniformsNeedUpdate_)
            UniformsUpdate::ClearAllUpdates();

        uniformsNeedUpdate_ = false;
    }

    void Graphics::SetVertexAttrPointers()
    {
        glVertexAttribPointer(ATTRIBUTE_LOC::POSITION,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              reinterpret_cast<void*>(offsetof(VertexData, position_)));

        glVertexAttribPointer(ATTRIBUTE_LOC::NORMAL,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              reinterpret_cast<void*>(offsetof(VertexData, normal_)));

        glVertexAttribPointer(ATTRIBUTE_LOC::COORD,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              reinterpret_cast<void*>(offsetof(VertexData, uv_)));

        glVertexAttribPointer(ATTRIBUTE_LOC::COLOR,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              reinterpret_cast<void*>(offsetof(VertexData, color_)));

    }

    void Graphics::SetAttributes(const Mesh* mesh, const Program* program)
    {
        if (lastMesh_ != mesh)
        {
            GLuint position_loc = program->GetAttPositionLoc();
            GLuint texcoord_loc = program->GetAttTextCoordLoc();
            GLuint normal_loc = program->GetAttNormalLoc();
            GLuint color_loc = program->GetAttColorLoc();

            unsigned newAttributes = 0;

            if (position_loc != -1)
            {
                unsigned positionBit = 1 << position_loc;
                newAttributes |= positionBit;

                if ((enabledAttributes_ & positionBit) == 0)
                {
                    enabledAttributes_ |= positionBit;
                    glEnableVertexAttribArray(position_loc);
                }

            }

            if (normal_loc != -1)
            {
                unsigned positionBit = 1 << normal_loc;
                newAttributes |= positionBit;

                if ((enabledAttributes_ & positionBit) == 0)
                {
                    enabledAttributes_ |= positionBit;
                    glEnableVertexAttribArray(normal_loc);
                }
            }

            if (texcoord_loc != -1)
            {
                unsigned positionBit = 1 << texcoord_loc;
                newAttributes |= positionBit;

                if ((enabledAttributes_ & positionBit) == 0)
                {
                    enabledAttributes_ |= positionBit;
                    glEnableVertexAttribArray(texcoord_loc);
                }
            }

            if (color_loc != -1)
            {
                unsigned positionBit = 1 << color_loc;
                newAttributes |= positionBit;

                if ((enabledAttributes_ & positionBit) == 0)
                {
                    enabledAttributes_ |= positionBit;
                    glEnableVertexAttribArray(color_loc);
                }
            }

            SetVertexAttrPointers();

            {
                /////////////////////////////
                // Disable unused attributes
                /////////////////////////////
                unsigned disableAttributes = enabledAttributes_ & (~newAttributes);
                unsigned disableIndex = 0;
                while (disableAttributes)
                {
                    if (disableAttributes & 1)
                    {
                        glDisableVertexAttribArray(disableIndex);
                        enabledAttributes_ &= ~(1 << disableIndex);
                    }
                    disableAttributes >>= 1;
                    ++disableIndex;
                }
            }
        }
    }

    bool Graphics::Draw(bool solid)
    {
        if ((activeMaterial_ && !activeMaterial_->IsReady()) || !activeMesh_->IsReady())
            return false;

        CHECK_GL_STATUS(__FILE__, __LINE__);

        program_->SetVariables(activeMaterial_, activeNode_);

        activeMesh_->Draw(solid, program_);

        lastMesh_ = activeMesh_;
        lastMaterial_ = activeMaterial_;
        lastNode_ = activeNode_;

        if (AppStatistics::this_)
            AppStatistics::this_->NewDrawCall();

        CHECK_GL_STATUS(__FILE__, __LINE__);

        return true;
    }
}