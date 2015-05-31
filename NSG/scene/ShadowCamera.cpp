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
            SetNearClip(0.002f);
            SetFarClip(light->GetDistance());
            SetFOV(light->GetSpotCutOff());
            SetAspectRatio(1.0);
            DisableOrtho();
        }
        else if (LightType::POINT == type_)
        {
            SetPosition(light->GetGlobalPosition());
            SetOrientation(light->GetGlobalOrientation());
            SetNearClip(0.002f);
            SetFarClip(light->GetDistance());
            SetFOV(90.f);
            SetAspectRatio(1.0);
            DisableOrtho();
        }
        else
        {
            float extrusionDistance = camera->GetZFar();
            //Vector3 pos = camera->GetGlobalPosition() - extrusionDistance * light->GetLookAtDirection();
            SetPosition(VECTOR3_ZERO);
            SetOrientation(light->GetGlobalOrientation());
            //BoundingBox viewBox(Vector3(-1), Vector3(1));
            //viewBox.Transform(GetGlobalModelInvMatrix() * camera->GetViewProjectionInverseMatrix());
            //viewBox.Transform(camera->GetViewProjectionInverseMatrix());
            BoundingBox viewBox(camera->GetFrustum()->GetVerticesTransform(GetView()));
            EnableOrtho();

            orthoProjection_.left_ = viewBox.min_.x;
            orthoProjection_.right_ = viewBox.max_.x;
            orthoProjection_.bottom_ = viewBox.min_.y;
            orthoProjection_.top_ = viewBox.max_.y;
            orthoProjection_.near_ = viewBox.min_.z;
            orthoProjection_.far_ = viewBox.max_.z;
       




            // Set initial position & rotation
            Vector3 pos = viewBox.Center();
            //pos.z -= (viewBox.max_.z - viewBox.min_.z) * 0.5f;
            //pos -= extrusionDistance * light->GetLookAtDirection();
            SetPosition(pos);
            //SetLocalLookAt(light->GetLookAtDirection(), VECTOR3_UP);
            //SetOrientation(light->GetGlobalOrientation());

            Vector3 center = viewBox.Center();
            // Center shadow camera to the view space bounding box
            Quaternion rot(GetOrientation());
            Vector3 adjust(center.x, center.y, 0.0f);
            Translate(rot * adjust, TransformSpace::TS_WORLD);

            #if 0
            // If the shadow map viewport is known, snap to whole texels
            if (shadowMapWidth > 0.0f)
            {
                Vector3 viewPos(glm::inverse(rot) * GetPosition());
                // Take into account that shadow map border will not be used
                float invActualSize = 1.0f / (shadowMapWidth - 2.0f);
                Vector2 texelSize(viewSize.x * invActualSize, viewSize.y * invActualSize);
                Vector3 snap(-fmodf(viewPos.x, texelSize.x), -fmodf(viewPos.y, texelSize.y), 0.0f);
                Translate(rot * snap, TransformSpace::TS_WORLD);
            }
            #endif
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

    void ShadowCamera::CalculateOrthoProjection() const
    {
    }

}