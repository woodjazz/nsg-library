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
		void Render(Window* window, Scene* scene);
		void GenerateBatches(std::vector<SceneNode*>& visibles, std::vector<PBatch>& batches);
		const Window* GetWindow() const { return window_; }
		const Scene* GetScene() const { return scene_; }
		void SortTransparentBackToFront();
		void SortSolidFrontToBack();
		void DrawShadowPass(Batch* batch, const Light* light);
		void EnableDebugPhysics(bool enable) { debugPhysics_ = enable; }
		PDebugRenderer GetDebugRenderer() const { return debugRenderer_; }
		static SignalDebugRenderer::PSignal SigDebugRenderer();
	private:
		void Draw(Batch* batch, const Pass* pass, const Light* light);
		void Generate2DShadowMap(const Light* light, std::vector<SceneNode*>& shadowCasters);
		void GenerateShadowMapCubeFace(const Light* light, const std::vector<SceneNode*>& shadowCasters);
		void GenerateCubeShadowMap(const Light* light, std::vector<SceneNode*>& shadowCasters);
		void GenerateShadowMap(Light* light, const std::vector<SceneNode*>& drawables);
		void ExtractTransparent();
		void GetLighted(std::vector<SceneNode*>& nodes, std::vector<SceneNode*>& result) const;
		void ShadowGenerationPass();
    	void DefaultOpaquePass();
	    void LitOpaquePass();
    	void DefaultTransparentPass();
	    void LitTransparentPass();
	    void SetShadowFrameBufferSize(FrameBuffer* frameBuffer);
	    void DebugPhysicsPass();
		void DebugRendererPass();

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
		std::vector<SceneNode*> visibles_; //visibles not transparent nodes
		std::vector<SceneNode*> transparent_;
		bool debugPhysics_;
		PMaterial debugMaterial_;
		PDebugRenderer debugRenderer_;
	};
}
