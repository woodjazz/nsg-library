#include "Light.h"
#include "Log.h"
#include "Check.h"
#include "Resource.h"
#include <assert.h>
#include <algorithm>

namespace NSG
{
    static Light::Lights s_Lights;

    Light::Light()
        : type_(POINT),
          diffuse_(1, 1, 1, 1),
          specular_(1, 1, 1, 1),
          spotCutOff_(45),
          exponent_(20)
    {
        attenuation_.constant = 1;
        attenuation_.linear = 0;
        attenuation_.quadratic = 0;

        s_Lights.push_back(this);
    }

    Light::~Light()
    {
        auto it = std::find(s_Lights.begin(), s_Lights.end(), this);
        CHECK_ASSERT(it != s_Lights.end(), __FILE__, __LINE__);
        s_Lights.erase(it);
    }

    const Light::Lights& Light::GetLights()
    {
        return s_Lights;
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
}