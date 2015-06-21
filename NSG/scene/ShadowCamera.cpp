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
#include "ShadowCamera.h"
#include "Light.h"
#include "Window.h"
#include "Frustum.h"
#include "Texture.h"
#include "Material.h"
#include "Sphere.h"
#include "Scene.h"
#include "Ray.h"

namespace NSG
{
    ShadowCamera::ShadowCamera(const Light* light)
        : Camera(light->GetName() + "ShadowCamera"),
          light_(light),
          nearSplit_(0),
          farSplit_(MAX_WORLD_SIZE),
          viewRange_(MAX_WORLD_SIZE)
    {
        dirPositiveX_.SetLocalLookAt(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
        dirNegativeX_.SetLocalLookAt(Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
        dirPositiveY_.SetLocalLookAt(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
        dirNegativeY_.SetLocalLookAt(Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
        dirPositiveZ_.SetLocalLookAt(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
        dirNegativeZ_.SetLocalLookAt(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f));
        EnableAutoAspectRatio(false);
    }

    ShadowCamera::~ShadowCamera()
    {

    }

    void ShadowCamera::SetupPoint(const Camera* camera)
    {
        CHECK_ASSERT(light_->GetType() == LightType::POINT, __FILE__, __LINE__);
        SetPosition(light_->GetGlobalPosition());
        SetOrientation(light_->GetGlobalOrientation());
        SetNearClip(0.1f);
        SetFarClip(light_->GetDistance());
        SetFOV(90.f);
        SetAspectRatio(1.0);
        DisableOrtho();
    }

    void ShadowCamera::SetupSpot(const Camera* camera)
    {
        CHECK_ASSERT(light_->GetType() == LightType::SPOT, __FILE__, __LINE__);
        SetPosition(light_->GetGlobalPosition());
        SetOrientation(light_->GetGlobalOrientation());
        SetNearClip(light_->GetShadowClipStart());
        auto farZ = glm::clamp(light_->GetShadowClipEnd(), light_->GetShadowClipStart(), light_->GetDistance());
        SetFarClip(farZ);
        SetFOV(light_->GetSpotCutOff());
        SetAspectRatio(1.0);
        DisableOrtho();
    }

    BoundingBox ShadowCamera::GetViewBox(const Frustum* frustum, bool receivers, bool casters)
    {
        auto scene = light_->GetScene().get();
        BoundingBox result;
        std::vector<SceneNode*> visibles;
        scene->GetVisibleNodes(frustum, visibles);
        for (auto& visible : visibles)
        {
            auto material = visible->GetMaterial().get();
            if ((receivers && material->ReceiveShadows()) || (casters && material->CastShadow()))
            {
                BoundingBox bb(visible->GetWorldBoundingBox());
                result.Merge(bb);
            }
        }
        return result;
    }

    #if 1
    void ShadowCamera::SetupDirectional(int split, const Camera* camera, float nearSplit, float farSplit)
    {
        CHECK_ASSERT(!GetParent(), __FILE__, __LINE__);
        nearSplit_ = nearSplit;
        farSplit_ = farSplit;

        auto scene = light_->GetScene().get();
        EnableOrtho();
        CHECK_ASSERT(light_->GetType() == LightType::DIRECTIONAL, __FILE__, __LINE__);
        Camera shadowCam;
        auto orientation = light_->GetGlobalOrientation();
        auto dir = light_->GetLookAtDirection();
        auto initialPos = camera->GetGlobalPosition() - MAX_WORLD_SIZE * dir;
        shadowCam.SetGlobalOrientation(orientation);
        shadowCam.SetGlobalPosition(initialPos);

        auto camFrustum = camera->GetFrustumSplit(nearSplit, farSplit);

        BoundingBox splitBB(*camFrustum);
        auto receiversBB = GetViewBox(camFrustum.get(), true, false);
        if (receiversBB.IsDefined())
            splitBB.Clip(receiversBB);

        {
            BoundingBox shadowCamSplit(splitBB);
            shadowCamSplit.Transform(shadowCam.GetView());
            auto viewSize = shadowCamSplit.Size();
            auto viewCenter = shadowCamSplit.Center();
            Vector3 adjust(viewCenter.x, viewCenter.y, 0);
            initialPos += orientation * adjust;
            shadowCam.EnableOrtho();
            shadowCam.SetPosition(initialPos);
            shadowCam.SetAspectRatio(viewSize.x / viewSize.y);
            shadowCam.SetOrthoScale(viewSize.x);
            shadowCam.SetNearClip(0);
            shadowCam.SetFarClip(-shadowCamSplit.min_.z);
        }

        auto shadowCamFrustum = shadowCam.GetFrustum();
        BoundingBox castersBB = GetViewBox(shadowCamFrustum.get(), false, true);
        if(castersBB.IsDefined())
            splitBB.Merge(castersBB);

        splitBB.Transform(shadowCam.GetView());
        BoundingBox viewBox(splitBB);

        auto nearZ = -viewBox.max_.z;
        auto finalPos = initialPos + nearZ * dir;

        auto viewCenter = viewBox.Center();
        Vector3 adjust(viewCenter.x, viewCenter.y, 0);
        SetPosition(finalPos + orientation * adjust);
        SetOrientation(orientation);

        auto viewSize = viewBox.Size();
        #if 0
        {
            const float QUANTIZE = 0.5f;
            const float MIN_VIEW_SIZE = 3.f;
            viewSize.x = ceilf(sqrtf(viewSize.x / QUANTIZE));
            viewSize.y = ceilf(sqrtf(viewSize.y / QUANTIZE));
            viewSize.x = std::max(viewSize.x * viewSize.x * QUANTIZE, MIN_VIEW_SIZE);
            viewSize.y = std::max(viewSize.y * viewSize.y * QUANTIZE, MIN_VIEW_SIZE);
        }
        #endif

        SetNearClip(0);
        auto farZ = -viewBox.min_.z - nearZ;
        SetFarClip(farZ);

        SetAspectRatio(viewSize.x / viewSize.y);
        SetOrthoScale(viewSize.x);
        auto c1 = viewSize.x;
        auto c2 = viewSize.y;
        auto diagonal = std::sqrt(c1 * c1 + c2 * c2);
        viewRange_ = std::max(farZ, diagonal);
    }
    #else
    void ShadowCamera::SetupDirectional(int split, const Camera* camera, float nearSplit, float farSplit)
    {
        nearSplit_ = nearSplit;
        farSplit_ = farSplit;
        CHECK_ASSERT(light_->GetType() == LightType::DIRECTIONAL, __FILE__, __LINE__);

        EnableOrtho();
        auto lightDirection = light_->GetLookAtDirection();
        auto lightOrientation = light_->GetGlobalOrientation();
        SetGlobalOrientation(lightOrientation);
        float extrusionDistance = farSplit - nearSplit;
        auto camDirection = camera->GetLookAtDirection();
        auto camPos = camera->GetGlobalPosition();
        auto camNearPos = camPos + nearSplit * camDirection;
        Vector3 pos = camNearPos - extrusionDistance * lightDirection;
        SetGlobalPosition(pos);

        auto camFrustum = camera->GetFrustumSplit(nearSplit, farSplit);
        bool isEmpty = true;
        BoundingBox vb(GetViewBoxAndAdjustPosition(camFrustum.get(), true, false, isEmpty));
        auto zFar = -vb.min_.z;
        SetNearClip(0);
        SetFarClip(zFar);
        QuantizeDirLightShadowCamera(split, vb);

        if (!isEmpty)
        {
            viewRange_ = std::max(viewRange_, zFar);
            // Recalculate zFar in order not to miss any object between camFrustum and light.
            // Light Frustum Intersected with Scene to Calculate Near=0 and Far Planes
            // See https://msdn.microsoft.com/en-us/library/windows/desktop/ee416324%28v=vs.85%29.aspx
            auto pos = GetGlobalPosition();
            float extrusionDistance = MAX_WORLD_SIZE;
            auto oldView = GetView();
            auto oldPos = GetGlobalPosition();
            SetGlobalPosition(pos - extrusionDistance * lightDirection);
            SetFarClip(extrusionDistance + zFar);
            auto shadowCamFrustum = GetFrustum().get();
            BoundingBox vb1(GetViewBoxAndAdjustPosition(shadowCamFrustum, false, true, isEmpty));
            auto currentPosFromLastView = oldView * Vector4(GetGlobalPosition(), 1);
            if (currentPosFromLastView.z < 0)
            {
                // The shadow camera has been moved too closer => rectify position
                SetGlobalPosition(oldPos);
            }
            else
            {
                // We found a caster between the camera frustum and the light position.
                // We keep the updated position and change far clip accordingly
                QuantizeDirLightShadowCamera(split, vb1);
                zFar = -vb1.min_.z;
                SetFarClip(zFar);
            }
            viewRange_ = std::max(viewRange_, zFar);
        }
        else
        {
            viewRange_ = -1;
        }
    }
    #endif

    BoundingBox ShadowCamera::GetViewBoxAndAdjustPosition(const Frustum* frustum, bool receivers, bool casters, bool& isEmpty)
    {
        std::vector<SceneNode*> visibles;
        light_->GetScene()->GetVisibleNodes(frustum, visibles);
        BoundingBox frustumVolume(*frustum);
        BoundingBox shadowCasters;
        for (auto& visible : visibles)
        {
            auto material = visible->GetMaterial().get();
            if ((!casters || material->IsShadowCaster()) && (!receivers || material->ReceiveShadows()))
                shadowCasters.Merge(visible->GetWorldBoundingBox());
        }
        if (shadowCasters.IsDefined())
        {
            isEmpty = false;
            frustumVolume.Clip(shadowCasters);
            BoundingBox cameraViewBox(frustumVolume);
            cameraViewBox.Transform(GetView());
            auto nearZ = -cameraViewBox.max_.z;
            if (nearZ > 0)
            {
                Vector3 pos = GetGlobalPosition();
                Vector3 dir = GetLookAtDirection();
                // Adjust camera position
                // This is needed because I use the length of
                // world2light ( = worldPos.xyz - u_directionalLight.position)
                // to quantize depth (similar to shadow cube map)
                // In this way we do not lose precission
                // See CalcShadowFactor() in lighting.glsl
                SetGlobalPosition(pos + nearZ * dir);
                cameraViewBox = BoundingBox(frustumVolume);
                cameraViewBox.Transform(GetView());
                // after moving the shadow cam position: the zNear has to be almost zero
                CHECK_ASSERT(std::abs(cameraViewBox.max_.z) <= 0.1, __FILE__, __LINE__);
            }
            return cameraViewBox;
        }
        else
        {
            isEmpty = true;
            frustumVolume.Transform(GetView());
            return frustumVolume;
        }
    }
    void ShadowCamera::QuantizeDirLightShadowCamera(int split, const BoundingBox& viewBox)
    {
        Vector2 center(viewBox.Center());
        Vector2 viewSize(viewBox.Size());
        //The bigger issue is that this produces a shadow frustum that continuously changes
        //size and position as the camera moves around. This leads to shadows "swimming",
        //which is a very distracting artifact.
        //In order to fix this, it's common to do the following additional two steps:

        #if 1
        {
            #if 1
            //STEP 1: Quantize size to reduce swimming
            const float QUANTIZE = 0.5f;
            const float MIN_VIEW_SIZE = 3.f;
            viewSize.x = ceilf(sqrtf(viewSize.x / QUANTIZE));
            viewSize.y = ceilf(sqrtf(viewSize.y / QUANTIZE));
            viewSize.x = std::max(viewSize.x * viewSize.x * QUANTIZE, MIN_VIEW_SIZE);
            viewSize.y = std::max(viewSize.y * viewSize.y * QUANTIZE, MIN_VIEW_SIZE);
            #endif
        }
        #else
        {
            //STEP 1: Quantize size to reduce swimming
            const float QUANTIZE = 0.5f;
            const float MIN_VIEW_SIZE = 3.f;
            viewSize.x = std::max(viewSize.x, viewSize.y);
            viewSize.x = ceilf(sqrtf(viewSize.x / QUANTIZE));
            viewSize.x = std::max(viewSize.x * viewSize.x * QUANTIZE, MIN_VIEW_SIZE);
            viewSize.y = viewSize.x;
        }

        #endif

        // Center shadow camera to the view space bounding box
        Quaternion rot(GetGlobalOrientation());
        Vector3 adjust(center.x, center.y, 0.0f);
        Translate(rot * adjust, TS_WORLD);

        float shadowMapWidth = (float)light_->GetShadowMap(split)->GetWidth();
        //viewSize.x *= (shadowMapWidth - 2.f)/shadowMapWidth;
        SetAspectRatio(viewSize.x / viewSize.y);
        SetOrthoScale(viewSize.x);
        auto c1 = viewSize.x;
        auto c2 = viewSize.y;
        auto diagonal = std::sqrt(c1 * c1 + c2 * c2);
        viewRange_ = diagonal;

        #if 1
        if (shadowMapWidth > 0)
        {
            //STEP 2: Discretize the position of the frustum
            //Snap to whole texels
            Vector3 viewPos(glm::inverse(rot) * GetPosition());
            // Take into account that shadow map border will not be used
            float invActualSize = 1.0f / (shadowMapWidth - 2.0f);
            Vector2 texelSize(viewSize.x * invActualSize, viewSize.y * invActualSize);
            Vector3 snap(-fmodf(viewPos.x, texelSize.x), -fmodf(viewPos.y, texelSize.y), 0.0f);
            Translate(rot * snap, TransformSpace::TS_WORLD);
        }
        #endif
    }

    void ShadowCamera::SetCurrentCubeShadowMapFace(TextureTarget target)
    {
        CHECK_ASSERT(light_->GetType() == LightType::POINT, __FILE__, __LINE__);
        switch (target)
        {
            case TextureTarget::TEXTURE_CUBE_MAP_POSITIVE_X:
                SetGlobalOrientation(dirPositiveX_.GetOrientation());
                break;
            case TextureTarget::TEXTURE_CUBE_MAP_NEGATIVE_X:
                SetGlobalOrientation(dirNegativeX_.GetOrientation());
                break;
            case TextureTarget::TEXTURE_CUBE_MAP_POSITIVE_Y:
                SetGlobalOrientation(dirPositiveY_.GetOrientation());
                break;
            case TextureTarget::TEXTURE_CUBE_MAP_NEGATIVE_Y:
                SetGlobalOrientation(dirNegativeY_.GetOrientation());
                break;
            case TextureTarget::TEXTURE_CUBE_MAP_POSITIVE_Z:
                SetGlobalOrientation(dirPositiveZ_.GetOrientation());
                break;
            case TextureTarget::TEXTURE_CUBE_MAP_NEGATIVE_Z:
                SetGlobalOrientation(dirNegativeZ_.GetOrientation());
                break;
            default:
                CHECK_ASSERT(!"Incorrect cube shadowmap face!!!", __FILE__, __LINE__);
                break;
        }
    }

    bool ShadowCamera::GetVisiblesShadowCasters(std::vector<SceneNode*>& result) const
    {
        CHECK_ASSERT(result.empty(), __FILE__, __LINE__);

        std::vector<SceneNode*> visibles;
        light_->GetScene()->GetVisibleNodes(GetFrustumPointer(), visibles);
        for (auto& visible : visibles)
            if (visible->GetMaterial()->IsShadowCaster())
                result.push_back(visible);
        return !result.empty();
    }

    float ShadowCamera::GetRange() const
    {
        if (LightType::DIRECTIONAL == light_->GetType())
            return viewRange_;
        return light_->GetRange();
    }

    const Vector3& ShadowCamera::GetLightGlobalPosition() const
    {
        if (LightType::DIRECTIONAL == light_->GetType())
            return GetGlobalPosition();
        return light_->GetGlobalPosition();
    }
    #if 0
    const Matrix4& ShadowCamera::GetProjection() const
    {
        projection_ = cropMatrix_ * Camera::GetProjection();
        return projection_;
    }

    const Matrix4& ShadowCamera::GetViewProjection() const
    {
        viewProjection_ = cropMatrix_ * Camera::GetProjection() * Camera::GetView();
        return viewProjection_;
    }
    #endif
}