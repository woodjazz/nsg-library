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
#include "UniformsUpdate.h"
#include "NonCopyable.h"
#include "BoundingBox.h"
#include <vector>
#include <string>
#include <map>

namespace NSG
{
    // Right-handed coordinate system ( like OpenGL)
    // 'look' along -z axis
    class Node : NonCopyable, public UniformsUpdate
    {
    public:
        Node(const std::string& name = "");
        virtual ~Node();
        IdType GetId() const { return id_;  }
        virtual void OnDirty() const {};
        void Translate(const Vector3& delta, TransformSpace space = TS_LOCAL);
        void SetPosition(const Vertex3& position);
        const Vertex3& GetPosition() const { return position_; }
        void SetOrientation(const Quaternion& q);
        const Quaternion& GetOrientation() const { return q_; };
        void SetScale(const Vertex3& scale);
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
        void SetLookAt(const Vertex3& lookAtPosition, const Vertex3& up = VECTOR3_UP);
        Quaternion GetLookAtOrientation(const Vertex3& lookAtPosition, const Vertex3& up);
        void SetGlobalPositionAndLookAt(const Vertex3& newPosition, const Vertex3& lookAtPosition, const Vertex3& up = VECTOR3_UP);
        const Vertex3& GetLookAtDirection() const;
        void SetInheritScale(bool inherit);
        bool IsPointInsideBB(const Vertex3& point) const;
        PNode GetChild(size_t idx) const { return children_.at(idx); }
        static void GetChildrenRecursive(PNode node, const std::string& name, std::vector<PNode>& result);
		static void GetChildrenRecursive(PNode node, std::vector<PNode>& result);
        static PNode GetUniqueNodeFrom(PNode node, const std::string& name);
        template <typename T> static std::vector<T*> GetChildrenRecursiveOfType(PNode node)
        {
            std::vector<T*> results;
            std::vector<PNode> nodes;
            GetChildrenRecursive(node, nodes);
            for (auto& obj : nodes)
            {
                T* ptr = dynamic_cast<T*>(obj.get());
                if (ptr) results.push_back(ptr);
            }
            return results;
        }
		template <typename T> static std::vector<T*> GetChildrenRecursiveOfType(PNode node, const std::string& name)
		{
			std::vector<T*> results;
			std::vector<PNode> nodes;
			GetChildrenRecursive(node, name, nodes);
			for (auto& obj : nodes)
			{
				T* ptr = dynamic_cast<T*>(obj.get());
				if (ptr) results.push_back(ptr);
			}
			return results;
		}
        template <typename T> static T* GetFirstChildOfType(PNode node, const std::string& name)
        {
            std::vector<PNode> result;
            GetChildrenRecursive(node, name, result);
            for (auto& obj : result)
            {
                T* ptr = dynamic_cast<T*>(obj.get());
                if (ptr) return ptr;
            }
            return nullptr;
        }
        const std::vector<PNode>& GetChildren() const { return children_; }
        bool IsDirty() const { return dirty_; }
        virtual void Save(pugi::xml_node& node);
        void SetName(const std::string& name) { name_ = name; }
        const std::string& GetName() const { return name_; }
        void SetParent(PNode parent);
        void SetParent(Node* parent);
        Node* GetParent() const { return parent_; }
        void AddChild(PNode node);
        void ClearAllChildren();
        virtual void Start() {}
        virtual void Update() {}
        virtual void ViewChanged(int width, int height) {}
        virtual void OnMouseMove(float x, float y) {}
        virtual void OnMouseDown(int button, float x, float y) {}
        virtual void OnMouseWheel(float x, float y) {}
        virtual void OnMouseUp(int button, float x, float y) {}
        virtual void OnKey(int key, int action, int modifier) {}
        virtual void OnChar(unsigned int character) {}
        bool IsEnabled() const { return enabled_; }
        void SetEnabled(bool enable, bool recursive = true);
        virtual void OnEnable() {}
        virtual void OnDisable() {}
        void SetBoneOffsetMatrix(const Matrix4& m);
        const Matrix4& GetBoneOffsetMatrix() { return boneOffsetMatrix_; }
        void Update(bool updateChildren = false) const;
    protected:
        Node* parent_;
        std::vector<PNode> children_;
        std::string name_;
    private:
        void RemoveChild(Node* node);
        void RemoveFromParent();
        void MarkAsDirty(bool recursive = true);
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
    };
}
