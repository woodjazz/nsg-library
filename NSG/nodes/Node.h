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
#pragma once
#include "Util.h"
#include "UniformsUpdate.h"
#include "NonCopyable.h"
#include "BoundingBox.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace NSG
{
    // Right-handed coordinate system ( like OpenGL)
    // 'look' along -z axis
    class Node : NonCopyable, public std::enable_shared_from_this<Node>, public UniformsUpdate
    {
    public:
        Node(const std::string& name = "");
        virtual ~Node();
        void Translate(const Vector3& delta, TransformSpace space = TS_LOCAL);
        void Rotate(const Quaternion& delta, TransformSpace space = TS_LOCAL);
        void SetPosition(const Vertex3& position);
        const Vertex3& GetPosition() const { return position_; }
        void SetOrientation(const Quaternion& q);
        const Quaternion& GetOrientation() const { return q_; };
        const Vector3& GetGUIRotation() const { return guiRotation_; };
		void SetGUIRotation(const Vector3& guiRotation) { guiRotation_ = guiRotation; };
        void SetScale(const Vertex3& scale);
        void SetScale(float scale);
        const Vertex3& GetScale() const { return scale_; }
        void SetGlobalPosition(const Vertex3& position);
        void SetGlobalOrientation(const Quaternion& q);
        void SetGlobalScale(const Vertex3& scale);
        const Vertex3& GetGlobalPosition() const;
        const Quaternion& GetGlobalOrientation() const;
        Vertex3 GetGlobalScale() const;
        // Rotate around the X axis.
        void Pitch(float angle, TransformSpace space = TS_LOCAL);
        // Rotate around the Y axis.
        void Yaw(float angle, TransformSpace space = TS_LOCAL);
        // Rotate around the Z axis.
        void Roll(float angle, TransformSpace space = TS_LOCAL);
        const Matrix4& GetGlobalModelMatrix() const;
        const Matrix3& GetGlobalModelInvTranspMatrix() const;
        const Matrix4& GetGlobalModelInvMatrix() const;
		void SetGlobalLookAtDirection(const Vertex3& direction);
        void SetGlobalLookAtPosition(const Vertex3& lookAtPosition, const Vertex3& up = Vertex3::Up);
        void SetLocalLookAtPosition(const Vertex3& lookAtPosition, const Vertex3& up = Vertex3::Up);
        Quaternion GetLookAtOrientation(const Vertex3& lookAtPosition, const Vertex3& up);
        void SetGlobalPositionAndLookAt(const Vertex3& newPosition, const Vertex3& lookAtPosition, const Vertex3& up = Vertex3::Up);
        const Vertex3& GetLookAtDirection() const;
        const Vertex3& GetUpDirection() const;
		const Vertex3& GetRightDirection() const;
        void SetInheritScale(bool inherit);
        bool IsPointInsideBB(const Vertex3& point) const;
        PNode GetChild(size_t idx) const { return children_.at(idx); }
        template <typename T> std::shared_ptr<T> GetChild(const std::string& name, bool recursive)
        {
            auto range = childrenHash_.equal_range(name);
            for (auto it = range.first; it != range.second; it++)
            {
                if (!it->second.expired())
                {
                    PNode child = it->second.lock();
                    auto p = std::dynamic_pointer_cast<T>(child);
                    if (p) return p;
                }
            }
            if (recursive)
            {
                for (auto& child : children_)
                {
                    auto p = child->GetChild<T>(name, recursive);
                    if (p) return p;
                }
            }
            return nullptr;
        }
        template <typename T> std::shared_ptr<T> GetOrCreateChild(const std::string& name)
        {
            auto p = GetChild<T>(name, false);
            if (p) return p;
            auto obj = std::make_shared<T>(name);
            AddChild(obj);
			obj->OnCreated();
            return obj;
        }
		template <typename T> std::shared_ptr<T> CreateChild(const std::string& name = GetUniqueName())
		{
			auto obj = std::make_shared<T>(name);
			AddChild(obj);
			obj->OnCreated();
			return obj;
		}
        template <typename T> std::shared_ptr<T> CreateChild(const std::string& name, const pugi::xml_node& node)
        {
            auto obj = std::make_shared<T>(name);
            obj->T::Load(node);
            AddChild(obj);
			obj->OnCreated();
            return obj;
        }
        
        const std::vector<PNode>& GetChildren() const { return children_; }
        bool IsDirty() const { return dirty_; }
        const std::string& GetName() const { return name_; }
        void SetParent(PNode parent);
        PNode GetParent() const;
        void ClearAllChildren();
        bool IsHidden() const { return hide_; }
        void Hide(bool hide, bool recursive = true);
        virtual void OnDirty() const {};
        virtual void OnHide(bool hide) {}
        virtual void OnScaleChange() {}
        virtual void OnCreated() {}
        void Update() const;
        bool IsScaleUniform() const;
        PScene GetScene() const { return scene_.lock(); }
        void MarkAsDirty(bool recursive = true, bool scaleChange = false);
		Matrix4 GetTransform() const;
        void SetTransform(const Matrix4& transform);
        virtual void Load(const pugi::xml_node& node);
        virtual void Save(pugi::xml_node& node) const;
        SignalEmpty::PSignal SigUpdated() { return signalUpdated_; }
        void SetUserData(void* p) { userData_ = p; }
        void* GetUserData() const { return userData_; }
    protected:
        std::string name_;
        std::vector<PNode> children_;
        std::unordered_map<std::string, PWeakNode> childrenHash_;
        PWeakScene scene_;
        SignalEmpty::PSignal signalUpdated_;
    private:
		void RemoveFromParent();
		void AddChild(PNode node);
        void RemoveChild(Node* node);
        std::weak_ptr<Node> parent_;
        mutable Matrix4 globalModel_;
        mutable Matrix4 globalModelInv_;
        mutable Matrix3 globalModelInvTransp_;
        Vertex3 position_;
        Quaternion q_;
        Vector3 scale_;
        mutable Vertex3 globalPosition_;
        mutable Quaternion globalOrientation_;
        mutable Vector3 globalScale_;
        mutable Vector3 lookAtDirection_;
        mutable Vector3 upDirection_;
		mutable Vector3 rightDirection_;
        bool inheritScale_;
        mutable bool dirty_;
        bool hide_;
        mutable bool isScaleUniform_;
        Vector3 guiRotation_;
        void* userData_;
    };
}
