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
#include "Check.h"

namespace NSG
{
    ShadowCamera::ShadowCamera(Light* light)
        : Camera(light->GetName() + "ShadowCamera"),
          light_(light),
          nearSplit_(0),
          farSplit_(MAX_WORLD_SIZE),
          invRange_(0)
    {
        dirPositiveX_.SetLocalLookAtPosition(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
        dirNegativeX_.SetLocalLookAtPosition(Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
        dirPositiveY_.SetLocalLookAtPosition(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
        dirNegativeY_.SetLocalLookAtPosition(Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
        dirPositiveZ_.SetLocalLookAtPosition(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
        dirNegativeZ_.SetLocalLookAtPosition(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f));
        EnableAutoAspectRatio(false);
        tempCam_.EnableOrtho();
        tempCam_.SetNearClip(0); // Set near plane in order to view all the scene
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

    void ShadowCamera::SetupDirectional(int split, const Camera* camera, float nearSplit, float farSplit, const BoundingBox& receiversFullFrustumViewBox)
    {
        CHECK_ASSERT(!GetParent(), __FILE__, __LINE__);
        nearSplit_ = nearSplit;
        farSplit_ = farSplit;

        auto scene = light_->GetScene().get();
        EnableOrtho();
        CHECK_ASSERT(light_->GetType() == LightType::DIRECTIONAL, __FILE__, __LINE__);
        
		auto orientation = light_->GetGlobalOrientation();
        auto dir = light_->GetLookAtDirection();
        //Set the initial pos far away in order not to miss any object
		auto initialPos = camera->GetGlobalPosition() - MAX_WORLD_SIZE * dir;
        tempCam_.SetOrientation(orientation);
		tempCam_.SetPosition(initialPos);
        BoundingBox splitBB;

#if 0
        if (farSplit < camera->GetZFar() || nearSplit > camera->GetZNear())
        {
            // We are using a split. 
			// Calculate receivers for the split.
			// Clip the frustum's split against the receivers.
            auto camSplitFrustum = camera->GetFrustumSplit(nearSplit, farSplit);
            splitBB = BoundingBox(*camSplitFrustum);
            auto receiversBB = Camera::GetViewBox(camSplitFrustum.get(), scene, true, false);
            if (receiversBB.IsDefined())
                splitBB.Clip(receiversBB);
        }
        else
#endif
        {
            // We are using the whole camera's frustum.
            // No need to recalculate the receivers (already here as a parameter)
			// Clip the full frustum against the receivers.
            splitBB = BoundingBox(*camera->GetFrustum());
            if (receiversFullFrustumViewBox.IsDefined())
                splitBB.Clip(receiversFullFrustumViewBox);
        }

        {
            // Setup/Adjust the shadowCam camera to calculate the casters
            BoundingBox shadowCamSplit(splitBB);
			shadowCamSplit.Transform(tempCam_.GetView());
            auto viewSize = shadowCamSplit.Size();
            auto viewCenter = shadowCamSplit.Center();
            // Calculate shadowCam's center to the view space bounding box
            Vector3 adjust(viewCenter.x, viewCenter.y, 0);
			auto initialZ = initialPos.z;
            initialPos += orientation * adjust;
			auto finalZ = initialPos.z;
			tempCam_.SetPosition(initialPos); //Center shadowCam
			tempCam_.SetAspectRatio(viewSize.x / viewSize.y);
			tempCam_.SetOrthoScale(viewSize.x);
			auto offsetZ = finalZ - initialZ;
			auto farZ = -(shadowCamSplit.min_.z + offsetZ);
			tempCam_.SetFarClip(farZ); // Set far plane in order to view all the scene
        }

		splitBB.Transform(tempCam_.GetView()); // transform view box the shadowCam's space
		auto shadowCamFrustum = tempCam_.GetFrustum();
		// Get caster for the current shadowCam's frustum
        BoundingBox castersBB = Camera::GetViewBox(shadowCamFrustum.get(), scene, false, true);
		if (castersBB.IsDefined())
		{
			// If there are casters visibles from current shadowCam's frustum:
			castersBB.Transform(tempCam_.GetView()); // transform caster's view box the shadowCam's space
			// Merge Z axis casters in order not to miss any between camFrustum and shadowCam's position
			castersBB.min_.z = std::min(castersBB.min_.z, splitBB.min_.z);
			castersBB.max_.z = std::max(castersBB.max_.z, splitBB.max_.z);
			splitBB = castersBB;
		}

        QuantizeAndSetup2ViewBox(split, initialPos, splitBB);
        //SetRange(Length(splitBB.Size())); //Sets the shadowCam's range used in the shader (See u_lightInvRange)
        invRange_ = 1.f / Length(splitBB.Size());
    }

    void ShadowCamera::QuantizeAndSetup2ViewBox(int split, const Vector3& initialPos, const BoundingBox& viewBox)
    {
        auto orientation = light_->GetGlobalOrientation();
        auto dir = light_->GetLookAtDirection();

        auto nearZ = -viewBox.max_.z;
        SetNearClip(0);
        auto farZ = -viewBox.min_.z - nearZ;
        SetFarClip(farZ);

        auto finalPos = initialPos + nearZ * dir;

        // Center shadow camera to the view space bounding box
        auto viewCenter = viewBox.Center();
        Vector3 adjust(viewCenter.x, viewCenter.y, 0);
        finalPos += orientation * adjust;

        auto viewSize = viewBox.Size();

        //The bigger issue is a shadow frustum that continuously changes
        //size and position as the camera moves around. This leads to shadows "swimming",
        //which is a very distracting artifact.
        //In order to fix this, it's common to do the following additional two steps:
        #if 1
        {
            //STEP 1: Quantize size to reduce swimming
            const float QUANTIZE = 2.5f;
            const float MIN_VIEW_SIZE = 3.f;
            viewSize.x = ceilf(sqrtf(viewSize.x / QUANTIZE));
            viewSize.y = ceilf(sqrtf(viewSize.y / QUANTIZE));
            viewSize.x = std::max(viewSize.x * viewSize.x * QUANTIZE, MIN_VIEW_SIZE);
            viewSize.y = std::max(viewSize.y * viewSize.y * QUANTIZE, MIN_VIEW_SIZE);
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

        SetAspectRatio(viewSize.x / viewSize.y);
        SetOrthoScale(viewSize.x);

        #if 0
        float shadowMapWidth = (float)light_->GetShadowMap(split)->GetWidth();
        if (shadowMapWidth > 0)
        {
            //STEP 2: Discretize the position of the frustum
            //Snap to whole texels
            Vector3 viewPos(Inverse(orientation) * GetPosition());
            // Take into account that shadow map border will not be used
            float invActualSize = 1.0f / (shadowMapWidth - 2.0f);
            Vector2 texelSize(viewSize.x * invActualSize, viewSize.y * invActualSize);
            Vector3 snap(-fmodf(viewPos.x, texelSize.x), -fmodf(viewPos.y, texelSize.y), 0.0f);
            finalPos += orientation * snap;
        }
        #endif

        SetPosition(finalPos);
        SetOrientation(orientation);
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

    void ShadowCamera::SetMaxShadowSplits(int splits)
    {
        CHECK_ASSERT(!"This is a shadow camera. Cannot manage splits!!!", __FILE__, __LINE__);
    }

    int ShadowCamera::GetMaxShadowSplits() const
    {
        CHECK_ASSERT(!"This is a shadow camera. Cannot manage splits!!!", __FILE__, __LINE__);
        return MAX_SHADOW_SPLITS;
    }

}