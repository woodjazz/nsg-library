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

	void Node::SetOrientation(const Quaternion& q)
	{
		q_ = q;
		Update();
	}

	void Node::SetScale(const Vertex3& scale)
	{
		scale_ = scale;
		Update();
	}

	void Node::SetGlobalPosition(const Vertex3& position) 
	{
		if(pParent_ == nullptr) 
		{
			SetPosition(position);
		} 
		else 
		{
			SetPosition(Vertex3(Vertex4(position, 1) * glm::inverse(pParent_->GetModelMatrix())));
		}
	}	

	void Node::SetGlobalOrientation(const Quaternion& q) 
	{
		if(pParent_ == nullptr) 
		{
			SetOrientation(q);
		} 
		else 
		{
			Matrix4 invParent(glm::inverse(pParent_->GetModelMatrix()));
			Matrix4 m(glm::mat4_cast(q) * invParent);
			SetOrientation(glm::quat_cast(m));
		}
	}	

	Vertex3 Node::GetGlobalPosition() const
	{
		return Vertex3(GetModelMatrix()[3]);
	}

	void Node::SetLookAt(const Vertex3& center, const Vertex3& up)
	{
        Vertex3 upVector(up);

        if(pParent_) 
            upVector = upVector * glm::inverse(Matrix3(pParent_->GetModelMatrix()));	

        Vertex3 zaxis = glm::normalize(GetGlobalPosition() - center);	
        if (glm::length(zaxis) > 0) 
        {
            Vertex3 xaxis = glm::normalize(glm::cross(upVector, zaxis));	
            Vertex3 yaxis = glm::cross(zaxis, xaxis);

            Matrix4 m;
            m[0] = Vertex4(xaxis.x, xaxis.y, xaxis.z, 0);
            m[1] = Vertex4(yaxis.x, yaxis.y, yaxis.z, 0);
            m[2] = Vertex4(zaxis.x, zaxis.y, zaxis.z, 0);

            SetGlobalOrientation(glm::quat_cast(m));
        }
/*        float length = glm::length(GetGlobalPosition() - center);
        
        if(length > 0)
        {
			Matrix4 m = glm::lookAt(GetGlobalPosition(), center, up);
			SetGlobalOrientation(glm::quat_cast(m));
		}*/
	}

    void Node::Update()
	{
		matModel_ = glm::translate(glm::mat4(), position_) * glm::mat4_cast(q_) * glm::scale(glm::mat4(1.0f), scale_);
		matModelInvTransp_ = glm::transpose(glm::inverse(Matrix3(GetModelMatrix())));
		OnUpdate();
	}

	const Matrix4& Node::GetModelMatrix() const 
	{ 
		if(pParent_)
		{
			matTemporal_ = matModel_ * pParent_->GetModelMatrix();

			return matTemporal_;
		}
		else
			return matModel_;
	}

	const Matrix3& Node::GetModelInvTranspMatrix() const
	{
		return matModelInvTransp_;	
	}

}