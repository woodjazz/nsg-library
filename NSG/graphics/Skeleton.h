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
#include <vector>

namespace NSG
{
	class Skeleton
	{
	public:
		Skeleton(PMesh mesh);
		virtual ~Skeleton();
		void SetScene(PWeakScene scene) { scene_ = scene; }
		void SetRoot(PWeakNode root) { root_ = root; }
		void SetBones(const std::vector<PWeakNode>& bones) { bones_ = bones; }
		unsigned GetBoneIndex(const std::string& name) const;
		void Save(pugi::xml_node& node);
		void Load(const pugi::xml_node& node);
		const std::vector<PWeakNode>& GetBones() const { return bones_; }
		const PWeakNode& GetRoot() const { return root_; }
	private:
		PWeakScene scene_;
		std::weak_ptr<Mesh> mesh_;
		PWeakNode root_;
		std::vector<PWeakNode> bones_;
	};
}