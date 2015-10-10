/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "Pass.h"
#include "Util.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Window.h"
#include "Batch.h"
#include "InstanceData.h"
#include "FrameBuffer.h"
#include "ShowTexture.h"
#include "Filter.h"
#include "GUI.h"
#include <functional>


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

    static const BLEND_MODE DEFAULT_BLEND_MODE = BLEND_MODE::NONE;
    static GLenum DEFAULT_BLEND_SFACTOR = GL_ONE;
    static GLenum DEFAULT_BLEND_DFACTOR = GL_ZERO;

    static const bool DEFAULT_DEPTH_TEST_ENABLE = false;

    static const bool DEFAULT_CULL_FACE_ENABLE = false;

    Graphics::Graphics()
        : currentFbo_(0),  //the default framebuffer (except for IOS)
          currentColorTarget_(TextureTarget::UNKNOWN),
          vertexArrayObj_(nullptr),
          vertexBuffer_(nullptr),
          indexBuffer_(nullptr),
          activeProgram_(nullptr),
          activeTexture_(0),
          enabledAttributes_(0),
          lastMesh_(nullptr),
          lastProgram_(nullptr),
          activeMesh_(nullptr),
          activeWindow_(nullptr),
          has_discard_framebuffer_ext_(false),
          has_vertex_array_object_ext_(false),
          has_map_buffer_range_ext_(false),
          has_depth_texture_ext_(false),
          has_depth_component24_ext_(false),
          has_texture_non_power_of_two_ext_(false),
          has_instanced_arrays_ext_(false),
          has_packed_depth_stencil_ext_(false),
          has_texture_compression_dxt1_ext_(false),
          has_texture_compression_dxt3_ext_(false),
          has_texture_compression_dxt5_ext_(false),
          has_compressed_ETC1_RGB8_texture_ext_(false),
          has_texture_compression_pvrtc_ext_(false),
          cullFaceMode_(CullFaceMode::DEFAULT),
          frontFaceMode_(FrontFaceMode::DEFAULT),
          maxVaryingVectors_(0),
          maxTexturesCombined_(0),
          maxVertexUniformVectors_(0),
          maxFragmentUniformVectors_(0),
          maxVertexAttribs_(0),
          depthFunc_(DepthFunc::LESS),
          maxTextureSize_(64),
          slopeScaledDepthBias_(0)
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

        LOGI("GL_VENDOR = %s", (const char*)glGetString(GL_VENDOR));
        LOGI("GL_RENDERER = %s", (const char*)glGetString(GL_RENDERER));
        LOGI("GL_VERSION = %s", (const char*)glGetString(GL_VERSION));
        LOGI("GL_SHADING_LANGUAGE_VERSION = %s", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
        LOGI("GL_EXTENSIONS = %s", (const char*)glGetString(GL_EXTENSIONS));

        viewport_ = Recti(0);

        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &systemFbo_); // On IOS default FBO is not zero
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize_);
        CHECK_ASSERT(maxTextureSize_ >= 64);
        LOGI("GL_MAX_TEXTURE_SIZE = %d", maxTextureSize_);
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexturesCombined_);
        LOGI("GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS = %d", maxTexturesCombined_);
        CHECK_CONDITION(maxTexturesCombined_ >= 8);
        textures_ = std::vector<Texture*>(maxTexturesCombined_, nullptr);

        #if 0
        {
            GLint numCompressedTexturesFormats = 0;
            glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &numCompressedTexturesFormats);
            if (numCompressedTexturesFormats)
            {
                GLint* compressedFormat = new int[numCompressedTexturesFormats];
                glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, compressedFormat);
                delete[] compressedFormat;
            }
        }
        #endif

        if (CheckExtension("EXT_texture_compression_dxt1"))
        {
            has_texture_compression_dxt1_ext_ = true;
            LOGI("Has extension: EXT_texture_compression_dxt1");
        }

        if (CheckExtension("WEBGL_compressed_texture_s3tc"))
        {
            has_texture_compression_dxt1_ext_ = true;
            has_texture_compression_dxt3_ext_ = true;
            has_texture_compression_dxt5_ext_ = true;
            LOGI("Has extension: WEBGL_compressed_texture_s3tc");
        }

        if (CheckExtension("EXT_texture_compression_s3tc"))
        {
            has_texture_compression_dxt1_ext_ = true;
            has_texture_compression_dxt3_ext_ = true;
            has_texture_compression_dxt5_ext_ = true;
            LOGI("Has extension: EXT_texture_compression_s3tc");
        }

        if (CheckExtension("OES_compressed_ETC1_RGB8_texture"))
        {
            has_compressed_ETC1_RGB8_texture_ext_ = true;
            LOGI("Has extension: OES_compressed_ETC1_RGB8_texture");
        }

        if (CheckExtension("IMG_texture_compression_pvrtc"))
        {
            has_texture_compression_pvrtc_ext_ = true;
            LOGI("Has extension: IMG_texture_compression_pvrtc");
        }

        if (CheckExtension("EXT_discard_framebuffer"))
        {
            has_discard_framebuffer_ext_ = true;
            LOGI("Using extension: EXT_discard_framebuffer");
        }

        if (CheckExtension("OES_vertex_array_object") || CheckExtension("ARB_vertex_array_object"))
        {
            has_vertex_array_object_ext_ = true;
            LOGI("Using extension: vertex_array_object");
        }

        if (CheckExtension("EXT_map_buffer_range"))
        {
            has_map_buffer_range_ext_ = true;
            LOGI("Using extension: EXT_map_buffer_range");
        }

        if (CheckExtension("GL_OES_depth_texture"))
        {
            has_depth_texture_ext_ = true;
            LOGI("Using extension: GL_OES_depth_texture");
        }

        if (CheckExtension("GL_ARB_depth_texture"))
        {
            has_depth_texture_ext_ = true;
            LOGI("Using extension: GL_ARB_depth_texture");
        }


        if (CheckExtension("GL_OES_depth24"))
        {
            has_depth_component24_ext_ = true;
            LOGI("Using extension: GL_OES_depth24");
        }

        if (CheckExtension("GL_EXT_packed_depth_stencil") || CheckExtension("GL_OES_packed_depth_stencil"))
        {
            has_packed_depth_stencil_ext_ = true;
            LOGI("Using extension: packed_depth_stencil");
        }

        if (CheckExtension("GL_ARB_texture_non_power_of_two"))
        {
            has_texture_non_power_of_two_ext_ = true;
            LOGI("Using extension: GL_ARB_texture_non_power_of_two");
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
                    LOGI("Using extension: instanced_arrays");
                }
                else
                {
                    LOGW("Has extension: instanced_arrays");
                    LOGW("Needed %d but graphics only supports %d attributes", attributesNeeded, maxVertexAtts);
                    LOGI("Disabling extension: instanced_arrays");
                }
            }
        }
        #endif


        {
            glGetIntegerv(GL_MAX_VARYING_VECTORS, &maxVaryingVectors_);
            GLenum status = glGetError();
            if (status == GL_NO_ERROR)
            {
                LOGI("GL_MAX_VARYING_VECTORS = %d", maxVaryingVectors_);
            }
            else
            {
                #ifdef GL_MAX_VARYING_COMPONENTS
                glGetIntegerv(GL_MAX_VARYING_COMPONENTS, &maxVaryingVectors_);
                status = glGetError();
                if (status == GL_NO_ERROR)
                {
                    maxVaryingVectors_ /= 4;
                    LOGI("GL_MAX_VARYING_VECTORS = %d", maxVaryingVectors_);
                }
                else
                #endif
                {
                    maxVaryingVectors_ = 8;
                    LOGW("Unknown GL_MAX_VARYING_VECTORS. Setting value to %d", maxVaryingVectors_);

                }
            }
        }

        {
            glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &maxVertexUniformVectors_);
            GLenum status = glGetError();
            if (status == GL_NO_ERROR)
            {
                LOGI("GL_MAX_VERTEX_UNIFORM_VECTORS = %d", maxVertexUniformVectors_);
            }
            else
            {
                #ifdef GL_MAX_VERTEX_UNIFORM_COMPONENTS
                glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniformVectors_);
                status = glGetError();
                if (status == GL_NO_ERROR)
                {
                    LOGI("GL_MAX_VERTEX_UNIFORM_VECTORS = %d", maxVertexUniformVectors_);
                }
                else
                #endif
                {
                    maxVaryingVectors_ = 128;
                    LOGW("Unknown GL_MAX_VERTEX_UNIFORM_VECTORS. Setting value to %d", maxVertexUniformVectors_);
                }
            }
        }

        {
            glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &maxFragmentUniformVectors_);
            GLenum status = glGetError();
            if (status == GL_NO_ERROR)
            {
                LOGI("GL_MAX_FRAGMENT_UNIFORM_VECTORS = %d", maxFragmentUniformVectors_);
            }
            else
            {
                #ifdef GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
                glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniformVectors_);
                status = glGetError();
                if (status == GL_NO_ERROR)
                {
                    LOGI("GL_MAX_FRAGMENT_UNIFORM_VECTORS = %d", maxFragmentUniformVectors_);
                }
                else
                #endif
                {
                    maxVaryingVectors_ = 128;
                    LOGW("Unknown GL_MAX_FRAGMENT_UNIFORM_VECTORS. Setting value to %d", maxFragmentUniformVectors_);
                }
            }
        }

        {
            glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs_);
            LOGI("GL_MAX_VERTEX_ATTRIBS = %d", maxVertexAttribs_);
        }

        // Set up texture data read/write alignment
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    Graphics::~Graphics()
    {
        Graphics::Destroy();
    }

    void Graphics::CreateGUI(Window* mainWindow)
    {
        imgui_ = std::make_shared<GUI>(mainWindow);
    }

    void Graphics::DestroyGUI()
    {
        imgui_ = nullptr;
    }

    bool Graphics::CheckExtension(const std::string& name)
    {
        std::string extensions = (const char*)glGetString(GL_EXTENSIONS);
        return extensions.find(name) != std::string::npos;
    }

    void Graphics::ResetCachedState()
    {
        Program::Clear();

        viewport_ = Recti(0);

        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &systemFbo_); // On IOS default FBO is not zero

        // Set up texture data read/write alignment
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        VertexArrayObj::Clear();
        lastMesh_ = nullptr;
        lastProgram_ = nullptr;
        activeMesh_ = nullptr;
        activeWindow_ = nullptr;

        CHECK_GL_STATUS();

        SetClearColor(Color(0, 0, 0, 1));
        SetClearDepth(1);
        SetClearStencil(0);
        SetFrameBuffer(nullptr);
        SetStencilTest(DEFAULT_STENCIL_ENABLE, DEFAULT_STENCIL_WRITEMASK, DEFAULT_STENCIL_SFAIL,
                       DEFAULT_STENCIL_DPFAIL, DEFAULT_STENCIL_DPPASS, DEFAULT_STENCIL_FUNC, DEFAULT_STENCIL_REF, DEFAULT_STENCIL_COMPAREMASK);
        SetScissorTest();

        CHECK_GL_STATUS();

        SetColorMask(DEFAULT_COLOR_MASK);
        SetDepthMask(DEFAULT_DEPTH_MASK);
        SetDepthFunc(DepthFunc::LESS);
        SetStencilMask(DEFAULT_STENCIL_MASK);
        SetBlendModeTest(DEFAULT_BLEND_MODE);
        EnableDepthTest(DEFAULT_DEPTH_TEST_ENABLE);
        EnableCullFace(DEFAULT_CULL_FACE_ENABLE);
        SetCullFace(CullFaceMode::DEFAULT);
        SetFrontFace(FrontFaceMode::DEFAULT);
        CHECK_GL_STATUS();

        UnboundTextures();
        SetVertexArrayObj(nullptr);
        SetVertexBuffer(nullptr);
        SetIndexBuffer(nullptr);
        SetProgram(nullptr);
        SetSlopeScaledBias(0);

        CHECK_GL_STATUS();
    }

    void Graphics::UnboundTextures()
    {
        for (int i = 0; i < maxTexturesCombined_; i++)
            SetTexture(i, nullptr);
    }

    FrameBuffer* Graphics::SetFrameBuffer(FrameBuffer* buffer)
    {
        auto old = currentFbo_;
        if (buffer != currentFbo_)
        {
            currentFbo_ = buffer;
            if (buffer == nullptr)
            {
                currentColorTarget_ = TextureTarget::UNKNOWN;
                glBindFramebuffer(GL_FRAMEBUFFER, systemFbo_);
            }
            else
            {
                currentColorTarget_ = buffer->GetDefaultTextureTarget();
                glBindFramebuffer(GL_FRAMEBUFFER, buffer->GetId());
                buffer->AttachTarget(currentColorTarget_);
            }
            SetUpViewport();
        }
        return old;
    }


    FrameBuffer* Graphics::SetFrameBuffer(FrameBuffer* buffer, TextureTarget colorTarget)
    {
        auto old = currentFbo_;
        if (buffer != currentFbo_ || currentColorTarget_ != colorTarget)
        {
            currentColorTarget_ = colorTarget;
            currentFbo_ = buffer;
            if (buffer == nullptr)
                glBindFramebuffer(GL_FRAMEBUFFER, systemFbo_);
            else
            {
                glBindFramebuffer(GL_FRAMEBUFFER, buffer->GetId());
                buffer->AttachTarget(colorTarget);
            }
            SetUpViewport();
        }
        return old;
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
        SetScissorTest();
        SetColorMask(true);
        SetDepthMask(true);
        SetStencilMask(~GLuint(0));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void Graphics::ClearBuffers(bool color, bool depth, bool stencil)
    {
        SetScissorTest();
        GLbitfield mask(0);
        if (color)
        {
            mask |= GL_COLOR_BUFFER_BIT;
            SetColorMask(true);
        }

        if (depth)
        {
            mask |= GL_DEPTH_BUFFER_BIT;
            SetDepthMask(true);
        }

        if (stencil)
        {
            mask |= GL_STENCIL_BUFFER_BIT;
            SetStencilMask(~GLuint(0));
        }

        glClear(mask);
    }

    void Graphics::ClearStencilBuffer(GLint value)
    {
        SetScissorTest();
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

    void Graphics::SetScissorTest(bool enable, GLint x, GLint y, GLsizei width, GLsizei height)
    {
        static bool enable_ = false;

        if (enable != enable_)
        {
            if (enable)
                glEnable(GL_SCISSOR_TEST);
            else
                glDisable(GL_SCISSOR_TEST);

            enable_ = enable;
        }

        if (enable)
        {
            static GLint x_ = 0;
            static GLint y_ = 0;
            static GLsizei width_ = 0;
            static GLsizei height_ = 0;

            if (x != x_ || y != y_ || width != width_ || height != height_)
            {
                glScissor(x, y, width, height);
                x_ = x;
                y_ = y;
                width_ = width;
                height_ = height;
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

    void Graphics::SetDepthFunc(DepthFunc depthFunc)
    {
        if (depthFunc_ != depthFunc)
        {
            switch (depthFunc)
            {
                case DepthFunc::NEVER:
                    glDepthFunc(GL_NEVER);
                    break;
                case DepthFunc::LESS:
                    glDepthFunc(GL_LESS);
                    break;
                case DepthFunc::EQUAL:
                    glDepthFunc(GL_EQUAL);
                    break;
                case DepthFunc::LEQUAL:
                    glDepthFunc(GL_LEQUAL);
                    break;
                case DepthFunc::GREATER:
                    glDepthFunc(GL_GREATER);
                    break;
                case DepthFunc::NOTEQUAL:
                    glDepthFunc(GL_NOTEQUAL);
                    break;
                case DepthFunc::GEQUAL:
                    glDepthFunc(GL_GEQUAL);
                    break;
                case DepthFunc::ALWAYS:
                    glDepthFunc(GL_ALWAYS);
                    break;
                default:
                    CHECK_ASSERT(false && "Invalid depth function");
                    break;
            }
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
                case BLEND_MODE::NONE:
                    glDisable(GL_BLEND);
                    if (blendSFactor_ != GL_ONE || blendDFactor_ != GL_ZERO)
                    {
                        glBlendFunc(GL_ONE, GL_ZERO);
                        blendSFactor_ = GL_ONE;
                        blendDFactor_ = GL_ZERO;
                    }
                    break;

                case BLEND_MODE::MULTIPLICATIVE:
                    glEnable(GL_BLEND);
                    if (blendSFactor_ != GL_ZERO || blendDFactor_ != GL_SRC_COLOR)
                    {
                        glBlendFunc(GL_ZERO, GL_SRC_COLOR);
                        blendSFactor_ = GL_ZERO;
                        blendDFactor_ = GL_SRC_COLOR;
                    }
                    break;

                case BLEND_MODE::ADDITIVE:
                    glEnable(GL_BLEND);
                    if (blendSFactor_ != GL_ONE || blendDFactor_ != GL_ONE)
                    {
                        glBlendFunc(GL_ONE, GL_ONE);
                        blendSFactor_ = GL_ONE;
                        blendDFactor_ = GL_ONE;
                    }
                    break;

                case BLEND_MODE::ALPHA:
                    glEnable(GL_BLEND);
                    if (blendSFactor_ != GL_SRC_ALPHA || blendDFactor_ != GL_ONE_MINUS_SRC_ALPHA)
                    {
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                        blendSFactor_ = GL_SRC_ALPHA;
                        blendDFactor_ = GL_ONE_MINUS_SRC_ALPHA;
                    }
                    break;

                default:
                    CHECK_ASSERT(false && "Undefined blend mode");
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
            cullFaceMode_ = mode;
            switch (mode)
            {
                case CullFaceMode::BACK:
                    glCullFace(GL_BACK);
                    break;
                case CullFaceMode::FRONT:
                    glCullFace(GL_FRONT);
                    break;
                case CullFaceMode::FRONT_AND_BACK:
                    glCullFace(GL_FRONT_AND_BACK);
                    break;
                default:
                    CHECK_ASSERT(!"Unknown CullFaceMode!!!");
                    break;
            }
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

    void Graphics::SetTexture(int index, GLuint id, GLenum target)
    {
        CHECK_CONDITION(index < maxTexturesCombined_);

        auto currentTexture = textures_[index];
        if (activeTexture_ != index)
        {
            glActiveTexture(GL_TEXTURE0 + index);
            textures_[index] = nullptr;
            glBindTexture(target, id);
            activeTexture_ = index;
        }
        else if (!currentTexture || currentTexture->GetID() != id)
        {
            textures_[index] = nullptr;
            glBindTexture(target, id);
        }
    }

    void Graphics::SetTexture(int index, Texture* texture)
    {
        CHECK_CONDITION(index < maxTexturesCombined_);

        if (texture)
        {
            if (activeTexture_ != index)
            {
                glActiveTexture(GL_TEXTURE0 + index);
                textures_[index] = texture;
                glBindTexture(texture->GetTarget(), texture->GetID());
                activeTexture_ = index;
            }
            else if (textures_[index] != texture)
            {
                textures_[index] = texture;
                glBindTexture(texture->GetTarget(), texture->GetID());
            }
        }
        else
        {
            if (activeTexture_ == index && index > 0)
            {
                glActiveTexture(GL_TEXTURE0); //default
                activeTexture_ = 0;
                glBindTexture(textures_[index]->GetTarget(), 0);
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
                if (!program->IsReady())
                    return false;
                glUseProgram(program->GetId());
            }
            else
                glUseProgram(0);
            activeProgram_ = program;
        }
        return true;
    }

    void Graphics::SetWindow(Window* window)
    {
        if (activeWindow_ != window)
        {
            activeWindow_ = window;
            if (window)
            {
                window->SetContext();
                SetUpViewport();
                Graphics::SigWindow()->Run(window);
            }
        }
    }

    void Graphics::SetUpViewport()
    {
        if (currentFbo_)
        {
            auto width = currentFbo_->GetWidth();
            auto height = currentFbo_->GetHeight();
            SetViewport(Recti(0, 0, width, height), false);
        }
        else if (activeWindow_)
        {
            auto width = activeWindow_->GetWidth();
            auto height = activeWindow_->GetHeight();
            SetViewport(Recti(0, 0, width, height), false);
        }
        else
            SetViewport(Recti(0, 0, 0, 0), false);

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

    void Graphics::SetBuffers(bool solid, bool allowInstancing)
    {
        VertexBuffer* vBuffer = activeMesh_->GetVertexBuffer();
        if (has_vertex_array_object_ext_ && !vBuffer->IsDynamic())
        {
            auto vao = VertexArrayObj::GetOrCreate(VAOKey{allowInstancing, activeProgram_, activeMesh_, solid});
            vao->Use();
        }
        else
        {
            SetVertexBuffer(vBuffer);
            SetAttributes(nullptr);
            if (allowInstancing && activeProgram_->GetMaterial()->IsBatched() && !vBuffer->IsDynamic())
                SetInstanceAttrPointers(activeProgram_);
            SetIndexBuffer(activeMesh_->GetIndexBuffer(solid));
        }
    }

    void Graphics::SetInstanceAttrPointers(Program* program)
    {
        if (!HasInstancedArrays())
            return;

        CHECK_ASSERT(program->GetMaterial()->IsBatched());

        CHECK_GL_STATUS();

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

        CHECK_GL_STATUS();
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

    void Graphics::SetAttributes(SetAttPointersFunction setAttPointersCallBack)
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

            if (setAttPointersCallBack)
                setAttPointersCallBack();
            else
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

    void Graphics::SetupPass(const Pass* pass)
    {
        CHECK_GL_STATUS();

        auto& data = pass->GetData();

        SetColorMask(data.enableColorBuffer_);

        SetStencilTest(data.enableStencilTest_, data.stencilMask_, data.sfailStencilOp_,
                       data.dpfailStencilOp_, data.dppassStencilOp_, data.stencilFunc_,
                       data.stencilRefValue_, data.stencilMaskValue_);

        SetScissorTest(data.enableScissorTest_,
                       data.scissorX_, data.scissorY_,
                       data.scissorWidth_, data.scissorHeight_);

        SetBlendModeTest(pass->GetBlendMode());
        EnableDepthTest(data.enableDepthTest_);
        if (data.enableDepthTest_)
        {
            SetDepthMask(data.enableDepthBuffer_);
            SetDepthFunc(data.depthFunc_);
        }

        SetFrontFace(data.frontFaceMode_);

        if (data.cullFaceMode_ != CullFaceMode::DISABLED)
        {
            EnableCullFace(true);
            SetCullFace(data.cullFaceMode_);
        }
        else
            EnableCullFace(false);

        CHECK_GL_STATUS();

    }

    bool Graphics::SetupProgram(const Pass* pass, const Scene* scene, const Camera* camera, SceneNode* sceneNode, Material* material, const Light* light)
    {
        SetupPass(pass);

        if (material)
        {
            auto cullFaceMode = material->GetCullFaceMode();
            if (cullFaceMode != CullFaceMode::DISABLED)
            {
                EnableCullFace(true);
                SetCullFace(cullFaceMode);
            }
            else
                EnableCullFace(false);
        }

        auto shaderdefines = Program::GetShaderVariation(pass, scene, camera, activeMesh_, material, light, sceneNode);
        auto program = Program::GetOrCreate(shaderdefines);
        program->Set(sceneNode);
        program->Set(material);
        program->Set(light);
        program->Set(camera);
        program->Set(scene);
        bool ready = SetProgram(program.get());
        if (ready)
        {
            auto shadowPass = PassType::SHADOW == pass->GetType();
            program->SetVariables(shadowPass);
        }
        CHECK_GL_STATUS();
        return ready;
    }

    void Graphics::DrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
    {
        glDrawElements(mode, count, type, indices);
        lastMesh_ = activeMesh_;
        lastProgram_ = activeProgram_;
    }

    void Graphics::DrawArrays(GLenum mode, GLint first, GLsizei count)
    {
        glDrawArrays(mode, first, count);
        lastMesh_ = activeMesh_;
        lastProgram_ = activeProgram_;
    }

    void Graphics::DrawActiveMesh()
    {
        if (!activeMesh_->IsReady())
            return;

        CHECK_GL_STATUS();

        bool solid = activeProgram_->GetMaterial()->GetFillMode() == FillMode::SOLID;
        SetBuffers(solid, false);
        CHECK_GL_STATUS();
        GLenum mode = solid ? activeMesh_->GetSolidDrawMode() : activeMesh_->GetWireFrameDrawMode();
        const VertexsData& vertexsData = activeMesh_->GetVertexsData();
        const Indexes& indexes = activeMesh_->GetIndexes(solid);

        if (!indexes.empty())
            glDrawElements(mode, GLsizei(indexes.size()), GL_UNSIGNED_SHORT, 0);
        else
            glDrawArrays(mode, 0, GLsizei(vertexsData.size()));

        SetVertexArrayObj(nullptr);

        lastMesh_ = activeMesh_;
        lastProgram_ = activeProgram_;

        CHECK_GL_STATUS();
    }

    void Graphics::DrawInstancedActiveMesh(const Batch& batch)
    {
        CHECK_ASSERT(has_instanced_arrays_ext_);

        if (!activeMesh_->IsReady())
            return;

        CHECK_GL_STATUS();

        bool solid = activeProgram_->GetMaterial()->GetFillMode() == FillMode::SOLID;
        activeProgram_->GetMaterial()->UpdateBatchBuffer(batch);
        SetBuffers(solid, true);
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
        lastProgram_ = activeProgram_;

        CHECK_GL_STATUS();
    }


    bool Graphics::IsTextureSizeCorrect(unsigned width, unsigned height)
    {
        if (width > (unsigned)maxTextureSize_ || height > (unsigned)maxTextureSize_)
            return false;
        return HasNonPowerOfTwo() || (IsPowerOfTwo(width) && IsPowerOfTwo(height));
    }


    bool Graphics::NeedsDecompress(TextureFormat format) const
    {
        switch (format)
        {
            case TextureFormat::DXT1:
                return !HasTextureCompressionDXT1();
            case TextureFormat::DXT3:
                return !HasTextureCompressionDXT3();
            case TextureFormat::DXT5:
                return !HasTextureCompressionDXT5();
            case TextureFormat::ETC1:
                return !HasTextureCompressionETC();
            case TextureFormat::PVRTC_RGB_2BPP:
            case TextureFormat::PVRTC_RGBA_2BPP:
            case TextureFormat::PVRTC_RGB_4BPP:
            case TextureFormat::PVRTC_RGBA_4BPP:
                return !HasTextureCompressionPVRTC();
            case TextureFormat::RGBA:
                return false;
            default:
                CHECK_CONDITION(!"Unknown texture format!!!");
                break;
        }
        return false;
    }

    GLenum Graphics::GetTexelDataType()
    {
        return GL_UNSIGNED_BYTE;
    }

    GLenum Graphics::GetTexelFormatType()
    {
        return GL_RGBA;
    }

    void Graphics::SetSlopeScaledBias(float slopeScaledBias)
    {
        if (slopeScaledBias != slopeScaledDepthBias_)
        {
            if (slopeScaledBias != 0.0f)
            {
                float adjustedSlopeScaledBias = slopeScaledBias + 1.0f;
                glEnable(GL_POLYGON_OFFSET_FILL);
                glPolygonOffset(adjustedSlopeScaledBias, 0.0f);
            }
            else
                glDisable(GL_POLYGON_OFFSET_FILL);

            slopeScaledDepthBias_ = slopeScaledBias;
        }
    }


    SignalWindow::PSignal Graphics::SigWindow()
    {
        static SignalWindow::PSignal signalWindow(new SignalWindow);
        return signalWindow;
    }
}