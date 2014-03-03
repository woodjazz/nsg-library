#include "GLES2Light.h"
#include "Log.h"
#include "Resource.h"
#include "GLES2Program.h"
#include "GLES2Material.h"
#include <assert.h>
#include <algorithm>

namespace NSG
{
	static GLES2Light::Lights s_Lights;

	GLES2Light::GLES2Light() 
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

		PResource pVResource(new Resource("shaders/LightSimple.vert"));
		PResource pFResource(new Resource("shaders/LightSimple.frag"));
		PGLES2Program pProgram(new GLES2Program(pVResource, pFResource));
		PGLES2Material pMaterial = PGLES2Material(new GLES2Material (nullptr, pProgram));

		pMesh_ = PGLES2BoxMesh(new GLES2BoxMesh(0.2f,0.2f,0.2f, 2,2,2, pMaterial, GL_STATIC_DRAW));
	}

	GLES2Light::~GLES2Light()
	{
		auto it = std::find(s_Lights.begin(), s_Lights.end(), this);
		assert(it != s_Lights.end());
		s_Lights.erase(it);
	}

	const GLES2Light::Lights& GLES2Light::GetLights()
	{
		return s_Lights;
	}

	void GLES2Light::SetAttenuation(float constant, float linear, float quadratic)
	{
		attenuation_.constant = constant;
        attenuation_.linear = linear;
        attenuation_.quadratic = quadratic;
	}

	void GLES2Light::SetPoint()
	{
		type_ = POINT;
	}

	void GLES2Light::SetDirectional()
	{
		type_ = DIRECTIONAL;
	}

	void GLES2Light::SetSpotLight(float spotCutOff, float exponent)
	{
		type_ = SPOT;
		spotCutOff_ = spotCutOff;
		exponent_ = exponent;
	}

	void GLES2Light::Render()
	{
		pMesh_->Render(this);
	}
}