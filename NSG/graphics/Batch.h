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
#include "Object.h"

namespace NSG
{
	class Batch
    {
	public:
		Batch();
		Batch(Material* material, Mesh* mesh);
    	~Batch();
		bool operator == (const Batch& obj) const;
		Batch(const Batch&) = delete;
		//Batch& operator = (const Batch&) = delete;
    	bool IsReady();
		void Draw();
		void Add(SceneNode* node);
		Mesh* GetMesh() const { return mesh_; }
		Material* GetMaterial() const { return material_; }
		const std::vector<SceneNode*>& GetNodes() const { return nodes_; }
		bool AllowInstancing() const;
		void Clear();
		bool IsEmpty() const { return !material_; }
	private:
        Material* material_;
        Mesh* mesh_;
        std::vector<SceneNode*> nodes_;
    };
}