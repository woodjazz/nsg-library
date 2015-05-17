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
#include "Camera.h"
#include "Frustum.h"
#include "Graphics.h"
#include "Util.h"
#include "Ray.h"
#include "Program.h"
#include "Scene.h"
#include "FrameBuffer.h"
#include "Window.h"
#include "SignalSlots.h"
#include "Log.h"
#include "Window.h"
#include "pugixml.hpp"
#include <sstream>

namespace NSG
{
    Camera::Camera(const std::string& name)
        : SceneNode(name),
          fovy_(glm::radians(45.0f)),
          zNear_(0.1f),
          zFar_(250),
          viewportFactor_(0, 0, 1, 1),
          isOrtho_(false),
          viewWidth_(0),
          viewHeight_(0),
          aspectRatio_(1),
          cameraIsDirty_(false),
          window_(nullptr),
          orthoScale_(2.f),
          sensorFit_(CameraSensorFit::HORIZONTAL)
    {
        SetInheritScale(false);
        UpdateProjection();
        frustum_ = std::make_shared<Frustum>(matViewProjection_);
        if (Graphics::this_)
        {
            auto window = Graphics::this_->GetWindow();
            if (window)
                SetWindow(window);
            Graphics::this_->SetCamera(this);
        }

        slotWindowCreated_ = Window::SigReady()->Connect([this](Window * window)
        {
            if (!window_)
                SetWindow(window);
        });
    }

    Camera::~Camera()
    {
        SignalBeingDestroy()->Run(this);
        if (Graphics::this_ && Graphics::this_->GetCamera() == this)
            Graphics::this_->SetCamera(nullptr);
    }

    void Camera::UnRegisterWindow()
    {
        slotWindowCreated_ = nullptr;
        SetWindow(nullptr);
    }

    void Camera::SetWindow(Window* window)
    {
        if (window_ != window)
        {
            window_ = window;
            if (window)
            {
                SetAspectRatio(window->GetWidth(), window->GetHeight());
                slotViewChanged_ = window->SigSizeChanged()->Connect([this](int width, int height)
                {
                    SetAspectRatio(width, height);
                });
            }
            else
            {
                slotViewChanged_ = nullptr;
                SetAspectRatio(1);
            }
        }
    }

    float Camera::CalculateAspectRatio() const
    {
        float aspect = 1;
        if (viewWidth_ > 0 && viewHeight_ > 0)
        {
            //if (sensorFit_ == CameraSensorFit::VERTICAL)
            //    aspect = static_cast<float>(viewHeight_) / viewWidth_;
            //else
            aspect = static_cast<float>(viewWidth_) / viewHeight_;
        }
        return aspect;
    }

    void Camera::SetAspectRatio(unsigned width, unsigned height)
    {
        if (viewWidth_ != width || viewHeight_ != height)
        {
            viewWidth_ = width;
            viewHeight_ = height;
            SetAspectRatio(CalculateAspectRatio());
        }
    }

    void Camera::SetAspectRatio(float aspect)
    {
        CHECK_ASSERT(aspect != 0, __FILE__, __LINE__);
        if (aspectRatio_ != aspect)
        {
            aspectRatio_ = aspect;
            UpdateProjection();
        }
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

    void Camera::SetFOV(float fovy)
    {
        fovy = glm::radians(fovy);
        if (fovy_ != fovy)
        {
            fovy_ = fovy;
            UpdateProjection();
        }
    }

    void Camera::SetHalfHorizontalFov(float hhfov)
    {
        CHECK_ASSERT(hhfov != 0, __FILE__, __LINE__);

        float fovy = hhfov / aspectRatio_;

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

    void Camera::SetOrthoScale(float orthoScale)
    {
        if (orthoScale != orthoScale_)
        {
            orthoScale_ = orthoScale;
            UpdateProjection();
        }
    }

    void Camera::SetSensorFit(CameraSensorFit sensorFit)
    {
        if (sensorFit != sensorFit_)
        {
            sensorFit_ = sensorFit;
            SetAspectRatio(CalculateAspectRatio());
            UpdateProjection();
        }
    }

    void Camera::SetViewportFactor(const Vector4& viewportFactor)
    {
        if (viewportFactor_ != viewportFactor)
        {
            viewportFactor_ = viewportFactor;
            if (Graphics::this_->GetCamera() == this)
                Graphics::this_->SetViewportFactor(viewportFactor);
        }
    }

    void Camera::CalculateOrthoProjection() const
    {
        auto width = orthoScale_;
        auto height = orthoScale_ ;
        if (sensorFit_ == CameraSensorFit::VERTICAL)
            width /= aspectRatio_;
        else
            height /= aspectRatio_;

        orthoProjection_.left_ = -width * 0.5f;
        orthoProjection_.right_ = width * 0.5f;
        orthoProjection_.bottom_ = -height * 0.5f;
        orthoProjection_.top_ = height * 0.5f;
        orthoProjection_.near_ = 0;
        orthoProjection_.far_ = zFar_;
    }

    void Camera::UpdateProjection() const
    {
        if (isOrtho_)
        {
            CalculateOrthoProjection();

            matProjection_ = glm::ortho(orthoProjection_.left_,
                                        orthoProjection_.right_,
                                        orthoProjection_.bottom_,
                                        orthoProjection_.top_,
                                        orthoProjection_.near_,
                                        orthoProjection_.far_);
        }
        else
        {
            CHECK_ASSERT(zNear_ > 0, __FILE__, __LINE__);

            #if 0
            float tan_fovy = tan(fovy_ * 0.5);
            float right  =  tan_fovy * aspectRatio_ * zNear_;
            float left   = -right;
            float top    =  tan_fovy * zNear_;
            float bottom =  -top;
            matProjection_ = glm::frustum(left, right, bottom, top, zNear_, zFar_);
            #else

            matProjection_ = glm::perspective(fovy_, aspectRatio_, zNear_, zFar_);
            #endif
        }

        UpdateViewProjection();
    }

    void Camera::UpdateViewProjection() const
    {
        cameraIsDirty_ = false;
        #if 0
        matViewInverse_ = GetGlobalModelMatrix();
        matView_ = glm::inverse(matViewInverse_);
        #else
        auto eye = GetGlobalPosition();
        auto center = eye + GetLookAtDirection();
        auto up = GetUpDirection();
        matView_ = glm::lookAt(eye, center, up);
        matViewInverse_ = glm::inverse(matView_);
        #endif
        matViewProjection_ = matProjection_ * matView_;
        matViewProjectionInverse_ = glm::inverse(matViewProjection_);

        auto tmp = std::make_shared<Frustum>(matViewProjection_);
        frustum_.swap(tmp);

        SetUniformsNeedUpdate();
    }

    const PFrustum Camera::GetFrustum() const
    {
        if (cameraIsDirty_)
            UpdateViewProjection();

        return frustum_;
    }

    const Frustum* Camera::GetFrustumPointer() const
    {
        if (cameraIsDirty_)
            UpdateViewProjection();

        return frustum_.get();

    }

    const Matrix4& Camera::GetMatViewProjection() const
    {
        if (cameraIsDirty_)
            UpdateViewProjection();

        return matViewProjection_;
    }


    const Matrix4& Camera::GetMatViewProj()
    {
        if (Graphics::this_->GetCamera())
        {
            return Graphics::this_->GetCamera()->GetMatViewProjection();
        }
        else
        {
            return IDENTITY_MATRIX;
        }
    }

    Matrix4 Camera::GetModelViewProjection(const Node* pNode)
    {
        if (Graphics::this_->GetCamera())
        {
            return Graphics::this_->GetCamera()->GetMatViewProjection() * pNode->GetGlobalModelMatrix();
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
            UpdateViewProjection();

        return matProjection_;
    }

    Matrix4 Camera::GetInverseView()
    {
        if (Graphics::this_->GetCamera())
        {
            return Graphics::this_->GetCamera()->GetInverseViewMatrix();
        }
        else
        {
            return IDENTITY_MATRIX;
        }
    }

    const Matrix4& Camera::GetViewMatrix()
    {
        if (Graphics::this_->GetCamera())
        {
            return Graphics::this_->GetCamera()->GetView();
        }
        else
        {
            return IDENTITY_MATRIX;
        }

    }

    const Matrix4& Camera::GetProjectionMatrix()
    {
        if (Graphics::this_->GetCamera())
        {
            return Graphics::this_->GetCamera()->GetMatProjection();
        }
        else
        {
            return IDENTITY_MATRIX;
        }
    }

    const Matrix4& Camera::GetView() const
    {
        if (cameraIsDirty_)
            UpdateViewProjection();

        return matView_;
    }

    const Matrix4& Camera::GetInverseViewMatrix() const
    {
        if (cameraIsDirty_)
            UpdateViewProjection();

        return matViewInverse_;
    }

    const Matrix4& Camera::GetViewProjectionMatrix() const
    {
        if (cameraIsDirty_)
            UpdateViewProjection();

        return matViewProjection_;
    }

    const Matrix4& Camera::GetViewProjectionInverseMatrix() const
    {
        if (cameraIsDirty_)
            UpdateViewProjection();

        return matViewProjectionInverse_;
    }

    Vertex3 Camera::ScreenToWorld(const Vertex3& screenXYZ) const
    {
        Vertex4 worldCoord = GetViewProjectionInverseMatrix() * Vertex4(screenXYZ, 1);
        return Vertex3(worldCoord.x / worldCoord.w, worldCoord.y / worldCoord.w, worldCoord.z / worldCoord.w);
    }

    Vertex3 Camera::WorldToScreen(const Vertex3& worldXYZ) const
    {
        Vertex4 screenCoord = GetViewProjectionMatrix() * Vertex4(worldXYZ, 1);
        return Vertex3(screenCoord.x / screenCoord.w, screenCoord.y / screenCoord.w, screenCoord.z / screenCoord.w);
    }

    Ray Camera::GetScreenRay(float screenX, float screenY) const
    {
        Vertex3 nearPoint(screenX, screenY, -1); //in normalized device coordinates (Z goes from near = -1 to far = 1)
        Vertex3 farPoint(screenX, screenY, 0); //in normalized device coordinates

        Vertex3 nearWorldCoord = ScreenToWorld(nearPoint);
        Vertex3 farWorldCoord = ScreenToWorld(farPoint);

        Vector3 direction(farWorldCoord - nearWorldCoord);

        return Ray(nearWorldCoord, direction);
    }

    Ray Camera::GetRay(float screenX, float screenY)
    {
        if (Graphics::this_->GetCamera())
            return Graphics::this_->GetCamera()->GetScreenRay(screenX, screenY);
        else
            return Ray(Vector3(screenX, screenY, 0), VECTOR3_FORWARD);
    }

    bool Camera::IsVisible(const Node& node, Mesh& mesh) const
    {
        return GetFrustum()->IsVisible(node, mesh);
    }

    void Camera::OnDirty() const
    {
        cameraIsDirty_ = true;
    }

    void Camera::Save(pugi::xml_node& node) const
    {
        if (!IsSerializable())
            return;

        node.append_attribute("name").set_value(GetName().c_str());
        node.append_attribute("nodeType").set_value("Camera");
        node.append_attribute("fovy").set_value(fovy_);
        node.append_attribute("zNear").set_value(zNear_);
        node.append_attribute("zFar").set_value(zFar_);
        node.append_attribute("viewportFactor").set_value(ToString(viewportFactor_).c_str());
        node.append_attribute("isOrtho").set_value(isOrtho_);
        node.append_attribute("orthoScale").set_value(orthoScale_);
        node.append_attribute("sensorFit").set_value((int)sensorFit_);
        node.append_attribute("position").set_value(ToString(GetPosition()).c_str());
        node.append_attribute("orientation").set_value(ToString(GetOrientation()).c_str());
        SaveChildren(node);
    }

    void Camera::Load(const pugi::xml_node& node)
    {
        name_ = node.attribute("name").as_string();

        Vertex3 position = GetVertex3(node.attribute("position").as_string());
        SetPosition(position);

        fovy_ = node.attribute("fovy").as_float();
        zNear_ = node.attribute("zNear").as_float();
        zFar_ = node.attribute("zFar").as_float();
        viewportFactor_ = GetVertex4(node.attribute("viewportFactor").as_string());
        isOrtho_ = node.attribute("isOrtho").as_bool();
        orthoScale_ = node.attribute("orthoScale").as_float();
        sensorFit_ = (CameraSensorFit)node.attribute("sensorFit").as_int();

        Quaternion orientation = GetQuaternion(node.attribute("orientation").as_string());
        SetOrientation(orientation);
        LoadChildren(node);

        UpdateProjection();
    }

    SignalCamera::PSignal Camera::SignalBeingDestroy()
    {
        static SignalCamera::PSignal sig(new SignalCamera);
        return sig;
    }

}