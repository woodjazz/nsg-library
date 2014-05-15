#include "GLES2Material.h"
#include "Log.h"
#include "Check.h"
#include "GLES2Camera.h"
#include "GLES2Light.h"
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
	const size_t MAX_LIGHTS_MATERIAL = 4;

	GLES2Material::GLES2Material() 
	: pProgram_(new GLES2Program(vShader, fShader)),
    pExtraMaterialUniforms_(nullptr),
	color_scene_ambient_loc_(-1),
	color_ambient_loc_(-1),
	color_diffuse_loc_(-1),
	color_specular_loc_(-1),
	shininess_loc_(-1),
	texture0_loc_(-1),
	texture1_loc_(-1),
	texcoord_loc_(-1),
	position_loc_(-1),
	normal_loc_(-1),
    color_loc_(-1),
	model_inv_transp_loc_(-1),
	v_inv_loc_(-1),
    mvp_loc_(-1),
    m_loc_(-1),
	ambient_(1,1,1,1),
	diffuse_(1,1,1,1),
	specular_(1,1,1,1),
	shininess_(1),
    hasLights_(false),
    blendMode_(ALPHA),
    enableDepthTest_(true),
    enableCullFace_(false)
	{
        CHECK_GL_STATUS(__FILE__, __LINE__);
        lightsLoc_.resize(MAX_LIGHTS_MATERIAL);
        memset(&lightsLoc_[0], 0, sizeof(lightsLoc_) * MAX_LIGHTS_MATERIAL);

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
        return pProgram_->IsReady() && (!pTexture0_ || pTexture0_->IsReady()) && (!pTexture1_ || pTexture1_->IsReady());
	}

	void GLES2Material::AllocateResources()
	{
	    CHECK_GL_STATUS(__FILE__, __LINE__);

	    if(!pTexture0_)
	    {
	        // Creates 1x1 white texture 
			char img[3];
			memset(&img[0], 0xFF, sizeof(unsigned char)*3);
	        pTexture0_ = PGLES2Texture(new GLES2Texture(GL_RGB, 1, 1, &img[0]));
	    }

	    if(pExtraMaterialUniforms_)
	    {
		    pExtraMaterialUniforms_->SetLocations();
	    }

	    color_ambient_loc_ = pProgram_->GetUniformLocation("u_material.ambient");
        color_scene_ambient_loc_ = pProgram_->GetUniformLocation("u_scene_ambient");
	    color_diffuse_loc_ = pProgram_->GetUniformLocation("u_material.diffuse");
	    color_specular_loc_ = pProgram_->GetUniformLocation("u_material.specular");
	    shininess_loc_ = pProgram_->GetUniformLocation("u_material.shininess");
        mvp_loc_ = pProgram_->GetUniformLocation("u_mvp");
        m_loc_ = pProgram_->GetUniformLocation("u_m");
	    model_inv_transp_loc_ = pProgram_->GetUniformLocation("u_model_inv_transp");
	    v_inv_loc_ = pProgram_->GetUniformLocation("u_v_inv");
	    texture0_loc_ = pProgram_->GetUniformLocation("u_texture0");
	    texture1_loc_ = pProgram_->GetUniformLocation("u_texture1");
	    texcoord_loc_ = pProgram_->GetAttributeLocation("a_texcoord");
	    position_loc_ = pProgram_->GetAttributeLocation("a_position");
	    normal_loc_ = pProgram_->GetAttributeLocation("a_normal");
        color_loc_ = pProgram_->GetAttributeLocation("a_color");

        const GLES2Light::Lights& ligths = GLES2Light::GetLights();
        size_t n = std::min(ligths.size(), MAX_LIGHTS_MATERIAL);

	    for(size_t i=0; i<n; i++)
	    {
		    std::stringstream u_light_index;
		    u_light_index << "u_light" << i << ".";

		    lightsLoc_[i].type_loc = pProgram_->GetUniformLocation(u_light_index.str() + "type");
		    lightsLoc_[i].position_loc = pProgram_->GetUniformLocation(u_light_index.str() + "position");
		    lightsLoc_[i].diffuse_loc = pProgram_->GetUniformLocation(u_light_index.str() + "diffuse");
		    lightsLoc_[i].specular_loc = pProgram_->GetUniformLocation(u_light_index.str() + "specular");
		    lightsLoc_[i].constantAttenuation_loc = pProgram_->GetUniformLocation(u_light_index.str() + "constantAttenuation");
		    lightsLoc_[i].linearAttenuation_loc = pProgram_->GetUniformLocation(u_light_index.str() + "linearAttenuation");
		    lightsLoc_[i].quadraticAttenuation_loc = pProgram_->GetUniformLocation(u_light_index.str() + "quadraticAttenuation");
		    lightsLoc_[i].spotCutoff_loc = pProgram_->GetUniformLocation(u_light_index.str() + "spotCutoff");
		    lightsLoc_[i].spotExponent_loc = pProgram_->GetUniformLocation(u_light_index.str() + "spotExponent");
		    lightsLoc_[i].spotDirection_loc = pProgram_->GetUniformLocation(u_light_index.str() + "spotDirection");

            if(lightsLoc_[i].type_loc != -1)
            {
                hasLights_ = true;
            }
	    }

	    CHECK_GL_STATUS(__FILE__, __LINE__);
	}

	void GLES2Material::ReleaseResources()
	{
	}

	void GLES2Material::Render(bool solid, Node* pNode, GLES2Mesh* pMesh)
	{
		if(IsReady())
		{
			UseMaterial useMaterial(*this, pNode);

			GLenum mode = solid ? pMesh->GetSolidDrawMode() : pMesh->GetWireFrameDrawMode();

	        pMesh->Render(mode, position_loc_, texcoord_loc_, normal_loc_, color_loc_);
	    }
	}

	UseMaterial::UseMaterial(GLES2Material& obj, Node* pNode)
	: obj_(obj),
	useProgram_(*obj.pProgram_)
	{
		CHECK_ASSERT(obj.IsReady(), __FILE__, __LINE__);

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

		if(obj.pExtraMaterialUniforms_)
		{
			obj.pExtraMaterialUniforms_->AssignValues();
		}
        

		if(obj.mvp_loc_ != -1 && pNode)
		{
			Matrix4 m = GLES2Camera::GetModelViewProjection(pNode);
			glUniformMatrix4fv(obj.mvp_loc_, 1, GL_FALSE, glm::value_ptr(m));
		}

		if(obj.m_loc_ != -1 && pNode)
		{
			const Matrix4& m = pNode->GetGlobalModelMatrix();
			glUniformMatrix4fv(obj.m_loc_, 1, GL_FALSE, glm::value_ptr(m));
		}

		if(obj.model_inv_transp_loc_ != -1 && pNode)
		{
			const Matrix3& m = pNode->GetGlobalModelInvTranspMatrix();
			glUniformMatrix3fv(obj.model_inv_transp_loc_, 1, GL_FALSE, glm::value_ptr(m));			
		}

		if(obj.v_inv_loc_ != -1)
		{
			const Matrix4& m = GLES2Camera::GetInverseViewMatrix();
			glUniformMatrix4fv(obj.v_inv_loc_, 1, GL_FALSE, glm::value_ptr(m));			
		}

		if(obj.texture0_loc_ != -1 && obj.pTexture0_)
		{
			glActiveTexture(GL_TEXTURE0);
			obj.pTexture0_->Bind();
			glUniform1i(obj.texture0_loc_, 0);
		}

		if(obj.texture1_loc_ != -1 && obj.pTexture1_)
		{
			glActiveTexture(GL_TEXTURE1);
			obj.pTexture1_->Bind();
			glUniform1i(obj.texture1_loc_, 1);
		}

		
		if(obj.color_ambient_loc_ != -1)
		{
			glUniform4fv(obj.color_ambient_loc_, 1, &obj.ambient_[0]);
		}

		if(obj.color_scene_ambient_loc_ != -1)
		{
			glUniform4fv(obj.color_scene_ambient_loc_, 1, &Scene::ambient[0]);
		}

		if(obj.color_diffuse_loc_ != -1)
		{
			glUniform4fv(obj.color_diffuse_loc_, 1, &obj.diffuse_[0]);
		}

		if(obj.color_specular_loc_ != -1)
		{
			glUniform4fv(obj.color_specular_loc_, 1, &obj.specular_[0]);
		}

		if(obj.shininess_loc_ != -1)
		{
			glUniform1f(obj.shininess_loc_, obj.shininess_);
		}

        if(obj.hasLights_)
        {
		    const GLES2Light::Lights& ligths = GLES2Light::GetLights();
		
		    size_t n = std::min(ligths.size(), MAX_LIGHTS_MATERIAL);

		    for(size_t i=0; i<n; i++)
		    {
			    GLint type = ligths[i]->GetType();

			    if(obj.lightsLoc_[i].type_loc != -1)
			    {
				    glUniform1i(obj.lightsLoc_[i].type_loc, type);
			    }

			    if(obj.lightsLoc_[i].position_loc != -1)
			    {
				    if(type == GLES2Light::DIRECTIONAL)
				    {
					    const Vertex3& direction = ligths[i]->GetLookAtDirection();
					    glUniform3fv(obj.lightsLoc_[i].position_loc, 1, &direction[0]);
				    }
				    else
				    {
					    const Vertex3& position = ligths[i]->GetGlobalPosition();
					    glUniform3fv(obj.lightsLoc_[i].position_loc, 1, &position[0]);
				    }
			    }
			
			    if(obj.lightsLoc_[i].diffuse_loc != -1)
			    {
				    const Color& diffuse = ligths[i]->GetDiffuseColor();
				    glUniform4fv(obj.lightsLoc_[i].diffuse_loc, 1, &diffuse[0]);
			    }

			    if(obj.lightsLoc_[i].specular_loc != -1)
			    {
				    const Color& specular = ligths[i]->GetSpecularColor();
				    glUniform4fv(obj.lightsLoc_[i].specular_loc, 1, &specular[0]);
			    }

			    if(obj.lightsLoc_[i].constantAttenuation_loc != -1)
			    {
				    const GLES2Light::Attenuation& attenuation = ligths[i]->GetAttenuation();
				    glUniform1f(obj.lightsLoc_[i].constantAttenuation_loc, attenuation.constant);
			    }

			    if(obj.lightsLoc_[i].linearAttenuation_loc != -1)
			    {
				    const GLES2Light::Attenuation& attenuation = ligths[i]->GetAttenuation();
				    glUniform1f(obj.lightsLoc_[i].linearAttenuation_loc, attenuation.linear);
			    }

			    if(obj.lightsLoc_[i].quadraticAttenuation_loc != -1)
			    {
				    const GLES2Light::Attenuation& attenuation = ligths[i]->GetAttenuation();
				    glUniform1f(obj.lightsLoc_[i].quadraticAttenuation_loc, attenuation.quadratic);
			    }

			    if(type == GLES2Light::SPOT)
			    {
				    if(obj.lightsLoc_[i].spotCutoff_loc != -1)
				    {
					    float cutOff = ligths[i]->GetSpotCutOff();
					    glUniform1f(obj.lightsLoc_[i].spotCutoff_loc, cutOff);
				    }

				    if(obj.lightsLoc_[i].spotExponent_loc != -1)
				    {
					    float exponent = ligths[i]->GetSpotExponent();
					    glUniform1f(obj.lightsLoc_[i].spotExponent_loc, exponent);
				    }

				    if(obj.lightsLoc_[i].spotDirection_loc != -1)
				    {
					    const Vertex3& direction = ligths[i]->GetLookAtDirection();
					    glUniform3fv(obj.lightsLoc_[i].spotDirection_loc, 1, &direction[0]);
				    }
			    }
		    }
        }
	}

	UseMaterial::~UseMaterial()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}