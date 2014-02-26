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
	Node::Node(PNode pParent) 
	: scale_(1,1,1),
	pParent_(pParent)
	{
		Update();
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

	void Node::SetScale(const Vertex3& scale)
	{
		scale_ = scale;
		Update();
	}

	void Node::Update()
	{
		matModel_ = glm::translate(glm::mat4(), position_) * glm::mat4_cast(q_) * glm::scale(glm::mat4(1.0f), scale_);
		matModelInvTransp_ = glm::transpose(glm::inverse(Matrix3(GetModelMatrix())));
	}

	const Matrix4&& Node::GetModelMatrix() const 
	{ 
		if(pParent_)
		{
			matTemporal_ = matModel_ * pParent_->GetModelMatrix();

			return std::move(matTemporal_);
		}
		else
			return std::move(matModel_);
	}

	const Matrix3&& Node::GetModelInvTRanspMatrix() const
	{
		return std::move(matModelInvTransp_);	
	}

}