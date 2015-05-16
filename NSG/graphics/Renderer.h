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
		void SortBackToFront(std::vector<SceneNode*>& nodes) const;
		void SortFrontToBack(std::vector<SceneNode*>& nodes) const;
		void GenerateBatches(std::vector<SceneNode*>& visibles, std::vector<PBatch>& batches);
	private:
		void ExtractTransparent(std::vector<SceneNode*>& nodes, std::vector<SceneNode*>& transparent) const;
		void ExtractLighted(std::vector<SceneNode*>& nodes, std::vector<SceneNode*>& result) const;
		void GetVisibleFromLight(const Light* light, std::vector<SceneNode*>& nodes, std::vector<SceneNode*>& result) const;
		void RenderVisibleSceneNodes(std::vector<SceneNode*>& visibles);
		void Draw(Batch* batch, Pass* pass, Light* light);
		static Renderer* this_;
		Graphics* graphics_;
		Window* window_;
		Scene* scene_;
		Camera* camera_;
	};
}
