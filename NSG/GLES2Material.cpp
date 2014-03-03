#include "GLES2Material.h"
#include "Log.h"
#include "GLES2Camera.h"
#include "GLES2Light.h"
#include "Scene.h"
#include <assert.h>
#include <sstream>
#include <algorithm>

namespace NSG
{
	const size_t MAX_LIGHTS_MATERIAL = 4;

	GLES2Material::GLES2Material(PGLES2Texture pTexture, PGLES2Program pProgram) 
	: pTexture_(pTexture), 
	pProgram_(pProgram),
	color_scene_ambient_loc_(-1),
	color_ambient_loc_(-1),
	color_diffuse_loc_(-1),
	color_specular_loc_(-1),
	shininess_loc_(-1),
	texture_loc_(-1),
	texcoord_loc_(-1),
	position_loc_(-1),
	normal_loc_(-1),
	model_inv_transp_loc_(-1),
	v_inv_loc_(-1),
    mvp_loc_(-1),
    m_loc_(-1),
	loaded_(false),
	ambient_(1,1,1,1),
	diffuse_(1,1,1,1),
	specular_(1,1,1,1),
	shininess_(1)
	{
        assert(glGetError() == GL_NO_ERROR);
        lightsLoc_.resize(MAX_LIGHTS_MATERIAL);
        memset(&lightsLoc_[0], 0, sizeof(lightsLoc_) * MAX_LIGHTS_MATERIAL);
	}

	GLES2Material::~GLES2Material()
	{
	}

	bool GLES2Material::IsReady()
	{
		if(!loaded_ && pProgram_->IsReady() && (!pTexture_ || pTexture_->IsReady()))
		{
			assert(glGetError() == GL_NO_ERROR);

			color_ambient_loc_ = pProgram_->GetUniformLocation("u_material.ambient");
            color_scene_ambient_loc_ = pProgram_->GetUniformLocation("u_scene_ambient");
			color_diffuse_loc_ = pProgram_->GetUniformLocation("u_material.diffuse");
			color_specular_loc_ = pProgram_->GetUniformLocation("u_material.specular");
			shininess_loc_ = pProgram_->GetUniformLocation("u_material.shininess");
		    mvp_loc_ = pProgram_->GetUniformLocation("u_mvp");
		    m_loc_ = pProgram_->GetUniformLocation("u_m");
			model_inv_transp_loc_ = pProgram_->GetUniformLocation("u_model_inv_transp");
			v_inv_loc_ = pProgram_->GetUniformLocation("u_v_inv");
			texture_loc_ = pProgram_->GetUniformLocation("u_texture");
			texcoord_loc_ = pProgram_->GetAttributeLocation("a_texcoord");
			position_loc_ = pProgram_->GetAttributeLocation("a_position");
			normal_loc_ = pProgram_->GetAttributeLocation("a_normal");

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
			}

			assert(glGetError() == GL_NO_ERROR);

			loaded_ = true;
		}

		return loaded_;
	}

	UseMaterial::UseMaterial(GLES2Material& obj, Node* pNode)
	: obj_(obj),
	useProgram_(*obj.pProgram_)
	{
		if(obj.mvp_loc_ != -1)
		{
			Matrix4&& m = GLES2Camera::GetModelViewProjection(pNode);
			glUniformMatrix4fv(obj.mvp_loc_, 1, GL_FALSE, glm::value_ptr(m));
		}

		if(obj.m_loc_ != -1)
		{
			const Matrix4& m = pNode->GetModelMatrix();
			glUniformMatrix4fv(obj.m_loc_, 1, GL_FALSE, glm::value_ptr(m));
		}

		if(obj.model_inv_transp_loc_ != -1)
		{
			const Matrix3& m = pNode->GetModelInvTranspMatrix();
			glUniformMatrix3fv(obj.model_inv_transp_loc_, 1, GL_FALSE, glm::value_ptr(m));			
		}

		if(obj.v_inv_loc_ != -1)
		{
			const Matrix4& m = GLES2Camera::GetInverseViewMatrix();
			glUniformMatrix4fv(obj.v_inv_loc_, 1, GL_FALSE, glm::value_ptr(m));			
		}

		if(obj.texture_loc_ != -1 && obj.pTexture_)
		{
			glActiveTexture(GL_TEXTURE0);
			obj.pTexture_->Bind();
			glUniform1i(obj.texture_loc_, 0);
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
					const Vertex3& direction = ligths[i]->GetDirection();
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
					const Vertex3& direction = ligths[i]->GetDirection();
					glUniform3fv(obj.lightsLoc_[i].spotDirection_loc, 1, &direction[0]);
				}
			}
		}
	}

	UseMaterial::~UseMaterial()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}