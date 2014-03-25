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
#include "Node.h"
#include <memory>
#include <vector>

namespace NSG
{
	class GLES2Camera;

	typedef std::shared_ptr<GLES2Camera> PGLES2Camera;

	class GLES2Camera : public Node
	{
	public:
		GLES2Camera();
		~GLES2Camera();
		void EnableOrtho();
		void DisableOrtho();
		void SetFov(float fovy);
		void SetNearClip(float zNear);
		void SetFarClip(float zFar);
		static void Deactivate();
		static GLES2Camera* GetActiveCamera();
		static Matrix4 GetModelViewProjection(Node* pNode);
		static const Matrix4& GetViewProjectionMatrix();
		static const Matrix4& GetInverseViewMatrix();
		void Activate();
		void SetViewport(float xo, float yo, float xf, float yf);
		void ViewChanged(int32_t width, int32_t height);
		typedef std::vector<GLES2Camera*> Cameras;
		static Cameras& GetCameras();
        bool IsOrtho() const { return isOrtho_; }
	private:
        void OnUpdate();
		void UpdateProjection();
		void UpdateViewProjection();
		Matrix4 matView_;
		Matrix4 matViewInverse_;
		Matrix4 matProjection_;
		Matrix4 matViewProjection_;
		float fovy_;
		float zNear_;
		float zFar_;
		int32_t width_;
		int32_t height_;
		float xo_;
		float yo_;
		float xf_;
		float yf_;
		bool isOrtho_;
	};
}
