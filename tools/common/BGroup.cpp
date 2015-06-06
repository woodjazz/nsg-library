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

#include "BGroup.h"

namespace BlenderConverter
{
    BGroup::BGroup(const std::string& name)
    : name_(name)
    {
    };

    void BGroup::AddGroup(const std::string& name, PSceneNode node)
    {
		groupInstances_.push_back(GroupInstance{name, node});
    }

    void BGroup::AddSceneNode(PSceneNode node)
    {
		auto result = objects_.insert(Objects::value_type(node->GetName(), node));
		//CHECK_ASSERT(result.second && "Insert for object in group has failed!!!", __FILE__, __LINE__);

    }

	void BGroup::CreateDupli(PSceneNode node, PSceneNode parent)
	{
		auto dupliNode = parent->CreateChild<SceneNode>(node->GetName());
		dupliNode->SetMesh(node->GetMesh());
		dupliNode->SetMaterial(node->GetMaterial());
		if (node->GetRigidBody())
			dupliNode->GetOrCreateRigidBody();
		dupliNode->SetPosition(node->GetPosition());
		dupliNode->SetOrientation(node->GetOrientation());
		dupliNode->SetScale(node->GetScale());
	}


	void BGroup::CreateObjects(PSceneNode parent)
	{
		for (auto& object : objects_)
			CreateDupli(object.second, parent);
	}
}
