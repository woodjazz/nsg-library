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
#pragma once
#include "Types.h"
#include "Constants.h"
#include "Util.h"
#include "UniformsUpdate.h"
#include "NonCopyable.h"
#include "BoundingBox.h"
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
        IdType GetId() const { return id_;  }
        void Translate(const Vector3& delta, TransformSpace space = TS_LOCAL);
        void Rotate(const Quaternion& delta, TransformSpace space = TS_LOCAL);
        void SetPosition(const Vertex3& position);
        const Vertex3& GetPosition() const { return position_; }
        void SetOrientation(const Quaternion& q);
        const Quaternion& GetOrientation() const { return q_; };
        void SetScale(const Vertex3& scale);
        void SetScale(float scale);
        const Vertex3& GetScale() const { return scale_; }
        void SetGlobalPosition(const Vertex3& position);
        void SetGlobalOrientation(const Quaternion& q);
        void SetGlobalScale(const Vertex3& scale);
        const Vertex3& GetGlobalPosition() const;
        const Quaternion& GetGlobalOrientation() const;
        Vertex3 GetGlobalScale() const;
        const Matrix4& GetGlobalModelMatrix() const;
        const Matrix3& GetGlobalModelInvTranspMatrix() const;
        const Matrix4& GetGlobalModelInvMatrix() const;
        void SetGlobalLookAt(const Vertex3& lookAtPosition, const Vertex3& up = VECTOR3_UP);
        void SetLocalLookAt(const Vertex3& lookAtPosition, const Vertex3& up = VECTOR3_UP);
        Quaternion GetLookAtOrientation(const Vertex3& lookAtPosition, const Vertex3& up);
        void SetGlobalPositionAndLookAt(const Vertex3& newPosition, const Vertex3& lookAtPosition, const Vertex3& up = VECTOR3_UP);
        const Vertex3& GetLookAtDirection() const;
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
            return obj;
        }
		template <typename T> std::shared_ptr<T> CreateChild(const std::string& name = GetUniqueName())
		{
			auto obj = std::make_shared<T>(name);
			AddChild(obj);
			return obj;
		}
        template <typename T> std::shared_ptr<T> CreateChild(const std::string& name, const pugi::xml_node& node)
        {
            auto obj = std::make_shared<T>(name);
            obj->T::Load(node);
            AddChild(obj);
            return obj;
        }
        
        const std::vector<PNode>& GetChildren() const { return children_; }
        bool IsDirty() const { return dirty_; }
        const std::string& GetName() const { return name_; }
        void SetParent(PNode parent);
        PNode GetParent() const;
        void ClearAllChildren();
        bool IsEnabled() const { return enabled_; }
        void SetEnabled(bool enable, bool recursive = true);
        virtual void OnDirty() const {};
        virtual void OnEnable() {}
        virtual void OnDisable() {}
        virtual void OnScaleChange() {}
        virtual void OnSceneSet() {}
        // Offset matrix that converts from vertex space to bone space
        void SetBoneOffsetMatrix(const Matrix4& m);
        const Matrix4& GetBoneOffsetMatrix() { return boneOffsetMatrix_; }
        void Update(bool updateChildren = false) const;
        bool IsScaleUniform() const;
        PScene GetScene() const { return scene_.lock(); }
        void MarkAsDirty(bool recursive = true, bool scaleChange = false);
		Matrix4 GetTransform() const;
    protected:
        std::string name_;
        std::vector<PNode> children_;
        std::unordered_map<std::string, PWeakNode> childrenHash_;
        PWeakScene scene_;
    private:
		void RemoveFromParent();
		void AddChild(PNode node);
        void RemoveChild(Node* node);
        std::weak_ptr<Node> parent_;
        IdType id_;
        mutable Matrix4 globalModel_;
        mutable Matrix4 globalModelInv_;
        mutable Matrix3 globalModelInvTransp_;
        mutable Matrix4 boneOffsetMatrix_;
        Vertex3 position_;
        Quaternion q_;
        Vertex3 scale_;
        mutable Vertex3 globalPosition_;
        mutable Quaternion globalOrientation_;
        mutable Vertex3 globalScale_;
        mutable Vertex3 lookAtDirection_;
        bool inheritScale_;
        mutable bool dirty_;
        bool enabled_;
        mutable bool isScaleUniform_;
    };
}
