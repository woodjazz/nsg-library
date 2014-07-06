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
#include "App.h"

namespace NSG
{
	Camera* activeCamera = nullptr;

	Camera::Camera() 
	: fovy_(45), 
	zNear_(0.1f), 
	zFar_(250),
	xo_(0),
	yo_(0),
	xf_(1),
	yf_(1),
	isOrtho_(false),
	dirty_(true)
	{
	}

	Camera::~Camera() 
	{
	}

	void Camera::Update() const
	{
		if (dirty_)
		{
			UpdateProjection();
			dirty_ = false;
		}
	}

	void Camera::EnableOrtho() 
	{
		if(!isOrtho_)
		{
			isOrtho_ = true;
			dirty_ = true;
		}
	}

	void Camera::DisableOrtho() 
	{
		if(isOrtho_)
		{
			isOrtho_ = false;
			dirty_ = true;
		}
	}	

	void Camera::SetFov(float fovy)
	{
        if(fovy_ != fovy)
        {
		    fovy_ = fovy;
			dirty_ = true;
        }
	}

	void Camera::SetNearClip(float zNear)
	{
        if(zNear_ != zNear)
        {
		    zNear_ = zNear;
			dirty_ = true;
        }
	}

	void Camera::SetFarClip(float zFar)
	{
        if(zFar_ != zFar)
        {
		    zFar_ = zFar;
			dirty_ = true;
        }
	}

	Camera* Camera::Deactivate()
	{
		Camera* pCurrent = activeCamera;
		activeCamera = nullptr;
		return pCurrent;
	}

	Camera* Camera::Activate(Camera* pCamera)
	{
		Camera* pCurrent = Camera::Deactivate();

		if(pCamera)
			pCamera->Activate();

		return pCurrent;

	}

	Camera* Camera::GetActiveCamera()
	{
		return activeCamera;
	}

	void Camera::SetViewportFactor(float xo, float yo, float xf, float yf)
	{
		xo_ = xo;
		yo_ = yo;
		xf_ = xf;
		yf_ = yf;

		dirty_ = true;
	}

	Recti Camera::GetViewport() const
	{
	    auto viewSize = App::this_->GetViewSize();
	    auto width = viewSize.first;
	    auto height = viewSize.second;

		return Recti((GLsizei)(width * xo_), (GLsizei)(height * yo_), (GLsizei)(width * xf_), (GLsizei)(height * yf_));
	}

	void Camera::Activate()
	{
		activeCamera = this;

        Recti viewport = GetViewport();

		glViewport(viewport.x, viewport.y, viewport.z, viewport.w);		
	}

	void Camera::UpdateProjection() const
	{
		auto viewSize = App::this_->GetViewSize();
		auto width = viewSize.first;
		auto height = viewSize.second;

		if(width > 0 && height > 0)
        {
            if(isOrtho_)
            {
                matProjection_ = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f,  zNear_, zFar_);
            }
            else
            {
                float aspect_ratio = static_cast<float>(width) / height;
            
                assert(zNear_ > 0);

		        matProjection_ = glm::perspective(fovy_, aspect_ratio, zNear_, zFar_);
            }
        
            UpdateViewProjection();
        }
	}

    void Camera::OnUpdate() const
    {
		dirty_ = true;
        Update();
    }

	void Camera::UpdateViewProjection() const
	{
		matViewInverse_ = GetGlobalModelMatrix();
		matView_ = glm::inverse(matViewInverse_);
		matViewProjection_ = matProjection_ * matView_;
		matViewProjectionInverse_ = glm::inverse(matViewProjection_);
	}

	const Matrix4& Camera::GetMatViewProjection() const
	{
		if (dirty_)
			Update();

		return matViewProjection_;

	}

	Matrix4 Camera::GetModelViewProjection(const Node* pNode)
	{
		if (activeCamera)
		{
			return activeCamera->GetMatViewProjection() * pNode->GetGlobalModelMatrix();
		}
		else
		{
			// if no Camera then position is in screen coordinates
			return pNode->GetGlobalModelMatrix();
		}
	}

    const Matrix4& Camera::GetView() const
    {
		if (dirty_)
			Update();

        return matView_;
    }

	const Matrix4& Camera::GetInverseViewMatrix() const
	{
		if (dirty_)
			Update();

		return matViewInverse_;
	}

	const Matrix4& Camera::GetViewProjectionMatrix() const
	{
		if (dirty_)
			Update();

		return matViewProjection_;
	}

	Vertex3 Camera::ScreenToWorld(const Vertex3& screenXYZ) const 
	{
		Vertex4 worldCoord = matViewProjectionInverse_ * Vertex4(screenXYZ, 1);
		return Vertex3(worldCoord.x/worldCoord.w, worldCoord.y/worldCoord.w, worldCoord.z/worldCoord.w);
	}	

    Vertex3 Camera::WorldToScreen(const Vertex3& worldXYZ) const
    {
		Vertex4 screenCoord = matViewProjection_ * Vertex4(worldXYZ, 1);
		return Vertex3(screenCoord.x/screenCoord.w, screenCoord.y/screenCoord.w, screenCoord.z/screenCoord.w);
    }
}