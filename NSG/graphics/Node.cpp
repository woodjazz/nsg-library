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
#include "Constants.h"
#include <algorithm>

namespace NSG
{
    Vertex3 Node::UP = Vertex3(0, 1, 0);

    static IdType s_node_id = 1;

    Node::Node(const std::string& name)
        : parent_(nullptr),
          id_(s_node_id++),
          scale_(1, 1, 1),
          globalScale_(1, 1, 1),
          inheritScale_(true),
          dirty_(true),
          name_(name)
    {
    }

    Node::~Node()
    {
        ClearAllChildren();
    }

    void Node::SetParent(PNode parent)
   	{
   		SetParent(parent.get());
   	}

    void Node::SetParent(Node* parent)
   	{
   		//Do not insert child in parent (IMGUI will make memory grow for ever)
   		parent_ = parent;
   		MarkAsDirty();
   	}

    void Node::RemoveFromParent()
    {
        if (parent_)
            parent_->RemoveChild(this);
    }

    void Node::ClearAllChildren()
    {
        for (unsigned i = children_.size() - 1; i < children_.size(); --i)
        {
            Node* childNode = children_[i].get();
            childNode->ClearAllChildren();
            children_.erase(children_.begin() + i);
        }
    }

    void Node::RemoveChild(Node* node)
    {
        int idx = 0;
        for (auto& child : children_)
        {
            if (child.get() == node)
            {
                children_.erase(children_.begin() + idx);
                break;
            }
            ++idx;
        }
    }

    void Node::AddChild(PNode node)
    {
        CHECK_ASSERT(node && node.get() != this && node->parent_ != this, __FILE__, __LINE__);
        node->RemoveFromParent();
        children_.push_back(node);
        node->parent_ = this;
        node->MarkAsDirty();
    }

    void Node::Save(pugi::xml_node& node)
    {

    }

    void Node::Translate(const Vector3& delta, TransformSpace space)
    {
        switch (space)
        {
        case TS_LOCAL:
            // Note: local space translation disregards local scale for scale-independent movement speed
			position_ += GetOrientation() * delta;
            break;

        case TS_PARENT:
            position_ += delta;
            break;

        case TS_WORLD:
			position_ += !parent_ ? delta : Vector3(parent_->GetGlobalModelInvMatrix() * Vector4(delta, 0.0f));
            break;
        }

        MarkAsDirty();
    }


    void Node::SetPosition(const Vertex3& position)
    {
        if (position_ != position)
        {
            position_ = position;
            MarkAsDirty();
        }
    }

    void Node::SetOrientation(const Quaternion& q)
    {
        if (q_ != q)
        {
            q_ = q;
            MarkAsDirty();
        }
    }

    void Node::SetScale(const Vertex3& scale)
    {
        if (scale_ != scale)
        {
            scale_ = scale;
            MarkAsDirty();
        }
    }

    void Node::SetGlobalScale(const Vertex3& scale)
    {
        if (parent_ == nullptr)
        {
            SetScale(scale);
        }
        else
        {
            Vertex3 globalScale(parent_->GetGlobalScale());
            globalScale.x = 1 / globalScale.x;
            globalScale.y = 1 / globalScale.y;
            globalScale.z = 1 / globalScale.z;

            SetScale(globalScale * scale);
        }
    }


    void Node::SetGlobalPosition(const Vertex3& position)
    {
        if (parent_ == nullptr)
        {
            SetPosition(position);
        }
        else
        {
            SetPosition(Vertex3(parent_->GetGlobalModelInvMatrix() * Vertex4(position, 1)));
        }
    }

    void Node::SetGlobalOrientation(const Quaternion& q)
    {
        if (parent_ == nullptr)
        {
            SetOrientation(q);
        }
        else
        {
            SetOrientation(glm::normalize(Quaternion(parent_->GetGlobalModelInvMatrix()) * q));
        }
    }

    const Vertex3& Node::GetGlobalPosition() const
    {
        if (dirty_)
            Update();

        return globalPosition_;
    }

    const Quaternion& Node::GetGlobalOrientation() const
    {
        if (dirty_)
            Update();

        return globalOrientation_;
    }

    Vertex3 Node::GetGlobalScale() const
    {
        if (dirty_)
            Update();

        return globalScale_;
    }


    void Node::SetInheritScale(bool inherit)
    {
        if (inheritScale_ != inherit)
        {
            inheritScale_ = inherit;
            MarkAsDirty();
        }
    }

    void Node::SetLookAt(const Vertex3& lookAtPosition, const Vertex3& up)
    {
        const Vertex3& position = GetGlobalPosition();
        float length = glm::length(position - lookAtPosition);

        if (length > 0)
        {
            // we are using glm::lookAt that generates a view matrix (for a camera) some we have to invert the result
            Matrix4 m = glm::inverse(glm::lookAt(position, lookAtPosition, up));
            SetGlobalOrientation(glm::quat_cast(m));
        }
    }

    void Node::SetGlobalPositionAndLookAt(const Vertex3& newPosition, const Vertex3& lookAtPosition, const Vertex3& up)
    {
        SetGlobalPosition(newPosition);
        SetLookAt(lookAtPosition, up);
    }

    void Node::Update(bool updateChildren) const
    {
        if (!dirty_)
            return;

        if (parent_)
        {
            if (parent_->dirty_)
                parent_->Update(false);

            globalPosition_ = parent_->globalOrientation_ * (parent_->globalScale_ * position_);
            globalPosition_ += parent_->globalPosition_;

            if (inheritScale_)
            {
                globalScale_ = parent_->globalScale_ * scale_;
            }
            else
            {
                globalScale_ = scale_;
            }

            globalOrientation_ = parent_->globalOrientation_ * q_;

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
        lookAtDirection_ = globalOrientation_ * VECTOR3_FORWARD;

        if (updateChildren)
        {
            auto it = children_.begin();
            while (it != children_.end())
            {
                (*it)->dirty_ = true;
                (*it)->SetUniformsNeedUpdate();
                (*it)->Update();
                ++it;
            }
        }

        dirty_ = false;
    }

    const Matrix4& Node::GetGlobalModelMatrix() const
    {
        if (dirty_)
            Update();

        return globalModel_;
    }

    const Matrix3& Node::GetGlobalModelInvTranspMatrix() const
    {
        if (dirty_)
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
        if (dirty_)
            Update();

        return lookAtDirection_;
    }

    bool Node::IsPointInsideBB(const Vertex3& point) const
    {
        if (dirty_)
            Update();

        BoundingBox box(*this);
        return box.IsInside(point);
    }

    void Node::MarkAsDirty()
    {
        dirty_ = true;
        SetUniformsNeedUpdate();
        OnDirty();
        auto it = children_.begin();
        while (it != children_.end())
            (*(it++))->MarkAsDirty();
    }

}