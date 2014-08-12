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
#include "Types.h"
namespace NSG
{
    class Buffer;
    bool CheckExtension(const std::string& name);
    void ResetCachedState();
    void SetClearColor(const Color& color);
    void SetClearDepth(GLclampf depth);
    void SetClearStencil(GLint clear);
	void ClearAllBuffers();
	void ClearBuffers(bool color, bool depth, bool stencil);
	void ClearStencilBuffer(GLint value = 0);
	void SetStencilTest(bool enable, GLuint writeMask, GLenum sfail, GLenum dpfail, GLenum dppass, GLenum func, GLint ref, GLuint compareMask);
	void SetColorMask(bool enable);
	void SetDepthMask(bool enable);
	void SetStencilMask(GLuint mask);
	void SetBlendModeTest(BLEND_MODE blendMode);
	void SetDepthTest(bool enable);
	void SetCullFace(bool enable);
	void SetTexture(unsigned index, Texture* texture);
	bool SetVertexBuffer(VertexBuffer* buffer);
	VertexBuffer* GetVertexBuffer();
	bool SetIndexBuffer(IndexBuffer* buffer);
	IndexBuffer* GetIndexBuffer();
	bool SetProgram(Program* program);
	Program* GetProgram();
	void SetFrameBuffer(GLuint value);
	void DiscardFramebuffer();
}