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
#pragma once

#if defined(NACL)
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#define glClearDepth glClearDepthf
#define GL_CLAMP GL_CLAMP_TO_EDGE
#elif defined(ANDROID)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#define glClearDepth glClearDepthf
#define GL_CLAMP GL_CLAMP_TO_EDGE
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
#include <GLKit/GLKit.h>
#define glClearDepth glClearDepthf
#define GL_CLAMP GL_CLAMP_TO_EDGE
#elif defined(EMSCRIPTEN)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#define glClearDepth glClearDepthf
#define GL_CLAMP GL_CLAMP_TO_EDGE
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
#else
#include <GL/glew.h>
#endif

#if defined(GLES2)
#define glDiscardFramebuffer glDiscardFramebufferEXT
#define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
#define glGenVertexArrays glGenVertexArraysOES
#define glBindVertexArray glBindVertexArrayOES
#define glDeleteVertexArrays glDeleteVertexArraysOES
#define glMapBufferRange glMapBufferRangeEXT
#define glFlushMappedBufferRange glFlushMappedBufferRangeEXT
#define glUnmapBuffer glUnmapBufferOES
#define GL_MAP_WRITE_BIT GL_MAP_WRITE_BIT_EXT
#define GL_MAP_FLUSH_EXPLICIT_BIT GL_MAP_FLUSH_EXPLICIT_BIT_EXT
#define GL_MAP_UNSYNCHRONIZED_BIT GL_MAP_UNSYNCHRONIZED_BIT_EXT
#define glVertexAttribDivisor glVertexAttribDivisorEXT
#define glDrawElementsInstanced glDrawElementsInstancedEXT
#define glDrawArraysInstanced glDrawArraysInstancedEXT
#endif

