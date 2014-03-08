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
#include <memory>
#include "GLES2Includes.h"
#include "Types.h"

namespace NSG
{
	class Node;

	typedef std::shared_ptr<Node> PNode;

	class Node
	{
	public:
		Node(PNode pParent = nullptr);
		~Node();
		GLushort GetId()const { return id_; }
		void EnableSelection(bool enable) { enableSelection_ = enable; }
		bool IsSelectionEnabled() const { return enableSelection_; }
        void SetParent(PNode pParent);
		virtual void OnUpdate() {}
		void SetPosition(const Vertex3& position);
		const Vertex3& GetPosition() const { return position_; }
		void SetOrientation(const Quaternion& q);
		void SetScale(const Vertex3& scale);
		const Vertex3& GetScale() const { return scale_; }
		void SetGlobalPosition(const Vertex3& position);
		void SetGlobalOrientation(const Quaternion& q);
		const Vertex3& GetGlobalPosition() const { return globalPosition_; }
		const Quaternion& GetGlobalOrientation() const { return globalOrientation_; }
		const Matrix4& GetModelMatrix() const;
		const Matrix3& GetModelInvTranspMatrix() const { return matModelInvTransp_; }
		void SetLookAt(const Vertex3& center, const Vertex3& up = Vertex3(0,1,0));
		const Vertex3& GetDirection() const { return direction_; }
	private:
		void Update();
		GLushort id_;	
		mutable Matrix4 matTemporal_;	
		Matrix4 matModel_;
		Matrix3 matModelInvTransp_;
		Vertex3 position_;
		Vertex3 globalPosition_;
		Quaternion q_;
		Quaternion globalOrientation_;
		Vertex3 scale_;
		Vertex3 direction_;
		PNode pParent_;
		bool enableSelection_;
	};
}
