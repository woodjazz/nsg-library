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
#include "AppListeners.h"

namespace NSG
{
	class Camera : public SceneNode, IViewChangedListener
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
		static Matrix4 GetInverseView();
		void Activate();
		void SetViewportFactor(float xo, float yo, float xf, float yf);
		Recti GetViewport() const;
        bool IsOrtho() const { return isOrtho_; }
        Vertex3 ScreenToWorld(const Vertex3& screenXYZ) const;
        Vertex3 WorldToScreen(const Vertex3& worldXYZ) const;
		const Matrix4& GetView() const;
		const Matrix4& GetMatViewProjection() const;
		const Matrix4& GetInverseViewMatrix() const;
		const Matrix4& GetViewProjectionMatrix() const;
		virtual void OnViewChanged(int32_t width, int32_t height) override;
		const PFrustum GetFrustum() const;
		bool IsVisible(const Node& node, Mesh& mesh) const;
	private:
		void UpdateProjection() const;
		void UpdateViewProjection() const;
		mutable Matrix4 matView_;
		mutable Matrix4 matViewInverse_;
		mutable Matrix4 matProjection_;
		mutable Matrix4 matViewProjection_;
		mutable Matrix4 matViewProjectionInverse_;
		float fovy_;
		float zNear_;
		float zFar_;
		float xo_;
		float yo_;
		float xf_;
		float yf_;
		bool isOrtho_;
		int32_t viewWidth_;
		int32_t viewHeight_;
		float aspectRatio_;
		mutable PFrustum frustum_;
	};
}
