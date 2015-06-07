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

namespace NSG
{
    ShadowCamera::ShadowCamera(const std::string& name)
        : Camera(name),
          type_(LightType::POINT)
    {
        dirPositiveX_.SetLocalLookAt(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
        dirNegativeX_.SetLocalLookAt(Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
        dirPositiveY_.SetLocalLookAt(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
        dirNegativeY_.SetLocalLookAt(Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
        dirPositiveZ_.SetLocalLookAt(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
        dirNegativeZ_.SetLocalLookAt(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f));
        SetWindow(nullptr);
    }

    ShadowCamera::~ShadowCamera()
    {

    }

    void ShadowCamera::Setup(const Light* light, const Window* window, const Camera* camera)
    {
        type_ = light->GetType();
        if (LightType::SPOT == type_)
        {
            SetPosition(light->GetGlobalPosition());
            SetOrientation(light->GetGlobalOrientation());
            SetNearClip(light->GetShadowClipStart());
            auto farZ = glm::clamp(light->GetShadowClipEnd(), light->GetShadowClipStart(), light->GetDistance());
            SetFarClip(farZ);
            SetFOV(light->GetSpotCutOff());
            SetAspectRatio(1.0);
            DisableOrtho();
        }
        else if (LightType::POINT == type_)
        {
            SetPosition(light->GetGlobalPosition());
            SetOrientation(light->GetGlobalOrientation());
            SetNearClip(0.1f);
            SetFarClip(light->GetDistance());
            SetFOV(90.f);
            SetAspectRatio(1.0);
            DisableOrtho();
        }
        else
        {
            EnableOrtho();
            float extrusionDistance = camera->GetZFar();
            Vector3 pos = camera->GetGlobalPosition() - extrusionDistance * light->GetLookAtDirection();
            SetGlobalPosition(pos);
            SetGlobalOrientation(light->GetGlobalOrientation());

            BoundingBox cameraViewBox(camera->GetFrustum()->GetVerticesTransform(GetView()));
            Vector2 viewSize(cameraViewBox.max_.x - cameraViewBox.min_.x, cameraViewBox.max_.y - cameraViewBox.min_.y);
            SetNearClip(0.f);
            auto farZ = cameraViewBox.max_.z - cameraViewBox.min_.z;
            SetFarClip(farZ);

            QuantizeDirLightShadowCamera(light, cameraViewBox);
        }
    }

    void ShadowCamera::QuantizeDirLightShadowCamera(const Light* light, const BoundingBox& viewBox)
    {
        float minX = viewBox.min_.x;
        float minY = viewBox.min_.y;
        float maxX = viewBox.max_.x;
        float maxY = viewBox.max_.y;

        Vector2 center((minX + maxX) * 0.5f, (minY + maxY) * 0.5f);
        Vector2 viewSize(maxX - minX, maxY - minY);

        //The bigger issue is that this produces a shadow frustum that continuously changes
        //size and position as the camera moves around. This leads to shadows "swimming",
        //which is a very distracting artifact.
        //In order to fix this, it's common to do the following additional two steps:

        {
            //STEP 1: Quantize size to reduce swimming
            const float QUANTIZE = 0.5f;
            const float MIN_VIEW_SIZE = 3.f;
            viewSize.x = ceilf(sqrtf(viewSize.x / QUANTIZE));
            viewSize.y = ceilf(sqrtf(viewSize.y / QUANTIZE));
            viewSize.x = std::max(viewSize.x * viewSize.x * QUANTIZE, MIN_VIEW_SIZE);
            viewSize.y = std::max(viewSize.y * viewSize.y * QUANTIZE, MIN_VIEW_SIZE);
            // TODO: Don't allow the shadow frustum to change size as the camera rotates.
        }

        // Center shadow camera to the view space bounding box
        Quaternion rot(GetGlobalOrientation());
        Vector3 adjust(center.x, center.y, 0.0f);
        Translate(rot * adjust, TS_WORLD);

        SetAspectRatio(viewSize.x, viewSize.y);
        SetOrthoScale(viewSize.x);

        float shadowMapWidth = (float)light->GetShadowMap()->GetWidth();
        if (shadowMapWidth > 0.0f)
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
    }

    void ShadowCamera::FinalizeShadowCamera(const Light* light, const Camera* camera, std::vector<SceneNode*> shadowCasters)
    {
        LightType type = light->GetType();

        if (type == LightType::DIRECTIONAL)
        {
#if 0
            BoundingBox shadowBox;
            shadowBox.max_.y = GetOrthoScale() * 0.5f;
            shadowBox.max_.x = GetAspectRatio() * shadowBox.max_.y;
            shadowBox.min_.y = -shadowBox.max_.y;
            shadowBox.min_.x = -shadowBox.max_.x;
#else
            BoundingBox shadowBox;
            for (auto& caster : shadowCasters)
                shadowBox.Merge(caster->GetWorldBoundingBox());
            shadowBox.Transform(GetView());
#endif

            // Requantize and snap to shadow map texels
            QuantizeDirLightShadowCamera(light, shadowBox);
        }
        #if 0
        BoundingBox cameraViewBox;
        for (auto& caster : shadowCasters)
            cameraViewBox.Merge(caster->GetWorldBoundingBox());

        //cameraViewBox.Transform(GetFrustum()->GetMatrix());

        auto farCorner1 = glm::inverse(GetGlobalOrientation()) * Vector3(cameraViewBox.min_.x, cameraViewBox.max_.y, cameraViewBox.max_.z);
        auto farCorner2 = glm::inverse(GetGlobalOrientation()) * Vector3(cameraViewBox.max_.x, cameraViewBox.min_.y, cameraViewBox.max_.z);
        auto origin = glm::inverse(GetGlobalOrientation()) * cameraViewBox.Center();
        auto nearposition = glm::inverse(GetGlobalOrientation()) * Vector3(origin.x, origin.y, cameraViewBox.min_.z);
        auto farposition = glm::inverse(GetGlobalOrientation()) * Vector3(origin.x, origin.y, cameraViewBox.max_.z);
        //auto position = Vector3(origin.x, origin.y, cameraViewBox.min_.z);
        auto direction = farposition - nearposition;
        SetGlobalPosition(nearposition);
        SetGlobalLookAt(glm::normalize(direction));



        Vector2 viewSize(glm::abs(farCorner2.x - farCorner1.x), glm::abs(farCorner2.y - farCorner1.y));
        SetNearClip(0.f);
        auto farZ = glm::length(direction);//cameraViewBox.max_.z - cameraViewBox.min_.z;
        SetFarClip(farZ);

        //The bigger issue is that this produces a shadow frustum that continuously changes
        //size and position as the camera moves around. This leads to shadows "swimming",
        //which is a very distracting artifact.
        //In order to fix this, it's common to do the following additional two steps:

        {
            //STEP 1: Quantize size to reduce swimming
            const float QUANTIZE = 0.5f;
            const float MIN_VIEW_SIZE = 3.f;
            viewSize.x = ceilf(sqrtf(viewSize.x / QUANTIZE));
            viewSize.y = ceilf(sqrtf(viewSize.y / QUANTIZE));
            viewSize.x = std::max(viewSize.x * viewSize.x * QUANTIZE, MIN_VIEW_SIZE);
            viewSize.y = std::max(viewSize.y * viewSize.y * QUANTIZE, MIN_VIEW_SIZE);
            // TODO: Don't allow the shadow frustum to change size as the camera rotates.
        }

        SetAspectRatio(viewSize.x, viewSize.y);
        SetOrthoScale(viewSize.x);
        /*
                Vector3 center = cameraViewBox.Center();
                // Center shadow camera to the view space bounding box
                Quaternion rot(GetGlobalOrientation());
                Vector3 adjust(center.x, center.y, 0.0f);
                Translate(rot * adjust, TransformSpace::TS_WORLD);

                float shadowMapWidth = (float)light->GetShadowMap()->GetWidth();
                if (shadowMapWidth > 0.0f)
                {
                    //STEP 2: Discretize the position of the frustum
                    //Snap to whole texels
                    Vector3 viewPos(glm::inverse(rot) * GetGlobalPosition());
                    // Take into account that shadow map border will not be used
                    float invActualSize = 1.0f / (shadowMapWidth - 2.0f);
                    Vector2 texelSize(viewSize.x * invActualSize, viewSize.y * invActualSize);
                    Vector3 snap(-fmodf(viewPos.x, texelSize.x), -fmodf(viewPos.y, texelSize.y), 0.0f);
                    Translate(rot * snap, TransformSpace::TS_WORLD);
                }
         */
        #endif
    }

    void ShadowCamera::SetCurrentCubeShadowMapFace(TextureTarget target)
    {
        CHECK_ASSERT(type_ == LightType::POINT, __FILE__, __LINE__);
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

    bool ShadowCamera::IsVisible(const SceneNode* node) const
    {
        auto mesh = node->GetMesh();
        auto material = node->GetMaterial();
        if (!mesh || !mesh->IsReady() || !material || !material->IsReady())
            return false;

        bool visible = false;

        if (!material->IsShadeless())
        {
            switch (type_)
            {
                case LightType::SPOT:
                    {
                        visible = GetFrustumPointer()->IsVisible(*node, *mesh);
                        break;
                    }
                case LightType::POINT:
                    {
                        auto& bb = node->GetWorldBoundingBox();
                        Sphere sphereLight(GetGlobalPosition(), GetZFar());
                        visible = sphereLight.IsInside(bb) != Intersection::OUTSIDE;
                        break;
                    }
                case LightType::DIRECTIONAL:
                    visible = true;
                    break;
                default:
                    break;
            }
        }
        return visible;
    }

    bool ShadowCamera::IsVisibleFromCurrentFace(const SceneNode* node) const
    {
        CHECK_ASSERT(type_ == LightType::POINT, __FILE__, __LINE__);
        auto mesh = node->GetMesh();
        auto material = node->GetMaterial();
        if (!mesh || !mesh->IsReady() || !material || !material->IsReady() || material->IsShadeless())
            return false;
        return GetFrustumPointer()->IsVisible(*node, *mesh);
    }

    bool ShadowCamera::GetVisibles(const std::vector<SceneNode*>& nodes, std::vector<SceneNode*>& result) const
    {
        CHECK_ASSERT(result.empty(), __FILE__, __LINE__);
        for (auto& node : nodes)
            if (IsVisible(node))
                result.push_back(node);
        return !result.empty();
    }

    bool ShadowCamera::GetVisiblesFromCurrentFace(const std::vector<SceneNode*>& nodes, std::vector<SceneNode*>& result) const
    {
        CHECK_ASSERT(type_ == LightType::POINT, __FILE__, __LINE__);
        CHECK_ASSERT(result.empty(), __FILE__, __LINE__);
        for (auto& node : nodes)
            if (IsVisibleFromCurrentFace(node))
                result.push_back(node);
        return !result.empty();
    }
}