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
#pragma once
#include "Types.h"
#include "Vector4.h"
#include "Color.h"
#include "Singleton.h"
#include "GLIncludes.h"

namespace NSG
{
	class RenderingCapabilities;
	// Keeps OpenGL's context
	class RenderingContext : public Singleton<RenderingContext>
	{
	public:
		~RenderingContext();
		void ResetCachedState();
		void SetClearColor(const Color& color);
		void SetClearDepth(GLclampf depth);
		void SetClearStencil(GLint clear);
		void ClearAllBuffers();
		void ClearBuffers(bool color, bool depth, bool stencil);
		void ClearStencilBuffer(GLint value = 0);
		void SetStencilTest(bool enable, GLuint writeMask, GLenum sfail, GLenum dpfail, GLenum dppass, GLenum func, GLint ref, GLuint compareMask);
		void SetScissorTest(bool enable = false, GLint x = 0, GLint y = 0, GLsizei width = 0, GLsizei height = 0);
		void SetColorMask(bool enable);
		void SetDepthMask(bool enable);
		void SetDepthFunc(DepthFunc depthFunc);
		void SetStencilMask(GLuint mask);
		void SetBlendModeTest(BLEND_MODE blendMode);
		void EnableDepthTest(bool enable);
		void EnableCullFace(bool enable);
		void SetCullFace(CullFaceMode mode);
		void SetFrontFace(FrontFaceMode mode);
		void SetTexture(int index, GLuint id, GLenum target = GL_TEXTURE_2D);
		void SetTexture(int index, Texture* texture);
		bool SetVertexArrayObj(VertexArrayObj* obj);
		VertexArrayObj* GetVertexArrayObj() const { return vertexArrayObj_; }
		bool SetVertexBuffer(Buffer* buffer, bool force = false);
		Buffer* GetVertexBuffer() const { return vertexBuffer_; }
		bool SetIndexBuffer(Buffer* buffer, bool force = false);
		Buffer* GetIndexBuffer() const { return indexBuffer_; }
		bool SetProgram(Program* program);
		Program* GetProgram() const { return activeProgram_; }
		FrameBuffer* SetFrameBuffer(FrameBuffer* buffer);
		FrameBuffer* SetFrameBuffer(FrameBuffer* buffer, TextureTarget colorTarget);
		FrameBuffer* GetFrameBuffer() const { return currentFbo_; }
		void DrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices);
		void DrawArrays(GLenum mode, GLint first, GLsizei count);
		void DrawActiveMesh();
		void DrawInstancedActiveMesh(const Batch& batch, InstanceBuffer* instancesBuffer);
		void DiscardFramebuffer();
		void SetBuffers(bool solid, InstanceBuffer* instancesBuffer);
		void SetInstanceAttrPointers(Program* program);
		void SetVertexAttrPointers();
		typedef std::function<void()> SetAttPointersFunction;
		void SetAttributes(SetAttPointersFunction setAttPointersCallBack);
		void SetMesh(Mesh* mesh) { activeMesh_ = mesh; }
		const Mesh* GetMesh() const { return activeMesh_; }
		bool IsTextureSizeCorrect(unsigned width, unsigned height);
		void UnboundTextures();
		bool NeedsDecompress(TextureFormat format) const;
		bool SetupProgram(const Pass* pass, const Scene* scene, const Camera* camera, SceneNode* sceneNode, Material* material, const Light* light);
		void SetupPass(const Pass* pass);
		void SetSlopeScaledBias(float slopeScaledBias);
		static std::string GetExtensions();
        void SetViewport(const Window& window);
	private:
        void SetViewport(const Vector4& viewport, bool force);
		RenderingContext();
        Vector4 viewport_;
        Vector4 windowViewport_;
		GLint systemFbo_;
		FrameBuffer* currentFbo_;
		TextureTarget currentColorTarget_;
		VertexArrayObj* vertexArrayObj_;
		Buffer* vertexBuffer_;
		Buffer* indexBuffer_;
		Program* activeProgram_;
		std::vector<Texture*> textures_;
		unsigned activeTexture_;
		unsigned enabledAttributes_; //positions' bits for enabled attributes
		const Mesh* lastMesh_; // last mesh drawn
		Program* lastProgram_; // last used program
		Mesh* activeMesh_; // mesh that is going to be drawn
		CullFaceMode cullFaceMode_;
		FrontFaceMode frontFaceMode_;
		DepthFunc depthFunc_;
		float slopeScaledDepthBias_;
		std::string extensions_;
        std::shared_ptr<RenderingCapabilities> capabilities_;
		friend class Singleton<RenderingContext>;
	};
}
