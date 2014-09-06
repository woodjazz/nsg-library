#include "Light.h"
#include "Log.h"
#include "Check.h"
#include "Constants.h"
#include "Resource.h"
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
          spotCutOff_(PI/4),
          exponent_(20)
    {
        attenuation_.constant = 1;
        attenuation_.linear = 0;
        attenuation_.quadratic = 0;
    }

    Light::~Light()
    {
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

    void Light::SetSpotLight(float spotCutOff, float exponent)
    {
        if ( type_ != SPOT || spotCutOff_ != spotCutOff || exponent_ != exponent)
        {
            type_ = SPOT;
            spotCutOff_ = spotCutOff;
            exponent_ = exponent;
            SetUniformsNeedUpdate();
        }
    }

    void Light::Save(pugi::xml_node& node)
    {

    }

    void Light::Load(const pugi::xml_node& node)
    {

    }

}