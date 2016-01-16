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
#include "Object.h"
#include "WeakFactory.h"
#include <vector>
#include <map>

namespace NSG
{
	class Skeleton : public std::enable_shared_from_this<Skeleton>, public Object, public WeakFactory<std::string, Skeleton>
    {
    public:
		Skeleton(const std::string& name);
        virtual ~Skeleton();
		size_t GetNumberOfBones() const;
        void Save(pugi::xml_node& node);
        void Load(const pugi::xml_node& node) override;
		const std::vector<std::string>& GetShaderOrder() const { return shaderOrder_; }
		const Matrix4& GetBoneOffsetMatrix(const std::string& name) const;
		bool IsEmpty() const { return shaderOrder_.empty(); }
		static void SaveSkeletons(pugi::xml_node& node);
		void Set(PResource resource);
		void CreateBonesFor(PSceneNode sceneNode) const;
    private:
		void SetBoneOffsetMatrix(const std::string& name, const Matrix4& offset);
		void CreateBonesFor(PNode parent, PBone bone) const;
        std::vector<std::string> shaderOrder_;
		// Offset matrix that converts from vertex space to bone space
		std::map<std::string, Matrix4> offsets_;
		std::vector<PBone> rootBones_;
    };
}