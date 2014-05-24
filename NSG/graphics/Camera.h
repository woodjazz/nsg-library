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
#include "SceneNode.h"
#include <memory>
#include <vector>

namespace NSG
{
	class Camera;

	class Camera : public SceneNode
	{
	public:
		Camera();
		~Camera();
		void EnableOrtho();
		void DisableOrtho();
		void SetFov(float fovy);
		void SetNearClip(float zNear);
		void SetFarClip(float zFar);
		static Camera* Deactivate();
		static Camera* Activate(Camera* pCamera);
		static Camera* GetActiveCamera();
		static Matrix4 GetModelViewProjection(const Node* pNode);
        Matrix4 GetView();
		static const Matrix4& GetViewProjectionMatrix();
		static const Matrix4& GetInverseViewMatrix();
		void Activate();
		void SetViewportFactor(float xo, float yo, float xf, float yf);
		Recti GetViewport() const;
        bool IsOrtho() const { return isOrtho_; }
        Vertex3 ScreenToWorld(const Vertex3& screenXYZ) const;
        Vertex3 WorldToScreen(const Vertex3& worldXYZ) const;
	private:
		void Invalidate();
        void OnUpdate();
		void UpdateProjection();
		void UpdateViewProjection();
		Matrix4 matView_;
		Matrix4 matViewInverse_;
		Matrix4 matProjection_;
		Matrix4 matViewProjection_;
		Matrix4 matViewProjectionInverse_;
		float fovy_;
		float zNear_;
		float zFar_;
		float xo_;
		float yo_;
		float xf_;
		float yf_;
		bool isOrtho_;
	};
}
