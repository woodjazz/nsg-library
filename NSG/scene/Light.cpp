#include "Light.h"
#include "Log.h"
#include "Check.h"
#include "Constants.h"
#include "Resource.h"
#include "Material.h"
#include "Util.h"
#include "Scene.h"
#include "Frustum.h"
#include "Sphere.h"
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
          invRange_(1.f/distance_),
          frustumDirty_(false)
    {
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
            type_ = type;
            OnDirty();
            //SetUniformsNeedUpdate();
        }
    }

    void Light::Save(pugi::xml_node& node) const
    {
        node.append_attribute("name").set_value(GetName().c_str());
        node.append_attribute("nodeType").set_value("Light");
        node.append_attribute("type").set_value((int)type_);
        node.append_attribute("energy").set_value(energy_);
        node.append_attribute("color").set_value(ToString(color_).c_str());
        node.append_attribute("diffuse").set_value(diffuse_);
        node.append_attribute("specular").set_value(specular_);
        node.append_attribute("spotCutOff").set_value(spotCutOff_);
        node.append_attribute("position").set_value(ToString(GetPosition()).c_str());
        node.append_attribute("orientation").set_value(ToString(GetOrientation()).c_str());
        node.append_attribute("distance").set_value(distance_);
        SaveChildren(node);
    }

    void Light::Load(const pugi::xml_node& node)
    {
        name_ = node.attribute("name").as_string();
        LightType type = (LightType)node.attribute("type").as_int();
        switch (type)
        {
            case LightType::DIRECTIONAL:
                SetType(LightType::DIRECTIONAL);
                break;
            case LightType::SPOT:
                SetType(LightType::SPOT);
                break;
            case LightType::POINT:
                SetType(LightType::POINT);
                break;
            default:
                CHECK_ASSERT(!"Unkwnown light type!", __FILE__, __LINE__);
                break;
        }
        SetEnergy(node.attribute("energy").as_float());
        SetColor(GetVertex4(node.attribute("color").as_string()));
        EnableDiffuseColor(node.attribute("diffuse").as_bool());
        EnableSpecularColor(node.attribute("specular").as_bool());
        SetSpotCutOff(node.attribute("spotCutOff").as_float());
        Vertex3 position = GetVertex3(node.attribute("position").as_string());
        SetPosition(position);
        Quaternion orientation = GetQuaternion(node.attribute("orientation").as_string());
        SetOrientation(orientation);
        SetDistance(node.attribute("distance").as_float());
        LoadChildren(node);
    }

    void Light::FillShaderDefines(std::string& defines)
    {
        if (LightType::POINT == type_)
            defines += "HAS_POINT_LIGHT\n";
        else if (LightType::DIRECTIONAL == type_)
            defines += "HAS_DIRECTIONAL_LIGHT\n";
        else
            defines += "HAS_SPOT_LIGHT\n";
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
    }

    void Light::OnDirty() const
    {
        if (LightType::SPOT == type_)
            frustumDirty_ = true;
    }

    void Light::UpdateFrustum() const
    {
        frustumDirty_ = false;
        matView_ = glm::inverse(GetGlobalModelMatrix());
        matProjection_ = glm::perspective(glm::radians(spotCutOff_), 1.f, 0.001f, distance_);
        matViewProjection_ = matProjection_ * matView_;
        auto tmp = std::make_shared<Frustum>(matViewProjection_);
        frustum_.swap(tmp);
        SetUniformsNeedUpdate();
    }

    PFrustum Light::GetFrustum() const
    {
        if (frustumDirty_)
            UpdateFrustum();
        return frustum_;
    }
}