/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "RenderingContext.h"
#include "Util.h"
#include "Ray.h"
#include "Program.h"
#include "Constants.h"
#include "Scene.h"
#include "FrameBuffer.h"
#include "Window.h"
#include "SignalSlots.h"
#include "Log.h"
#include "Window.h"
#include "Material.h"
#include "StringConverter.h"
#include "Check.h"
#include "DebugRenderer.h"
#include "imgui.h"
#include "SharedFromPointer.h"
#include "pugixml.hpp"
#include <sstream>

namespace NSG
{
    Camera::Camera(const std::string& name)
        : SceneNode(name),
          fovy_(Radians(45.0f)),
          zNear_(0.1f),
          zFar_(250),
          isOrtho_(false),
          viewWidth_(0),
          viewHeight_(0),
          aspectRatio_(1),
          orthoScale_(2.f),
          sensorFit_(CameraSensorFit::HORIZONTAL),
          isDirty_(true),
          autoAspectRatio_(true),
          shadowSplits_(MAX_SPLITS),
          colorSplits_(false),
          shadowSplitLogFactor_(0.5f),
          automaticSplits_(true),
          hasUserOrthoProjection_(false)
    {
        SetInheritScale(false);
        slotWindow_ = RenderingContext::SigWindow()->Connect([this](Window * window)
        {
            if (!window_.lock())
                SetWindow(SharedFromPointer(window));
        });
    }

    Camera::~Camera()
    {
        if (SignalsAlive())
            SignalBeingDestroy()->Run(this);
    }

    void Camera::UnRegisterWindow()
    {
        slotWindow_ = nullptr;
        SetWindow(nullptr);
    }

    void Camera::SetWindow(PWindow window)
    {
        if (window_.lock() != window)
        {
            window_ = window;
            if (window)
            {
                SetAspectRatio(window->GetWidth(), window->GetHeight());
                slotViewChanged_ = window->SigSizeChanged()->Connect([this](int width, int height)
                {
                    if (autoAspectRatio_)
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
        CHECK_ASSERT(aspect != 0);
        if (aspectRatio_ != aspect)
        {
            aspectRatio_ = aspect;
            isDirty_ = true;
            SetUniformsNeedUpdate();
        }
    }

    void Camera::EnableOrtho()
    {
        if (!isOrtho_)
        {
            isOrtho_ = true;
            isDirty_ = true;
            SetUniformsNeedUpdate();
        }
    }

    void Camera::DisableOrtho()
    {
        if (isOrtho_)
        {
            isOrtho_ = false;
            isDirty_ = true;
            SetUniformsNeedUpdate();
        }
    }

    void Camera::SetFOVDegrees(float fovy)
    {
        fovy = Clamp(fovy, 0.0f, CAMERA_MAX_FOV_DEGREES);
        fovy = Radians(fovy);
        if (fovy_ != fovy)
        {
            fovy_ = fovy;
            isDirty_ = true;
            SetUniformsNeedUpdate();
        }
    }

    float Camera::GetFOVDegrees() const
    {
        return Degrees(fovy_);
    }

    void Camera::SetFOVRadians(float fovy)
    {
        fovy = Clamp(fovy, 0.0f, CAMERA_MAX_FOV_RADIANS);
        if (fovy_ != fovy)
        {
            fovy_ = fovy;
            isDirty_ = true;
            SetUniformsNeedUpdate();
        }
    }

    float Camera::GetFOVRadians() const
    {
        return fovy_;
    }

    void Camera::SetHalfHorizontalFov(float hhfov)
    {
        CHECK_ASSERT(hhfov != 0);

        float fovy = hhfov / aspectRatio_;

        if (fovy_ != fovy)
        {
            fovy_ = fovy;
            isDirty_ = true;
            SetUniformsNeedUpdate();
        }
    }

    void Camera::SetNearClip(float zNear)
    {
        //zNear = std::max(zNear, CAMERA_MIN_NEARCLIP);
        if (zNear_ != zNear)
        {
            zNear_ = zNear;
            isDirty_ = true;
            SetUniformsNeedUpdate();
        }
    }

    void Camera::SetFarClip(float zFar)
    {
        //zFar = std::max(zFar, CAMERA_MIN_NEARCLIP);
        if (zFar_ != zFar)
        {
            zFar_ = zFar;
            isDirty_ = true;
            SetUniformsNeedUpdate();
        }
    }

    void Camera::SetOrthoScale(float orthoScale)
    {
        if (orthoScale != orthoScale_)
        {
            orthoScale_ = orthoScale;
            isDirty_ = true;
            SetUniformsNeedUpdate();
        }
    }

    void Camera::SetSensorFit(CameraSensorFit sensorFit)
    {
        if (sensorFit != sensorFit_)
        {
            sensorFit_ = sensorFit;
            SetAspectRatio(CalculateAspectRatio());
            isDirty_ = true;
        }
    }

    OrthoProjection Camera::CalculateOrthoProjection(float zNear, float zFar) const
    {
        auto width = orthoScale_;
        auto height = orthoScale_ ;
        if (sensorFit_ == CameraSensorFit::VERTICAL)
            width /= aspectRatio_;
        else
            height /= aspectRatio_;
        OrthoProjection orthoProjection;
        orthoProjection.left_ = -width * 0.5f;
        orthoProjection.right_ = width * 0.5f;
        orthoProjection.bottom_ = -height * 0.5f;
        orthoProjection.top_ = height * 0.5f;
        orthoProjection.near_ = zNear;
        orthoProjection.far_ = zFar;
        return orthoProjection;
    }

    void Camera::SetOrthoProjection(OrthoProjection projection)
    {
        if (memcmp(&orthoProjection_, &projection, sizeof(OrthoProjection)) != 0)
        {
            orthoProjection_ = projection;
            hasUserOrthoProjection_ = true;
            isDirty_ = true;
            SetUniformsNeedUpdate();
        }
    }

    void Camera::UpdateProjection() const
    {
        if (isOrtho_)
        {
            if (!hasUserOrthoProjection_)
                orthoProjection_ = CalculateOrthoProjection(zNear_, zFar_);
            matProjection_ = Ortho(orthoProjection_.left_,
                                   orthoProjection_.right_,
                                   orthoProjection_.bottom_,
                                   orthoProjection_.top_,
                                   orthoProjection_.near_,
                                   orthoProjection_.far_);
        }
        else
        {
            CHECK_ASSERT(zNear_ > 0);
            matProjection_ = Perspective(fovy_, aspectRatio_, zNear_, zFar_);
        }
        SetUniformsNeedUpdate();
    }

    void Camera::UpdateViewProjection() const
    {
        matViewInverse_ = GetGlobalModelMatrix();
        matView_ = Inverse(matViewInverse_);
        matViewProjection_ = matProjection_ * matView_;
        matViewProjectionInverse_ = Inverse(matViewProjection_);
        auto tmp = std::make_shared<Frustum>(matViewProjection_);
        frustum_.swap(tmp);
        SetUniformsNeedUpdate();
    }

    void Camera::Update() const
    {
        if (isDirty_)
        {
            isDirty_ = false;
            UpdateProjection();
            UpdateViewProjection();
            signalUpdated_->Run();
        }
    }

    const PFrustum Camera::GetFrustum() const
    {
        Update();
        return frustum_;
    }

    PFrustum Camera::GetFrustumSplit(float nearSplit, float farSplit) const
    {
        Update();

        Matrix4 matProjection;
        if (isOrtho_)
        {
            OrthoProjection orthoProjection = CalculateOrthoProjection(nearSplit, farSplit);
            matProjection = Ortho(orthoProjection.left_,
                                  orthoProjection.right_,
                                  orthoProjection.bottom_,
                                  orthoProjection.top_,
                                  orthoProjection.near_,
                                  orthoProjection.far_);
        }
        else
        {
            CHECK_ASSERT(nearSplit > 0);
            matProjection = Perspective(fovy_, aspectRatio_, nearSplit, farSplit);
        }

        return std::make_shared<Frustum>(matProjection * matView_);
    }

    const Frustum* Camera::GetFrustumPointer() const
    {
        Update();
        return frustum_.get();
    }

    const Matrix4& Camera::GetViewProjection() const
    {
        Update();
        return matViewProjection_;
    }

    const Matrix4& Camera::GetProjection() const
    {
        Update();
        return matProjection_;
    }

    const Matrix4& Camera::GetView() const
    {
        Update();
        return matView_;
    }

    const Matrix4& Camera::GetViewProjectionInverse() const
    {
        Update();
        return matViewProjectionInverse_;
    }

    Vertex3 Camera::ScreenToWorld(const Vertex3& screenXYZ) const
    {
        Update();
        Vertex4 worldCoord = GetViewProjectionInverse() * Vertex4(screenXYZ, 1);
        return Vertex3(worldCoord.x / worldCoord.w, worldCoord.y / worldCoord.w, worldCoord.z / worldCoord.w);
    }

    Vertex3 Camera::WorldToScreen(const Vertex3& worldXYZ) const
    {
        Update();
        Vertex4 screenCoord = GetViewProjection() * Vertex4(worldXYZ, 1);
        return Vertex3(screenCoord.x / screenCoord.w, screenCoord.y / screenCoord.w, screenCoord.z / screenCoord.w);
    }

    Ray Camera::GetScreenRay(float screenX, float screenY) const
    {
        Update();
        Vertex3 nearPoint(screenX, screenY, -1); //in normalized device coordinates (Z goes from near = -1 to far = 1)
        Vertex3 farPoint(screenX, screenY, 0); //in normalized device coordinates

        Vertex3 nearWorldCoord = ScreenToWorld(nearPoint);
        Vertex3 farWorldCoord = ScreenToWorld(farPoint);

        Vector3 direction(farWorldCoord - nearWorldCoord);

        return Ray(nearWorldCoord, direction);
    }

    Ray Camera::GetRay(const Camera* camera, float screenX, float screenY)
    {
        return camera->GetScreenRay(screenX, screenY);
    }

    Ray Camera::GetRay(float screenX, float screenY)
    {
        return Ray(Vector3(screenX, screenY, 0), VECTOR3_LOOKAT_DIRECTION);
    }

    bool Camera::IsVisible(const Node& node, Mesh& mesh) const
    {
        Update();
        return GetFrustum()->IsVisible(node, mesh);
    }

    bool Camera::IsVisible(const SceneNode& node) const
    {
        Update();
        return GetFrustum()->IsVisible(node);
    }

    bool Camera::IsVisible(const BoundingBox& bb) const
    {
        Update();
        return GetFrustum()->IsInside(bb) != Intersection::OUTSIDE;
    }

    void Camera::OnDirty() const
    {
        SceneNode::OnDirty();
        isDirty_ = true;
        SetUniformsNeedUpdate();
    }

    const OrthoProjection& Camera::GetOrthoProjection() const
    {
        Update();
        return orthoProjection_;
    }


    void Camera::Save(pugi::xml_node& node) const
    {
        if (!IsSerializable())
            return;

        Update();

        node.append_attribute("name").set_value(GetName().c_str());
        node.append_attribute("nodeType").set_value("Camera");
        node.append_attribute("fovy").set_value(fovy_);
        node.append_attribute("zNear").set_value(zNear_);
        node.append_attribute("zFar").set_value(zFar_);
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

        Vertex3 position = ToVertex3(node.attribute("position").as_string());
        SetPosition(position);

        fovy_ = node.attribute("fovy").as_float();
        zNear_ = node.attribute("zNear").as_float();
        zFar_ = node.attribute("zFar").as_float();
        isOrtho_ = node.attribute("isOrtho").as_bool();
        orthoScale_ = node.attribute("orthoScale").as_float();
        sensorFit_ = (CameraSensorFit)node.attribute("sensorFit").as_int();

        Quaternion orientation = ToQuaternion(node.attribute("orientation").as_string());
        SetOrientation(orientation);
        LoadChildren(node);

        isDirty_ = true;
        SetUniformsNeedUpdate();
    }

    SignalCamera::PSignal Camera::SignalBeingDestroy()
    {
        static SignalCamera::PSignal sig(new SignalCamera);
        return sig;
    }

    void Camera::SetMaxShadowSplits(int splits)
    {
        if (shadowSplits_ != splits)
        {
            shadowSplits_ = Clamp(splits, 1, MAX_SPLITS);
            SetUniformsNeedUpdate();
        }
    }

    int Camera::GetMaxShadowSplits() const
    {
        #ifdef IS_TARGET_MOBILE
        return 1;
        #else
        return shadowSplits_;
        #endif
    }

    void Camera::FillShaderDefines(std::string& defines, PassType passType) const
    {
        if (PassType::LIT == passType && colorSplits_)
            defines += "COLOR_SPLITS\n";
    }

    void Camera::EnableColorSplits(bool enable)
    {
        if (colorSplits_ != enable)
        {
            colorSplits_ = enable;
            SetUniformsNeedUpdate();
        }
    }

    void Camera::SetShadowSplitLogFactor(float factor)
    {
        shadowSplitLogFactor_ = factor;
    }

    float Camera::GetShadowSplitLogFactor() const
    {
        return shadowSplitLogFactor_;
    }

    BoundingBox Camera::GetViewBox(const Frustum* frustum, const Scene* scene, bool receivers, bool casters)
    {
        BoundingBox result;
        std::vector<SceneNode*> visibles;
        scene->GetVisibleNodes(frustum, visibles);
        for (auto& visible : visibles)
        {
            auto material = visible->GetMaterial().get();
            if (!material) continue;
            if ((receivers && material->ReceiveShadows()) || (casters && material->CastShadow()))
            {
                BoundingBox bb(visible->GetWorldBoundingBox());
                result.Merge(bb);
            }
        }
        return result;
    }

    void Camera::Debug(DebugRenderer* debugRenderer, const Color& color)
    {
        GetFrustum()->Debug(VECTOR3_ZERO, debugRenderer, color);
    }
}