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
#include "App.h"
#include <algorithm>

namespace NSG
{
	static GLES2Camera::Cameras s_Cameras;

	GLES2Camera* s_pActiveGLES2Camera;

	GLES2Camera::GLES2Camera() 
	: fovy_(45), 
	zNear_(0.1f), 
	zFar_(100),
	width_(0), 
	height_(0),
	xo_(0),
	yo_(0),
	xf_(1),
	yf_(1),
	isOrtho_(false)
	{
		s_Cameras.push_back(this);

        App* pApp = App::GetPtrInstance();

        if(pApp)
        {
		    auto viewSize = pApp->GetViewSize();
		    ViewChanged(viewSize.first, viewSize.second);
        }
	}

	GLES2Camera::~GLES2Camera() 
	{
		auto it = std::find(s_Cameras.begin(), s_Cameras.end(), this);
		assert(it != s_Cameras.end());
		s_Cameras.erase(it);
	}

	GLES2Camera::Cameras& GLES2Camera::GetCameras()
	{
		return s_Cameras;
	}

	void GLES2Camera::EnableOrtho() 
	{
		if(!isOrtho_)
		{
			isOrtho_ = true;
			UpdateProjection();
		}
	}

	void GLES2Camera::DisableOrtho() 
	{
		if(isOrtho_)
		{
			isOrtho_ = false;
			UpdateProjection();
		}
	}	

	void GLES2Camera::SetFov(float fovy)
	{
        if(fovy_ != fovy)
        {
		    fovy_ = fovy;
            UpdateProjection();
        }
	}

	void GLES2Camera::SetNearClip(float zNear)
	{
        if(zNear_ != zNear)
        {
		    zNear_ = zNear;
            UpdateProjection();
        }
	}

	void GLES2Camera::SetFarClip(float zFar)
	{
        if(zFar_ != zFar)
        {
		    zFar_ = zFar;
            UpdateProjection();
        }
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
		width_ = width;
		height_ = height;

		UpdateProjection();
	}

	void GLES2Camera::UpdateProjection()
	{
		if(isOrtho_)
		{
			matProjection_ = glm::ortho(0.0f, (float)width_, 0.0f,(float)height_,  zNear_, zFar_);
		}
		else
		{
			if(height_ > 0)
            {
    			float aspect_ratio = static_cast<float>(width_) / height_;

			    matProjection_ = glm::perspective(fovy_, aspect_ratio, zNear_, zFar_);
            }
		}

		UpdateViewProjection();
	}

    void GLES2Camera::OnUpdate()
    {
        UpdateViewProjection();
    }

	void GLES2Camera::UpdateViewProjection()
	{
/*		matViewInverse_ = GetModelMatrix();
		matView_ = glm::inverse(matViewInverse_);
		matViewProjection_ = matProjection_ * matView_;*/

		matView_ = GetModelMatrix();
		matViewInverse_ = glm::inverse(matView_);
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