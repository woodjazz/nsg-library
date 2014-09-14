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
#include "InstanceBuffer.h"
#include "Program.h"
#include "Material.h"
#include "Mesh.h"
#include "AppStatistics.h"
#include "Scene.h"
#include "Constants.h"
#include "Technique.h"
#include "Pass.h"
#include "SceneNode.h"
#include "InstanceData.h"

#if defined(ANDROID) || defined(EMSCRIPTEN)
PFNGLDISCARDFRAMEBUFFEREXTPROC glDiscardFramebufferEXT;
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOES;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOES;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOES;
PFNGLISVERTEXARRAYOESPROC glIsVertexArrayOES;
PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisorEXT;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstancedEXT;
PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstancedEXT;
#endif

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
        : currentFbo_(0),  //the default framebuffer (except for IOS)
          vertexArrayObj_(nullptr),
          vertexBuffer_(nullptr),
          indexBuffer_(nullptr),
          activeProgram_(nullptr),
          activeTexture_(0),
          enabledAttributes_(0),
          uniformsNeedUpdate_(true),
          lastMesh_(nullptr),
          lastMaterial_(nullptr),
          lastProgram_(nullptr),
          lastNode_(nullptr),
          activeMesh_(nullptr),
          activeMaterial_(nullptr),
          activeNode_(nullptr),
          has_discard_framebuffer_ext_(false),
          has_vertex_array_object_ext_(false),
          has_map_buffer_range_ext_(false),
          has_depth_texture_ext_(false),
          has_depth_component24_ext_(false),
          has_texture_non_power_of_two_ext_(false),
          has_instanced_arrays_ext_(false),
          cullFaceMode_(CullFaceMode::DEFAULT),
          frontFaceMode_(FrontFaceMode::DEFAULT)
    {

#if defined(ANDROID) || defined(EMSCRIPTEN)
        glDiscardFramebufferEXT = (PFNGLDISCARDFRAMEBUFFEREXTPROC)eglGetProcAddress ( "glDiscardFramebufferEXT" );
        glGenVertexArraysOES = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress ( "glGenVertexArraysOES" );
        glBindVertexArrayOES = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress ( "glBindVertexArrayOES" );
        glDeleteVertexArraysOES = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress ( "glDeleteVertexArraysOES" );
        glIsVertexArrayOES = (PFNGLISVERTEXARRAYOESPROC)eglGetProcAddress ( "glIsVertexArrayOES" );
        glVertexAttribDivisorEXT = (PFNGLVERTEXATTRIBDIVISORPROC)eglGetProcAddress ( "glVertexAttribDivisorEXT" );
        glDrawElementsInstancedEXT = (PFNGLDRAWELEMENTSINSTANCEDPROC)eglGetProcAddress ( "glDrawElementsInstancedEXT" );
        glDrawArraysInstancedEXT = (PFNGLDRAWARRAYSINSTANCEDPROC)eglGetProcAddress ( "glDrawArraysInstancedEXT" );
#endif
        TRACE_LOG("GL_VENDOR = " << (const char*)glGetString(GL_VENDOR));
        TRACE_LOG("GL_RENDERER = " << (const char*)glGetString(GL_RENDERER));
        TRACE_LOG("GL_VERSION = " << (const char*)glGetString(GL_VERSION));
        TRACE_LOG("GL_SHADING_LANGUAGE_VERSION = " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
        TRACE_LOG("GL_EXTENSIONS = " << (const char*)glGetString(GL_EXTENSIONS));

        glGetIntegerv(GL_VIEWPORT, &viewport_[0]);
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &systemFbo_); // On IOS default FBO is not zero

        memset(&textures_[0], 0, sizeof(textures_));

        if (CheckExtension("EXT_discard_framebuffer"))
        {
            has_discard_framebuffer_ext_ = true;
            TRACE_LOG("Using extension: EXT_discard_framebuffer");
        }

        if (CheckExtension("OES_vertex_array_object") || CheckExtension("ARB_vertex_array_object"))
        {
#if !defined(NACL)
            has_vertex_array_object_ext_ = true;
            TRACE_LOG("Using extension: vertex_array_object");
#endif
        }

        if (CheckExtension("EXT_map_buffer_range"))
        {
            has_map_buffer_range_ext_ = true;
            TRACE_LOG("Using extension: EXT_map_buffer_range");
        }

        if (CheckExtension("GL_OES_depth_texture"))
        {
            has_depth_texture_ext_ = true;
            TRACE_LOG("Using extension: GL_OES_depth_texture");
        }

        if (CheckExtension("GL_OES_depth24"))
        {
            has_depth_component24_ext_ = true;
            TRACE_LOG("Using extension: GL_OES_depth24");
        }

        if (CheckExtension("GL_ARB_texture_non_power_of_two"))
        {
            has_texture_non_power_of_two_ext_ = true;
            TRACE_LOG("Using extension: GL_ARB_texture_non_power_of_two");
        }

        if (CheckExtension("GL_EXT_instanced_arrays") || CheckExtension("GL_ARB_instanced_arrays"))
        {
            GLint maxVertexAtts = 0;
            glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAtts);
            int attributesNeeded = (int)AttributesLoc::MAX_ATTS;
            if (maxVertexAtts >= attributesNeeded)
            {
                has_instanced_arrays_ext_ = true;
                TRACE_LOG("Using extension: instanced_arrays");
            }
            else
            {
                TRACE_LOG("Has extension: instanced_arrays");
                TRACE_LOG("Needed " << attributesNeeded << " but graphics only supports " << maxVertexAtts << " attributes");
                TRACE_LOG("Disabling extension: instanced_arrays");
            }
        }

        // Set up texture data read/write alignment
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
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

    void Graphics::InitializeBuffers()
    {
        if (has_instanced_arrays_ext_)
            instanceBuffer_ = PInstanceBuffer(new InstanceBuffer);
    }

    void Graphics::ResetCachedState()
    {
        glGetIntegerv(GL_VIEWPORT, &viewport_[0]);

        // Set up texture data read/write alignment
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        uniformsNeedUpdate_ = true;
        lastMesh_ = nullptr;
        lastMaterial_ = nullptr;
        lastProgram_ = nullptr;
        lastNode_ = nullptr;
        activeMesh_ = nullptr;
        activeMaterial_ = nullptr;
        activeNode_ = nullptr;

        CHECK_GL_STATUS(__FILE__, __LINE__);

        SetClearColor(Color(0, 0, 0, 1));
        SetClearDepth(1);
        SetClearStencil(0);
        SetFrameBuffer(0);
        SetStencilTest(DEFAULT_STENCIL_ENABLE, DEFAULT_STENCIL_WRITEMASK, DEFAULT_STENCIL_SFAIL,
                       DEFAULT_STENCIL_DPFAIL, DEFAULT_STENCIL_DPPASS, DEFAULT_STENCIL_FUNC, DEFAULT_STENCIL_REF, DEFAULT_STENCIL_COMPAREMASK);

        CHECK_GL_STATUS(__FILE__, __LINE__);

        SetColorMask(DEFAULT_COLOR_MASK);
        SetDepthMask(DEFAULT_DEPTH_MASK);
        SetStencilMask(DEFAULT_STENCIL_MASK);
        SetBlendModeTest(DEFAULT_BLEND_MODE);
        EnableDepthTest(DEFAULT_DEPTH_TEST_ENABLE);
        EnableCullFace(DEFAULT_CULL_FACE_ENABLE);
        SetCullFace(CullFaceMode::DEFAULT);
        SetFrontFace(FrontFaceMode::DEFAULT);
        CHECK_GL_STATUS(__FILE__, __LINE__);

        for (unsigned idx = 0; idx < MAX_TEXTURE_UNITS; idx++)
            SetTexture(idx, nullptr);

        SetVertexArrayObj(nullptr);
        SetVertexBuffer(nullptr);
        SetIndexBuffer(nullptr);
        InitializeBuffers();
        SetProgram(nullptr);

        CHECK_GL_STATUS(__FILE__, __LINE__);
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
        static Color color_(0, 0, 0, 0);

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

    bool Graphics::SetVertexBuffer(Buffer* buffer, bool force)
    {
        if (buffer != vertexBuffer_ || force)
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

    bool Graphics::SetIndexBuffer(Buffer* buffer, bool force)
    {
        if (buffer != indexBuffer_ || force)
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
        if (program != activeProgram_)
        {
            activeProgram_ = program;
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
#if defined(GLES2) && !defined(NACL)
        if (has_discard_framebuffer_ext_)
        {
            const GLenum attachments[] = { GL_DEPTH_ATTACHMENT , GL_STENCIL_ATTACHMENT };
            glDiscardFramebuffer( GL_FRAMEBUFFER , sizeof(attachments) / sizeof(GLenum), attachments);
        }
#endif
    }

    void Graphics::BeginFrame()
    {
    }

    void Graphics::EndFrame()
    {
        if (!uniformsNeedUpdate_)
            UniformsUpdate::ClearAllUpdates();

        uniformsNeedUpdate_ = false;
    }

    void Graphics::SetBuffers()
    {
        if (has_vertex_array_object_ext_)
        {
            VertexBuffer* vBuffer = activeMesh_->GetVertexBuffer();
            IndexBuffer* iBuffer = activeMesh_->GetIndexBuffer();
            VAOKey key {activeProgram_, vBuffer, iBuffer};
            VertexArrayObj* vao(nullptr);
            auto it = vaoMap_.find(key);
            if (it != vaoMap_.end())
            {
                vao = it->second.get();
            }
            else
            {
                vao = new VertexArrayObj(activeProgram_, vBuffer, iBuffer);
                CHECK_CONDITION(vaoMap_.insert(VAOMap::value_type(key, PVertexArrayObj(vao))).second, __FILE__, __LINE__);
            }
            vao->Use();
        }
        else
        {
            SetVertexBuffer(activeMesh_->GetVertexBuffer());
            SetAttributes();
            SetInstanceAttrPointers(activeProgram_);
            SetIndexBuffer(activeMesh_->GetIndexBuffer());
        }
    }

    void Graphics::RedoVAO(Program* program, VertexBuffer* vBuffer, IndexBuffer* iBuffer)
    {
        if (has_vertex_array_object_ext_)
        {
            VAOKey key {activeProgram_, vBuffer, iBuffer};
            auto it = vaoMap_.find(key);
            if (it != vaoMap_.end())
                it->second->MarkAsDirty();
        }
    }

    void Graphics::UpdateBatchBuffer()
    {
        if (has_instanced_arrays_ext_)
        {
            CHECK_ASSERT(activeNode_, __FILE__, __LINE__);
            Batch batch;
            batch.nodes_.push_back(activeNode_);
            UpdateBatchBuffer(batch);
        }
    }

    void Graphics::UpdateBatchBuffer(const Batch& batch)
    {
        CHECK_ASSERT(has_instanced_arrays_ext_, __FILE__, __LINE__);

        std::vector<InstanceData> instancesData;
        instancesData.reserve(batch.nodes_.size());
        for (auto& node : batch.nodes_)
        {
            InstanceData data;
            const Matrix4& m = node->GetGlobalModelMatrix();
            data.modelMatrixRow0_ = glm::row(m, 0);
            data.modelMatrixRow1_ = glm::row(m, 1);
            data.modelMatrixRow2_ = glm::row(m, 2);
            const Matrix3& normal = node->GetGlobalModelInvTranspMatrix();
            data.normalMatrixCol0_ = glm::column(normal, 0);
            data.normalMatrixCol1_ = glm::column(normal, 1);
            data.normalMatrixCol2_ = glm::column(normal, 2);
            instancesData.push_back(data);
        }

        SetVertexBuffer(instanceBuffer_.get());
        glBufferData(GL_ARRAY_BUFFER, instancesData.size() * sizeof(InstanceData), &(instancesData[0]), GL_DYNAMIC_DRAW);
    }


    void Graphics::SetInstanceAttrPointers(Program* program)
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);
        if (has_instanced_arrays_ext_)
        {
            SetVertexBuffer(instanceBuffer_.get());

            GLuint modelMatrixLoc = program->GetAttModelMatrixLoc();

            if (modelMatrixLoc != -1)
            {
                for (int i = 0; i < 3; i++)
                {
                    glEnableVertexAttribArray(modelMatrixLoc + i);
                    glVertexAttribPointer(modelMatrixLoc + i,
                                          4,
                                          GL_FLOAT,
                                          GL_FALSE,
                                          sizeof(InstanceData),
                                          reinterpret_cast<void*>(offsetof(InstanceData, modelMatrixRow0_) + sizeof(float) * 4 * i));

                    glVertexAttribDivisor(modelMatrixLoc + i, 1);
                }
            }
            else
            {
                glDisableVertexAttribArray((int)AttributesLoc::MODEL_MATRIX_ROW0);
                glDisableVertexAttribArray((int)AttributesLoc::MODEL_MATRIX_ROW1);
                glDisableVertexAttribArray((int)AttributesLoc::MODEL_MATRIX_ROW2);
            }

            GLuint normalMatrixLoc = program->GetAttNormalMatrixLoc();
            if (normalMatrixLoc != -1)
            {
                for (int i = 0; i < 3; i++)
                {
                    glEnableVertexAttribArray(normalMatrixLoc + i);
                    glVertexAttribPointer(normalMatrixLoc + i,
                                          3,
                                          GL_FLOAT,
                                          GL_FALSE,
                                          sizeof(InstanceData),
                                          reinterpret_cast<void*>(offsetof(InstanceData, normalMatrixCol0_) + sizeof(float) * 3 * i));

                    glVertexAttribDivisor(normalMatrixLoc + i, 1);
                }
            }
            else
            {
                glDisableVertexAttribArray((int)AttributesLoc::NORMAL_MATRIX_COL0);
                glDisableVertexAttribArray((int)AttributesLoc::NORMAL_MATRIX_COL1);
                glDisableVertexAttribArray((int)AttributesLoc::NORMAL_MATRIX_COL2);
            }
        }
        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    void Graphics::SetVertexAttrPointers()
    {
        glVertexAttribPointer((int)AttributesLoc::POSITION,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              reinterpret_cast<void*>(offsetof(VertexData, position_)));

        glVertexAttribPointer((int)AttributesLoc::NORMAL,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              reinterpret_cast<void*>(offsetof(VertexData, normal_)));

        glVertexAttribPointer((int)AttributesLoc::TEXTURECOORD,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              reinterpret_cast<void*>(offsetof(VertexData, uv_)));

        glVertexAttribPointer((int)AttributesLoc::COLOR,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              reinterpret_cast<void*>(offsetof(VertexData, color_)));

        glVertexAttribPointer((int)AttributesLoc::TANGENT,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              reinterpret_cast<void*>(offsetof(VertexData, tangent_)));

    }

    void Graphics::SetAttributes()
    {
        if (lastMesh_ != activeMesh_ || lastProgram_ != activeProgram_)
        {
            GLuint position_loc = activeProgram_->GetAttPositionLoc();
            GLuint texcoord_loc = activeProgram_->GetAttTextCoordLoc();
            GLuint normal_loc = activeProgram_->GetAttNormalLoc();
            GLuint color_loc = activeProgram_->GetAttColorLoc();
            GLuint tangent_loc = activeProgram_->GetAttTangentLoc();

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

            if (tangent_loc != -1)
            {
                unsigned positionBit = 1 << tangent_loc;
                newAttributes |= positionBit;

                if ((enabledAttributes_ & positionBit) == 0)
                {
                    enabledAttributes_ |= positionBit;
                    glEnableVertexAttribArray(tangent_loc);
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

        activeProgram_->SetVariables(activeMaterial_, activeNode_);

        CHECK_GL_STATUS(__FILE__, __LINE__);

        UpdateBatchBuffer();
        SetBuffers();
        GLenum mode = solid ? activeMesh_->GetSolidDrawMode() : activeMesh_->GetWireFrameDrawMode();
        const VertexsData& vertexsData = activeMesh_->GetVertexsData();
        const Indexes& indexes = activeMesh_->GetIndexes();

        if (!indexes.empty())
        {
            Buffer::Data* bufferIndexData = activeMesh_->GetBufferIndexData();
            const GLvoid* offset = reinterpret_cast<const GLvoid*>(bufferIndexData->offset_);
            glDrawElements(mode, indexes.size(), GL_UNSIGNED_SHORT, offset);
        }
        else
        {
            Buffer::Data* bufferVertexData = activeMesh_->GetBufferVertexData();
            GLint first = bufferVertexData->offset_ / sizeof(VertexData);
            glDrawArrays(mode, first, vertexsData.size());
        }

        if (solid)
            AppStatistics::this_->NewTriangles(activeMesh_->GetNumberOfTriangles());

        SetVertexArrayObj(nullptr);

        CHECK_GL_STATUS(__FILE__, __LINE__);

        lastMesh_ = activeMesh_;
        lastMaterial_ = activeMaterial_;
        lastNode_ = activeNode_;
        lastProgram_ = activeProgram_;

        AppStatistics::this_->NewDrawCall();

        CHECK_GL_STATUS(__FILE__, __LINE__);

        return true;
    }

    bool Graphics::Draw(bool solid, Batch& batch)
    {
        if ((activeMaterial_ && !activeMaterial_->IsReady()) || !activeMesh_->IsReady())
            return false;

        CHECK_GL_STATUS(__FILE__, __LINE__);

        activeProgram_->SetVariables(activeMaterial_);
        UpdateBatchBuffer(batch);
        SetBuffers();
        GLenum mode = solid ? activeMesh_->GetSolidDrawMode() : activeMesh_->GetWireFrameDrawMode();
        unsigned instances = batch.nodes_.size();
        const VertexsData& vertexsData = activeMesh_->GetVertexsData();
        const Indexes& indexes = activeMesh_->GetIndexes();
        if (!indexes.empty())
        {
            Buffer::Data* bufferIndexData = activeMesh_->GetBufferIndexData();
            const GLvoid* offset = reinterpret_cast<const GLvoid*>(bufferIndexData->offset_);
            glDrawElementsInstanced(mode, indexes.size(), GL_UNSIGNED_SHORT, offset, instances);
        }
        else
        {
            Buffer::Data* bufferVertexData = activeMesh_->GetBufferVertexData();
            GLint first = bufferVertexData->offset_ / sizeof(VertexData);
            glDrawArraysInstanced(mode, first, vertexsData.size(), instances);
        }

        if (solid)
            AppStatistics::this_->NewTriangles(activeMesh_->GetNumberOfTriangles() * instances);

        SetVertexArrayObj(nullptr);

        lastMesh_ = activeMesh_;
        lastMaterial_ = activeMaterial_;
        lastNode_ = activeNode_;
        lastProgram_ = activeProgram_;

        AppStatistics::this_->NewDrawCall();

        CHECK_GL_STATUS(__FILE__, __LINE__);

        return true;
    }


    void Graphics::Render(Batch& batch)
    {
        bool node_resources_loaded = true;
        for (auto& node : batch.nodes_)
        {
            SceneNode* sn = (SceneNode*)node;
            node_resources_loaded &= sn->IsReady(); // forces load from xml
        }

        if (node_resources_loaded && batch.material_)
        {
            PTechnique technique = batch.material_->GetTechnique();
            if (technique)
            {
                Set(batch.material_.get());
                Set(batch.mesh_.get());
                if (has_instanced_arrays_ext_ && technique->GetNumPasses() == 1)
                {
                    SetNode(nullptr);
                    PPass pass = technique->GetPass(0);
                    pass->Render(batch);
                }
                else
                {
                    for (auto& node : batch.nodes_)
                    {
                        SceneNode* sn = (SceneNode*)node;
                        SetNode(sn);
                        technique->Render();
                    }
                }
            }
        }
    }
}