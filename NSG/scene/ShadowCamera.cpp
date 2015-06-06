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
            SetFarClip(light->GetShadowClipEnd());
            SetFOV(light->GetSpotCutOff());
            SetAspectRatio(1.0);
            DisableOrtho();
        }
        else if (LightType::POINT == type_)
        {
            SetPosition(light->GetGlobalPosition());
            SetOrientation(light->GetGlobalOrientation());
            SetNearClip(light->GetShadowClipStart());
            SetFarClip(light->GetShadowClipEnd());
            SetFOV(90.f);
            SetAspectRatio(1.0);
            DisableOrtho();
        }
        else
        {
            EnableOrtho();
            float extrusionDistance = camera->GetZFar();
            Vector3 pos = camera->GetGlobalPosition() - extrusionDistance * light->GetLookAtDirection();
            SetPosition(pos);
            SetOrientation(light->GetGlobalOrientation());
            
            BoundingBox cameraViewBox(camera->GetFrustum()->GetVerticesTransform(GetView()));
			Vector2 viewSize(cameraViewBox.max_.x - cameraViewBox.min_.x, cameraViewBox.max_.y - cameraViewBox.min_.y);
			SetNearClip(0.f);
            auto farZ = cameraViewBox.max_.z - cameraViewBox.min_.z;
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

            Vector3 center = cameraViewBox.Center();
            // Center shadow camera to the view space bounding box
            Quaternion rot(GetOrientation());
            Vector3 adjust(center.x, center.y, 0.0f);
            Translate(rot * adjust, TransformSpace::TS_WORLD);

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