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
#include "Log.h"

namespace NSG
{
	static GLushort s_node_id = 1;

	Node::Node() 
	: id_(s_node_id++),
	scale_(1,1,1),
	globalScale_(1,1,1),
	inheritScale_(true),
	updateEnabled_(true)
	{
        Update(false);
	}

	Node::~Node() 
	{
		if(pParent_)
			pParent_->children_.erase(this);
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

	void Node::SetGlobalPosition(const Vertex3& position) 
	{
		if(pParent_ == nullptr) 
		{
			SetPosition(position);
		} 
		else 
		{
			SetPosition(Vertex3(pParent_->globalModelInv_ * Vertex4(position, 1)));
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
            SetOrientation(glm::normalize(Quaternion(pParent_->globalModelInv_) * q));
		}
	}	

	void Node::SetInheritScale(bool inherit) 
	{ 
		if(inheritScale_ != inherit)
		{
			inheritScale_ = inherit; 
			Update();
		}
	}

	void Node::SetLookAt(const Vertex3& center, const Vertex3& up)
	{
#if 0		
        Vertex3 upVector(up);

        if(pParent_) 
            upVector = Matrix3(pParent_->globalModelInv_) * upVector;   

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
#endif
#if 1
        float length = glm::length(GetGlobalPosition() - center);
        
        if(length > 0)
        {
			Matrix4 m = glm::inverse(glm::lookAt(GetGlobalPosition(), center, up));
			SetGlobalOrientation(glm::quat_cast(m));
		}
#endif
	}

    void Node::Update(bool notify)
	{
		if(!updateEnabled_)
			return;

		if(pParent_)
		{
			globalPosition_ = pParent_->globalOrientation_ * (pParent_->globalScale_ * position_);
            globalPosition_ += pParent_->globalPosition_;

			if(inheritScale_)
			{
				globalScale_ = pParent_->globalScale_ * scale_;
			}
			else
			{
				globalScale_ = scale_;
			}

			globalOrientation_ = pParent_->globalOrientation_ * q_;
			
		}
		else
        {
        	globalPosition_ = position_;
        	globalOrientation_ = q_;
        	globalScale_ = scale_;
        }

        globalModel_ = glm::translate(glm::mat4(), globalPosition_) * glm::mat4_cast(globalOrientation_) * glm::scale(glm::mat4(1.0f), globalScale_);
        globalModelInv_ = glm::inverse(globalModel_);
		globalModelInvTransp_ = glm::transpose(glm::inverse(Matrix3(globalModel_)));
		static Vertex3 localAtDir(0,0,-1);
		lookAtDirection_ = globalOrientation_ * localAtDir;

		auto it = children_.begin();
		while(it != children_.end())
        {
			(*it)->Update();
            ++it;
        }

        if(notify)
        {
		    OnUpdate();
        }
	}

	const Matrix4& Node::GetGlobalModelMatrix() const 
	{ 
        return globalModel_;
	}

	void Node::EnableUpdate(bool enable) 
	{ 
		updateEnabled_ = enable; 

		auto it = children_.begin();
		while(it != children_.end())
        {
			(*it)->EnableUpdate(enable);
            ++it;
        }

	}


}