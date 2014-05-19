#include "GLES2Material.h"
#include "Log.h"
#include "Check.h"
#include "GLES2Camera.h"
#include "Context.h"
#include "Scene.h"
#include <assert.h>
#include <sstream>
#include <algorithm>

static const char* vShader = STRINGIFY(
	struct LightSource
	{
		int type;
		vec3 position;
		vec4 diffuse;
		vec4 specular;
		float constantAttenuation;
        float linearAttenuation;
        float quadraticAttenuation;
		float spotCutoff;
        float spotExponent;
		vec3 spotDirection;
	};

	struct Material
	{
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		float shininess;
	};

	const int POINT_LIGHT = 0;
	const int DIRECTIONAL_LIGHT = 1;
	const int SPOT_LIGHT = 2;
    
	uniform LightSource u_light0;
	uniform Material u_material;
	uniform mat4 u_m;
    uniform mat4 u_mvp;
	uniform mat3 u_model_inv_transp;
	uniform mat4 u_v_inv;
	uniform vec4 u_scene_ambient;
	attribute vec2 a_texcoord;
	attribute vec4 a_position;
	attribute vec3 a_normal;
	attribute vec4 a_color;
	varying vec4 v_color;
    varying vec4 v_vertex_color;
	varying vec2 v_texcoord;

	void main()
	{
		vec3 normalDirection = normalize(u_model_inv_transp * a_normal);

		vec3 viewDirection = normalize(vec3(u_v_inv * vec4(0.0, 0.0, 0.0, 1.0) - u_m * a_position));

		vec3 lightDirection;
		
		float attenuation;
		
		if(u_light0.type == DIRECTIONAL_LIGHT)
		{
			// directional light
			attenuation = 1.0; // no attenuation
			lightDirection = normalize(u_light0.position);
		}
		else
		{
			// point or spot light (or other kind of light)
			vec3 vertexToLightSource = u_light0.position - vec3(u_m * a_position);
			float distance = length(vertexToLightSource);
			lightDirection = normalize(vertexToLightSource);
			attenuation = 1.0 / (u_light0.constantAttenuation + u_light0.linearAttenuation * distance + u_light0.quadraticAttenuation * distance * distance);

			if (u_light0.type == SPOT_LIGHT)
			{
				// spotlight
				float clampedCosine = max(0.0, dot(-lightDirection, normalize(u_light0.spotDirection)));
				
				if(clampedCosine < cos(u_light0.spotCutoff * 3.14159 / 180.0))
				{
					// outside of spotlight cone
					attenuation = 0.0;
				}
				else
				{
					attenuation = attenuation * pow(clampedCosine, u_light0.spotExponent);
				}
			}
		}
		
		vec3 ambientLighting = vec3(u_scene_ambient) * vec3(u_material.ambient);

		vec3 diffuseReflection = attenuation * vec3(u_light0.diffuse) * vec3(u_material.diffuse) * max(0.0, dot(normalDirection, lightDirection));

		vec3 specularReflection;

		if (dot(normalDirection, lightDirection) < 0.0) 
		{
			// light source on the wrong side => no specular reflection
			specularReflection = vec3(0.0, 0.0, 0.0);
		}
		else 
		{
			// light source on the right side
			specularReflection = attenuation * vec3(u_light0.specular) * vec3(u_material.specular) 
			* pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), u_material.shininess);
		}	

		v_color = vec4(ambientLighting + diffuseReflection + specularReflection, u_material.diffuse.w);
		gl_Position = u_mvp * a_position;
		v_texcoord = a_texcoord;
		v_vertex_color = a_color;
	}
);

static const char* fShader = STRINGIFY(
	varying vec4 v_vertex_color;
	varying vec4 v_color;
	varying vec2 v_texcoord;
	uniform sampler2D u_texture0;
	void main()
	{
		gl_FragColor = v_color * texture2D(u_texture0, v_texcoord) * v_vertex_color;
	}
);

namespace NSG
{
	GLES2Material::GLES2Material() 
	: pProgram_(new GLES2Program(vShader, fShader)),
	ambient_(1,1,1,1),
	diffuse_(1,1,1,1),
	specular_(1,1,1,1),
	shininess_(1),
    blendMode_(ALPHA),
    enableDepthTest_(true),
    enableCullFace_(false)
	{
        Context::this_->Add(this);

	}

	GLES2Material::~GLES2Material()
	{
        Context::this_->Remove(this);
	}

	void GLES2Material::SetBlendMode(BLEND_MODE mode)
	{
		blendMode_ = mode;
	}

	void GLES2Material::EnableDepthTest(bool enable)
	{
		enableDepthTest_ = enable;
	}

	void GLES2Material::EnableCullFace(bool enable)
	{
		enableCullFace_ = enable;
	}

	void GLES2Material::SetProgram(PGLES2Program pProgram)
	{
		if(pProgram_ != pProgram)
		{
			pProgram_ = pProgram;
            Invalidate();

		}
	}

	void GLES2Material::SetTexture0(PGLES2Texture pTexture)
	{
        if(pTexture0_ != pTexture)
        {
	    	pTexture0_ = pTexture;
            Invalidate();
        }
	}

	void GLES2Material::SetTexture1(PGLES2Texture pTexture)
	{
        if(pTexture1_ != pTexture)
        {
		    pTexture1_ = pTexture;
            Invalidate();
        }
	}

	bool GLES2Material::IsValid()
	{
        bool isReady = pProgram_->IsReady();
        if(isReady)
        {
            if(pTexture0_)
            {
                isReady = pTexture0_->IsReady();
            }
            else
            {
                pTexture0_ = Context::this_->whiteTexture_;
                isReady = pTexture0_->IsReady();
            }

            if(isReady && pTexture1_)
            {
                isReady = pTexture1_->IsReady();
            }
        }

        return isReady;
	}

	void GLES2Material::AllocateResources()
	{
	}

	void GLES2Material::ReleaseResources()
	{
	}

	void GLES2Material::Render(bool solid, Node* pNode, GLES2Mesh* pMesh)
	{
		if(IsReady())
		{
			UseMaterial useMaterial(*this, pNode);

			pProgram_->Render(solid, pMesh);
	    }
	}

	UseMaterial::UseMaterial(GLES2Material& obj, Node* pNode)
	: obj_(obj),
	useProgram_(*obj.pProgram_, &obj, pNode)
	{
		if(obj.enableCullFace_)
		{
			glEnable(GL_CULL_FACE);
			//glCullFace(GL_FRONT);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

        switch(obj.blendMode_)
       	{
        	case NONE:
	        	glDisable(GL_BLEND);
	        	break;

	        case ALPHA:
	        	glEnable(GL_BLEND);
	        	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	        	break;

	        default:
	        	CHECK_ASSERT(false && "Undefined blend mode", __FILE__, __LINE__);
	        	break;
        }
        
        if(obj.enableDepthTest_)
        {
        	glEnable(GL_DEPTH_TEST);
        }
        else
        {
        	glDisable(GL_DEPTH_TEST);
        }
	}

	UseMaterial::~UseMaterial()
	{
	}
}