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
#include <vector>

namespace NSG
{
	class Skeleton
	{
	public:
		Skeleton(PMesh mesh);
		virtual ~Skeleton();
		void SetRoot(PNode root) { root_ = root; }
		void SetBones(const std::vector<PNode>& bones) { bones_ = bones; }
		unsigned GetBoneIndex(const std::string& name) const;
		void SetBlendData(const std::vector<std::vector<unsigned>>& blendIndices, const std::vector<std::vector<float>>& blendWeights);
		void Save(pugi::xml_node& node);
		void Load(const pugi::xml_node& node);
		const std::vector<PNode>& GetBones() const { return bones_; }
		const PNode& GetRoot() const { return root_; }
	private:
		std::weak_ptr<Mesh> mesh_;
		PNode root_;
		std::vector<PNode> bones_;
		std::vector<std::vector<unsigned>> blendIndices_; // per vertex indices
        std::vector<std::vector<float>> blendWeights_; // per vertex weights
	};
}