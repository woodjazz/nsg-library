#include "Light.h"
#include "Log.h"
#include "Resource.h"
#include <assert.h>
#include <algorithm>

namespace NSG
{
	static Light::Lights s_Lights;

	Light::Light() 
	: type_(POINT),
	diffuse_(1,1,1,1),
	specular_(1,1,1,1),
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
		assert(it != s_Lights.end());
		s_Lights.erase(it);
	}

	const Light::Lights& Light::GetLights()
	{
		return s_Lights;
	}

	void Light::SetAttenuation(float constant, float linear, float quadratic)
	{
		attenuation_.constant = constant;
        attenuation_.linear = linear;
        attenuation_.quadratic = quadratic;
	}

	void Light::SetPoint()
	{
		type_ = POINT;
	}

	void Light::SetDirectional()
	{
		type_ = DIRECTIONAL;
	}

	void Light::SetSpotLight(float spotCutOff, float exponent)
	{
		type_ = SPOT;
		spotCutOff_ = spotCutOff;
		exponent_ = exponent;
	}
}