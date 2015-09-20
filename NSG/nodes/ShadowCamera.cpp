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
#include "StringConverter.h"
#include "Check.h"

namespace NSG
{
    ShadowCamera::ShadowCamera(Light* light)
        : Camera(light->GetName() + "ShadowCamera"),
          light_(light),
          farSplit_(MAX_WORLD_SIZE),
          invRange_(0),
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
        auto farZ = Clamp(light_->GetShadowClipEnd(), light_->GetShadowClipStart(), light_->GetDistance());
        SetFarClip(farZ);
        SetFOV(light_->GetSpotCutOff());
        SetAspectRatio(1.0);
        DisableOrtho();
    }

    void ShadowCamera::SetupDirectional(const Camera* camera, float nearSplit, float farSplit)
    {
        CHECK_ASSERT(!GetParent(), __FILE__, __LINE__);

        auto scene = light_->GetScene().get();
        CHECK_ASSERT(light_->GetType() == LightType::DIRECTIONAL, __FILE__, __LINE__);

		EnableOrtho();
		SetAspectRatio(1);
		DisableUserOrthoProjection();

        // Calculate initial position & rotation
        auto pos = camera->GetGlobalPosition() - MAX_WORLD_SIZE * light_->GetLookAtDirection();
        SetPosition(pos);
        SetOrientation(light_->GetGlobalOrientation());
        {
			// setup camera to split frustum
            auto camSplitFrustum = camera->GetFrustumSplit(nearSplit, farSplit);
            BoundingBox shadowBox(*camSplitFrustum);
            shadowBox.Transform(GetView()); // Transform shadowBox to light space
            SetNearClip(0.f);
            SetFarClip(-shadowBox.min_.z);
            auto viewSize = shadowBox.Size();
			SetAspectRatio(viewSize.x / viewSize.y);
			SetOrthoScale(viewSize.x);
            auto viewCenter = shadowBox.Center();
            Vector3 adjust(viewCenter.x, viewCenter.y, 0);
            Translate(adjust);
        }
        {
			// Calculate receivers for current split
            auto camSplitFrustum = GetFrustum();
            BoundingBox shadowBox(*camSplitFrustum);
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
			shadowBox.Clip(receiversBox);
			shadowBox.Transform(GetView()); // transform view box to shadowCam's space
            castersBox.Transform(GetView()); // transform casters view box to shadowCam's space
			auto zFar = -shadowBox.min_.z; // from cam point of view: set zFar to more distance z shadowBox
			auto zNear = -castersBox.max_.z; // from cam point of view: set zNear to closest z caster
			auto zLength = zFar - zNear;
			CHECK_ASSERT(zLength > 0, __FILE__, __LINE__);

			SetFarClip(zLength);
			auto viewSize = castersBox.Size();
			SetAspectRatio(viewSize.x / viewSize.y);
			SetOrthoScale(viewSize.x);
			auto viewCenter = castersBox.Center();
            // center and move closer to casters
            // and in order to avoid  lack of shadows
			auto adjust = Vector3(viewCenter.x, viewCenter.y, -zNear); 
			Translate(adjust);
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

			{
				auto camSplitFrustum = GetFrustum();
				BoundingBox viewBox(*camSplitFrustum);
				viewBox.Transform(camera->GetView()); // Transform worldBox to camera space
				auto range = Length(viewBox.Size());
                invRange_ = 1.f / range;
				//auto nearSplit = -viewBox.max_.z;
				farSplit_ = -viewBox.min_.z;
			}

        }
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
        {
            auto material = visible->GetMaterial();
            if (material && material->IsShadowCaster())
                result.push_back(visible);
        }
        return !result.empty();
    }

    const Vector3& ShadowCamera::GetLightGlobalPosition() const
    {
        if (LightType::DIRECTIONAL == light_->GetType())
            return GetGlobalPosition();
        return light_->GetGlobalPosition();
    }
}
