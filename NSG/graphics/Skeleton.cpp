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
#include "Skeleton.h"
#include "SceneNode.h"
#include "Bone.h"
#include "Check.h"
#include "ModelMesh.h"
#include "Scene.h"
#include "Util.h"
#include "StringConverter.h"
#include "pugixml.hpp"
#include <sstream>

namespace NSG
{
    template<> std::map<std::string, PWeakSkeleton> WeakFactory<std::string, Skeleton>::objsMap_ = std::map<std::string, PWeakSkeleton>{};

    Skeleton::Skeleton(const std::string& name)
        : Object(name)
    {
    }

    Skeleton::~Skeleton()
    {
    }

    void Skeleton::Save(pugi::xml_node& node)
    {
        pugi::xml_node skeletonNode = node.append_child("Skeleton");
        skeletonNode.append_attribute("name") = name_.c_str();
        {
            auto shaderOrderNode = skeletonNode.append_child("ShaderOrder");
            for (auto& obj : shaderOrder_)
            {
                auto boneNode = shaderOrderNode.append_child("Bone");
                boneNode.append_attribute("name") = obj.c_str();
                const Matrix4& offset = GetBoneOffsetMatrix(obj);
                boneNode.append_attribute("offsetMatrix").set_value(ToString(offset).c_str());
            }
        }
        {
            auto bonesNode = skeletonNode.append_child("Bones");
            for (auto bone : rootBones_)
                bone->Save(bonesNode);
        }
    }

    void Skeleton::Load(const pugi::xml_node& node)
    {
        std::string name = node.attribute("name").as_string();
        CHECK_ASSERT(name == name_);
		shaderOrder_.clear();
        offsets_.clear();
        rootBones_.clear();
        {
            auto boneNode = node.child("ShaderOrder").child("Bone");
            while (boneNode)
            {
                std::string name = boneNode.attribute("name").as_string();
                SetBoneOffsetMatrix(name, ToMatrix4(boneNode.attribute("offsetMatrix").as_string()));
                shaderOrder_.push_back(name);
                boneNode = boneNode.next_sibling("Bone");
            }
        }

        {
            auto boneNode = node.child("Bones").child("Bone");
            while (boneNode)
            {
                std::string name = boneNode.attribute("name").as_string();
                auto bone = std::make_shared<Bone>(name);
                bone->Load(boneNode);
                rootBones_.push_back(bone);
                boneNode = boneNode.next_sibling("Bone");
            }
        }
    }

    void Skeleton::SetBoneOffsetMatrix(const std::string& name, const Matrix4& offset)
    {
        offsets_[name] = offset;
    }

    const Matrix4& Skeleton::GetBoneOffsetMatrix(const std::string& name) const
    {
        auto it = offsets_.find(name);
        return it->second;
    }

    void Skeleton::SaveSkeletons(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Skeletons");
        auto skeletons = Skeleton::GetObjs();
        for (auto& obj : skeletons)
            obj->Save(child);
    }

    void Skeleton::CreateBonesFor(PNode parent, PBone bone) const
    {
    	auto clone = bone->Clone();
    	clone->SetParent(parent);
        for (auto child : bone->GetChildren())
			CreateBonesFor(clone, std::dynamic_pointer_cast<Bone>(child));
    }

    void Skeleton::CreateBonesFor(PSceneNode sceneNode) const
    {
        for (auto node : rootBones_)
            CreateBonesFor(sceneNode, node);
    }

	size_t Skeleton::GetNumberOfBones() const
	{
		return shaderOrder_.size();
	}
}