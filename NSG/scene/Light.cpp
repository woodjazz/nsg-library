#include "Light.h"
#include "Log.h"
#include "Check.h"
#include "Constants.h"
#include "Resource.h"
#include "Util.h"
#include "Scene.h"
#include "pugixml.hpp"
#include <assert.h>
#include <algorithm>
#include <sstream>

namespace NSG
{
    Light::Light(const std::string& name)
        : SceneNode(name),
          type_(LightType::POINT),
          diffuse_(1, 1, 1, 1),
          specular_(1, 1, 1, 1),
          spotCutOff_(45)
    {
        attenuation_.constant = 1;
        attenuation_.linear = 0;
        attenuation_.quadratic = 0;
    }

    Light::~Light()
    {
        SignalBeingDestroy()->Run(this);
    }

    void Light::SetAmbientColor(Color ambient)
    {
        if (ambient_ != ambient)
        {
            ambient_ = ambient;
            SetUniformsNeedUpdate();
        }
    }

    void Light::SetDiffuseColor(Color diffuse)
    {
        if (diffuse_ != diffuse)
        {
            diffuse_ = diffuse;
            SetUniformsNeedUpdate();
        }
    }

    void Light::SetSpecularColor(Color specular)
    {
        if (specular_ != specular)
        {
            specular_ = specular;
            SetUniformsNeedUpdate();
        }
    }


    void Light::SetAttenuation(float constant, float linear, float quadratic)
    {
        if (attenuation_.constant != constant || attenuation_.linear != linear || attenuation_.quadratic != quadratic)
        {
            attenuation_.constant = constant;
            attenuation_.linear = linear;
            attenuation_.quadratic = quadratic;
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
            type_ = type;
            //SetUniformsNeedUpdate();
        }
    }

    void Light::Save(pugi::xml_node& node) const
    {
        node.append_attribute("name").set_value(GetName().c_str());
        node.append_attribute("nodeType").set_value("Light");
        node.append_attribute("type").set_value((int)type_);
        node.append_attribute("attenuationConstant").set_value(attenuation_.constant);
        node.append_attribute("attenuationLinear").set_value(attenuation_.linear);
        node.append_attribute("attenuationQuadratic").set_value(attenuation_.quadratic);
        node.append_attribute("spotCutOff").set_value(spotCutOff_);
        node.append_attribute("ambient").set_value(ToString(ambient_).c_str());
        node.append_attribute("diffuse").set_value(ToString(diffuse_).c_str());
        node.append_attribute("specular").set_value(ToString(specular_).c_str());
        node.append_attribute("position").set_value(ToString(GetPosition()).c_str());
        node.append_attribute("orientation").set_value(ToString(GetOrientation()).c_str());
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
                {
                    float cutOff = node.attribute("spotCutOff").as_float();
                    SetSpotCutOff(cutOff);
                    SetType(LightType::SPOT);
                    break;
                }
            case LightType::POINT:
                SetType(LightType::POINT);
                break;
            default:
                CHECK_ASSERT(false && "Missing light type!", __FILE__, __LINE__);
                break;
        }

        {
            float constant = node.attribute("attenuationConstant").as_float();
            float linear = node.attribute("attenuationLinear").as_float();
            float quadratic = node.attribute("attenuationQuadratic").as_float();
            SetAttenuation(constant, linear, quadratic);
        }

        SetAmbientColor(GetVertex4(node.attribute("ambient").as_string()));
        SetDiffuseColor(GetVertex4(node.attribute("diffuse").as_string()));
        SetSpecularColor(GetVertex4(node.attribute("specular").as_string()));

        Vertex3 position = GetVertex3(node.attribute("position").as_string());
        SetPosition(position);

        Quaternion orientation = GetQuaternion(node.attribute("orientation").as_string());
        SetOrientation(orientation);

        LoadChildren(node);
    }

    void Light::FillShaderDefines(std::string& defines)
    {
        if (LightType::POINT == type_)
            defines += "#define HAS_POINT_LIGHT\n";
        else if (LightType::DIRECTIONAL == type_)
            defines += "#define HAS_DIRECTIONAL_LIGHT\n";
        else
            defines += "#define HAS_SPOT_LIGHT\n";
    }

    SignalLight::PSignal Light::SignalBeingDestroy()
    {
        static SignalLight::PSignal sig(new SignalLight);
        return sig;
    }
}