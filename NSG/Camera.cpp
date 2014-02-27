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
#include "Camera.h"
#include "Log.h"
#include "GLES2Includes.h"

namespace NSG
{
	Camera* s_pActiveCamera;

	Camera::Camera(float fovy, float zNear, float zFar) 
	: fovy_(fovy), 
	zNear_(zNear), 
	zFar_(zFar),
	width_(0), 
	height_(0),
	xo_(0),
	yo_(0),
	xf_(1),
	yf_(1)
	{
	}

	Camera::~Camera() 
	{
	}

	void Camera::Deactivate()
	{
		s_pActiveCamera = nullptr;
	}

	Camera* Camera::GetActiveCamera()
	{
		return s_pActiveCamera;
	}

	void Camera::SetViewport(float xo, float yo, float xf, float yf)
	{
		xo_ = xo;
		yo_ = yo;
		xf_ = xf;
		yf_ = yf;
	}

	void Camera::Activate()
	{
		s_pActiveCamera = this;

		glViewport(width_ * xo_, height_ * yo_, width_ * xf_, height_ * yf_);		
	}

	void Camera::ViewChanged(int32_t width, int32_t height)
	{
		if(height > 0)
		{
			width_ = width;
			height_ = height;

			float aspect_ratio = static_cast<float>(width) / height;

			matProjection_ = glm::perspective(fovy_, aspect_ratio, zNear_, zFar_);

			Update();
		}
	}

	void Camera::SetLookAt(const Vertex3& eye, const Vertex3& center, const Vertex3& up)
	{
		matView_ = glm::lookAt(eye, center, up);
		Update();
	}

	void Camera::Update()
	{
		matViewProjection_ = matProjection_ * matView_;
	}

	Matrix4 Camera::GetModelViewProjection(PNode pNode)
	{
		if (s_pActiveCamera)
		{
			return s_pActiveCamera->matViewProjection_ * pNode->GetModelMatrix();
		}
		else
		{
			// if no camera then position is in screen coordinates
			return pNode->GetModelMatrix();
		}
	}

	const Matrix4& Camera::GetViewProjectionMatrix()
	{
		if (s_pActiveCamera)
		{
			return s_pActiveCamera->matViewProjection_;
		}
		else
		{
			static Matrix4 m(1.0f);
			return m;
		}

	}


}