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
#include "SharedPointers.h"
#include <vector>

namespace NSG
{
	typedef std::vector<PGLES2Mesh> MESHES;
	class Pass
	{
	public:
		Pass();
		virtual ~Pass();
		enum Mode {SOLID, WIREFRAME};
		void SetDrawMode(Mode mode) { drawMode_ = mode; }
		void SetNode(Node* node);
		void Set(PGLES2Material material);
		void Set(GLES2Material* pMaterial);
		void Add(PGLES2Mesh mesh);
		virtual void Render();
		PGLES2Mesh GetMesh(int idx) const;
		const MESHES& GetMeshes() const { return meshes_; }
		PGLES2Material GetMaterial() const { return material_; }
	protected:
		Node* node_;
		PGLES2Material material_;
		MESHES meshes_;
		Mode drawMode_;
		friend class Technique;

	};
}