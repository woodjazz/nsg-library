/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "Node.h"

namespace NSG
{
	class Bone : public Node
	{
	public:
		Bone(const std::string& name);
		~Bone();
        void SetPose(const Matrix4& pose) { pose_ = pose; }
        const Matrix4& GetPose() const { return pose_; }
		void Load(const pugi::xml_node& node) override;
		void Save(pugi::xml_node& node) const override;
		PBone Clone() const;
        static size_t GetMaxPlatformBones(size_t nBones);
        static const size_t MaxBones = 64;
        static const size_t MaxBonesPerVertex = 4;
	private:
		Matrix4 pose_;
	};
}
