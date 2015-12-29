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
#include "Singleton.h"

namespace NSG
{
	class Renderer : public Singleton<Renderer>
	{
	public:
		Renderer();
		~Renderer();
		void Render(const Pass* pass, Mesh* mesh, Material* material);
		void Render(const Pass* pass, const Scene* scene, const Camera* camera, SceneNode* node, const Light* light);
		void Render(Window* window, Scene* scene, Camera* camera);
		void Render(Window* window, Scene* scene);
		void GenerateBatches(std::vector<SceneNode*>& visibles, std::vector<PBatch>& batches);
		void DrawShadowPass(Batch* batch, const Light* light, const ShadowCamera* camera);
		void EnableDebugPhysics(bool enable) { debugPhysics_ = enable; }
		PDebugRenderer GetDebugRenderer() const { return debugRenderer_; }
		static SignalDebugRenderer::PSignal SigDebugRenderer();
		RendererContext SetContext(RendererContext context);
		RendererContext GetContext() const { return context_; }
	private:
		void SortTransparentBackToFront(std::vector<SceneNode*>& objs);
		void SortSolidFrontToBack(std::vector<SceneNode*>& objs);
		void SortOverlaysBackToFront(std::vector<SceneNode*>& objs);
		void Draw(Batch* batch, const Pass* pass, const Light* light, const Camera* camera);
		void Generate2DShadowMap(const Light* light, std::vector<SceneNode*>& shadowCasters);
		void GenerateShadowMapCubeFace(const Light* light, const std::vector<SceneNode*>& shadowCasters);
		void GenerateCubeShadowMap(const Light* light, std::vector<SceneNode*>& shadowCasters);
		void GenerateShadowMap(Light* light, const std::vector<SceneNode*>& drawables);
		std::vector<SceneNode*> ExtractTransparent(const std::vector<SceneNode*>& objs);
		std::vector<SceneNode*> ExtractFiltered(const std::vector<SceneNode*>& objs);
		void RemoveFrom(std::vector<SceneNode*>& from, const std::vector<SceneNode*>& objs);
		void GetLighted(const std::vector<SceneNode*>& nodes, std::vector<SceneNode*>& result) const;
		void ShadowGenerationPass();
    	void DefaultOpaquePass(std::vector<SceneNode*>& objs);
	    void LitOpaquePass(const std::vector<SceneNode*>& objs);
    	void DefaultTransparentPass(const std::vector<SceneNode*>& objs);
	    void LitTransparentPass(const std::vector<SceneNode*>& objs);
	    void FilterPass(std::vector<SceneNode*>& objs);
	    void SetShadowFrameBufferSize(FrameBuffer* frameBuffer);
	    void DebugPhysicsPass();
		void DebugRendererPass();
		void RenderOverlays();
		void RenderFiltered(const std::vector<SceneNode*>& objs);
		PGraphics graphics_;
		Window* window_;
		Scene* scene_;
		Camera* camera_;
		PPass shadowPass_;
		PPass defaultOpaquePass_;
		PPass litOpaquePass_;
		PPass defaultTransparentPass_;
		PPass litTransparentPass_;
		PPass debugPass_;
		PPass filterPass_;
		bool debugPhysics_;
		PMaterial debugMaterial_;
		PDebugRenderer debugRenderer_;
		RendererContext context_;
		std::vector<SceneNode*> overlays_;
		PCamera overlaysCamera_;
	};
}
