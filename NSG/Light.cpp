#include "Light.h"
#include "Log.h"
#include "Resource.h"
#include "GLES2Program.h"
#include "GLES2Material.h"
#include <assert.h>
#include <algorithm>

namespace NSG
{
	static Light::Lights s_lights;

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

		s_lights.push_back(this);

		PResource pVResource(new Resource("shaders/LightSimple.vert"));
		PResource pFResource(new Resource("shaders/LightSimple.frag"));
		PGLES2Program pProgram(new GLES2Program(pVResource, pFResource));
		PGLES2Material pMaterial = PGLES2Material(new GLES2Material (nullptr, pProgram));

		pMesh_ = PBoxMesh(new BoxMesh(Color(1,0,0,1), 0.2f,0.2f,0.2f, 2,2,2, pMaterial, GL_STATIC_DRAW));
	}

	Light::~Light()
	{
		auto it = std::find(s_lights.begin(), s_lights.end(), this);
		assert(it != s_lights.end());
		s_lights.erase(it);
	}

	const Light::Lights& Light::GetLights()
	{
		return s_lights;
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

	void Light::SetSpotlight(float spotCutOff, float exponent)
	{
		type_ = SPOT;
		spotCutOff_ = spotCutOff;
		exponent_ = exponent;
	}

	void Light::Render()
	{
		pMesh_->Render(this);
	}
}