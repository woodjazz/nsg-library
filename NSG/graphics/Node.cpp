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
#include "BoundingBox.h"
#include <algorithm>

namespace NSG
{
	Vertex3 Node::UP = Vertex3(0,1,0);

	static IdType s_node_id = 1;

	Node::Node() 
	: id_(s_node_id++),
	scale_(1,1,1),
	globalScale_(1,1,1),
	inheritScale_(true),
	dirty_(true)
	{
	}

	Node::~Node() 
	{
		if(pParent_)
		{
			RemoveFromParent();
		}
	}

	void Node::RemoveFromParent() 
	{
		CHECK_ASSERT(pParent_ && "parent does not exist!!!", __FILE__, __LINE__);
		
		auto it = std::find(pParent_->children_.begin(), pParent_->children_.end(), this);
		CHECK_ASSERT(it != pParent_->children_.end() && "Child not found!!!", __FILE__, __LINE__);
		pParent_->children_.erase(it);
	}

    void Node::SetParent(PNode pParent)
    {
    	if(pParent_ != pParent)
    	{
			if(pParent)
			{
				pParent->children_.push_back(this);
			}
			else
			{
				RemoveFromParent();
			}
            
            pParent_ = pParent;

			MarkAsDirty();
        }
    }

    void Node::SetParent(Node* pParent)
    {
		struct D 
		{ 
		    void operator()(Node* p) const 
		    {
		        //delete p; //do not delete
		    }
		};    	

		PNode pNode(pParent, D());
		SetParent(pNode);
    }

	void Node::SetPosition(const Vertex3& position)
	{
        if(position_ != position)
        {
		    position_ = position;
			MarkAsDirty();
        }
	}

	void Node::SetOrientation(const Quaternion& q)
	{
        if(q_ != q)
        {
		    q_ = q;
			MarkAsDirty();
        }
	}

	void Node::SetScale(const Vertex3& scale)
	{
        if(scale_ != scale)
        {
		    scale_ = scale;
			MarkAsDirty();
        }
	}

	void Node::SetGlobalScale(const Vertex3& scale)
	{
		if(pParent_ == nullptr) 
		{
			SetScale(scale);
		}		
        else
        {
			Vertex3 globalScale(pParent_->GetGlobalScale());
			globalScale.x = 1 / globalScale.x;
			globalScale.y = 1 / globalScale.y;
			globalScale.z = 1 / globalScale.z;

			SetScale(globalScale * scale);
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
			SetPosition(Vertex3(pParent_->GetGlobalModelInvMatrix() * Vertex4(position, 1)));
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
			SetOrientation(glm::normalize(Quaternion(pParent_->GetGlobalModelInvMatrix()) * q));
		}
	}	

	const Vertex3& Node::GetGlobalPosition() const
	{ 
		if(dirty_)
			Update();

		return globalPosition_; 
	}

	const Quaternion& Node::GetGlobalOrientation() const
	{ 
		if(dirty_)
			Update();

		return globalOrientation_; 
	}

	Vertex3 Node::GetGlobalScale() const
	{ 
		if(dirty_)
			Update();

		return globalScale_; 
	}


	void Node::SetInheritScale(bool inherit) 
	{ 
		if(inheritScale_ != inherit)
		{
			inheritScale_ = inherit; 
			dirty_ = true;
		}
	}

	void Node::SetLookAt(const Vertex3& center, const Vertex3& up)
	{
#if 0		
        Vertex3 upVector(up);

        if(pParent_) 
			upVector = Matrix3(pParent_->GetGlobalModelInvMatrix()) * upVector;   

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
#else
        float length = glm::length(GetGlobalPosition() - center);
        
        if(length > 0)
        {
			Matrix4 m = glm::inverse(glm::lookAt(GetGlobalPosition(), center, up));
			SetGlobalOrientation(glm::quat_cast(m));
		}
#endif
	}

    void Node::Update(bool updateChildren) const
	{
		if(!dirty_)
			return;

		if(pParent_)
		{
			if(pParent_->dirty_)
				pParent_->Update(false);

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

		if (updateChildren)
		{
			auto it = children_.begin();
			while (it != children_.end())
			{
				(*it)->dirty_ = true;
				(*it)->Update();
				++it;
			}
		}

		dirty_ = false;

		OnUpdate();
	}

	const Matrix4& Node::GetGlobalModelMatrix() const
	{ 
		if(dirty_)
			Update();

        return globalModel_;
	}

	const Matrix3& Node::GetGlobalModelInvTranspMatrix() const
	{ 
		if(dirty_)
			Update();

		return globalModelInvTransp_; 
	}	

	const Matrix4& Node::GetGlobalModelInvMatrix() const
	{
		if (dirty_)
			Update();

		return globalModelInv_;
	}


	const Vertex3& Node::GetLookAtDirection() const
	{ 
		if(dirty_)
			Update();

		return lookAtDirection_; 
	}

	bool Node::IsPointInsideBB(const Vertex3& point) const
	{
		if(dirty_)
			Update();

		BoundingBox box(*this);
		return box.IsInside(point);
	}

	void Node::MarkAsDirty()
	{
		dirty_ = true;
		auto it = children_.begin();
		while (it != children_.end())
			(*(it++))->MarkAsDirty();
	}

}