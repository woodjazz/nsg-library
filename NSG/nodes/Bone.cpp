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
#include "Bone.h"
#include "pugixml.hpp"

namespace NSG
{
    Bone::Bone(const std::string& name)
        : Node(name)
    {
    }

    Bone::~Bone()
    {
    }

    void Bone::Load(const pugi::xml_node& node)
    {
        Node::Load(node);
        SetPose(Matrix4(GetPosition(), GetOrientation(), GetScale()));
        pugi::xml_node child = node.child("Bone");
        while (child)
        {
            std::string name = child.attribute("name").as_string();
            auto childBone = CreateChild<Bone>(name);
            childBone->Load(child);
            child = child.next_sibling("Bone");
        }
    }

    void Bone::Save(pugi::xml_node& node) const
    {
        pugi::xml_node child = node.append_child("Bone");
        Node::Save(child);
        for (auto childObj : GetChildren())
            std::dynamic_pointer_cast<Bone>(childObj)->Save(child);
    }

    PBone Bone::Clone() const
    {
        auto clone = std::make_shared<Bone>(GetName());
        clone->SetPosition(GetPosition());
        clone->SetOrientation(GetOrientation());
        clone->SetScale(GetScale());
        clone->SetPose(GetPose());
        return clone;
    }

}
