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
#include "GLES2Includes.h"
#include "Types.h"
#include <set>

namespace NSG
{
	class Node
	{
	public:
		Node();
		virtual ~Node();
		GLushort GetId()const { return id_; }
        void SetParent(PNode pParent);
        void SetParent(Node* pParent);
        PNode GetParent() const { return pParent_; }
		virtual void OnUpdate() const {}
		void SetPosition(const Vertex3& position);
		const Vertex3& GetPosition() const { return position_; }
		void SetOrientation(const Quaternion& q);
		const Quaternion& GetOrientation() const { return q_; };
		void SetScale(const Vertex3& scale);
		const Vertex3& GetScale() const { return scale_; }
		void SetGlobalPosition(const Vertex3& position);
		void SetGlobalOrientation(const Quaternion& q);
		const Vertex3& GetGlobalPosition() const;
		const Quaternion& GetGlobalOrientation() const;
		Vertex3 GetGlobalScale() const;
		const Matrix4& GetGlobalModelMatrix() const;
		const Matrix3& GetGlobalModelInvTranspMatrix() const;
		const Matrix4& GetGlobalModelInvMatrix() const;
		static Vertex3 UP;
		void SetLookAt(const Vertex3& center, const Vertex3& up = UP);
		const Vertex3& GetLookAtDirection() const;
		void SetInheritScale(bool inherit);
		bool IsPointInsideBB(const Vertex3& point) const;
    protected:
		PNode pParent_;
        std::set<Node*> children_;
	private:
		void MarkAsDirty();
		void Update(bool updateChildren = true) const;
		GLushort id_;	
        mutable Matrix4 globalModel_;	
        mutable Matrix4 globalModelInv_;
		mutable Matrix3 globalModelInvTransp_;
		Vertex3 position_;
		Quaternion q_;
		Vertex3 scale_;
		mutable Vertex3 globalPosition_;
		mutable Quaternion globalOrientation_;
		mutable Vertex3 globalScale_;
		mutable Vertex3 lookAtDirection_;
		bool inheritScale_;
		mutable bool dirty_;
	};
}
