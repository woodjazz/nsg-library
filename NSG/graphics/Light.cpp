#include "Light.h"
#include "Log.h"
#include "Check.h"
#include "Constants.h"
#include "Resource.h"
#include "Util.h"
#include "pugixml.hpp"
#include <assert.h>
#include <algorithm>

namespace NSG
{
    Light::Light(const std::string& name, Scene* scene)
        : SceneNode(name, scene),
          type_(POINT),
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

    void Light::SetPoint()
    {
        if (type_ != POINT)
        {
            type_ = POINT;
            SetUniformsNeedUpdate();
        }
    }

    void Light::SetDirectional()
    {
        if (type_ != DIRECTIONAL)
        {
            type_ = DIRECTIONAL;
            SetUniformsNeedUpdate();
        }
    }

    void Light::SetSpotLight(float spotCutOff)
    {
        if ( type_ != SPOT || spotCutOff_ != spotCutOff)
        {
            type_ = SPOT;
            spotCutOff_ = spotCutOff;
            SetUniformsNeedUpdate();
        }
    }

	void Light::SetType(Type type)
	{
		if (type_ != type)
		{
			type_ = type;
			SetUniformsNeedUpdate();
		}
	}

    void Light::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Light");

        {
            std::stringstream ss;
            ss << GetName();
            child.append_attribute("name") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << type_;
            child.append_attribute("type") = ss.str().c_str();
        }

        {
            pugi::xml_node attenuationChild = child.append_child("attenuation");

            {
                std::stringstream ss;
                ss << attenuation_.constant;
                attenuationChild.append_attribute("constant") = ss.str().c_str();
            }

            {
                std::stringstream ss;
                ss << attenuation_.linear;
                attenuationChild.append_attribute("linear") = ss.str().c_str();
            }

            {
                std::stringstream ss;
                ss << attenuation_.quadratic;
                attenuationChild.append_attribute("quadratic") = ss.str().c_str();
            }
        }

        {
            std::stringstream ss;
            ss << spotCutOff_;
            child.append_attribute("spotCutOff") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << ambient_;
            child.append_attribute("ambient") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << diffuse_;
            child.append_attribute("diffuse") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << specular_;
            child.append_attribute("specular") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << GetPosition();
            child.append_attribute("position") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << GetOrientation();
            child.append_attribute("orientation") = ss.str().c_str();
        }

    }

    void Light::Load(const pugi::xml_node& node)
    {
        SetName(node.attribute("name").as_string());

        int type = node.attribute("type").as_int();

        switch(type)
        {
            case DIRECTIONAL:
                SetDirectional();
                break;
            case SPOT:
            {
                float cutOff = node.attribute("spotCutOff").as_float();
                SetSpotLight(cutOff);
                break;
            }
            case POINT:
                SetPoint();
                break;
            default:
                CHECK_ASSERT(false && "Missing light type!", __FILE__, __LINE__);
                SetPoint();
                break;
        }

        {
            pugi::xml_node attenuationChild = node.child("attenuation");
            float constant = attenuationChild.attribute("constant").as_float();
            float linear = attenuationChild.attribute("linear").as_float();
            float quadratic = attenuationChild.attribute("quadratic").as_float();
            SetAttenuation(constant, linear, quadratic);
        }

        SetAmbientColor(GetVertex4(node.attribute("ambient").as_string()));
        SetDiffuseColor(GetVertex4(node.attribute("diffuse").as_string()));
        SetSpecularColor(GetVertex4(node.attribute("specular").as_string()));

        Vertex3 position = GetVertex3(node.attribute("position").as_string());
        SetPosition(position);

        Quaternion orientation = GetQuaternion(node.attribute("orientation").as_string());
        SetOrientation(orientation);
    }

}