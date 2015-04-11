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
#pragma once
#include "Types.h"
#include "Constants.h"
#include "Singleton.h"
#include <map>

namespace NSG
{
    // Keeps OpenGL's status
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
        void SetDepthFunc(DepthFunc depthFunc);
        void SetStencilMask(GLuint mask);
        void SetBlendModeTest(BLEND_MODE blendMode);
        void EnableDepthTest(bool enable);
        void EnableCullFace(bool enable);
        void SetCullFace(CullFaceMode mode);
        void SetFrontFace(FrontFaceMode mode);
        void SetTexture(int index, Texture* texture);
        void SetViewport(const Recti& viewport, bool force);
        void InvalidateVAOFor(const Program* program);
        bool SetBuffers(Mesh* mesh);
        bool SetVertexArrayObj(VertexArrayObj* obj);
        VertexArrayObj* GetVertexArrayObj() const { return vertexArrayObj_; }
        bool SetVertexBuffer(Buffer* buffer, bool force = false);
        Buffer* GetVertexBuffer() const { return vertexBuffer_; }
        bool SetIndexBuffer(Buffer* buffer, bool force = false);
        Buffer* GetIndexBuffer() const { return indexBuffer_; }
        bool SetProgram(Program* program);
        Program* GetProgram() const { return activeProgram_; }
        void SetScene(Scene* scene) { activeScene_ = scene; }
        Scene* GetScene() const { return activeScene_; }
        void SetCamera(Camera* camera);
        Camera* GetCamera() const { return activeCamera_; }
        void SetWindow(Window* window);
        Window* GetWindow() const { return activeWindow_; }
        void SetFrameBuffer(FrameBuffer* buffer);
		void DrawActiveMesh(Pass* pass);
		void DrawInstancedActiveMesh(Pass* pass, const Batch& batch);
        void DiscardFramebuffer();
        bool HasVertexArrayObject() const { return has_vertex_array_object_ext_; }
        bool HasMapBufferRange() const { return has_map_buffer_range_ext_; }
        bool HasDepthTexture() const { return has_depth_texture_ext_; }
        bool HasDepthComponent24() const { return has_depth_component24_ext_; }
        bool HasNonPowerOfTwo() const { return has_texture_non_power_of_two_ext_; }
        bool HasInstancedArrays() const { return has_instanced_arrays_ext_; }
        bool HasPackedDepthStencil() const { return has_packed_depth_stencil_ext_; }
		void SetBuffers(bool solid);
        void UpdateBatchBuffer(const Batch& batch);
        void SetInstanceAttrPointers(Program* program);
        void SetVertexAttrPointers();
        void SetAttributes();
        void InsertUniformObj(UniformsUpdate* obj) { uniformObjs_.insert(obj); }
        void RemoveUniformObj(UniformsUpdate* obj) { uniformObjs_.erase(obj); }
        UniformObjs& GetUniformObjs() { return uniformObjs_; }
        void SetMesh(Mesh* mesh) { activeMesh_ = mesh; }
        const Mesh* GetMesh() const { return activeMesh_; }
		void SetNode(SceneNode* node) { activeNode_ = node; }
		void ExtractTransparent(std::vector<SceneNode*>& nodes, std::vector<SceneNode*>& transparent) const;
		void SortBackToFront(std::vector<SceneNode*>& nodes) const;
        void SortFrontToBack(std::vector<SceneNode*>& nodes) const;
		void RenderVisibleSceneNodes();
        bool IsTextureSizeCorrect(unsigned width, unsigned height);
        void GenerateBatches(std::vector<SceneNode*>& visibles, std::vector<PBatch>& batches);
        GLint GetMaxVaryingVectors() const { return maxVaryingVectors_; }
        GLint GetMaxTexturesCombined() const { return maxTexturesCombined_; }
        GLint GetMaxVertexUniformVectors() const { return maxVertexUniformVectors_; }
        GLint GetMaxFragmentUniformVectors() const { return maxFragmentUniformVectors_; }
        GLint GetMaxVertexAttribs() const { return maxVertexAttribs_; }
        bool BeginFrameRender();
		void Render(Camera* camera);
        void EndFrameRender();
		void UnboundTextures();
        void SetViewportFactor(const Vector4& viewportFactor);
        SignalEmpty::PSignal signalBeginFrame_;
    private:
        void SetUpViewport();
        Recti viewport_;
        GLint systemFbo_;
        FrameBuffer* currentFbo_;
        VertexArrayObj* vertexArrayObj_;
        Buffer* vertexBuffer_;
        Buffer* indexBuffer_;
        Program* activeProgram_;
        std::vector<Texture*> textures_;
        unsigned activeTexture_;
        unsigned enabledAttributes_; //positions' bits for enabled attributes
        const Mesh* lastMesh_; // last mesh drawn
        Program* lastProgram_; // last used program
		SceneNode* lastNode_; // last used node
        Mesh* activeMesh_; // mesh that is going to be drawn
        SceneNode* activeNode_; //node that is going to be used to draw
        Scene* activeScene_;
        Camera* activeCamera_;
        Window* activeWindow_;
        bool has_discard_framebuffer_ext_;
        bool has_vertex_array_object_ext_;
        bool has_map_buffer_range_ext_;
        bool has_depth_texture_ext_;
        bool has_depth_component24_ext_;
        bool has_texture_non_power_of_two_ext_;
        bool has_instanced_arrays_ext_;
        bool has_packed_depth_stencil_ext_;
        UniformObjs uniformObjs_; // just a repository to keep track which objects need uniform updates
        CullFaceMode cullFaceMode_;
        FrontFaceMode frontFaceMode_;
        struct VAOKey
        {
            Program* program_;
            VertexBuffer* vBuffer_;
            IndexBuffer* iBuffer_;

            bool operator < (const VAOKey& obj) const
            {
                return program_ < obj.program_ ||
                       (!(obj.program_ < program_) && vBuffer_ < obj.vBuffer_) ||
                       (!(obj.program_ < program_) && !(obj.vBuffer_ < vBuffer_) && iBuffer_ < obj.iBuffer_);
            };
        };
        typedef std::map<VAOKey, PVertexArrayObj> VAOMap;
        VAOMap vaoMap_;
        GLint maxVaryingVectors_;
        GLint maxTexturesCombined_;
        GLint maxVertexUniformVectors_;
        GLint maxFragmentUniformVectors_;
        GLint maxVertexAttribs_;
        DepthFunc depthFunc_;
        Vector4 viewportFactor_;
    };
}