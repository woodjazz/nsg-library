/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
		void SetWindow(Window* window);
		void EnableOrtho();
		void DisableOrtho();
		void SetFOV(float fovy); // in degrees
		void SetHalfHorizontalFov(float hhfov); // in radians
		void SetNearClip(float zNear);
		void SetFarClip(float zFar);
		void SetAspectRatio(unsigned width, unsigned height);
		void SetAspectRatio(float aspect);
		void SetViewportFactor(const Vector4& viewportFactor);
		const Vector4& GetViewportFactor() const { return viewportFactor_; }
		void SetOrthoScale(float orthoScale);
        bool IsOrtho() const { return isOrtho_; }
        void SetSensorFit(CameraSensorFit sensorFit);
        //XYZ are in normalized device coordinates (-1, 1)
        Vertex3 ScreenToWorld(const Vertex3& screenXYZ) const; 
        //Returned XYZ values are in normalized device coordinates (-1, 1)
        //Returned Z value is interpreted as a distance from the camera.
        Vertex3 WorldToScreen(const Vertex3& worldXYZ) const;
		//XY are in normalized device coordinates (-1, 1)
        Ray GetScreenRay(float screenX, float screenY) const;
		static Ray GetRay(float screenX, float screenY);
		const Matrix4& GetView() const;
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
		void Save(pugi::xml_node& node) const override;
		void Load(const pugi::xml_node& node) override;
		const OrthoProjection& GetOrthoProjection() const;
		void UnRegisterWindow();
		static SignalCamera::PSignal SignalBeingDestroy();
	private:
		void CalculateOrthoProjection() const;
		float CalculateAspectRatio() const;
		void SetScale(const Vertex3& scale); // not implemented (does not make sense for cameras and will make normals wrong)
		void UpdateProjection() const;
		void UpdateViewProjection() const;
		void UpdateFrustum();
		void Update() const;
		mutable Matrix4 matView_;
		mutable Matrix4 matViewInverse_;
		mutable Matrix4 matProjection_;
		mutable Matrix4 matViewProjection_;
		mutable Matrix4 matViewProjectionInverse_;
		float fovy_; // in radians
		float zNear_;
		float zFar_;
		Vector4 viewportFactor_;
		bool isOrtho_;
		unsigned viewWidth_;
		unsigned viewHeight_;
		float aspectRatio_;
		mutable PFrustum frustum_;
		SignalSizeChanged::PSlot slotViewChanged_;
		Window* window_;
		SignalWindow::PSlot slotWindowCreated_;
		float orthoScale_;
		CameraSensorFit sensorFit_;
		mutable OrthoProjection orthoProjection_;
		mutable bool isDirty_;
	};
}
