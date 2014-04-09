#include "GLES2Light.h"
#include "Log.h"
#include "Resource.h"
#include "GLES2Program.h"
#include "GLES2Material.h"
#include <assert.h>
#include <algorithm>

#define STRINGIFY(S) #S

static const char* vShader = STRINGIFY(
	uniform mat4 u_mvp;
	attribute vec4 a_position;
	void main()
	{
		gl_Position = u_mvp * a_position;
	}
);

static const char* fShader = STRINGIFY(
	void main()
	{
		gl_FragColor = vec4(1,1,0,1);
	}
);

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

		PGLES2Program pProgram(new GLES2Program(vShader, fShader));
		PGLES2Material pMaterial = PGLES2Material(new GLES2Material());
        pMaterial->SetProgram(pProgram);

		pMesh_ = PGLES2BoxMesh(new GLES2BoxMesh(0.2f,0.2f,0.2f, 2,2,2, GL_STATIC_DRAW));
        pMesh_->SetMaterial(pMaterial);
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