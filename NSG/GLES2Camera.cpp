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
#include "GLES2Camera.h"
#include "Log.h"
#include "GLES2Includes.h"

namespace NSG
{
	GLES2Camera* s_pActiveGLES2Camera;

	GLES2Camera::GLES2Camera(float fovy, float zNear, float zFar) 
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

	GLES2Camera::~GLES2Camera() 
	{
	}

	void GLES2Camera::Deactivate()
	{
		s_pActiveGLES2Camera = nullptr;
	}

	GLES2Camera* GLES2Camera::GetActiveCamera()
	{
		return s_pActiveGLES2Camera;
	}

	void GLES2Camera::SetViewport(float xo, float yo, float xf, float yf)
	{
		xo_ = xo;
		yo_ = yo;
		xf_ = xf;
		yf_ = yf;
	}

	void GLES2Camera::Activate()
	{
		s_pActiveGLES2Camera = this;

		glViewport(width_ * xo_, height_ * yo_, width_ * xf_, height_ * yf_);		
	}

	void GLES2Camera::ViewChanged(int32_t width, int32_t height)
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

	void GLES2Camera::SetLookAt(const Vertex3& eye, const Vertex3& center, const Vertex3& up)
	{
		matView_ = glm::lookAt(eye, center, up);
		matViewInverse_ = glm::inverse(matView_);
		Update();
	}

	void GLES2Camera::Update()
	{
		matViewProjection_ = matProjection_ * matView_;
	}

	Matrix4 GLES2Camera::GetModelViewProjection(Node* pNode)
	{
		if (s_pActiveGLES2Camera)
		{
			return s_pActiveGLES2Camera->matViewProjection_ * pNode->GetModelMatrix();
		}
		else
		{
			// if no GLES2Camera then position is in screen coordinates
			return pNode->GetModelMatrix();
		}
	}

	const Matrix4& GLES2Camera::GetViewProjectionMatrix()
	{
		if (s_pActiveGLES2Camera)
		{
			return s_pActiveGLES2Camera->matViewProjection_;
		}
		else
		{
			static Matrix4 m(1.0f);
			return m;
		}
	}

	const Matrix4& GLES2Camera::GetInverseViewMatrix()
	{
		if (s_pActiveGLES2Camera)
		{
			return s_pActiveGLES2Camera->matViewInverse_;
		}
		else
		{
			static Matrix4 m(1.0f);
			return m;
		}
	}
}