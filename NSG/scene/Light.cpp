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
          shadowColor_(0, 0, 0, 1),
          distance_(30),
          invRange_(1.f / distance_),
          shadows_(true),
          shadowClipStart_(0.1f), // same minimum as blender
          shadowClipEnd_(30.f), // same as distance_
          onlyShadow_(false),
          width_(0),
          height_(0),
          shadowBias_(0.000125f)
    {
        FrameBuffer::Flags flags((unsigned int)(FrameBuffer::COLOR | FrameBuffer::COLOR_USE_TEXTURE | FrameBuffer::COLOR_CUBE_TEXTURE | FrameBuffer::DEPTH));
        shadowFrameBuffer_ = std::make_shared<FrameBuffer>(GetUniqueName("LightCubeFrameBuffer"), flags);
        CalculateInvRange();
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

    void Light::SetShadowColor(Color color)
    {
        if (shadowColor_ != color)
        {
            shadowColor_ = color;
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
            CalculateInvRange();
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
        node.append_attribute("shadowClipStart").set_value(shadowClipStart_);
        node.append_attribute("shadowClipEnd").set_value(shadowClipEnd_);
        node.append_attribute("onlyShadow").set_value(onlyShadow_);
        node.append_attribute("shadowColor").set_value(ToString(shadowColor_).c_str());
        node.append_attribute("shadowBias").set_value(shadowBias_);


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
        SetShadowClipStart(node.attribute("shadowClipStart").as_float());
        SetShadowClipEnd(node.attribute("shadowClipEnd").as_float());
        SetOnlyShadow(node.attribute("onlyShadow").as_bool());
        SetShadowColor(ToVertex4(node.attribute("shadowColor").as_string()));
        SetBias(node.attribute("shadowBias").as_float());
        LoadChildren(node);
    }

    void Light::FillShaderDefines(std::string& defines, PassType passType, Material* material) const
    {
        bool litPass =  PassType::LIT == passType;
        if (litPass)
        {
            if (LightType::POINT == type_)
                defines += "HAS_POINT_LIGHT\n";
            else if (LightType::DIRECTIONAL == type_)
                defines += "HAS_DIRECTIONAL_LIGHT\n";
            else
            {
                defines += "HAS_SPOT_LIGHT\n";
            }

            if (DoShadows() && material->ReceiveShadows())
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

    void Light::CalculateColor()
    {
        diffuseColor_ = diffuse_ ? color_ * energy_ : COLOR_BLACK;
        specularColor_ = specular_ ? color_ * energy_ : COLOR_BLACK;
    }

    void Light::SetDistance(float distance)
    {
        distance_ = distance;
        CalculateInvRange();
        SetUniformsNeedUpdate();
    }

    bool Light::DoShadows() const
    {
        return shadows_;
    }

    PTexture Light::GetShadowMap() const
    {
        return shadowFrameBuffer_->GetColorTexture();
    }

    void Light::CalculateInvRange()
    {
        if (LightType::SPOT == type_)
        {
            auto range = glm::clamp((shadowClipEnd_ - shadowClipStart_), 0.f, distance_);
            invRange_ = 1.f / std::max(range, glm::epsilon<float>());
        }
        else if (LightType::POINT == type_)
            invRange_ = 1.f / std::max(distance_, glm::epsilon<float>());
        else
            invRange_ = glm::epsilon<float>();

    }

    void Light::SetShadowClipStart(float value)
    {
        shadowClipStart_ = value;
        CalculateInvRange();
    }

    void Light::SetShadowClipEnd(float value)
    {
        shadowClipEnd_ = value;
        CalculateInvRange();
    }

}