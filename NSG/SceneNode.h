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
#include "Node.h"
#include "GLES2Includes.h"
#include "GLES2Material.h"
#include "GLES2Mesh.h"
#include "Behavior.h"
#include "GLES2FrameColorSelection.h"
#include <memory>

namespace NSG
{
	class App;
	class SceneNode : public Node
	{
	public:
		SceneNode();
		~SceneNode();
		PGLES2Material GetMaterial() const { return pMaterial_; }
		void SetMaterial(PGLES2Material pMaterial);
		void SetMesh(PGLES2Mesh pMesh);
		PGLES2Mesh GetMesh() const { return pMesh_; }
		void SetBehavior(PBehavior pBehavior);
		PBehavior GetBehavior() const { return pBehavior_; }
		void Render(bool solid);
		void Render2Select();
		void RenderForSelect(GLuint position_loc);
		
	private:
		App* pApp_;
		PGLES2FrameColorSelection pSelection_;
		PGLES2Material pMaterial_;
		PGLES2Mesh pMesh_;
		PBehavior pBehavior_;
	};

	typedef std::shared_ptr<SceneNode> PSceneNode;
}