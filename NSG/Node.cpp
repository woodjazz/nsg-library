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
	static GLushort s_node_id = 1;

	Node::Node() 
	: id_(s_node_id++),
	scale_(1,1,1),
	enableSelection_(true)
	{
        Update(false);
	}

	Node::~Node() 
	{
	}

    void Node::SetParent(PNode pParent)
    {
    	if(pParent_ != pParent)
    	{
			if(pParent)
			{
				pParent->children_.insert(this);
			}
			else
			{
				pParent_->children_.erase(this);
			}
            
            pParent_ = pParent;

			Update();
        }
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

	Vertex3 Node::GetLocalPositionFromGlobal(const Vertex3& position) const
	{
		if(pParent_ == nullptr) 
		{
			return position;
		} 
		else 
		{
			return Vertex3(Vertex4(position, 1) * glm::inverse(pParent_->GetModelMatrix()));
		}
	}	

	void Node::SetGlobalPosition(const Vertex3& position) 
	{
		if(pParent_ == nullptr) 
		{
			SetPosition(position);
		} 
		else 
		{
			SetPosition(Vertex3(glm::inverse(pParent_->GetModelMatrix()) * Vertex4(position, 1)));
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
			Matrix4 m(invParent * glm::mat4_cast(q));
			SetOrientation(glm::quat_cast(m));
		}
	}	

	Vertex3 Node::GetGlobalScale() const 
	{
		if(pParent_) 
			return pParent_->GetGlobalScale() * scale_;
		else 
			return scale_;
	}

	void Node::SetLookAt(const Vertex3& center, const Vertex3& up)
	{
        Vertex3 upVector(up);

        if(pParent_) 
            upVector = glm::inverse(Matrix3(pParent_->GetModelMatrix())) * upVector;	

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
        /*
        float length = glm::length(GetGlobalPosition() - center);
        
        if(length > 0)
        {
			Matrix4 m = glm::lookAt(GetGlobalPosition(), center, up);
			SetGlobalOrientation(glm::quat_cast(m));
		}*/
	}

    void Node::Update(bool notify)
	{
		matModel_ = glm::translate(glm::mat4(), position_) * glm::mat4_cast(q_) * glm::scale(glm::mat4(1.0f), scale_);
        //matModel_ = glm::scale(glm::mat4(1.0f), scale_) * glm::mat4_cast(q_) * glm::translate(glm::mat4(), position_);
		const Matrix4& globalModel = GetModelMatrix();
		matModelInvTransp_ = glm::transpose(glm::inverse(Matrix3(globalModel)));
		globalPosition_ = Vertex3(globalModel[3]);
		globalOrientation_ = glm::quat_cast(globalModel);
		static Vertex3 localDir(0,0,-1);
		direction_ = globalOrientation_ * localDir;

		auto it = children_.begin();
		while(it != children_.end())
        {
			(*it)->Update();
            ++it;
        }

        if(notify)
		    OnUpdate();
	}

	const Matrix4& Node::GetModelMatrix() const 
	{ 
		if(pParent_)
		{
			matTemporal_ = pParent_->GetModelMatrix() * matModel_;

			return matTemporal_;
		}
		else
			return matModel_;
	}


}