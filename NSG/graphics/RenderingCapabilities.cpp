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
#include "RenderingCapabilities.h"
#include "Check.h"
#include "GLIncludes.h"
#include "Log.h"

#if defined(ANDROID) || defined(EMSCRIPTEN)
PFNGLDISCARDFRAMEBUFFEREXTPROC glDiscardFramebufferEXT;
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOES;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOES;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOES;
PFNGLISVERTEXARRAYOESPROC glIsVertexArrayOES;
PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisorEXT;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstancedEXT;
PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstancedEXT;
#elif defined(IS_TARGET_WINDOWS)
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
PFNGLFLUSHMAPPEDBUFFERRANGEAPPLEPROC glFlushMappedBufferRange;
PFNGLUNMAPBUFFERPROC glUnmapBuffer;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORM2FVPROC glUniform2fv;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
PFNGLUNIFORM4FVPROC glUniform4fv;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
#endif

namespace NSG {
RenderingCapabilities::RenderingCapabilities()
    : has_discard_framebuffer_ext_(false), has_vertex_array_object_ext_(false),
      has_map_buffer_range_ext_(false), has_depth_texture_ext_(false),
      has_depth_component24_ext_(false),
      has_texture_non_power_of_two_ext_(false),
      has_instanced_arrays_ext_(false), has_packed_depth_stencil_ext_(false),
      has_texture_compression_dxt1_ext_(false),
      has_texture_compression_dxt3_ext_(false),
      has_texture_compression_dxt5_ext_(false),
      has_compressed_ETC1_RGB8_texture_ext_(false),
      has_texture_compression_pvrtc_ext_(false), maxVaryingVectors_(0),
      maxTexturesCombined_(0), maxVertexUniformVectors_(0),
      maxFragmentUniformVectors_(0), maxVertexAttribs_(0), maxTextureSize_(64) {

#if defined(ANDROID) || defined(EMSCRIPTEN)
    {
        glDiscardFramebufferEXT =
            (PFNGLDISCARDFRAMEBUFFEREXTPROC)eglGetProcAddress(
                "glDiscardFramebufferEXT");
        glGenVertexArraysOES = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress(
            "glGenVertexArraysOES");
        glBindVertexArrayOES = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress(
            "glBindVertexArrayOES");
        glDeleteVertexArraysOES =
            (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress(
                "glDeleteVertexArraysOES");
        glIsVertexArrayOES =
            (PFNGLISVERTEXARRAYOESPROC)eglGetProcAddress("glIsVertexArrayOES");
        glVertexAttribDivisorEXT =
            (PFNGLVERTEXATTRIBDIVISORPROC)eglGetProcAddress(
                "glVertexAttribDivisorEXT");
        glDrawElementsInstancedEXT =
            (PFNGLDRAWELEMENTSINSTANCEDPROC)eglGetProcAddress(
                "glDrawElementsInstancedEXT");
        glDrawArraysInstancedEXT =
            (PFNGLDRAWARRAYSINSTANCEDPROC)eglGetProcAddress(
                "glDrawArraysInstancedEXT");
    }
#elif defined(IS_TARGET_WINDOWS)
    {
        glGenBuffers =
            (PFNGLGENBUFFERSPROC)wglGetProcAddress((LPCSTR) "glGenBuffers");
        glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress(
            (LPCSTR) "glDeleteBuffers");
        glBindBuffer =
            (PFNGLBINDBUFFERPROC)wglGetProcAddress((LPCSTR) "glBindBuffer");
        glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)wglGetProcAddress(
            (LPCSTR) "glMapBufferRange");
        glFlushMappedBufferRange =
            (PFNGLFLUSHMAPPEDBUFFERRANGEAPPLEPROC)wglGetProcAddress(
                (LPCSTR) "glFlushMappedBufferRange");
        glUnmapBuffer =
            (PFNGLUNMAPBUFFERPROC)wglGetProcAddress((LPCSTR) "glUnmapBuffer");
        glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress(
            (LPCSTR) "glBufferSubData");
        glBufferData =
            (PFNGLBUFFERDATAPROC)wglGetProcAddress((LPCSTR) "glBufferData");
        glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress(
            (LPCSTR) "glGenFramebuffers");
        glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress(
            (LPCSTR) "glGenRenderbuffers");
        glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress(
            (LPCSTR) "glBindRenderbuffer");
        glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress(
            (LPCSTR) "glRenderbufferStorage");
        glFramebufferRenderbuffer =
            (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress(
                (LPCSTR) "glFramebufferRenderbuffer");
        glCheckFramebufferStatus =
            (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress(
                (LPCSTR) "glCheckFramebufferStatus");
        glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress(
            (LPCSTR) "glDeleteRenderbuffers");
        glFramebufferTexture2D =
            (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress(
                (LPCSTR) "glFramebufferTexture2D");
        glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress(
            (LPCSTR) "glDeleteFramebuffers");
        glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress(
            (LPCSTR) "glGenVertexArrays");
        glEnableVertexAttribArray =
            (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress(
                (LPCSTR) "glEnableVertexAttribArray");
        glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress(
            (LPCSTR) "glDeleteVertexArrays");
        glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress(
            (LPCSTR) "glBindVertexArray");
        glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress(
            (LPCSTR) "glVertexAttribPointer");
        glDetachShader =
            (PFNGLDETACHSHADERPROC)wglGetProcAddress((LPCSTR) "glDetachShader");
        glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress(
            (LPCSTR) "glDeleteProgram");
        glCreateShader =
            (PFNGLCREATESHADERPROC)wglGetProcAddress((LPCSTR) "glCreateShader");
        glShaderSource =
            (PFNGLSHADERSOURCEPROC)wglGetProcAddress((LPCSTR) "glShaderSource");
        glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress(
            (LPCSTR) "glCompileShader");
        glGetShaderiv =
            (PFNGLGETSHADERIVPROC)wglGetProcAddress((LPCSTR) "glGetShaderiv");
        glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress(
            (LPCSTR) "glGetShaderInfoLog");
        glDeleteShader =
            (PFNGLDELETESHADERPROC)wglGetProcAddress((LPCSTR) "glDeleteShader");
        glUniform1i =
            (PFNGLUNIFORM1IPROC)wglGetProcAddress((LPCSTR) "glUniform1i");
        glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress(
            (LPCSTR) "glCreateProgram");
        glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress(
            (LPCSTR) "glBindAttribLocation");
        glAttachShader =
            (PFNGLATTACHSHADERPROC)wglGetProcAddress((LPCSTR) "glAttachShader");
        glLinkProgram =
            (PFNGLLINKPROGRAMPROC)wglGetProcAddress((LPCSTR) "glLinkProgram");
        glGetProgramiv =
            (PFNGLGETPROGRAMIVPROC)wglGetProcAddress((LPCSTR) "glGetProgramiv");
        glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress(
            (LPCSTR) "glGetProgramInfoLog");
        glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress(
            (LPCSTR) "glGetAttribLocation");
        glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress(
            (LPCSTR) "glGetUniformLocation");
        glUniform2fv =
            (PFNGLUNIFORM2FVPROC)wglGetProcAddress((LPCSTR) "glUniform2fv");
        glUniform3fv =
            (PFNGLUNIFORM3FVPROC)wglGetProcAddress((LPCSTR) "glUniform3fv");
        glUniform1f =
            (PFNGLUNIFORM1FPROC)wglGetProcAddress((LPCSTR) "glUniform1f");
        glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress(
            (LPCSTR) "glUniformMatrix4fv");
        glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress(
            (LPCSTR) "glUniformMatrix3fv");
        glUniform4fv =
            (PFNGLUNIFORM4FVPROC)wglGetProcAddress((LPCSTR) "glUniform4fv");
        glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress(
            (LPCSTR) "glBindFramebuffer");
        glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress(
            (LPCSTR) "glActiveTexture");
        glUseProgram =
            (PFNGLUSEPROGRAMPROC)wglGetProcAddress((LPCSTR) "glUseProgram");
        glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)wglGetProcAddress(
            (LPCSTR) "glVertexAttribDivisor");
        glDisableVertexAttribArray =
            (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress(
                (LPCSTR) "glDisableVertexAttribArray");
        glDrawElementsInstanced =
            (PFNGLDRAWELEMENTSINSTANCEDPROC)wglGetProcAddress(
                (LPCSTR) "glDrawElementsInstanced");
        glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)wglGetProcAddress(
            (LPCSTR) "glDrawArraysInstanced");
        glCompressedTexImage2D =
            (PFNGLCOMPRESSEDTEXIMAGE2DPROC)wglGetProcAddress(
                (LPCSTR) "glCompressedTexImage2D");
        glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress(
            (LPCSTR) "glGenerateMipmap");
    }
#endif

    LOGI("GL_VENDOR = %s", (const char*)glGetString(GL_VENDOR));
    LOGI("GL_RENDERER = %s", (const char*)glGetString(GL_RENDERER));
    LOGI("GL_VERSION = %s", (const char*)glGetString(GL_VERSION));
    LOGI("GL_SHADING_LANGUAGE_VERSION = %s",
         (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    extensions_ = RenderingCapabilities::GetExtensions();
    LOGI("GL_EXTENSIONS = %s", extensions_.c_str());
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize_);
    CHECK_ASSERT(maxTextureSize_ >= 64);
    LOGI("GL_MAX_TEXTURE_SIZE = %d", maxTextureSize_);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexturesCombined_);
    LOGI("GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS = %d", maxTexturesCombined_);
    CHECK_CONDITION(maxTexturesCombined_ >= MaterialTexture::MAX_MAPS);
    maxTexturesCombined_ = MaterialTexture::MAX_MAPS;

    if (CheckExtension("EXT_texture_compression_dxt1")) {
        has_texture_compression_dxt1_ext_ = true;
        LOGI("Has extension: EXT_texture_compression_dxt1");
    }

    if (CheckExtension("WEBGL_compressed_texture_s3tc")) {
        has_texture_compression_dxt1_ext_ = true;
        has_texture_compression_dxt3_ext_ = true;
        has_texture_compression_dxt5_ext_ = true;
        LOGI("Has extension: WEBGL_compressed_texture_s3tc");
    }

    if (CheckExtension("EXT_texture_compression_s3tc")) {
        has_texture_compression_dxt1_ext_ = true;
        has_texture_compression_dxt3_ext_ = true;
        has_texture_compression_dxt5_ext_ = true;
        LOGI("Has extension: EXT_texture_compression_s3tc");
    }

    if (CheckExtension("OES_compressed_ETC1_RGB8_texture")) {
        has_compressed_ETC1_RGB8_texture_ext_ = true;
        LOGI("Has extension: OES_compressed_ETC1_RGB8_texture");
    }

    if (CheckExtension("IMG_texture_compression_pvrtc")) {
        has_texture_compression_pvrtc_ext_ = true;
        LOGI("Has extension: IMG_texture_compression_pvrtc");
    }

    if (CheckExtension("EXT_discard_framebuffer")) {
        has_discard_framebuffer_ext_ = true;
        LOGI("Using extension: EXT_discard_framebuffer");
    }

    if (CheckExtension("OES_vertex_array_object") ||
        CheckExtension("ARB_vertex_array_object")) {
        has_vertex_array_object_ext_ = true;
        LOGI("Using extension: vertex_array_object");
    }

    if (CheckExtension("EXT_map_buffer_range")) {
        has_map_buffer_range_ext_ = true;
        LOGI("Using extension: EXT_map_buffer_range");
    }

    if (CheckExtension("GL_OES_depth_texture")) {
        has_depth_texture_ext_ = true;
        LOGI("Using extension: GL_OES_depth_texture");
    }

    if (CheckExtension("GL_ARB_depth_texture")) {
        has_depth_texture_ext_ = true;
        LOGI("Using extension: GL_ARB_depth_texture");
    }

    if (CheckExtension("GL_OES_depth24")) {
        has_depth_component24_ext_ = true;
        LOGI("Using extension: GL_OES_depth24");
    }

    if (CheckExtension("GL_EXT_packed_depth_stencil") ||
        CheckExtension("GL_OES_packed_depth_stencil")) {
        has_packed_depth_stencil_ext_ = true;
        LOGI("Using extension: packed_depth_stencil");
    }

    if (CheckExtension("GL_ARB_texture_non_power_of_two")) {
        has_texture_non_power_of_two_ext_ = true;
        LOGI("Using extension: GL_ARB_texture_non_power_of_two");
    }

#if !defined(EMSCRIPTEN)
    {
        if (CheckExtension("GL_EXT_instanced_arrays") ||
            CheckExtension("GL_ARB_instanced_arrays") ||
            CheckExtension("GL_ANGLE_instanced_arrays")) {

            GLint maxVertexAtts = 0;
            glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAtts);
            int attributesNeeded = (int)AttributesLoc::MAX_ATTS;
            if (maxVertexAtts >= attributesNeeded) {
                has_instanced_arrays_ext_ = true;
                LOGI("Using extension: instanced_arrays");
            } else {
                LOGW("Has extension: instanced_arrays");
                LOGW("Needed %d but graphics only supports %d attributes",
                     attributesNeeded, maxVertexAtts);
                LOGI("Disabling extension: instanced_arrays");
            }
        }
    }
#endif

    {
        glGetIntegerv(GL_MAX_VARYING_VECTORS, &maxVaryingVectors_);
        GLenum status = glGetError();
        if (status == GL_NO_ERROR) {
            LOGI("GL_MAX_VARYING_VECTORS = %d", maxVaryingVectors_);
        } else {
#ifdef GL_MAX_VARYING_COMPONENTS
            glGetIntegerv(GL_MAX_VARYING_COMPONENTS, &maxVaryingVectors_);
            status = glGetError();
            if (status == GL_NO_ERROR) {
                maxVaryingVectors_ /= 4;
                LOGI("GL_MAX_VARYING_VECTORS = %d", maxVaryingVectors_);
            } else
#endif
            {
                maxVaryingVectors_ = 8;
                LOGW("Unknown GL_MAX_VARYING_VECTORS. Setting value to %d",
                     maxVaryingVectors_);
            }
        }
    }

    {
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &maxVertexUniformVectors_);
        GLenum status = glGetError();
        if (status == GL_NO_ERROR) {
            LOGI("GL_MAX_VERTEX_UNIFORM_VECTORS = %d",
                 maxVertexUniformVectors_);
        } else {
#ifdef GL_MAX_VERTEX_UNIFORM_COMPONENTS
            glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS,
                          &maxVertexUniformVectors_);
            status = glGetError();
            if (status == GL_NO_ERROR) {
                LOGI("GL_MAX_VERTEX_UNIFORM_VECTORS = %d",
                     maxVertexUniformVectors_);
            } else
#endif
            {
                maxVaryingVectors_ = 128;
                LOGW("Unknown GL_MAX_VERTEX_UNIFORM_VECTORS. Setting value to "
                     "%d",
                     maxVertexUniformVectors_);
            }
        }
    }

    {
        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS,
                      &maxFragmentUniformVectors_);
        GLenum status = glGetError();
        if (status == GL_NO_ERROR) {
            LOGI("GL_MAX_FRAGMENT_UNIFORM_VECTORS = %d",
                 maxFragmentUniformVectors_);
        } else {
#ifdef GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
            glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
                          &maxFragmentUniformVectors_);
            status = glGetError();
            if (status == GL_NO_ERROR) {
                LOGI("GL_MAX_FRAGMENT_UNIFORM_VECTORS = %d",
                     maxFragmentUniformVectors_);
            } else
#endif
            {
                maxVaryingVectors_ = 128;
                LOGW("Unknown GL_MAX_FRAGMENT_UNIFORM_VECTORS. Setting value "
                     "to %d",
                     maxFragmentUniformVectors_);
            }
        }
    }

    {
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs_);
        LOGI("GL_MAX_VERTEX_ATTRIBS = %d", maxVertexAttribs_);
    }

    CHECK_GL_STATUS();
}

RenderingCapabilities::~RenderingCapabilities() {}

std::string RenderingCapabilities::GetExtensions() {
#ifndef GL_NUM_EXTENSIONS
    return (const char*)glGetString(GL_EXTENSIONS);
#else
    std::string extensions;
    GLint numExtensions = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
    for (auto index = 0; index < numExtensions; ++index) {
        const char* name = (const char*)glGetStringi(GL_EXTENSIONS, index);
        extensions += name;
        extensions += " ";
    }
    return extensions;
#endif
}

bool RenderingCapabilities::CheckExtension(const std::string& name) {
    return extensions_.find(name) != std::string::npos;
}

GLenum RenderingCapabilities::GetTexelDataType() { return GL_UNSIGNED_BYTE; }

GLenum RenderingCapabilities::GetTexelFormatType() { return GL_RGBA; }
}
