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
#include "Frustum.h"
#include "Graphics.h"
#include "Util.h"
#include "Ray.h"
#include "pugixml.hpp"

namespace NSG
{
    Camera* activeCamera = nullptr;

	Camera::Camera(const std::string& name, Scene* scene)
        : SceneNode(name, scene),
          fovy_(glm::radians(45.0f)),
          zNear_(0.1f),
          zFar_(250),
          xo_(0),
          yo_(0),
          xf_(1),
          yf_(1),
          isOrtho_(false),
          orthoCoords_(-1.0f, 1.0f, -1.0f, 1.0f),
          cameraIsDirty_(false)
    {
        viewWidth_ = app_.GetViewSize().first;
        viewHeight_ = app_.GetViewSize().second;
        CHECK_ASSERT(viewHeight_ > 0, __FILE__, __LINE__);
        aspectRatio_ = static_cast<float>(viewWidth_) / viewHeight_;
        UpdateProjection();
        App::Add(this);
    }

    Camera::~Camera()
    {
        App::Remove(this);
    }

    void Camera::EnableOrtho()
    {
        if (!isOrtho_)
        {
            isOrtho_ = true;
            UpdateProjection();
        }
    }

    void Camera::DisableOrtho()
    {
        if (isOrtho_)
        {
            isOrtho_ = false;
            UpdateProjection();
        }
    }

    void Camera::SetFov(float fovy)
    {
        fovy = glm::radians(fovy);
        if (fovy_ != fovy)
        {
            fovy_ = fovy;
            UpdateProjection();
        }
    }

    float Camera::GetVerticalFov(float hhfov) const
    {
        return 2*atan(tan(hhfov) * viewHeight_/viewWidth_);
    }

    void Camera::SetHalfHorizontalFov(float hhfov)
    {
        float fovy = GetVerticalFov(hhfov);

        if (fovy_ != fovy)
        {
            fovy_ = fovy;
            UpdateProjection();
        }
    }

    void Camera::SetNearClip(float zNear)
    {
        if (zNear_ != zNear)
        {
            zNear_ = zNear;
            UpdateProjection();
        }
    }

    void Camera::SetFarClip(float zFar)
    {
        if (zFar_ != zFar)
        {
            zFar_ = zFar;
            UpdateProjection();
        }
    }

    void Camera::SetOrtho(float left, float right, float bottom, float top)
    {
        if(orthoCoords_.x != left || orthoCoords_.y != right || orthoCoords_.z != bottom || orthoCoords_.w != top)
        {
            orthoCoords_ = Vector4(left, right, bottom, top);
            UpdateProjection();
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

        if (pCamera)
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
    }

    Recti Camera::GetViewport() const
    {
        return Recti((GLsizei)(viewWidth_ * xo_), (GLsizei)(viewHeight_ * yo_), (GLsizei)(viewWidth_ * xf_), (GLsizei)(viewHeight_ * yf_));
    }

    void Camera::Activate()
    {
        activeCamera = this;

        Graphics::this_->SetViewport(GetViewport());
    }

    void Camera::UpdateProjection() const
    {
        if (isOrtho_)
        {
            matProjection_ = glm::ortho(orthoCoords_.x, orthoCoords_.y, orthoCoords_.z, orthoCoords_.w,  zNear_, zFar_);
        }
        else
        {
            CHECK_ASSERT(zNear_ > 0, __FILE__, __LINE__);

            matProjection_ = glm::perspective(fovy_, aspectRatio_, zNear_, zFar_);
        }

        UpdateViewProjection();

        SetUniformsNeedUpdate();
    }

    void Camera::UpdateViewProjection() const
    {
        cameraIsDirty_ = false;

        matViewInverse_ = GetGlobalModelMatrix();
        matView_ = glm::inverse(matViewInverse_);

        matViewProjection_ = matProjection_ * matView_;
        matViewProjectionInverse_ = glm::inverse(matViewProjection_);

        frustum_ = PFrustum(new Frustum(this));
    }

    const PFrustum Camera::GetFrustum() const
    {
        if (cameraIsDirty_)
            UpdateProjection();

        return frustum_;
    }

    const Frustum* Camera::GetFrustumPointer() const
    {
        if (cameraIsDirty_)
            UpdateProjection();

        return frustum_.get();

    }

	const Matrix4& Camera::GetMatViewProjection() const
	{
		if (cameraIsDirty_)
			UpdateProjection();

		return matViewProjection_;
	}


    const Matrix4& Camera::GetMatViewProj()
    {
		if (activeCamera)
		{
			return activeCamera->GetMatViewProjection();
		}
		else
		{
			return IDENTITY_MATRIX;
		}
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

    const Matrix4& Camera::GetMatProjection() const
    {
        if (cameraIsDirty_)
            UpdateProjection();

        return matProjection_;
    }

    Matrix4 Camera::GetInverseView()
    {
        if (activeCamera)
        {
            return activeCamera->GetInverseViewMatrix();
        }
        else
        {
            return IDENTITY_MATRIX;
        }
    }

	const Matrix4& Camera::GetViewMatrix()
	{
		if (activeCamera)
		{
			return activeCamera->GetView();
		}
		else
		{
			return IDENTITY_MATRIX;
		}

	}

    const Matrix4& Camera::GetView() const
    {
        if (cameraIsDirty_)
            UpdateProjection();

        return matView_;
    }

    const Matrix4& Camera::GetInverseViewMatrix() const
    {
        if (cameraIsDirty_)
            UpdateProjection();

        return matViewInverse_;
    }

    const Matrix4& Camera::GetViewProjectionMatrix() const
    {
        if (cameraIsDirty_)
            UpdateProjection();

        return matViewProjection_;
    }

    const Matrix4& Camera::GetViewProjectionInverseMatrix() const
    {
        if (cameraIsDirty_)
            UpdateProjection();

        return matViewProjectionInverse_;
    }

    Vertex3 Camera::ScreenToWorld(const Vertex3& screenXYZ) const
    {
        Vertex4 worldCoord = GetViewProjectionInverseMatrix() * Vertex4(screenXYZ, 1);
		return Vertex3(worldCoord.x / worldCoord.w, worldCoord.y / worldCoord.w, worldCoord.z / worldCoord.w);
    }

    Vertex4 Camera::WorldToScreen(const Vertex3& worldXYZ) const
    {
        Vertex4 screenCoord = GetViewProjectionMatrix() * Vertex4(worldXYZ, 1);
		return Vertex4(screenCoord.x / screenCoord.w, screenCoord.y / screenCoord.w, screenCoord.z / screenCoord.w, screenCoord.z);
    }

    PRay Camera::GetScreenRay(float screenX, float screenY)
    {
        Vertex3 nearPoint(screenX, screenY, -1); //in normalized device coordinates (Z goes from near = -1 to far = 1)
        Vertex3 farPoint(screenX, screenY, 0); //in normalized device coordinates

		Vertex3 nearWorldCoord = ScreenToWorld(nearPoint);// Vertex3(GetViewProjectionInverseMatrix() * Vertex4(nearPoint, 1));
		Vertex3 farWorldCoord = ScreenToWorld(farPoint);// Vertex3(GetViewProjectionInverseMatrix() * Vertex4(farPoint, 1));

        Vector3 direction(farWorldCoord - nearWorldCoord);
        PRay ray(new Ray(Vertex3(nearWorldCoord), direction));
        return ray;
    }


    void Camera::OnViewChanged(int width, int height)
    {
        if (viewWidth_ != width || viewHeight_ != height)
        {
            viewWidth_ = width;
            viewHeight_ = height;
            if (viewHeight_ > 0)
                aspectRatio_ = static_cast<float>(viewWidth_) / viewHeight_;
            else
                aspectRatio_ = 1;

            UpdateProjection();
        }
    }

    bool Camera::IsVisible(const Node& node, Mesh& mesh) const
    {
        return GetFrustum()->IsVisible(node, mesh);
    }

    void Camera::OnDirty() const
    {
        cameraIsDirty_ = true;
    }

    void Camera::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Camera");

        {
            std::stringstream ss;
            ss << GetName();
            child.append_attribute("name") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << fovy_;
            child.append_attribute("fovy") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << zNear_;
            child.append_attribute("zNear") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << zFar_;
            child.append_attribute("zFar") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << xo_;
            child.append_attribute("xo") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << yo_;
            child.append_attribute("yo") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << xf_;
            child.append_attribute("xf") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << yf_;
            child.append_attribute("yf") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << isOrtho_;
            child.append_attribute("isOrtho") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << GetPosition();
            child.append_attribute("position") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << GetOrientation();
            child.append_attribute("orientation") = ss.str().c_str();
        }
    }

    void Camera::Load(const pugi::xml_node& node)
    {
        SetName(node.attribute("name").as_string());

        Vertex3 position = GetVertex3(node.attribute("position").as_string());
        SetPosition(position);

        fovy_ = node.attribute("fovy").as_float();
        zNear_ = node.attribute("zNear").as_float();
        zFar_ = node.attribute("zFar").as_float();
        xo_ = node.attribute("xo").as_float();
        yo_ = node.attribute("yo").as_float();
        xf_ = node.attribute("xf").as_float();
        yf_ = node.attribute("yf").as_float();
        isOrtho_ = node.attribute("isOrtho").as_bool();

        Quaternion orientation = GetQuaternion(node.attribute("orientation").as_string());
        SetOrientation(orientation);

		Activate();
    }
}