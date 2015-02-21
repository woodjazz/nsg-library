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
#include "Scene.h"
#include "Constants.h"
#include "Technique.h"
#include "Pass.h"
#include "Util.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Window.h"
#include "Batch.h"
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
          lastMesh_(nullptr),
          lastProgram_(nullptr),
          lastNode_(nullptr),
          activeMesh_(nullptr),
          activeNode_(nullptr),
          activeScene_(nullptr),
          activeCamera_(nullptr),
          activeWindow_(nullptr),
          has_discard_framebuffer_ext_(false),
          has_vertex_array_object_ext_(false),
          has_map_buffer_range_ext_(false),
          has_depth_texture_ext_(false),
          has_depth_component24_ext_(false),
          has_texture_non_power_of_two_ext_(false),
          has_instanced_arrays_ext_(false),
          has_packed_depth_stencil_ext_(false),
          cullFaceMode_(CullFaceMode::DEFAULT),
          frontFaceMode_(FrontFaceMode::DEFAULT),
          maxVaryingVectors_(0),
          maxTexturesCombined_(0),
          maxVertexUniformVectors_(0),
          maxFragmentUniformVectors_(0),
          maxVertexAttribs_(0)
    {

        #if defined(ANDROID) || defined(EMSCRIPTEN)
        {
            glDiscardFramebufferEXT = (PFNGLDISCARDFRAMEBUFFEREXTPROC)eglGetProcAddress ( "glDiscardFramebufferEXT" );
            glGenVertexArraysOES = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress ( "glGenVertexArraysOES" );
            glBindVertexArrayOES = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress ( "glBindVertexArrayOES" );
            glDeleteVertexArraysOES = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress ( "glDeleteVertexArraysOES" );
            glIsVertexArrayOES = (PFNGLISVERTEXARRAYOESPROC)eglGetProcAddress ( "glIsVertexArrayOES" );
            glVertexAttribDivisorEXT = (PFNGLVERTEXATTRIBDIVISORPROC)eglGetProcAddress ( "glVertexAttribDivisorEXT" );
            glDrawElementsInstancedEXT = (PFNGLDRAWELEMENTSINSTANCEDPROC)eglGetProcAddress ( "glDrawElementsInstancedEXT" );
            glDrawArraysInstancedEXT = (PFNGLDRAWARRAYSINSTANCEDPROC)eglGetProcAddress ( "glDrawArraysInstancedEXT" );
        }
        #endif

        TRACE_LOG("GL_VENDOR = " << (const char*)glGetString(GL_VENDOR));
        TRACE_LOG("GL_RENDERER = " << (const char*)glGetString(GL_RENDERER));
        TRACE_LOG("GL_VERSION = " << (const char*)glGetString(GL_VERSION));
        TRACE_LOG("GL_SHADING_LANGUAGE_VERSION = " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
        TRACE_LOG("GL_EXTENSIONS = " << (const char*)glGetString(GL_EXTENSIONS));

        viewport_ = Recti(0);

        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &systemFbo_); // On IOS default FBO is not zero

        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexturesCombined_);
        TRACE_LOG("GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS = " << maxTexturesCombined_);
        CHECK_CONDITION(maxTexturesCombined_ >= 8, __FILE__, __LINE__);
        textures_ = std::vector<Texture*>(maxTexturesCombined_, nullptr);

        if (CheckExtension("EXT_discard_framebuffer"))
        {
            has_discard_framebuffer_ext_ = true;
            TRACE_LOG("Using extension: EXT_discard_framebuffer");
        }

        if (CheckExtension("OES_vertex_array_object") || CheckExtension("ARB_vertex_array_object"))
        {
            has_vertex_array_object_ext_ = true;
            TRACE_LOG("Using extension: vertex_array_object");
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

        if (CheckExtension("GL_EXT_packed_depth_stencil") || CheckExtension("GL_OES_packed_depth_stencil"))
        {
            has_packed_depth_stencil_ext_ = true;
            TRACE_LOG("Using extension: packed_depth_stencil");
        }

        if (CheckExtension("GL_ARB_texture_non_power_of_two"))
        {
            has_texture_non_power_of_two_ext_ = true;
            TRACE_LOG("Using extension: GL_ARB_texture_non_power_of_two");
        }

        #if !defined(EMSCRIPTEN)
        {
            if (CheckExtension("GL_EXT_instanced_arrays") || CheckExtension("GL_ARB_instanced_arrays") || CheckExtension("GL_ANGLE_instanced_arrays"))
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
        }
        #endif


        {
            glGetIntegerv(GL_MAX_VARYING_VECTORS, &maxVaryingVectors_);
            GLenum status = glGetError();
            if (status == GL_NO_ERROR)
            {
                TRACE_LOG("GL_MAX_VARYING_VECTORS = " << maxVaryingVectors_);
            }
            else
            {
                #ifdef GL_MAX_VARYING_COMPONENTS
                glGetIntegerv(GL_MAX_VARYING_COMPONENTS, &maxVaryingVectors_);
                status = glGetError();
                if (status == GL_NO_ERROR)
                {
                    maxVaryingVectors_ /= 4;
                    TRACE_LOG("GL_MAX_VARYING_VECTORS = " << maxVaryingVectors_);
                }
                else
                #endif
                {
                    maxVaryingVectors_ = 8;
                    TRACE_LOG("*** Unknown GL_MAX_VARYING_VECTORS ***. Setting value to " << maxVaryingVectors_);

                }
            }
        }

        {
            glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &maxVertexUniformVectors_);
            GLenum status = glGetError();
            if (status == GL_NO_ERROR)
            {
                TRACE_LOG("GL_MAX_VERTEX_UNIFORM_VECTORS = " << maxVertexUniformVectors_);
            }
            else
            {
                #ifdef GL_MAX_VERTEX_UNIFORM_COMPONENTS
                glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniformVectors_);
                status = glGetError();
                if (status == GL_NO_ERROR)
                {
                    TRACE_LOG("GL_MAX_VERTEX_UNIFORM_VECTORS = " << maxVertexUniformVectors_);
                }
                else
                #endif
                {
                    maxVaryingVectors_ = 128;
                    TRACE_LOG("*** Unknown GL_MAX_VERTEX_UNIFORM_VECTORS ***. Setting value to " << maxVertexUniformVectors_);
                }
            }
        }

        {
            glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &maxFragmentUniformVectors_);
            GLenum status = glGetError();
            if (status == GL_NO_ERROR)
            {
                TRACE_LOG("GL_MAX_FRAGMENT_UNIFORM_VECTORS = " << maxFragmentUniformVectors_);
            }
            else
            {
                #ifdef GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
                glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniformVectors_);
                status = glGetError();
                if (status == GL_NO_ERROR)
                {
                    TRACE_LOG("GL_MAX_FRAGMENT_UNIFORM_VECTORS = " << maxFragmentUniformVectors_);
                }
                else
                #endif
                {
                    maxVaryingVectors_ = 128;
                    TRACE_LOG("*** Unknown GL_MAX_FRAGMENT_UNIFORM_VECTORS ***. Setting value to " << maxFragmentUniformVectors_);
                }
            }
        }


        {
            glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs_);
            TRACE_LOG("GL_MAX_VERTEX_ATTRIBS = " << maxVertexAttribs_);
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

    void Graphics::ResetCachedState()
    {
        viewport_ = Recti(0);

        // Set up texture data read/write alignment
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        vaoMap_.clear();
        lastMesh_ = nullptr;
        lastProgram_ = nullptr;
        lastNode_ = nullptr;
        activeMesh_ = nullptr;
        activeNode_ = nullptr;
        activeScene_ = nullptr;
        activeCamera_ = nullptr;
        activeWindow_ = nullptr;

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

        textures_ = std::vector<Texture*>(maxTexturesCombined_, nullptr);

        SetVertexArrayObj(nullptr);
        SetVertexBuffer(nullptr);
        SetIndexBuffer(nullptr);
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
                
                case BLEND_MULTIPLICATIVE:
                    glEnable(GL_BLEND);
                    if (blendSFactor_ != GL_ZERO || blendDFactor_ != GL_SRC_COLOR)
                    {
                        glBlendFunc(GL_ZERO, GL_SRC_COLOR);
                        blendSFactor_ = GL_ZERO;
                        blendDFactor_ = GL_SRC_COLOR;
                    }
                    break;

                case BLEND_ADDITIVE:
                    glEnable(GL_BLEND);
                    if (blendSFactor_ != GL_ONE || blendDFactor_ != GL_ONE)
                    {
                        glBlendFunc(GL_ONE, GL_ONE);
                        blendSFactor_ = GL_ONE;
                        blendDFactor_ = GL_ONE;
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

    void Graphics::SetTexture(int index, Texture* texture)
    {
        CHECK_CONDITION(index < maxTexturesCombined_, __FILE__, __LINE__);

        if (texture)
        {
            if (activeTexture_ != index)
            {
                glActiveTexture(GL_TEXTURE0 + index);
                textures_[index] = texture;
                glBindTexture(GL_TEXTURE_2D, texture->GetID());
                activeTexture_ = index;
            }
            else if (textures_[index] != texture)
            {
                textures_[index] = texture;
                glBindTexture(GL_TEXTURE_2D, texture->GetID());
            }
        }
        else
        {
            if (activeTexture_ == index && index > 0)
            {
                glActiveTexture(GL_TEXTURE0); //default
                activeTexture_ = 0;
                glBindTexture(GL_TEXTURE_2D, 0);
            }

            textures_[index] = nullptr;
        }
    }

    void Graphics::SetViewport(const Recti& viewport, bool force)
    {
        if (force || viewport_ != viewport)
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
            if (program)
            {
                if (program->IsReady())
                    glUseProgram(program->GetId());
                else
                    return false;
            }
            else
                glUseProgram(0);

            activeProgram_ = program;

            return true;
        }
        return false;
    }

    void Graphics::SetCamera(Camera* camera)
    {
        if (activeCamera_ != camera)
        {
            activeCamera_ = camera;
            if (camera != nullptr)
                SetViewport(camera->GetViewport(), false);
        }
    }

    void Graphics::SetWindow(Window* window)
    {
        if (activeWindow_ != window)
        {
            activeWindow_ = window;
            if (window)
                SetViewport(window->GetViewport(), true);
        }
    }

    void Graphics::DiscardFramebuffer()
    {
        #if defined(GLES2)
        if (has_discard_framebuffer_ext_)
        {
            const GLenum attachments[] = { GL_DEPTH_ATTACHMENT , GL_STENCIL_ATTACHMENT };
            glDiscardFramebuffer( GL_FRAMEBUFFER , sizeof(attachments) / sizeof(GLenum), attachments);
        }
        #endif
    }

    void Graphics::InvalidateVAOFor(const Program* program)
    {
        auto it = vaoMap_.begin();
        while (it != vaoMap_.end())
        {
            if (it->first.program_ == program)
                it->second->Invalidate();
            ++it;
        }
    }

    void Graphics::SetBuffers(bool solid)
    {
        VertexBuffer* vBuffer = activeMesh_->GetVertexBuffer();

        if (has_vertex_array_object_ext_ && !vBuffer->IsDynamic())
        {
			IndexBuffer* iBuffer = activeMesh_->GetIndexBuffer(solid);
            CHECK_ASSERT(!iBuffer || !iBuffer->IsDynamic(), __FILE__, __LINE__);
            VAOKey key { activeProgram_, vBuffer, iBuffer };
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
            SetVertexBuffer(vBuffer);
            SetAttributes();
			if (activeProgram_->GetMaterial()->IsBatched())
				SetInstanceAttrPointers(activeProgram_);
			SetIndexBuffer(activeMesh_->GetIndexBuffer(solid));
        }
    }

    void Graphics::SetInstanceAttrPointers(Program* program)
    {
		CHECK_ASSERT(program->GetMaterial()->IsBatched(), __FILE__, __LINE__);

        CHECK_GL_STATUS(__FILE__, __LINE__);

		SetVertexBuffer(program->GetMaterial()->GetInstanceBuffer().get());

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

        glVertexAttribPointer((int)AttributesLoc::TEXTURECOORD0,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              reinterpret_cast<void*>(offsetof(VertexData, uv_[0])));

        glVertexAttribPointer((int)AttributesLoc::TEXTURECOORD1,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              reinterpret_cast<void*>(offsetof(VertexData, uv_[1])));


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

        glVertexAttribPointer((int)AttributesLoc::BONES_ID,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              reinterpret_cast<void*>(offsetof(VertexData, bonesID_)));

        glVertexAttribPointer((int)AttributesLoc::BONES_WEIGHT,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              reinterpret_cast<void*>(offsetof(VertexData, bonesWeight_)));
    }

    void Graphics::SetAttributes()
    {
        if (lastMesh_ != activeMesh_ || lastProgram_ != activeProgram_)
        {
            GLuint position_loc = activeProgram_->GetAttPositionLoc();
            GLuint texcoord_loc0 = activeProgram_->GetAttTextCoordLoc0();
            GLuint texcoord_loc1 = activeProgram_->GetAttTextCoordLoc1();
            GLuint normal_loc = activeProgram_->GetAttNormalLoc();
            GLuint color_loc = activeProgram_->GetAttColorLoc();
            GLuint tangent_loc = activeProgram_->GetAttTangentLoc();
            GLuint bones_id_loc = activeProgram_->GetAttBonesIDLoc();
            GLuint bones_weight = activeProgram_->GetAttBonesWeightLoc();


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

            if (texcoord_loc0 != -1)
            {
                unsigned positionBit = 1 << texcoord_loc0;
                newAttributes |= positionBit;

                if ((enabledAttributes_ & positionBit) == 0)
                {
                    enabledAttributes_ |= positionBit;
                    glEnableVertexAttribArray(texcoord_loc0);
                }
            }

            if (texcoord_loc1 != -1)
            {
                unsigned positionBit = 1 << texcoord_loc1;
                newAttributes |= positionBit;

                if ((enabledAttributes_ & positionBit) == 0)
                {
                    enabledAttributes_ |= positionBit;
                    glEnableVertexAttribArray(texcoord_loc1);
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

            if (bones_id_loc != -1)
            {
                unsigned positionBit = 1 << bones_id_loc;
                newAttributes |= positionBit;

                if ((enabledAttributes_ & positionBit) == 0)
                {
                    enabledAttributes_ |= positionBit;
                    glEnableVertexAttribArray(bones_id_loc);
                }
            }

            if (bones_weight != -1)
            {
                unsigned positionBit = 1 << bones_weight;
                newAttributes |= positionBit;

                if ((enabledAttributes_ & positionBit) == 0)
                {
                    enabledAttributes_ |= positionBit;
                    glEnableVertexAttribArray(bones_weight);
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

	void Graphics::DrawActiveMesh(Pass* pass)
    {
		CHECK_ASSERT(pass, __FILE__, __LINE__);
		pass->SetupPass();

		if (!activeMesh_->IsReady() || !activeProgram_ || !activeProgram_->IsReady() || (!activeNode_ || !activeNode_->IsReady()))
            return;

        CHECK_GL_STATUS(__FILE__, __LINE__);

        activeProgram_->SetVariables(activeMesh_, activeNode_);

		CHECK_GL_STATUS(__FILE__, __LINE__);

        if (!activeProgram_)
            return; // the program has been invalidated (due some shader needs to be recompiled)

		bool solid = pass->GetDrawMode() == DrawMode::SOLID;
        SetBuffers(solid);
		CHECK_GL_STATUS(__FILE__, __LINE__);
        GLenum mode = solid ? activeMesh_->GetSolidDrawMode() : activeMesh_->GetWireFrameDrawMode();
        const VertexsData& vertexsData = activeMesh_->GetVertexsData();
        const Indexes& indexes = activeMesh_->GetIndexes(solid);

        if (!indexes.empty())
			glDrawElements(mode, GLsizei(indexes.size()), GL_UNSIGNED_SHORT, 0);
        else
			glDrawArrays(mode, 0, GLsizei(vertexsData.size()));

		CHECK_GL_STATUS(__FILE__, __LINE__);
        SetVertexArrayObj(nullptr);

        lastMesh_ = activeMesh_;
        lastNode_ = activeNode_;
        lastProgram_ = activeProgram_;

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

	void Graphics::DrawInstancedActiveMesh(Pass* pass, const Batch& batch)
    {
		CHECK_ASSERT(pass, __FILE__, __LINE__);
		pass->SetupPass();

        CHECK_ASSERT(has_instanced_arrays_ext_, __FILE__, __LINE__);

		if (!activeMesh_->IsReady() || !activeProgram_ || !activeProgram_->IsReady())
            return;

        CHECK_GL_STATUS(__FILE__, __LINE__);

        activeProgram_->SetVariables(activeMesh_, nullptr);

        if (!activeProgram_)
            return; // the program has been invalidated (due some shader needs to be recompiled)

		bool solid = pass->GetDrawMode() == DrawMode::SOLID;
		activeProgram_->GetMaterial()->UpdateBatchBuffer(batch);
		SetBuffers(solid);
        GLenum mode = solid ? activeMesh_->GetSolidDrawMode() : activeMesh_->GetWireFrameDrawMode();
		GLsizei instances = (GLsizei)batch.GetNodes().size();
        const Indexes& indexes = activeMesh_->GetIndexes(solid);
        if (!indexes.empty())
			glDrawElementsInstanced(mode, (GLsizei)indexes.size(), GL_UNSIGNED_SHORT, 0, instances);
        else
        {
            const VertexsData& vertexsData = activeMesh_->GetVertexsData();
			glDrawArraysInstanced(mode, 0, (GLsizei)vertexsData.size(), instances);
        }

        SetVertexArrayObj(nullptr);

        lastMesh_ = activeMesh_;
        lastNode_ = activeNode_;
        lastProgram_ = activeProgram_;

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    void Graphics::RenderVisibleSceneNodes()
    {
        std::vector<SceneNode*> visibles;
		activeScene_->GetVisibleNodes(activeCamera_, visibles);
		if (!visibles.empty())
		{
			std::vector<SceneNode*> transparent;
			ExtractTransparent(visibles, transparent);

			if (!visibles.empty())
			{
				// First draw non-transparent nodes
				std::vector<PBatch> allBatches;
				GenerateBatches(visibles, allBatches);
				for (auto& batch : allBatches)
					batch->Draw();
			}

			if (!transparent.empty())
			{
				// Transparent nodes cannot be batched
				SortBackToFront(transparent);
				for (auto& node : transparent)
					node->Draw();
			}
		}
    }

	void Graphics::ExtractTransparent(std::vector<SceneNode*>& nodes, std::vector<SceneNode*>& transparent) const
	{
		CHECK_ASSERT(transparent.empty(), __FILE__, __LINE__);

		for (auto& node : nodes)
			if (node->GetMaterial()->IsTransparent())
				transparent.push_back(node);

        // remove tranparent from nodes 
		nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
			[&](SceneNode* node) {return transparent.end() != std::find(transparent.begin(), transparent.end(), node); }),
			nodes.end());
	}

	void Graphics::SortBackToFront(std::vector<SceneNode*>& nodes) const
	{
		Vector3 cameraPos;
		if (activeCamera_)
			cameraPos = activeCamera_->GetGlobalPosition();
		std::sort(nodes.begin(), nodes.end(), [&](const SceneNode* a, const SceneNode* b) -> bool
		{
			auto da = glm::distance2(a->GetGlobalPosition(), cameraPos);
			auto db = glm::distance2(b->GetGlobalPosition(), cameraPos);
			return db < da;
		});
	}

    void Graphics::GenerateBatches(std::vector<SceneNode*>& visibles, std::vector<PBatch>& batches)
    {
        struct MeshNode
        {
            PMesh mesh_;
            SceneNode* node_;
        };

        struct MaterialData
        {
            PMaterial material_;
            std::vector<MeshNode> data_;
        };

        std::sort(visibles.begin(), visibles.end(), [](const SceneNode * a, const SceneNode * b) -> bool
        {
            return a->GetMaterial().get() < b->GetMaterial().get();
        });

        std::vector<MaterialData> materials;
        PMaterial usedMaterial;
        for (auto& node : visibles)
        {
            PMaterial material = node->GetMaterial();
            PMesh mesh = node->GetMesh();

            if (usedMaterial != material || !material)
            {
                usedMaterial = material;
                MaterialData materialData;
                materialData.material_ = material;
                materialData.data_.push_back({mesh, node});
                if (!materials.empty())
                {
                    MaterialData& lastMaterialData = materials.back();
                    std::sort(lastMaterialData.data_.begin(), lastMaterialData.data_.end(), [](const MeshNode & a, const MeshNode & b) -> bool
                    {
                        return a.mesh_.get() < b.mesh_.get();
                    });
                }
                materials.push_back(materialData);
            }
            else
            {
                MaterialData& lastMaterial = materials.back();
                lastMaterial.data_.push_back({mesh, node});
            }
        }

        for (auto& material : materials)
        {
            PMesh usedMesh;
            for (auto& obj : material.data_)
            {
				bool limitReached = batches.size() && batches.back()->GetNodes().size() >= MAX_NODES_IN_BATCH;
                if (obj.mesh_ != usedMesh || !obj.mesh_ || limitReached)
                {
                    usedMesh = obj.mesh_;
					auto batch(std::make_shared<Batch>(material.material_.get(), usedMesh.get()));
                    batch->Add(obj.node_);
                    batches.push_back(batch);
                }
                else
                {
                    auto& lastBatch = batches.back();
                    lastBatch->Add(obj.node_);
                }
            }
        }
    }

	bool Graphics::IsTextureSizeCorrect(unsigned width, unsigned height)
	{
		int max_supported_size = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_supported_size);
		CHECK_ASSERT(max_supported_size >= 64, __FILE__, __LINE__);
		if (width > (unsigned)max_supported_size || height > (unsigned)max_supported_size)
			return false;
		return HasNonPowerOfTwo() || (IsPowerOfTwo(width) && IsPowerOfTwo(height));
	}
}