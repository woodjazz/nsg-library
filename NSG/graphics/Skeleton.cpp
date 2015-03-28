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
#include "Skeleton.h"
#include "Node.h"
#include "Check.h"
#include "ModelMesh.h"
#include "Scene.h"
#include "Util.h"
#include "pugixml.hpp"
#include <sstream>

namespace NSG
{
    Skeleton::Skeleton(PMesh mesh)
        : mesh_(mesh)
    {

    }

    Skeleton::~Skeleton()
    {

    }

    unsigned Skeleton::GetBoneIndex(const std::string& name) const
    {
        unsigned result = -1;
        unsigned idx = 0;
        for (auto& obj : bones_)
        {
			PNode bone = obj.lock();
            if (bone->GetName() == name)
            {
                result = idx;
                break;
            }
            ++idx;
        }
        return result;
    }

    void Skeleton::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Skeleton");
        child.append_attribute("meshName") = mesh_.lock()->GetName().c_str();
        child.append_attribute("rootName") = root_.lock()->GetName().c_str();

        {
            pugi::xml_node childBones = child.append_child("Bones");
            for (auto& obj : bones_)
            {
				PNode bone = obj.lock();
                pugi::xml_node childBone = childBones.append_child("Bone");
				childBone.append_attribute("boneName") = bone->GetName().c_str();

				{
					const Matrix4& offset = bone->GetBoneOffsetMatrix();

					std::stringstream ss;
					ss << ToString(offset);
					childBone.append_attribute("offsetMatrix") = ss.str().c_str();
				}

            }
        }
    }

    void Skeleton::Load(const pugi::xml_node& node)
    {
        std::string meshName = node.attribute("meshName").as_string();
		mesh_ = Mesh::Get<ModelMesh>(meshName);
		CHECK_CONDITION(mesh_.lock(), __FILE__, __LINE__);
        std::string rootName = node.attribute("rootName").as_string();
        PScene scene = scene_.lock();
        root_ = scene->GetChild<Node>(rootName, true);
        CHECK_ASSERT(root_.lock(), __FILE__, __LINE__);
        bones_.clear();
        pugi::xml_node childBones = node.child("Bones");
        if (childBones)
        {
            pugi::xml_node child = childBones.child("Bone");
            while (child)
            {
                std::string boneName = child.attribute("boneName").as_string();
                PNode bone = scene->GetChild<Node>(boneName, true);
                CHECK_ASSERT(bone, __FILE__, __LINE__);
				bone->SetBoneOffsetMatrix(GetMatrix4(child.attribute("offsetMatrix").as_string()));
                bones_.push_back(bone);
                child = child.next_sibling("Bone");
            }
        }
    }

}