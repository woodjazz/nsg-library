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
#include "ShadowCamera.h"
#include "Light.h"
#include "Window.h"
#include "Frustum.h"
#include "Texture.h"
#include "Material.h"
#include "Sphere.h"
#include "Scene.h"
#include "Ray.h"
#include "StringConverter.h"
#include "Check.h"
#include "Maths.h"

namespace NSG
{
    ShadowCamera::ShadowCamera(Light* light)
        : Camera(light->GetName() + "ShadowCamera"),
          light_(light),
          farSplit_(Scene::MAX_WORLD_SIZE),
          disabled_(false)
    {
        dirPositiveX_.SetLocalLookAtPosition(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
        dirNegativeX_.SetLocalLookAtPosition(Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
        dirPositiveY_.SetLocalLookAtPosition(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
        dirNegativeY_.SetLocalLookAtPosition(Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
        dirPositiveZ_.SetLocalLookAtPosition(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
        dirNegativeZ_.SetLocalLookAtPosition(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f));
        EnableAutoAspectRatio(false);
    }

    ShadowCamera::~ShadowCamera()
    {
    }

    void ShadowCamera::SetupPoint(const Camera* camera)
    {
        CHECK_ASSERT(light_->GetType() == LightType::POINT);
        SetPosition(light_->GetGlobalPosition());
        SetOrientation(light_->GetGlobalOrientation());
        SetNearClip(0.1f);
        SetFarClip(light_->GetDistance());
        SetFOVDegrees(90.f);
        SetAspectRatio(1.0);
        DisableOrtho();
    }

    void ShadowCamera::SetupSpot(const Camera* camera)
    {
        CHECK_ASSERT(light_->GetType() == LightType::SPOT);
        SetPosition(light_->GetGlobalPosition());
        SetOrientation(light_->GetGlobalOrientation());
        SetNearClip(light_->GetShadowClipStart());
        auto farZ = Clamp(light_->GetShadowClipEnd(), light_->GetShadowClipStart(), light_->GetDistance());
        SetFarClip(farZ);
        SetFOVDegrees(light_->GetSpotCutOff());
        SetAspectRatio(1.0);
        DisableOrtho();
    }

    void ShadowCamera::SetupDirectional(const Camera* camera, float nearSplit, float farSplit)
    {
        farSplit_ = farSplit;

        CHECK_ASSERT(!GetParent());

        auto scene = light_->GetScene().get();
        CHECK_ASSERT(light_->GetType() == LightType::DIRECTIONAL);

        EnableOrtho();
        SetAspectRatio(1);
        DisableUserOrthoProjection();

        // Calculate initial position & rotation
        auto pos = camera->GetGlobalPosition() - Scene::MAX_WORLD_SIZE * light_->GetLookAtDirection();
        SetPosition(pos);
        SetOrientation(light_->GetGlobalOrientation());
        {
            // setup camera to split frustum
            auto camSplitFrustum = camera->GetFrustumSplit(nearSplit, farSplit);
            BoundingBox shadowBox(*camSplitFrustum);
            shadowBox.Transform(GetView()); // Transform shadowBox to light space
            SetNearClip(0);//-shadowBox.max_.z);
            SetFarClip(-shadowBox.min_.z);
            auto viewSize = shadowBox.Size();
            SetAspectRatio(viewSize.x / viewSize.y);
            SetOrthoScale(viewSize.x);
            auto viewCenter = shadowBox.Center();
            viewCenter.z = 0;
            Translate(viewCenter);
        }
        {
            // Calculate receivers for current split
            auto camSplitFrustum = GetFrustum();
            auto receiversBox = Camera::GetViewBox(camSplitFrustum.get(), scene, true, false);
            if (!receiversBox.IsDefined())
            {
                disabled_ = true;
                return; // no receivers for this split => nothing to do
            }
            // Calculate casters for current split
            auto castersBox = Camera::GetViewBox(camSplitFrustum.get(), scene, false, true);
            if (!castersBox.IsDefined())
            {
                disabled_ = true;
                return; // no casters for this split => nothing to do
            }
            disabled_ = false;
            receiversBox.Transform(GetView()); // transform receivers view box to shadowCam's space
            castersBox.Transform(GetView()); // transform casters view box to shadowCam's space
            auto zFar = -receiversBox.min_.z; // from cam point of view: set zFar to more distance z receiver
            auto zNear = -castersBox.max_.z; // from cam point of view: set zNear to closest z caster
            CHECK_ASSERT(zNear < zFar);
            SetNearClip(zNear);
            SetFarClip(zFar);
            auto viewSize = castersBox.Size();
            SetAspectRatio(viewSize.x / viewSize.y);
            SetOrthoScale(viewSize.x);
            auto viewCenter = castersBox.Center();
            // center camera
            viewCenter.z = 0;
            Translate(viewCenter);
        }
        {
#if 0
            //The bigger issue is a shadow frustum that continuously changes
            //size and position as the camera moves around. This leads to shadows "swimming",
            //which is a very distracting artifact.
            {
                auto camSplitFrustum = GetFrustum();
                BoundingBox shadowBox(*camSplitFrustum);
                shadowBox.Transform(GetView()); // Transform shadowBox to shadowCamera space
                auto shadowBoxSize = shadowBox.Size();
                //Quantize size to reduce swimming
                const float QUANTIZE = 0.5f;
                const float MIN_VIEW_SIZE = 3.f;
                shadowBoxSize.x = ceilf(sqrtf(shadowBoxSize.x / QUANTIZE));
                shadowBoxSize.y = ceilf(sqrtf(shadowBoxSize.y / QUANTIZE));
                shadowBoxSize.x = std::max(shadowBoxSize.x * shadowBoxSize.x * QUANTIZE, MIN_VIEW_SIZE);
                shadowBoxSize.y = std::max(shadowBoxSize.y * shadowBoxSize.y * QUANTIZE, MIN_VIEW_SIZE);
                SetAspectRatio(shadowBoxSize.x / shadowBoxSize.y);
                SetOrthoScale(shadowBoxSize.x);
            }
#endif

        }
    }

    void ShadowCamera::SetCurrentCubeShadowMapFace(TextureTarget target)
    {
        CHECK_ASSERT(light_->GetType() == LightType::POINT);
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
                CHECK_ASSERT(!"Incorrect cube shadowmap face!!!");
                break;
        }
    }

    bool ShadowCamera::GetVisiblesShadowCasters(std::vector<SceneNode*>& result) const
    {
        CHECK_ASSERT(result.empty());

        std::vector<SceneNode*> visibles;
        light_->GetScene()->GetVisibleNodes(GetFrustumPointer(), visibles);
        for (auto& visible : visibles)
        {
            auto material = visible->GetMaterial();
            if (material && material->IsShadowCaster())
                result.push_back(visible);
        }
        return !result.empty();
    }
}
