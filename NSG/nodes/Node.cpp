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
#include "Node.h"
#include "Scene.h"
#include "Light.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include "Octree.h"
#include "Log.h"
#include "BoundingBox.h"
#include "Constants.h"
#include "Util.h"
#include "StringConverter.h"
#include "Check.h"
#include "SharedFromPointer.h"
#include "imgui.h"
#include "pugixml.hpp"
#include <algorithm>
#include <iterator>

namespace NSG
{
    static IdType s_node_id = 1;

    Node::Node(const std::string& name)
        : name_(name),
          signalUpdated_(new SignalEmpty()),
          id_(s_node_id++),
          scale_(1, 1, 1),
          globalScale_(1, 1, 1),
          inheritScale_(true),
          dirty_(true),
          hide_(false),
          isScaleUniform_(true),
          userData_(nullptr)
    {
    }

    Node::~Node()
    {
        ClearAllChildren();
    }

    void Node::SetParent(PNode parent)
    {
        RemoveFromParent();
        if (parent)
            parent->AddChild(SharedFromPointerNode(this));
        MarkAsDirty();
    }

    PNode Node::GetParent() const
    {
        return parent_.lock();
    }

    void Node::RemoveFromParent()
    {
        auto parent = parent_.lock();
        if (parent)
            parent->RemoveChild(this);
    }

    void Node::ClearAllChildren()
    {
        childrenHash_.clear();
        for (auto i = children_.size() - 1; i < children_.size(); --i)
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
                auto range = childrenHash_.equal_range(node->name_);
                auto it = range.first;
                while (it != range.second)
                {
                    PNode child = it->second.lock();
                    if (!child)
                        it = childrenHash_.erase(it);
                    else if (child.get() == node)
                        it = childrenHash_.erase(it);
                    else
                        ++it;
                }
                break;
            }
            ++idx;
        }
    }

    void Node::AddChild(PNode node)
    {
        CHECK_ASSERT(node && node.get() != this);
        children_.push_back(node);
        childrenHash_.insert(std::make_pair(node->name_, node));
        PNode thisNode = SharedFromPointerNode(this);
        node->parent_ = thisNode;
        PScene scene = scene_.lock();
        if (!scene) scene = std::dynamic_pointer_cast<Scene>(thisNode);
        node->scene_ = scene;
        if (scene)
        {
            PLight light = std::dynamic_pointer_cast<Light>(node);
            if (light)
                scene->AddLight(light.get());
            else
            {
                PCamera camera = std::dynamic_pointer_cast<Camera>(node);
                if (camera) scene->AddCamera(camera.get());
                else
                {
                    auto ps = std::dynamic_pointer_cast<ParticleSystem>(node);
                    if (ps) scene->AddParticleSystem(ps.get());
                }
            }
        }
        node->MarkAsDirty();
    }

    void Node::Translate(const Vector3& delta, TransformSpace space)
    {
        switch (space)
        {
            case TS_LOCAL:
                // Note: local space translation disregards local scale for scale-independent movement speed
                position_ = position_ + GetOrientation() * delta;
                break;

            case TS_PARENT:
                position_ = position_ + delta;
                break;

            case TS_WORLD:
                {
                    PNode parent = parent_.lock();
                    position_ = position_ + ((parent == scene_.lock() || !parent) ? delta : Vector3(parent->GetGlobalModelInvMatrix() * Vector4(delta, 0.0f)));
                    break;
                }
        }

        MarkAsDirty();
    }

    void Node::Rotate(const Quaternion& delta, TransformSpace space)
    {
        switch (space)
        {
            case TS_LOCAL:
                q_ = (q_ * delta).Normalize();
                break;

            case TS_PARENT:
                q_ = (delta * q_).Normalize();
                break;

            case TS_WORLD:
                {
                    PNode parent = parent_.lock();
                    if (parent == scene_.lock() || !parent)
                        q_ = (delta * q_).Normalize();
                    else
                    {
                        Quaternion worldRotation = GetGlobalOrientation();
                        q_ = q_ * worldRotation.Inverse() * delta * worldRotation;
                    }
                    break;
                }
        }

        MarkAsDirty();
    }

    void Node::Yaw(float angle, TransformSpace space)
    {
        Rotate(Quaternion(angle, VECTOR3_UP), space);
    }

    void Node::Pitch(float angle, TransformSpace space)
    {
        Rotate(Quaternion(angle, VECTOR3_RIGHT), space);
    }

    void Node::Roll(float angle, TransformSpace space)
    {
        Rotate(Quaternion(angle, VECTOR3_FORWARD), space);
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
            MarkAsDirty(true, true);
        }
    }

    void Node::SetScale(float scale)
    {
        SetScale(Vertex3(scale));
    }

    void Node::SetGlobalScale(const Vertex3& scale)
    {
        PNode parent = parent_.lock();

        if (parent == nullptr)
        {
            SetScale(scale);
        }
        else
        {
            Vertex3 globalScale(parent->GetGlobalScale());
            globalScale.x = 1 / globalScale.x;
            globalScale.y = 1 / globalScale.y;
            globalScale.z = 1 / globalScale.z;

            SetScale(globalScale * scale);
        }
    }


    void Node::SetGlobalPosition(const Vertex3& position)
    {
        PNode parent = parent_.lock();

        if (parent == nullptr)
        {
            SetPosition(position);
        }
        else
        {
            SetPosition(Vertex3(parent->GetGlobalModelInvMatrix() * Vertex4(position, 1)));
        }
    }

    void Node::SetGlobalOrientation(const Quaternion& q)
    {
        PNode parent = parent_.lock();

        if (parent == nullptr)
        {
            SetOrientation(q);
        }
        else
        {
            SetOrientation((Quaternion(parent->GetGlobalModelInvMatrix()) * q).Normalize());
        }
    }

    const Vertex3& Node::GetGlobalPosition() const
    {
        Update();
        return globalPosition_;
    }

    const Quaternion& Node::GetGlobalOrientation() const
    {
        Update();
        return globalOrientation_;
    }

    Vertex3 Node::GetGlobalScale() const
    {
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

    void Node::SetGlobalLookAtDirection(const Vertex3& direction)
    {
        float length = direction.Length();
        if (length > 0)
        {
            auto rot = Quaternion(-direction, GetUpDirection());
            PNode parent = parent_.lock();
            if (parent)
            {
                Quaternion q = parent->GetGlobalOrientation().Inverse();
                SetOrientation(q * rot);
            }
            else
            {
                SetOrientation(rot);
            }
        }
    }

    void Node::SetGlobalLookAtPosition(const Vertex3& lookAtPosition, const Vertex3& up)
    {
        const Vertex3& position = GetGlobalPosition();
        auto direction = lookAtPosition - position;
        float length = direction.Length();
        if (length > 0)
        {
			Quaternion rot(-direction, up);
            PNode parent = parent_.lock();
            if (parent)
            {
                Quaternion q = parent->GetGlobalOrientation().Inverse();
                SetOrientation(q * rot);
            }
            else
            {
                SetOrientation(rot);
            }
        }
    }

    void Node::SetLocalLookAtPosition(const Vertex3& lookAtPosition, const Vertex3& up)
    {
        auto direction = lookAtPosition - position_;
        float length = direction.Length();
        if (length > 0)
        {
            Quaternion rot(-direction, up);
            SetOrientation(rot);
        }
    }

    Quaternion Node::GetLookAtOrientation(const Vertex3& lookAtPosition, const Vertex3& up)
    {
        const Vertex3& position = GetGlobalPosition();
        auto direction = lookAtPosition - position;
        float length = direction.Length();
        if (length > 0)
        {
            Quaternion rot(-direction, up);
            PNode parent(parent_.lock());
            if (parent)
                return parent->GetGlobalOrientation().Inverse() * rot;
            else
                return rot;
        }
        return GetOrientation();
    }


    void Node::SetGlobalPositionAndLookAt(const Vertex3& newPosition, const Vertex3& lookAtPosition, const Vertex3& up)
    {
        SetGlobalPosition(newPosition);
        SetGlobalLookAtPosition(lookAtPosition, up);
    }

    void Node::Update() const
    {
        if (!dirty_ || hide_)
            return;

        dirty_ = false;

        PNode parent = parent_.lock();

        if (parent)
        {
            globalModel_ = parent->GetGlobalModelMatrix() * GetTransform();
            globalPosition_ = globalModel_.Translation();
            globalOrientation_ = parent->GetGlobalOrientation() * q_;
            globalScale_ = globalModel_.Scale();
        }
        else
        {
            globalModel_ = GetTransform();
            globalPosition_ = position_;
            globalOrientation_ = q_;
            globalScale_ = scale_;
        }

        isScaleUniform_ = globalScale_.IsUniform();
        globalModelInv_ = globalModel_.Inverse();
        globalModelInvTransp_ = Matrix3(globalModel_).Inverse().Transpose();
        lookAtDirection_ = globalOrientation_ * VECTOR3_LOOKAT_DIRECTION;
        upDirection_ = globalOrientation_ * VECTOR3_UP;
		rightDirection_ = globalOrientation_ * VECTOR3_RIGHT;
        signalUpdated_->Run();
    }

    Matrix4 Node::GetTransform() const
    {
        Update();
        return Matrix4(position_, q_, scale_);
    }

    void Node::SetTransform(const Matrix4& transform)
    {
		transform.Decompose(position_, q_, scale_);
        MarkAsDirty(true, true);
    }

    const Matrix4& Node::GetGlobalModelMatrix() const
    {
        Update();
        return globalModel_;
    }

    const Matrix3& Node::GetGlobalModelInvTranspMatrix() const
    {
        Update();
        return globalModelInvTransp_;
    }

    const Matrix4& Node::GetGlobalModelInvMatrix() const
    {
        Update();
        return globalModelInv_;
    }


    const Vertex3& Node::GetLookAtDirection() const
    {
        Update();
        return lookAtDirection_;
    }

    const Vertex3& Node::GetUpDirection() const
    {
        Update();
        return upDirection_;
    }

	const Vertex3& Node::GetRightDirection() const
	{
		Update();
		return rightDirection_;
	}

    bool Node::IsPointInsideBB(const Vertex3& point) const
    {
        Update();
        BoundingBox box(*this);
        return box.IsInside(point);
    }

    void Node::MarkAsDirty(bool recursive, bool scaleChange)
    {
        dirty_ = true;
        SetUniformsNeedUpdate();
        OnDirty();

        if (scaleChange)
            OnScaleChange();

        if (recursive)
            for (auto child : children_)
                child->MarkAsDirty(recursive, scaleChange);
    }

    void Node::Hide(bool hide, bool recursive)
    {
        if (hide != hide_)
        {
            hide_ = hide;
            SetUniformsNeedUpdate();
            OnHide(hide);
        }

        if (recursive)
            for (auto child : children_)
                child->Hide(hide, recursive);
    }

    bool Node::IsScaleUniform() const
    {
        Update();
        return isScaleUniform_;
    }

    void Node::Load(const pugi::xml_node& node)
    {
        CHECK_ASSERT(name_ == node.attribute("name").as_string());
        Vertex3 position(VECTOR3_ZERO);
        auto posAtt = node.attribute("position");
        if (posAtt)
            position = ToVertex3(posAtt.as_string());
        SetPosition(position);
        Quaternion orientation(QUATERNION_IDENTITY);
        auto rotAtt = node.attribute("orientation");
        if (rotAtt)
            orientation = ToQuaternion(rotAtt.as_string());
        SetOrientation(orientation);
        Vertex3 scale(VECTOR3_ONE);
        auto scaAtt = node.attribute("scale");
        if (scaAtt)
            scale = ToVertex3(scaAtt.as_string());
        SetScale(scale);
    }

    void Node::Save(pugi::xml_node& node) const
    {
        node.append_attribute("name").set_value(GetName().c_str());
        auto position = GetPosition();
        if (position != VECTOR3_ZERO)
            node.append_attribute("position").set_value(ToString(position).c_str());
        auto orientation = GetOrientation();
        if (orientation != QUATERNION_IDENTITY)
            node.append_attribute("orientation").set_value(ToString(orientation).c_str());
        auto scale = GetScale();
        if (scale != VECTOR3_ONE)
            node.append_attribute("scale").set_value(ToString(scale).c_str());
    }
}
