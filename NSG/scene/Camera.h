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

namespace NSG
{
	class Camera : public SceneNode
	{
	public:
		Camera(const std::string& name);
		~Camera();
		void EnableOrtho();
		void DisableOrtho();
		void SetFOV(float fovy); // in degrees
		void SetHalfHorizontalFov(float hhfov); // in radians
		void SetNearClip(float zNear);
		void SetFarClip(float zFar);
		void SetAspectRatio(int width, int height);
		void SetAspectRatio(float aspect);
		static Matrix4 GetModelViewProjection(const Node* pNode);
		static Matrix4 GetInverseView();
		void SetViewportFactor(float xo, float yo, float xf, float yf);
		Recti GetViewport() const;
		void SetOrtho(float left, float right, float bottom, float top);
        bool IsOrtho() const { return isOrtho_; }
        //XYZ are in normalized device coordinates (-1, 1)
        Vertex3 ScreenToWorld(const Vertex3& screenXYZ) const; 
        //Returned XYZ values are in normalized device coordinates (-1, 1)
        //Returned Z value is interpreted as a distance from the camera.
        Vertex3 WorldToScreen(const Vertex3& worldXYZ) const;
		//XY are in normalized device coordinates (-1, 1)
        Ray GetScreenRay(float screenX, float screenY) const;
		static const Matrix4& GetViewMatrix();
		const Matrix4& GetView() const;
		static const Matrix4& GetMatViewProj();
		const Matrix4& GetMatViewProjection() const;
		const Matrix4& GetInverseViewMatrix() const;
		const Matrix4& GetViewProjectionMatrix() const;
		const Matrix4& GetViewProjectionInverseMatrix() const;
		const Matrix4& GetMatProjection() const;
		const PFrustum GetFrustum() const;
		const Frustum* GetFrustumPointer() const;
		bool IsVisible(const Node& node, Mesh& mesh) const;
		void OnDirty() const override;
		float GetZNear() const { return zNear_; }
		float GetZFar() const { return zFar_; }
		float GetFov() const { return fovy_; }
		void Save(pugi::xml_node& node) override;
		void Load(const pugi::xml_node& node, const CachedData& data) override;
		void AddBlurFilter(int output_width, int output_height);
		void AddBlendFilter(int output_width, int output_height);
		PFilter AddUserFilter(PResource fragmentShader, int output_width, int output_height);
		void Render();
		void SetRender2Texture(PRender2Texture render2Texture);
	private:
		void Render(PRender2Texture render2Texture);
		void AddFilter(PFilter filter);
		void SetScale(const Vertex3& scale); // not implemented (does not make sense for cameras and will make normals wrong)
		void UpdateProjection() const;
		void UpdateViewProjection() const;
		void UpdateFrustum();

		mutable Matrix4 matView_;
		mutable Matrix4 matViewInverse_;
		mutable Matrix4 matProjection_;
		mutable Matrix4 matViewProjection_;
		mutable Matrix4 matViewProjectionInverse_;
		float fovy_; // in radians
		float zNear_;
		float zFar_;
		float xo_;
		float yo_;
		float xf_;
		float yf_;
		bool isOrtho_;
		Vector4 orthoCoords_;
        Graphics* graphics_;
		int viewWidth_;
		int viewHeight_;
		float aspectRatio_;
		mutable PFrustum frustum_;
		mutable bool cameraIsDirty_;
		std::vector<PFilter> filters_;
		PRender2Texture render2Texture_;
		PShowTexture showTexture_;
		SignalViewChanged::PSlot slotViewChanged_;
	};
}
