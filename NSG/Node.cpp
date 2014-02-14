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
#include "Node.h"
#include "NSG/Log.h"

namespace NSG
{
	Node::Node() 
	{
	}

	Node::~Node() 
	{
	}

	void Node::SetPosition(const Vertex3& position)
	{
		position_ = position;
		Update();
	}

	void Node::SetRotation(const Quaternion& q)
	{
		q_ = q;
		Update();
	}

	void Node::Update()
	{
		matModelView_ = glm::translate(glm::mat4(), position_) * glm::mat4_cast(q_);
	}

}