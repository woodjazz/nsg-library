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
#pragma once

#if defined(ANDROID)
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
extern PFNGLDISCARDFRAMEBUFFEREXTPROC glDiscardFramebufferEXT;
extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOES;
extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOES;
extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOES;
extern PFNGLISVERTEXARRAYOESPROC glIsVertexArrayOES;
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIBDIVISORPROC) (GLuint index, GLuint divisor);
extern PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisorEXT;
typedef void (GL_APIENTRYP PFNGLDRAWELEMENTSINSTANCEDPROC) (GLenum, GLsizei, GLenum, const GLvoid*, GLsizei);
extern PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstancedEXT;
typedef void (GL_APIENTRYP PFNGLDRAWARRAYSINSTANCEDPROC) (GLenum, GLint, GLsizei, GLsizei);
extern PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstancedEXT;
#elif defined(IOS)
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#include <GLKit/GLKit.h>
#include <OpenGLES/ES2/glext.h>
#elif defined(EMSCRIPTEN)
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
extern PFNGLDISCARDFRAMEBUFFEREXTPROC glDiscardFramebufferEXT;
extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOES;
extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOES;
extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOES;
extern PFNGLISVERTEXARRAYOESPROC glIsVertexArrayOES;
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIBDIVISORPROC) (GLuint index, GLuint divisor);
extern PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisorEXT;
typedef void (GL_APIENTRYP PFNGLDRAWELEMENTSINSTANCEDPROC) (GLenum, GLsizei, GLenum, const GLvoid*, GLsizei);
extern PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstancedEXT;
typedef void (GL_APIENTRYP PFNGLDRAWARRAYSINSTANCEDPROC) (GLenum, GLint, GLsizei, GLsizei);
extern PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstancedEXT;
#elif defined(IS_TARGET_OSX)
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#elif defined(IS_TARGET_LINUX)
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#elif defined(IS_TARGET_WINDOWS)
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define CALLBACK    __stdcall
#define WINAPI      __stdcall
#define WINAPIV     __cdecl
#define APIENTRY    WINAPI
#define APIPRIVATE  __stdcall
#define PASCAL      __stdcall
#else
#define CALLBACK
#define WINAPI
#define WINAPIV
#define APIENTRY    WINAPI
#define APIPRIVATE
#define PASCAL      pascal
#endif
#if (defined(_M_IX86) || defined(_M_IA64) || defined(_M_AMD64) || defined(_M_ARM)) && !defined(MIDL_PASS)
#define DECLSPEC_IMPORT __declspec(dllimport)
#else
#define DECLSPEC_IMPORT
#endif
#if !defined(_GDI32_)
#define WINGDIAPI DECLSPEC_IMPORT
#else
#define WINGDIAPI
#endif
// Definitions of common types
#ifdef _WIN64
typedef unsigned __int64 size_t;
typedef __int64          ptrdiff_t;
typedef __int64          intptr_t;
#else
typedef unsigned int     size_t;
typedef int              ptrdiff_t;
typedef int              intptr_t;
#endif
#include <GL/gl.h>
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
typedef char GLchar;

#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A

#define GL_STATIC_DRAW 0x88E4

#define GL_MAP_READ_BIT 0x0001
#define GL_MAP_WRITE_BIT 0x0002
#define GL_MAP_FLUSH_EXPLICIT_BIT 0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT 0x0020

#define GL_ELEMENT_ARRAY_BUFFER 0x8893

#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_COLOR_ATTACHMENT4 0x8CE4
#define GL_COLOR_ATTACHMENT5 0x8CE5
#define GL_COLOR_ATTACHMENT6 0x8CE6
#define GL_COLOR_ATTACHMENT7 0x8CE7
#define GL_COLOR_ATTACHMENT8 0x8CE8
#define GL_COLOR_ATTACHMENT9 0x8CE9
#define GL_COLOR_ATTACHMENT10 0x8CEA
#define GL_COLOR_ATTACHMENT11 0x8CEB
#define GL_COLOR_ATTACHMENT12 0x8CEC
#define GL_COLOR_ATTACHMENT13 0x8CED
#define GL_COLOR_ATTACHMENT14 0x8CEE
#define GL_COLOR_ATTACHMENT15 0x8CEF
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_STENCIL_ATTACHMENT 0x8D20
#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41

#define GL_DEPTH24_STENCIL8 0x88F0
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_DEPTH_COMPONENT32 0x81A7

#define GL_STENCIL_INDEX1 0x8D46
#define GL_STENCIL_INDEX4 0x8D47
#define GL_STENCIL_INDEX8 0x8D48
#define GL_STENCIL_INDEX16 0x8D49

#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_ARRAY_BUFFER 0x8892

#define GL_DYNAMIC_DRAW 0x88E8
#define GL_STREAM_DRAW 0x88E0


#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_LINK_STATUS 0x8B82

#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_MAX_VERTEX_ATTRIBS 0x8869
#define GL_MAX_VARYING_VECTORS 0x8DFC
#define GL_MAX_VERTEX_UNIFORM_VECTORS 0x8DFB
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS 0x8DFD

#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF

#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_MIRRORED_REPEAT 0x8370

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

#ifndef GLAPIENTRY
#define GLAPIENTRY WINAPI
#endif
typedef void (GLAPIENTRY * PFNGLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
extern PFNGLGENBUFFERSPROC glGenBuffers;
typedef void (GLAPIENTRY * PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint* buffers);
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
typedef void (GLAPIENTRY * PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
extern PFNGLBINDBUFFERPROC glBindBuffer;
typedef GLvoid * (GLAPIENTRY * PFNGLMAPBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
extern PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
typedef void (GLAPIENTRY * PFNGLFLUSHMAPPEDBUFFERRANGEAPPLEPROC) (GLenum target, GLintptr offset, GLsizeiptr size);
extern PFNGLFLUSHMAPPEDBUFFERRANGEAPPLEPROC glFlushMappedBufferRange;
typedef GLboolean(GLAPIENTRY * PFNGLUNMAPBUFFERPROC) (GLenum target);
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;
typedef void (GLAPIENTRY * PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
typedef void (GLAPIENTRY * PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
extern PFNGLBUFFERDATAPROC glBufferData;
typedef void (GLAPIENTRY * PFNGLGENFRAMEBUFFERSPROC) (GLsizei n, GLuint* framebuffers);
extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
typedef void (GLAPIENTRY * PFNGLGENRENDERBUFFERSPROC) (GLsizei n, GLuint* renderbuffers);
extern PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
typedef void (GLAPIENTRY * PFNGLBINDRENDERBUFFERPROC) (GLenum target, GLuint renderbuffer);
extern PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERRENDERBUFFERPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
typedef GLenum(GLAPIENTRY * PFNGLCHECKFRAMEBUFFERSTATUSPROC) (GLenum target);
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
typedef void (GLAPIENTRY * PFNGLDELETERENDERBUFFERSPROC) (GLsizei n, const GLuint* renderbuffers);
extern PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
typedef void (GLAPIENTRY * PFNGLDELETEFRAMEBUFFERSPROC) (GLsizei n, const GLuint* framebuffers);
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
typedef void (GLAPIENTRY * PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint* arrays);
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint);
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
typedef void (GLAPIENTRY * PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint* arrays);
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
typedef void (GLAPIENTRY * PFNGLBINDVERTEXARRAYPROC) (GLuint array);
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
typedef void (GLAPIENTRY * PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
extern PFNGLDETACHSHADERPROC glDetachShader;
typedef void (GLAPIENTRY * PFNGLDELETEPROGRAMPROC) (GLuint program);
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
typedef GLuint(GLAPIENTRY * PFNGLCREATESHADERPROC) (GLenum type);
extern PFNGLCREATESHADERPROC glCreateShader;
typedef void (GLAPIENTRY * PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths);
extern PFNGLSHADERSOURCEPROC glShaderSource;
typedef void (GLAPIENTRY * PFNGLCOMPILESHADERPROC) (GLuint shader);
extern PFNGLCOMPILESHADERPROC glCompileShader;
typedef void (GLAPIENTRY * PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* param);
extern PFNGLGETSHADERIVPROC glGetShaderiv;
typedef void (GLAPIENTRY * PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
typedef void (GLAPIENTRY * PFNGLDELETESHADERPROC) (GLuint shader);
extern PFNGLDELETESHADERPROC glDeleteShader;
typedef void (GLAPIENTRY * PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
extern PFNGLUNIFORM1IPROC glUniform1i;
typedef GLuint(GLAPIENTRY * PFNGLCREATEPROGRAMPROC) (void);
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
typedef void (GLAPIENTRY * PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, const GLchar* name);
extern PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
typedef void (GLAPIENTRY * PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
extern PFNGLATTACHSHADERPROC glAttachShader;
typedef void (GLAPIENTRY * PFNGLLINKPROGRAMPROC) (GLuint program);
extern PFNGLLINKPROGRAMPROC glLinkProgram;
typedef void (GLAPIENTRY * PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint* param);
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
typedef void (GLAPIENTRY * PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
typedef GLint(GLAPIENTRY * PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar* name);
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
typedef GLint(GLAPIENTRY * PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar* name);
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
typedef void (GLAPIENTRY * PFNGLUNIFORM2FVPROC) (GLint location, GLsizei count, const GLfloat* value);
extern PFNGLUNIFORM2FVPROC glUniform2fv;
typedef void (GLAPIENTRY * PFNGLUNIFORM3FVPROC) (GLint location, GLsizei count, const GLfloat* value);
extern PFNGLUNIFORM3FVPROC glUniform3fv;
typedef void (GLAPIENTRY * PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
extern PFNGLUNIFORM1FPROC glUniform1f;
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
typedef void (GLAPIENTRY * PFNGLUNIFORM4FVPROC) (GLint location, GLsizei count, const GLfloat* value);
extern PFNGLUNIFORM4FVPROC glUniform4fv;
typedef void (GLAPIENTRY * PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
typedef void (GLAPIENTRY * PFNGLACTIVETEXTUREPROC) (GLenum texture);
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
typedef void (GLAPIENTRY * PFNGLUSEPROGRAMPROC) (GLuint program);
extern PFNGLUSEPROGRAMPROC glUseProgram;
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBDIVISORPROC) (GLuint index, GLuint divisor);
extern PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint);
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDPROC) (GLenum, GLsizei, GLenum, const GLvoid*, GLsizei);
extern PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINSTANCEDPROC) (GLenum, GLint, GLsizei, GLsizei);
extern PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
typedef void (GLAPIENTRY * PFNGLGENERATEMIPMAPPROC) (GLenum target);
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
#else
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#error "Unknown platform!"
#endif

#if defined(GLES2)
#define GL_CLAMP GL_CLAMP_TO_EDGE
#define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
#define GL_DEPTH_COMPONENT24 GL_DEPTH_COMPONENT24_OES
#define glClearDepth glClearDepthf
#define glDiscardFramebuffer glDiscardFramebufferEXT
#define glGenVertexArrays glGenVertexArraysOES
#define glBindVertexArray glBindVertexArrayOES
#define glDeleteVertexArrays glDeleteVertexArraysOES
#define glUnmapBuffer glUnmapBufferOES
#define glMapBufferRange glMapBufferRangeEXT
#define glFlushMappedBufferRange glFlushMappedBufferRangeEXT
#define GL_MAP_WRITE_BIT GL_MAP_WRITE_BIT_EXT
#define GL_MAP_FLUSH_EXPLICIT_BIT GL_MAP_FLUSH_EXPLICIT_BIT_EXT
#define GL_MAP_UNSYNCHRONIZED_BIT GL_MAP_UNSYNCHRONIZED_BIT_EXT
#define glVertexAttribDivisor glVertexAttribDivisorEXT
#define glDrawElementsInstanced glDrawElementsInstancedEXT
#define glDrawArraysInstanced glDrawArraysInstancedEXT
#endif

#ifndef GL_EXT_texture_compression_s3tc
#define GL_EXT_texture_compression_s3tc 1
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3
#endif /* GL_EXT_texture_compression_s3tc */

#ifndef GL_IMG_texture_compression_pvrtc
#define GL_IMG_texture_compression_pvrtc 1
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG 0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG 0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG 0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG 0x8C03
#endif /* GL_IMG_texture_compression_pvrtc */

#ifndef GL_OES_compressed_ETC1_RGB8_texture
#define GL_OES_compressed_ETC1_RGB8_texture 1
#define GL_ETC1_RGB8_OES                  0x8D64
#endif /* GL_OES_compressed_ETC1_RGB8_texture */

enum class FrontFaceMode
{
    DEFAULT = GL_CCW,
    CCW = GL_CCW,
    CW = GL_CW
};

enum class TextureTarget
{
    UNKNOWN = -1,
    TEXTURE_2D = GL_TEXTURE_2D,
    TEXTURE_CUBE_MAP_POSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    TEXTURE_CUBE_MAP_NEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    TEXTURE_CUBE_MAP_POSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    TEXTURE_CUBE_MAP_NEGATIVE_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    TEXTURE_CUBE_MAP_POSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    TEXTURE_CUBE_MAP_NEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

