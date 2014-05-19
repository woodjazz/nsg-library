/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 Néstor Silveira Gorski

-------------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/

#include <stdlib.h>
#include <stdio.h>
#if !defined(__APPLE__)
#include <malloc.h>
#endif
#include <assert.h>
#include <cstring>
#include <string>
#include <algorithm>
#include "GLES2Program.h"
#include "Log.h"
#include "Check.h"
#include "Context.h"
#include "ExtraUniforms.h"
#include "GLES2Light.h"
#include "GLES2Camera.h"
#include "Scene.h"

static const char s_fragmentShaderHeader[] = {
#include "GLES2FragmentCompatibility.h"
};

static const char s_vertexShaderHeader[] = {
#include "GLES2VertexCompatibility.h"
};

namespace NSG 
{
	GLES2Program::GLES2Program(PResource pRVShader, PResource pRFShader)
	: pRVShader_(pRVShader),
	pRFShader_(pRFShader),
    pExtraUniforms_(nullptr),
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
	hasLights_(false)
	{
		CHECK_ASSERT(pRVShader && pRFShader, __FILE__, __LINE__);

		Context::this_->Add(this);
	}

	GLES2Program::GLES2Program(const char* vShader, const char* fShader)
	: pRVShader_(new Resource(vShader, strlen(vShader))),
	pRFShader_(new Resource(fShader, strlen(fShader))),
    vShader_(vShader),
    fShader_(fShader),
    pExtraUniforms_(nullptr),
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
	hasLights_(false)
	{
		CHECK_ASSERT(vShader && fShader, __FILE__, __LINE__);
		Context::this_->Add(this);
	}

	bool GLES2Program::IsValid()
	{
		return pRVShader_->IsLoaded() && pRFShader_->IsLoaded();
	}

	void GLES2Program::AllocateResources()
	{
        lightsLoc_.resize(MAX_LIGHTS);
        memset(&lightsLoc_[0], 0, sizeof(lightsLoc_) * MAX_LIGHTS);

		std::string vbuffer;
		size_t vHeaderSize = strlen(s_vertexShaderHeader);
		size_t fHeaderSize = strlen(s_fragmentShaderHeader);
		vbuffer.resize(vHeaderSize + pRVShader_->GetBytes());
		vbuffer = s_vertexShaderHeader;
		memcpy(&vbuffer[0] + vHeaderSize, pRVShader_->GetData(), pRVShader_->GetBytes());
		CHECK_GL_STATUS(__FILE__, __LINE__);
		pVShader_ = PGLES2VShader(new GLES2VShader(vbuffer.c_str()));
		CHECK_GL_STATUS(__FILE__, __LINE__);
		vbuffer = s_fragmentShaderHeader;
		vbuffer.resize(fHeaderSize + pRFShader_->GetBytes());
		memcpy(&vbuffer[0] + fHeaderSize, pRFShader_->GetData(), pRFShader_->GetBytes());
		CHECK_GL_STATUS(__FILE__, __LINE__);
		pFShader_ = PGLES2FShader(new GLES2FShader(vbuffer.c_str()));
		CHECK_GL_STATUS(__FILE__, __LINE__);
	    if(Initialize())
	    {
		    CHECK_GL_STATUS(__FILE__, __LINE__);

		    if(pExtraUniforms_)
		    {
			    pExtraUniforms_->SetLocations();
		    }

		    color_ambient_loc_ = GetUniformLocation("u_material.ambient");
	        color_scene_ambient_loc_ = GetUniformLocation("u_scene_ambient");
		    color_diffuse_loc_ = GetUniformLocation("u_material.diffuse");
		    color_specular_loc_ = GetUniformLocation("u_material.specular");
		    shininess_loc_ = GetUniformLocation("u_material.shininess");
	        mvp_loc_ = GetUniformLocation("u_mvp");
	        m_loc_ = GetUniformLocation("u_m");
		    model_inv_transp_loc_ = GetUniformLocation("u_model_inv_transp");
		    v_inv_loc_ = GetUniformLocation("u_v_inv");
		    texture0_loc_ = GetUniformLocation("u_texture0");
		    texture1_loc_ = GetUniformLocation("u_texture1");
		    texcoord_loc_ = GetAttributeLocation("a_texcoord");
		    position_loc_ = GetAttributeLocation("a_position");
		    normal_loc_ = GetAttributeLocation("a_normal");
	        color_loc_ = GetAttributeLocation("a_color");

			hasLights_ = false;

	        const GLES2Light::Lights& ligths = GLES2Light::GetLights();
	        size_t n = std::min(ligths.size(), MAX_LIGHTS);

		    for(size_t i=0; i<n; i++)
		    {
			    std::stringstream u_light_index;
			    u_light_index << "u_light" << i << ".";

			    lightsLoc_[i].type_loc = GetUniformLocation(u_light_index.str() + "type");
			    
			    if(lightsLoc_[i].type_loc == -1)
			    {
			    	break;
			    }

			    lightsLoc_[i].position_loc = GetUniformLocation(u_light_index.str() + "position");
			    lightsLoc_[i].diffuse_loc = GetUniformLocation(u_light_index.str() + "diffuse");
			    lightsLoc_[i].specular_loc = GetUniformLocation(u_light_index.str() + "specular");
			    lightsLoc_[i].constantAttenuation_loc = GetUniformLocation(u_light_index.str() + "constantAttenuation");
			    lightsLoc_[i].linearAttenuation_loc = GetUniformLocation(u_light_index.str() + "linearAttenuation");
			    lightsLoc_[i].quadraticAttenuation_loc = GetUniformLocation(u_light_index.str() + "quadraticAttenuation");
			    lightsLoc_[i].spotCutoff_loc = GetUniformLocation(u_light_index.str() + "spotCutoff");
			    lightsLoc_[i].spotExponent_loc = GetUniformLocation(u_light_index.str() + "spotExponent");
			    lightsLoc_[i].spotDirection_loc = GetUniformLocation(u_light_index.str() + "spotDirection");
		    }

	        if(lightsLoc_[0].type_loc != -1)
	        {
	            hasLights_ = true;
	        }

		    CHECK_GL_STATUS(__FILE__, __LINE__);
	    }
	}

	void GLES2Program::ReleaseResources()
	{
		glDetachShader(id_, pVShader_->GetId());
		glDetachShader(id_, pFShader_->GetId());
		pVShader_ = nullptr;
		pFShader_ = nullptr;
        glDeleteProgram(id_);
	}

	bool GLES2Program::Initialize()
	{
        CHECK_GL_STATUS(__FILE__, __LINE__);

		// Creates the program name/index.
		id_ = glCreateProgram();

		// Will attach the fragment and vertex shaders to the program object.
		glAttachShader(id_, pVShader_->GetId());
		glAttachShader(id_, pFShader_->GetId());

		// Will link the program into OpenGL core.
		glLinkProgram(id_);

		GLint link_status = GL_FALSE;

		glGetProgramiv(id_, GL_LINK_STATUS, &link_status);

		if (link_status != GL_TRUE) 
		{
			GLint logLength = 0;

			// Instead use GL_INFO_LOG_LENGTH we could use COMPILE_STATUS.
			// I prefer to take the info log length, because it'll be 0 if the
			// shader was successful compiled. If we use COMPILE_STATUS
			// we will need to take info log length in case of a fail anyway.
			glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &logLength);

			if (logLength > 0)
			{
				// Allocates the necessary memory to retrieve the message.
				GLchar *log = (GLchar *)malloc(logLength);

				// Get the info log message.
				glGetProgramInfoLog(id_, logLength, &logLength, log);

				TRACE_LOG("Error in Program Creation: " << log);

				// Frees the allocated memory.
				free(log);

                CHECK_ASSERT(false && "Error in program(shader) creation", __FILE__, __LINE__);
			}

			return false;
		}

        CHECK_GL_STATUS(__FILE__, __LINE__);

        return true;
	}

	GLES2Program::~GLES2Program()
	{
		Context::this_->Remove(this);
	}

	GLuint GLES2Program::GetAttributeLocation(const std::string& name) 
	{
		return glGetAttribLocation(id_, name.c_str());
	}

	GLuint GLES2Program::GetUniformLocation(const std::string& name)
	{
		return glGetUniformLocation(id_, name.c_str());
	}

	void GLES2Program::Render(bool solid, GLES2Mesh* pMesh)
	{
		pMesh->Render(solid, position_loc_, texcoord_loc_, normal_loc_, color_loc_);
	}

	UseProgram::UseProgram(GLES2Program& obj, GLES2Material* material, Node* node)
	: obj_(obj)
	{
		obj.Use();

		if(obj.pExtraUniforms_)
		{
			obj.pExtraUniforms_->AssignValues();
		}

		if(node)
		{
			if(obj.mvp_loc_ != -1)
			{
				Matrix4 m = GLES2Camera::GetModelViewProjection(node);
				glUniformMatrix4fv(obj.mvp_loc_, 1, GL_FALSE, glm::value_ptr(m));
			}

			if(obj.m_loc_ != -1)
			{
				const Matrix4& m = node->GetGlobalModelMatrix();
				glUniformMatrix4fv(obj.m_loc_, 1, GL_FALSE, glm::value_ptr(m));
			}

			if(obj.model_inv_transp_loc_ != -1)
			{
				const Matrix3& m = node->GetGlobalModelInvTranspMatrix();
				glUniformMatrix3fv(obj.model_inv_transp_loc_, 1, GL_FALSE, glm::value_ptr(m));			
			}
		}

		if(obj.v_inv_loc_ != -1)
		{
			const Matrix4& m = GLES2Camera::GetInverseViewMatrix();
			glUniformMatrix4fv(obj.v_inv_loc_, 1, GL_FALSE, glm::value_ptr(m));			
		}

        if(material)
        {
		    if(obj.texture0_loc_ != -1 && material->pTexture0_)
		    {
			    glActiveTexture(GL_TEXTURE0);
			    material->pTexture0_->Bind();
			    glUniform1i(obj.texture0_loc_, 0);
		    }

		    if(obj.texture1_loc_ != -1 && material->pTexture1_)
		    {
			    glActiveTexture(GL_TEXTURE1);
			    material->pTexture1_->Bind();
			    glUniform1i(obj.texture1_loc_, 1);
		    }

		    if(obj.color_ambient_loc_ != -1)
		    {
			    glUniform4fv(obj.color_ambient_loc_, 1, &material->ambient_[0]);
		    }

		    if(obj.color_scene_ambient_loc_ != -1)
		    {
			    glUniform4fv(obj.color_scene_ambient_loc_, 1, &Scene::ambient[0]);
		    }

		    if(obj.color_diffuse_loc_ != -1)
		    {
			    glUniform4fv(obj.color_diffuse_loc_, 1, &material->diffuse_[0]);
		    }

		    if(obj.color_specular_loc_ != -1)
		    {
			    glUniform4fv(obj.color_specular_loc_, 1, &material->specular_[0]);
		    }

		    if(obj.shininess_loc_ != -1)
		    {
			    glUniform1f(obj.shininess_loc_, material->shininess_);
		    }
        }

        if(obj.hasLights_)
        {
		    const GLES2Light::Lights& ligths = GLES2Light::GetLights();
		
		    size_t n = std::min(ligths.size(), MAX_LIGHTS);

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

	UseProgram::~UseProgram()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}

}
