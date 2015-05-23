#include "Light.h"
#include "Log.h"
#include "Check.h"
#include "Constants.h"
#include "Resource.h"
#include "Material.h"
#include "Util.h"
#include "Scene.h"
#include "Frustum.h"
#include "Renderer.h"
#include "Window.h"
#include "Sphere.h"
#include "FrameBuffer.h"
#include "pugixml.hpp"
#include <assert.h>
#include <algorithm>
#include <sstream>

namespace NSG
{
    Light::Light(const std::string& name)
        : SceneNode(name),
          type_(LightType::POINT),
          energy_(1),
          color_(1),
          diffuse_(true),
          specular_(true),
          spotCutOff_(45),
          diffuseColor_(1),
          specularColor_(1),
          distance_(30),
          invRange_(1.f / distance_),
          shadows_(true),
          width_(0),
          height_(0),
          frustumDirty_(false),
          viewProjectionDirty_(false)
    {
        FrameBuffer::Flags flags((unsigned int)(FrameBuffer::COLOR | FrameBuffer::COLOR_USE_TEXTURE | FrameBuffer::COLOR_CUBE_TEXTURE | FrameBuffer::DEPTH));
        shadowFrameBuffer_ = std::make_shared<FrameBuffer>(GetUniqueName("LightCubeFrameBuffer"), flags);

        dirPositiveX_.SetLocalLookAt(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
        dirNegativeX_.SetLocalLookAt(Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
        dirPositiveY_.SetLocalLookAt(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
        dirNegativeY_.SetLocalLookAt(Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
        dirPositiveZ_.SetLocalLookAt(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
        dirNegativeZ_.SetLocalLookAt(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f));
    }

    Light::~Light()
    {
        SignalBeingDestroy()->Run(this);
    }

    void Light::SetEnergy(float energy)
    {
        if (energy_ != energy)
        {
            energy_ = energy;
            CalculateColor();
            SetUniformsNeedUpdate();
        }
    }

    void Light::SetColor(Color color)
    {
        if (color_ != color)
        {
            color_ = color;
            CalculateColor();
            SetUniformsNeedUpdate();
        }
    }

    void Light::EnableDiffuseColor(bool enable)
    {
        if (diffuse_ != enable)
        {
            diffuse_ = enable;
            CalculateColor();
            SetUniformsNeedUpdate();
        }
    }

    void Light::EnableSpecularColor(bool enable)
    {
        if (specular_ != enable)
        {
            specular_ = enable;
            CalculateColor();
            SetUniformsNeedUpdate();
        }
    }

    void Light::SetSpotCutOff(float spotCutOff)
    {
        if (spotCutOff_ != spotCutOff)
        {
            spotCutOff_ = spotCutOff;
            OnDirty();
            SetUniformsNeedUpdate();
        }
    }

    void Light::SetType(LightType type)
    {
        if (type_ != type)
        {
            if (type == LightType::POINT)
            {
                FrameBuffer::Flags flags((unsigned int)(FrameBuffer::COLOR | FrameBuffer::COLOR_USE_TEXTURE | FrameBuffer::COLOR_CUBE_TEXTURE | FrameBuffer::DEPTH));
                shadowFrameBuffer_ = std::make_shared<FrameBuffer>(GetUniqueName("Light2DFrameBuffer"), flags);
            }
            else if (type_ == LightType::POINT)
            {
                FrameBuffer::Flags flags((unsigned int)(FrameBuffer::COLOR | FrameBuffer::COLOR_USE_TEXTURE | FrameBuffer::DEPTH));
                shadowFrameBuffer_ = std::make_shared<FrameBuffer>(GetUniqueName("LightCubeFrameBuffer"), flags);
            }

            type_ = type;
            OnDirty();
            SetUniformsNeedUpdate();
        }
    }

    void Light::Save(pugi::xml_node& node) const
    {
        node.append_attribute("name").set_value(GetName().c_str());
        node.append_attribute("nodeType").set_value("Light");
        node.append_attribute("type").set_value(ToString(type_));
        node.append_attribute("energy").set_value(energy_);
        node.append_attribute("color").set_value(ToString(color_).c_str());
        node.append_attribute("diffuse").set_value(diffuse_);
        node.append_attribute("specular").set_value(specular_);
        node.append_attribute("spotCutOff").set_value(spotCutOff_);
        node.append_attribute("position").set_value(ToString(GetPosition()).c_str());
        node.append_attribute("orientation").set_value(ToString(GetOrientation()).c_str());
        node.append_attribute("distance").set_value(distance_);
        node.append_attribute("shadows").set_value(shadows_);
        SaveChildren(node);
    }

    void Light::Load(const pugi::xml_node& node)
    {
        name_ = node.attribute("name").as_string();
        SetType(ToLightType(node.attribute("type").as_string()));
        SetEnergy(node.attribute("energy").as_float());
        SetColor(ToVertex4(node.attribute("color").as_string()));
        EnableDiffuseColor(node.attribute("diffuse").as_bool());
        EnableSpecularColor(node.attribute("specular").as_bool());
        SetSpotCutOff(node.attribute("spotCutOff").as_float());
        Vertex3 position = ToVertex3(node.attribute("position").as_string());
        SetPosition(position);
        Quaternion orientation = ToQuaternion(node.attribute("orientation").as_string());
        SetOrientation(orientation);
        SetDistance(node.attribute("distance").as_float());
        EnableShadows(node.attribute("shadows").as_bool());
        LoadChildren(node);
    }

    void Light::FillShaderDefines(std::string& defines, PassType passType) const
    {
        bool shadowPass =  PassType::SHADOW == passType;
        if (!shadowPass)
        {
            if (LightType::POINT == type_)
                defines += "HAS_POINT_LIGHT\n";
            else if (LightType::DIRECTIONAL == type_)
                defines += "HAS_DIRECTIONAL_LIGHT\n";
            else
            {
                defines += "HAS_SPOT_LIGHT\n";
            }

            if (DoShadows())
            {
                if (LightType::POINT == type_)
                    defines += "CUBESHADOWMAP\n";
                else
                    defines += "SHADOWMAP\n";
            }
        }
    }

    SignalLight::PSignal Light::SignalBeingDestroy()
    {
        static SignalLight::PSignal sig(new SignalLight);
        return sig;
    }

    bool Light::IsVisible(const SceneNode* node) const
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
                        auto frustum = GetFrustum();
                        visible = frustum->IsVisible(*node, *mesh);
                        break;
                    }
                case LightType::POINT:
                    {
                        auto& bb = node->GetWorldBoundingBox();
                        Sphere sphereLight(GetGlobalPosition(), distance_);
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

    bool Light::IsVisibleFromCurrentLightFace(const SceneNode* node) const
    {
        CHECK_ASSERT(type_ == LightType::POINT, __FILE__, __LINE__);
        auto mesh = node->GetMesh();
        auto material = node->GetMaterial();
        if (!mesh || !mesh->IsReady() || !material || !material->IsReady() || material->IsShadeless())
            return false;
        return GetFrustum()->IsVisible(*node, *mesh);
    }

    void Light::CalculateColor()
    {
        diffuseColor_ = diffuse_ ? color_ * energy_ : COLOR_BLACK;
        specularColor_ = specular_ ? color_ * energy_ : COLOR_BLACK;
    }

    void Light::SetDistance(float distance)
    {
        distance_ = distance;
        invRange_ = 1.0f / std::max(distance_, glm::epsilon<float>());
        OnDirty();
        SetUniformsNeedUpdate();
    }

    void Light::OnDirty() const
    {
        viewProjectionDirty_ = true;
        if (LightType::DIRECTIONAL != type_)
            frustumDirty_ = true;
    }

    void Light::SetCurrentCubeShadowMapFace(TextureTarget target)
    {
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
                SetGlobalOrientation(dirNegativeY_.GetOrientation());
                break;
            default:
                CHECK_ASSERT(!"Incorrect cube shadowmap face!!!", __FILE__, __LINE__);
                break;
        }
    }

    void Light::UpdateViewProjection() const
    {
        viewProjectionDirty_ = false;
        matView_ = glm::inverse(GetGlobalModelMatrix());

        if (LightType::SPOT == type_)
            matProjection_ = glm::perspective(glm::radians(spotCutOff_), 1.f, 0.001f, distance_);
        else if (LightType::POINT == type_)
            matProjection_ = glm::perspective(glm::radians(90.f), 1.f, 0.001f, distance_);
        else
        {
            auto left = -width_ * 0.5f;
            auto right = width_ * 0.5f;
            auto bottom = -height_ * 0.5f;
            auto top = height_ * 0.5f;
            auto nearP = 0.f;
            auto farP = distance_;

            matProjection_ = glm::ortho(left, right, bottom, top, nearP, farP);

        }

        matViewProjection_ = matProjection_ * matView_;
        SetUniformsNeedUpdate();
    }

    void Light::UpdateFrustum() const
    {
        frustumDirty_ = false;
        auto tmp = std::make_shared<Frustum>(GetMatViewProj());
        frustum_.swap(tmp);
        SetUniformsNeedUpdate();
    }

    const Matrix4& Light::GetMatViewProj() const
    {
        auto window = Renderer::GetPtr()->GetWindow();
        auto width = window->GetWidth();
        auto height = window->GetHeight();
        if (width_ != width || height_ != height)
        {
            width_ = width;
            height_ = height;
            viewProjectionDirty_ = true;
        }
        if (viewProjectionDirty_)
            UpdateViewProjection();
        return matViewProjection_;
    }

    PFrustum Light::GetFrustum() const
    {
        if (frustumDirty_)
            UpdateFrustum();
        return frustum_;
    }

    bool Light::DoShadows() const
    {
        return shadows_;
    }

    PTexture Light::GetShadowMap() const
    {
        return shadowFrameBuffer_->GetColorTexture();
    }
}