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

#include "Program.h"
#include "Texture.h"
#include "Check.h"
#include "Context.h"
#include "ExtraUniforms.h"
#include "Light.h"
#include "Camera.h"
#include "Mesh.h"
#include "Scene.h"
#include "ResourceMemory.h"
#include "Material.h"
#include "Graphics.h"
#include <stdlib.h>
#include <stdio.h>
#if !defined(__APPLE__)
#include <malloc.h>
#endif
#include <assert.h>
#include <cstring>
#include <string>
#include <algorithm>

static const char s_fragmentShaderHeader[] =
{
#include "FragmentCompatibility.h"
};

static const char s_vertexShaderHeader[] =
{
#include "VertexCompatibility.h"
};

namespace NSG
{
    Program::Program(PResource pRVShader, PResource pRFShader)
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
          color_loc_(-1),
          att_texcoord_loc_(-1),
          att_position_loc_(-1),
          att_normal_loc_(-1),
          att_color_loc_(-1),
          model_inv_transp_loc_(-1),
          v_inv_loc_(-1),
          mvp_loc_(-1),
          m_loc_(-1),
          numOfLights_loc_(-1),
          hasLights_(false),
          nLights_(0),
          activeCamera_(nullptr),
          activeMaterial_(nullptr),
          activeNode_(nullptr)
    {
        CHECK_ASSERT(pRVShader &&  pRFShader, __FILE__, __LINE__);
    }

    Program::Program(const char* vShader, const char* fShader)
        : pRVShader_(new ResourceMemory(vShader, strlen(vShader))),
          pRFShader_(new ResourceMemory(fShader, strlen(fShader))),
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
          color_loc_(-1),
          att_texcoord_loc_(-1),
          att_position_loc_(-1),
          att_normal_loc_(-1),
          att_color_loc_(-1),
          model_inv_transp_loc_(-1),
          v_inv_loc_(-1),
          mvp_loc_(-1),
          m_loc_(-1),
          numOfLights_loc_(-1),
          hasLights_(false),
          nLights_(0),
          activeCamera_(nullptr),
          activeMaterial_(nullptr),
          activeNode_(nullptr)
    {
        CHECK_ASSERT(vShader && fShader, __FILE__, __LINE__);
    }

    bool Program::IsValid()
    {
        return pRVShader_->IsLoaded() && pRFShader_->IsLoaded();
    }

    void Program::AllocateResources()
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
        pVShader_ = PVertexShader(new VertexShader(vbuffer.c_str()));
        CHECK_GL_STATUS(__FILE__, __LINE__);
        vbuffer = s_fragmentShaderHeader;
        vbuffer.resize(fHeaderSize + pRFShader_->GetBytes());
        memcpy(&vbuffer[0] + fHeaderSize, pRFShader_->GetData(), pRFShader_->GetBytes());
        CHECK_GL_STATUS(__FILE__, __LINE__);
        pFShader_ = PFragmentShader(new FragmentShader(vbuffer.c_str()));
        CHECK_GL_STATUS(__FILE__, __LINE__);
        if (Initialize())
        {
            CHECK_GL_STATUS(__FILE__, __LINE__);

            if (pExtraUniforms_)
                pExtraUniforms_->SetLocations();

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
            color_loc_ = GetUniformLocation("u_color");

            att_position_loc_ = GetAttributeLocation("a_position");
            att_normal_loc_ = GetAttributeLocation("a_normal");
            att_texcoord_loc_ = GetAttributeLocation("a_texcoord");
            att_color_loc_ = GetAttributeLocation("a_color");

            hasLights_ = false;

            const Light::Lights& ligths = Light::GetLights();
            size_t n = std::min(ligths.size(), MAX_LIGHTS);

            numOfLights_loc_ = GetUniformLocation("u_numOfLights");

            for (size_t i = 0; i < n; i++)
            {
                std::stringstream u_light_index;
                u_light_index << "u_light" << i << ".";

                lightsLoc_[i].type_loc = GetUniformLocation(u_light_index.str() + "type");

                if (lightsLoc_[i].type_loc == -1)
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

            if (lightsLoc_[0].type_loc != -1)
            {
                hasLights_ = true;
            }

            CHECK_GL_STATUS(__FILE__, __LINE__);
        }
    }

    void Program::ReleaseResources()
    {
        glDetachShader(id_, pVShader_->GetId());
        glDetachShader(id_, pFShader_->GetId());
        pVShader_ = nullptr;
        pFShader_ = nullptr;
        glDeleteProgram(id_);

        if (Graphics::this_->GetProgram() == this)
            Graphics::this_->SetProgram(nullptr);
    }

    bool Program::Initialize()
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
                GLchar* log = (GLchar*)malloc(logLength);

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

    Program::~Program()
    {
        Context::RemoveObject(this);
    }

    GLuint Program::GetAttributeLocation(const std::string& name)
    {
        return glGetAttribLocation(id_, name.c_str());
    }

    GLuint Program::GetUniformLocation(const std::string& name)
    {
        return glGetUniformLocation(id_, name.c_str());
    }

    void Program::Use(Node* node)
    {
        if (node)
        {
            if (mvp_loc_ != -1)
            {
                Matrix4 m = Camera::GetModelViewProjection(node);
                glUniformMatrix4fv(mvp_loc_, 1, GL_FALSE, glm::value_ptr(m));
            }

            if (m_loc_ != -1)
            {
                const Matrix4& m = node->GetGlobalModelMatrix();
                glUniformMatrix4fv(m_loc_, 1, GL_FALSE, glm::value_ptr(m));
            }

            if (model_inv_transp_loc_ != -1)
            {
                const Matrix3& m = node->GetGlobalModelInvTranspMatrix();
                glUniformMatrix3fv(model_inv_transp_loc_, 1, GL_FALSE, glm::value_ptr(m));
            }

            node->ClearUniformsNeedUpdate();
        }
    }

    void Program::Use(Material* material)
    {
        if (material)
        {
            if (texture0_loc_ != -1)
            {
                Graphics::this_->SetTexture(0, material->pTexture0_.get());
                glUniform1i(texture0_loc_, 0);
            }

            if (texture1_loc_ != -1)
            {
                Graphics::this_->SetTexture(1, material->pTexture1_.get());
                glUniform1i(texture1_loc_, 1);
            }

            if (color_loc_ != -1)
            {
                glUniform4fv(color_loc_, 1, &material->color_[0]);
            }

            if (color_ambient_loc_ != -1)
            {
                glUniform4fv(color_ambient_loc_, 1, &material->ambient_[0]);
            }

            if (color_diffuse_loc_ != -1)
            {
                glUniform4fv(color_diffuse_loc_, 1, &material->diffuse_[0]);
            }

            if (color_specular_loc_ != -1)
            {
                glUniform4fv(color_specular_loc_, 1, &material->specular_[0]);
            }

            if (shininess_loc_ != -1)
            {
                glUniform1f(shininess_loc_, material->shininess_);
            }

            material->ClearUniformsNeedUpdate();
        }
    }

    bool Program::Use(Material* material, Node* node)
    {
        bool programChanged = Graphics::this_->SetProgram(this);

        if (activeMaterial_ != material || (material && material->UniformsNeedUpdate()) || programChanged)
        {
            activeMaterial_ = material;
            Use(material);
        }

        if (activeNode_ != node || (node && node->UniformsNeedUpdate()) || programChanged)
        {
            activeNode_ = node;
            Use(node);
        }

        if (pExtraUniforms_)
        {
            pExtraUniforms_->AssignValues();
        }

        if (v_inv_loc_ != -1)
        {
            Camera* camera = Camera::GetActiveCamera();

            if (camera)
            {
                if (activeCamera_ != camera || camera->UniformsNeedUpdate() || programChanged)
                {
                    activeCamera_ = camera;
                    glUniformMatrix4fv(v_inv_loc_, 1, GL_FALSE, glm::value_ptr(Camera::GetActiveCamera()->GetInverseViewMatrix()));
                    camera->ClearUniformsNeedUpdate();
                }
            }
            else if (activeCamera_ != camera || programChanged)
            {
                activeCamera_ = nullptr;
                glUniformMatrix4fv(v_inv_loc_, 1, GL_FALSE, glm::value_ptr(IDENTITY_MATRIX));
            }
        }

        if (hasLights_)
        {
            const Light::Lights& ligths = Light::GetLights();

            size_t n = std::min(ligths.size(), MAX_LIGHTS);

            if (nLights_ != n || programChanged)
            {
                glUniform1i(numOfLights_loc_, n);
                nLights_ = n;
            }

            for (size_t i = 0; i < n; i++)
            {
                Light* light = ligths[i];

                if (light->UniformsNeedUpdate() || programChanged)
                {
                    light->ClearUniformsNeedUpdate();

                    GLint type = light->GetType();

                    if (lightsLoc_[i].type_loc != -1)
                    {
                        glUniform1i(lightsLoc_[i].type_loc, type);
                    }

                    if (lightsLoc_[i].position_loc != -1)
                    {
                        if (type == Light::DIRECTIONAL)
                        {
                            const Vertex3& direction = light->GetLookAtDirection();
                            glUniform3fv(lightsLoc_[i].position_loc, 1, &direction[0]);
                        }
                        else
                        {
                            const Vertex3& position = light->GetGlobalPosition();
                            glUniform3fv(lightsLoc_[i].position_loc, 1, &position[0]);
                        }
                    }

                    if (lightsLoc_[i].diffuse_loc != -1)
                    {
                        const Color& diffuse = light->GetDiffuseColor();
                        glUniform4fv(lightsLoc_[i].diffuse_loc, 1, &diffuse[0]);
                    }

                    if (lightsLoc_[i].specular_loc != -1)
                    {
                        const Color& specular = light->GetSpecularColor();
                        glUniform4fv(lightsLoc_[i].specular_loc, 1, &specular[0]);
                    }

                    if (lightsLoc_[i].constantAttenuation_loc != -1)
                    {
                        const Light::Attenuation& attenuation = light->GetAttenuation();
                        glUniform1f(lightsLoc_[i].constantAttenuation_loc, attenuation.constant);
                    }

                    if (lightsLoc_[i].linearAttenuation_loc != -1)
                    {
                        const Light::Attenuation& attenuation = light->GetAttenuation();
                        glUniform1f(lightsLoc_[i].linearAttenuation_loc, attenuation.linear);
                    }

                    if (lightsLoc_[i].quadraticAttenuation_loc != -1)
                    {
                        const Light::Attenuation& attenuation = light->GetAttenuation();
                        glUniform1f(lightsLoc_[i].quadraticAttenuation_loc, attenuation.quadratic);
                    }

                    if (type == Light::SPOT)
                    {
                        if (lightsLoc_[i].spotCutoff_loc != -1)
                        {
                            float cutOff = ligths[i]->GetSpotCutOff();
                            glUniform1f(lightsLoc_[i].spotCutoff_loc, cutOff);
                        }

                        if (lightsLoc_[i].spotExponent_loc != -1)
                        {
                            float exponent = light->GetSpotExponent();
                            glUniform1f(lightsLoc_[i].spotExponent_loc, exponent);
                        }

                        if (lightsLoc_[i].spotDirection_loc != -1)
                        {
                            const Vertex3& direction = light->GetLookAtDirection();
                            glUniform3fv(lightsLoc_[i].spotDirection_loc, 1, &direction[0]);
                        }
                    }
                }
            }
        }

        return programChanged;
    }
}
