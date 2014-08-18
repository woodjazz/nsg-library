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
#include "Constants.h"
#include "Singleton.h"
#include <set>

namespace NSG
{
    class Graphics : public Singleton<Graphics>
    {
    public:
    	Graphics();
    	~Graphics();
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
        void SetViewport(const Recti& viewport);
        bool SetBuffers(Mesh* mesh);
        bool SetVertexArrayObj(VertexArrayObj* obj);
        VertexArrayObj* GetVertexArrayObj() const { return vertexArrayObj_; }
        bool SetVertexBuffer(VertexBuffer* buffer);
        VertexBuffer* GetVertexBuffer() const { return vertexBuffer_; }
        bool SetIndexBuffer(IndexBuffer* buffer);
        IndexBuffer* GetIndexBuffer() const { return indexBuffer_; }
        bool SetProgram(Program* program);
        Program* GetProgram() const { return program_; }
        void SetFrameBuffer(GLuint value);
        void SetSceneVariables(Program* program);
        bool Draw(bool solid, Material* material, Node* node, Mesh* mesh);
        void DiscardFramebuffer();
        void BeginFrame();
        void EndFrame();
        void SetUniformsNeedUpdate() { uniformsNeedUpdate_ = true;}
        bool HasVertexArrayObject() const { return has_vertex_array_object_ext_; }
        bool HasMapBufferRange() const { return has_map_buffer_range_ext_; }
        void SetVertexAttrPointers();
        void SetAttributes(const Mesh* mesh, const Program* program);
        void InsertUniformObj(UniformsUpdate* obj) { uniformObjs_.insert(obj); }
        void RemoveUniformObj(UniformsUpdate* obj) { uniformObjs_.erase(obj); }
        UniformObjs& GetUniformObjs() { return uniformObjs_; }

      private:
        Recti viewport_;
      	GLint systemFbo_;
      	GLuint currentFbo_;
        VertexArrayObj* vertexArrayObj_;
      	VertexBuffer* vertexBuffer_;
      	IndexBuffer* indexBuffer_;
      	Program* program_;
      	Texture* textures_[MAX_TEXTURE_UNITS];
      	unsigned activeTexture_;
      	unsigned enabledAttributes_; //positions' bits for enabled attributes
        bool uniformsNeedUpdate_;
        const Mesh* activeMesh_; // last mesh drawn
        bool has_discard_framebuffer_ext_;
        bool has_vertex_array_object_ext_;
        bool has_map_buffer_range_ext_;
        UniformObjs uniformObjs_; // just a repository to keep track which objects need uniform updates
    };
}