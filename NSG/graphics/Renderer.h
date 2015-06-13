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

namespace NSG
{
	class Renderer
	{
	public:
		Renderer(Graphics* graphics);
		~Renderer();
		void Render(Window* window, Scene* scene);
		static Renderer* GetPtr() { return Renderer::this_; }
		void GenerateBatches(std::vector<SceneNode*>& visibles, std::vector<PBatch>& batches);
		const Window* GetWindow() const { return window_; }
		const ShadowCamera* GetShadowCamera() const { return shadowCamera_.get(); }
		const Scene* GetScene() const { return scene_; }
		void SortTransparentBackToFront();
		void SortSolidFrontToBack();
	private:
		void Generate2DShadowMap(const Light* light, std::vector<SceneNode*>& shadowCasters);
		void GenerateShadowMapCubeFace(const Light* light, const std::vector<SceneNode*>& shadowCasters);
		void GenerateCubeShadowMap(const Light* light, std::vector<SceneNode*>& shadowCasters);
		void GenerateShadowMap(const Light* light, const std::vector<SceneNode*>& drawables);
		void ExtractTransparent();
		void GetLighted(std::vector<SceneNode*>& nodes, std::vector<SceneNode*>& result) const;
		void Draw(Batch* batch, const Pass* pass, const Light* light);
    	void AmbientPass();
	    void GenerateShadowMaps();
	    void LitPass();
	    void TransparentPass();
	    void SetShadowFrameBufferSize(FrameBuffer* frameBuffer);

		static Renderer* this_;
		Graphics* graphics_;
		Window* window_;
		Scene* scene_;
		Camera* camera_;
		PPass ambientPass_;
		PPass lightPass_;
		PPass transparentPass_;
		PPass shadowPass_;
		PShadowCamera shadowCamera_;
		std::vector<SceneNode*> visibles_; //visibles not transparent nodes
		std::vector<SceneNode*> transparent_;
	};
}
