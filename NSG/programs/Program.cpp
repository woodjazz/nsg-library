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
#include "VertexShader.h"
#include "FragmentShader.h"
#include "Texture.h"
#include "Check.h"
#include "Context.h"
#include "ExtraUniforms.h"
#include "Light.h"
#include "Camera.h"
#include "Mesh.h"
#include "Scene.h"
#include "Skeleton.h"
#include "ResourceMemory.h"
#include "Material.h"
#include "Graphics.h"
#include "Constants.h"
#include "App.h"
#include "pugixml.hpp"
#include "autogenerated/Common_glsl.inl"
#include "autogenerated/Transforms_glsl.inl"
#include "autogenerated/Lighting_glsl.inl"
#include "autogenerated/PostProcess_glsl.inl"
#include "autogenerated/VS_glsl.inl"
#include "autogenerated/FS_glsl.inl"
#include <stdlib.h>
#include <stdio.h>
#if !defined(__APPLE__)
#include <malloc.h>
#endif
#include <assert.h>
#include <cstring>
#include <string>
#include <algorithm>

namespace NSG
{
    Program::Program(const std::string& name)
        : flags_((int)ProgramFlag::NONE),
          id_(0),
          pExtraUniforms_(nullptr),
          att_texcoordLoc0_(-1),
          att_texcoordLoc1_(-1),
          att_positionLoc_(-1),
          att_normalLoc_(-1),
          att_colorLoc_(-1),
          att_tangentLoc_(-1),
          att_bonesIDLoc_(-1),
          att_bonesWeightLoc_(-1),
          att_modelMatrixRow0Loc_(-1),
          att_normalMatrixCol0Loc_(-1),
          modelLoc_(-1),
          normalMatrixLoc_(-1),
          viewLoc_(-1),
          viewProjectionLoc_(-1),
          sceneColorAmbientLoc_(-1),
          eyeWorldPosLoc_(-1),
          texture0Loc_(-1),
          texture1Loc_(-1),
          texture2Loc_(-1),
          nBones_(0),
          nDirectionalLights_(0),
          nPointLights_(0),
          nSpotLights_(0),
          activeCamera_(nullptr),
          neverUsed_(true),
          activeMaterial_(nullptr),
          activeSkeleton_(nullptr),
          activeNode_(nullptr),
          activeScene_(nullptr),
          sceneColor_(-1),
          name_(name),
          graphics_(*Graphics::this_)
    {
        memset(&materialLoc_, -1, sizeof(materialLoc_));
    }

    void Program::SetVertexShader(PResource resource)
    {
        vertexShader_ = resource;
    }

    void Program::SetFragmentShader(PResource resource)
    {
        fragmentShader_ = resource;
    }

    bool Program::IsValid()
    {
        return (!vertexShader_ || vertexShader_->IsLoaded()) && (!fragmentShader_ || fragmentShader_->IsLoaded());
    }

    void Program::AllocateResources()
    {
        std::string preDefines;

#ifdef GL_ES_VERSION_2_0
        preDefines = "#version 100\n#define GLES2\n";
#else
        preDefines = "#version 120\n";
#endif

        if (graphics_.HasInstancedArrays())
        {
            preDefines += "#define INSTANCED\n";
        }

        bool hasLights = false;

        PScene scene = App::this_->GetCurrentScene();

        CHECK_ASSERT(scene, __FILE__, __LINE__);

        if (nBones_)
        {
            std::stringstream ss;
            ss << "const int NUM_BONES = " << nBones_ << ";\n";
            preDefines += ss.str();
            flags_ |= (int)ProgramFlag::SKINNED;
            preDefines += "#define SKINNED\n";
        }

        const Scene::Lights& directionalLigths = scene->GetLights(Light::DIRECTIONAL);
        nDirectionalLights_ = directionalLigths.size();

        activeDirectionalLights_ = std::vector<Light*>(nDirectionalLights_, nullptr);

        const Scene::Lights& pointLigths = scene->GetLights(Light::POINT);
        nPointLights_ = pointLigths.size();

        activePointLights_ = std::vector<Light*>(nPointLights_, nullptr);

        const Scene::Lights& spotLigths = scene->GetLights(Light::SPOT);
        nSpotLights_ = spotLigths.size();

        activeSpotLights_ = std::vector<Light*>(nSpotLights_, nullptr);


        if (nDirectionalLights_ || nPointLights_ || nSpotLights_)
            hasLights = true;

        {
            std::stringstream ss;
            ss << "const int NUM_DIRECTIONAL_LIGHTS = " << nDirectionalLights_ << ";\n";
            preDefines += ss.str();
        }

        {
            std::stringstream ss;
            ss << "const int NUM_POINT_LIGHTS = " << nPointLights_ << ";\n";
            preDefines += ss.str();
        }

        {
            std::stringstream ss;
            ss << "const int NUM_SPOT_LIGHTS = " << nSpotLights_ << ";\n";
            preDefines += ss.str();
        }


        {
            std::stringstream ss;
            ss << "const int NUM_DIRECTIONAL_LIGHTS_ARRAY = ";
            if (nDirectionalLights_)
                ss << nDirectionalLights_ << ";\n";
            else
                ss << "1;\n";
            preDefines += ss.str();
        }

        {
            std::stringstream ss;
            ss << "const int NUM_POINT_LIGHTS_ARRAY = ";
            if (nPointLights_)
                ss << nPointLights_ << ";\n";
            else
                ss << "1;\n";
            preDefines += ss.str();
        }

        {
            std::stringstream ss;
            ss << "const int NUM_SPOT_LIGHTS_ARRAY = ";
            if (nSpotLights_)
                ss << nSpotLights_ << ";\n";
            else
                ss << "1;\n";
            preDefines += ss.str();
        }

        ///////////////////////////////////////////////////


        if ((int)ProgramFlag::PER_VERTEX_LIGHTING & flags_ && hasLights)
            preDefines += "#define PER_VERTEX_LIGHTING\n";
        else if ((int)ProgramFlag::PER_PIXEL_LIGHTING & flags_ && hasLights)
            preDefines += "#define PER_PIXEL_LIGHTING\n";
        else if ((int)ProgramFlag::BLEND & flags_)
            preDefines += "#define BLEND\n";
        else if ((int)ProgramFlag::BLUR & flags_)
            preDefines += "#define BLUR\n";
        else if ((int)ProgramFlag::TEXT & flags_)
            preDefines += "#define TEXT\n";
        else if ((int)ProgramFlag::SHOW_TEXTURE0 & flags_)
            preDefines += "#define SHOW_TEXTURE0\n";
        else if ((int)ProgramFlag::STENCIL & flags_)
            preDefines += "#define STENCIL\n";
        else if ((int)ProgramFlag::UNLIT & flags_)
            preDefines += "#define UNLIT\n";

        if ((int)ProgramFlag::NORMALMAP & flags_)
            preDefines += "#define NORMALMAP\n";

        if ((int)ProgramFlag::LIGHTMAP & flags_)
            preDefines += "#define LIGHTMAP\n";

        if (vertexShader_)
            preDefines += "#define HAS_USER_VERTEX_SHADER\n";

        if (fragmentShader_)
            preDefines += "#define HAS_USER_FRAGMENT_SHADER\n";

        std::string buffer = preDefines + "#define COMPILEVS\n";
        buffer += COMMON_GLSL + TRANSFORMS_GLSL + LIGHTING_GLSL + VS_GLSL;
        if (vertexShader_)
        {
            size_t bufferSize = buffer.size();
            buffer.resize(bufferSize + vertexShader_->GetBytes());
            memcpy(&buffer[0] + bufferSize, vertexShader_->GetData(), vertexShader_->GetBytes());
        }

        pVShader_ = PVertexShader(new VertexShader(buffer.c_str()));

        buffer = preDefines  + "#define COMPILEFS\n";
        buffer += COMMON_GLSL + TRANSFORMS_GLSL + LIGHTING_GLSL + POSTPROCESS_GLSL + FS_GLSL;
        if (fragmentShader_)
        {
            size_t bufferSize = buffer.size();
            buffer.resize(bufferSize + fragmentShader_->GetBytes());
            memcpy(&buffer[0] + bufferSize, fragmentShader_->GetData(), fragmentShader_->GetBytes());
        }

        pFShader_ = PFragmentShader(new FragmentShader(buffer.c_str()));

        CHECK_GL_STATUS(__FILE__, __LINE__);

        if (Initialize())
        {
            CHECK_GL_STATUS(__FILE__, __LINE__);

            if (pExtraUniforms_)
                pExtraUniforms_->SetLocations();

            att_positionLoc_ = GetAttributeLocation("a_position");
            att_normalLoc_ = GetAttributeLocation("a_normal");
            att_texcoordLoc0_ = GetAttributeLocation("a_texcoord0");
            att_texcoordLoc1_ = GetAttributeLocation("a_texcoord1");
            att_colorLoc_ = GetAttributeLocation("a_color");
            att_tangentLoc_ = GetAttributeLocation("a_tangent");
            att_bonesIDLoc_ = GetAttributeLocation("a_boneIDs");
            att_bonesWeightLoc_ = GetAttributeLocation("a_weights");

            att_modelMatrixRow0Loc_ = GetAttributeLocation("a_mMatrixRow0");
            att_normalMatrixCol0Loc_ = GetAttributeLocation("a_normalMatrixCol0");

            modelLoc_ = GetUniformLocation("u_model");
            normalMatrixLoc_ = GetUniformLocation("u_normalMatrix");
            viewLoc_ = GetUniformLocation("u_view");
            viewProjectionLoc_ = GetUniformLocation("u_viewProjection");
            sceneColorAmbientLoc_ = GetUniformLocation("u_sceneAmbientColor");
            eyeWorldPosLoc_ = GetUniformLocation("u_eyeWorldPos");
            texture0Loc_ = GetUniformLocation("u_texture0");
            texture1Loc_ = GetUniformLocation("u_texture1");
            texture2Loc_ = GetUniformLocation("u_texture2");
            materialLoc_.color_ = GetUniformLocation("u_material.color");
            materialLoc_.ambient_ = GetUniformLocation("u_material.ambient");
            materialLoc_.diffuse_ = GetUniformLocation("u_material.diffuse");
            materialLoc_.specular_ = GetUniformLocation("u_material.specular");
            materialLoc_.shininess_ = GetUniformLocation("u_material.shininess");

            for (unsigned idx = 0; idx < nBones_; idx++)
            {
                std::stringstream boneIndex;
                boneIndex << "u_bones[" << idx << "]";

                GLuint boneLoc = GetUniformLocation(boneIndex.str());
                bonesLoc_.push_back(boneLoc);
            }

            for (unsigned idx = 0; idx < nDirectionalLights_; idx++)
            {
                std::stringstream lightIndex;
                lightIndex << "u_directionalLight[" << idx << "]";

                DirectionalLightLoc directionalLightLoc;
                directionalLightLoc.enabled_ = GetUniformLocation(lightIndex.str() + ".enabled");
                directionalLightLoc.base_.ambient_ = GetUniformLocation(lightIndex.str() + ".base.ambient");
                directionalLightLoc.base_.diffuse_ = GetUniformLocation(lightIndex.str() + ".base.diffuse");
                directionalLightLoc.base_.specular_ = GetUniformLocation(lightIndex.str() + ".base.specular");
                directionalLightLoc.direction_ = GetUniformLocation(lightIndex.str() + ".direction");
                directionalLightsLoc_.push_back(directionalLightLoc);
            }

            for (unsigned idx = 0; idx < nPointLights_; idx++)
            {
                std::stringstream lightIndex;
                lightIndex << "u_pointLights[" << idx << "]";
                PointLightLoc pointLightLoc;
                pointLightLoc.enabled_ = GetUniformLocation(lightIndex.str() + ".enabled");
                pointLightLoc.base_.ambient_ = GetUniformLocation(lightIndex.str() + ".base.ambient");
                pointLightLoc.base_.diffuse_ = GetUniformLocation(lightIndex.str() + ".base.diffuse");
                pointLightLoc.base_.specular_ = GetUniformLocation(lightIndex.str() + ".base.specular");
                pointLightLoc.position_ = GetUniformLocation(lightIndex.str() + ".position");
                pointLightLoc.atten_.constant_ = GetUniformLocation(lightIndex.str() + ".atten.constant");
                pointLightLoc.atten_.linear_ = GetUniformLocation(lightIndex.str() + ".atten.linear");
                pointLightLoc.atten_.quadratic_ = GetUniformLocation(lightIndex.str() + ".atten.quadratic");
                pointLightsLoc_.push_back(pointLightLoc);
            }

            for (unsigned idx = 0; idx < nSpotLights_; idx++)
            {
                std::stringstream lightIndex;
                lightIndex << "u_spotLights[" << idx << "]";
                SpotLightLoc spotLightLoc;
                spotLightLoc.point_.enabled_ = GetUniformLocation(lightIndex.str() + ".point.enabled");
                spotLightLoc.point_.base_.ambient_ = GetUniformLocation(lightIndex.str() + ".point.base.ambient");
                spotLightLoc.point_.base_.diffuse_ = GetUniformLocation(lightIndex.str() + ".point.base.diffuse");
                spotLightLoc.point_.base_.specular_ = GetUniformLocation(lightIndex.str() + ".point.base.specular");
                spotLightLoc.point_.position_ = GetUniformLocation(lightIndex.str() + ".point.position");
                spotLightLoc.point_.atten_.constant_ = GetUniformLocation(lightIndex.str() + ".point.atten.constant");
                spotLightLoc.point_.atten_.linear_ = GetUniformLocation(lightIndex.str() + ".point.atten.linear");
                spotLightLoc.point_.atten_.quadratic_ = GetUniformLocation(lightIndex.str() + ".point.atten.quadratic");
                spotLightLoc.direction_ = GetUniformLocation(lightIndex.str() + ".direction");
                spotLightLoc.cutOff_ = GetUniformLocation(lightIndex.str() + ".cutOff");
                spotLightsLoc_.push_back(spotLightLoc);
            }

            graphics_.SetProgram(this);

            if (texture0Loc_ != -1)
            {
                glUniform1i(texture0Loc_, 0);
            }

            if (texture1Loc_ != -1)
            {
                glUniform1i(texture1Loc_, 1);
            }

            if (texture2Loc_ != -1)
            {
                glUniform1i(texture2Loc_, 2);
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

        if (graphics_.GetProgram() == this)
            graphics_.SetProgram(nullptr);

        nBones_ = 0;
        nDirectionalLights_ = 0;
        nPointLights_ = 0;
        nSpotLights_ = 0;

        activeDirectionalLights_.clear();
        activePointLights_.clear();
        activeSpotLights_.clear();

        activeCamera_ = nullptr;
        neverUsed_ = true;
        activeMaterial_ = nullptr;
        activeSkeleton_ = nullptr;
        activeNode_ = nullptr;
        activeScene_ = nullptr;
        sceneColor_ = Color(-1);
        material_ = MaterialProgram();

        bonesLoc_.clear();
        pointLightsLoc_.clear();
        directionalLightsLoc_.clear();
        spotLightsLoc_.clear();

        graphics_.InvalidateVAOFor(this);
    }

    bool Program::HasLighting() const
    {
        return (int)ProgramFlag::PER_VERTEX_LIGHTING & flags_ || (int)ProgramFlag::PER_PIXEL_LIGHTING & flags_;
    }

    bool Program::Initialize()
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        // Creates the program name/index.
        id_ = glCreateProgram();

        // Bind vertex attribute locations to ensure they are the same in all shaders
        glBindAttribLocation(id_, (int)AttributesLoc::POSITION, "a_position");
        glBindAttribLocation(id_, (int)AttributesLoc::NORMAL, "a_normal");
        glBindAttribLocation(id_, (int)AttributesLoc::TEXTURECOORD0, "a_texcoord0");
        glBindAttribLocation(id_, (int)AttributesLoc::TEXTURECOORD1, "a_texcoord1");
        glBindAttribLocation(id_, (int)AttributesLoc::COLOR, "a_color");
        glBindAttribLocation(id_, (int)AttributesLoc::TANGENT, "a_tangent");
        glBindAttribLocation(id_, (int)AttributesLoc::BONES_ID, "a_boneIDs");
        glBindAttribLocation(id_, (int)AttributesLoc::BONES_WEIGHT, "a_weights");
        glBindAttribLocation(id_, (int)AttributesLoc::MODEL_MATRIX_ROW0, "a_mMatrixRow0");
        glBindAttribLocation(id_, (int)AttributesLoc::MODEL_MATRIX_ROW1, "a_mMatrixRow1");
        glBindAttribLocation(id_, (int)AttributesLoc::MODEL_MATRIX_ROW2, "a_mMatrixRow2");
        glBindAttribLocation(id_, (int)AttributesLoc::NORMAL_MATRIX_COL0, "a_normalMatrixCol0");
        glBindAttribLocation(id_, (int)AttributesLoc::NORMAL_MATRIX_COL1, "a_normalMatrixCol1");
        glBindAttribLocation(id_, (int)AttributesLoc::NORMAL_MATRIX_COL2, "a_normalMatrixCol2");

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

    void Program::SetSceneVariables(Scene* scene)
    {
        if (sceneColorAmbientLoc_ != -1)
        {
            if (scene)
            {
                if (activeScene_ != scene || scene->UniformsNeedUpdate())
                {
                    glUniform4fv(sceneColorAmbientLoc_, 1, &scene->GetAmbientColor()[0]);
                }
            }
            else if (activeScene_ != scene || sceneColor_ == Color(-1))
            {
                sceneColor_ = Color(0, 0, 0, 1);
                glUniform4fv(sceneColorAmbientLoc_, 1, &sceneColor_[0]);
            }

            activeScene_ = scene;
        }
    }

    void Program::SetNodeVariables(Node* node)
    {
        if (activeNode_ != node || (node && node->UniformsNeedUpdate()))
        {
            if (modelLoc_ != -1)
            {
                const Matrix4& m = node->GetGlobalModelMatrix();
                glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(m));
            }

            if (normalMatrixLoc_ != -1)
            {
                const Matrix3& m = node->GetGlobalModelInvTranspMatrix();
                glUniformMatrix3fv(normalMatrixLoc_, 1, GL_FALSE, glm::value_ptr(m));
            }
        }
    }

    void Program::SetMaterialVariables(Material* material)
    {
        if (material)
        {
            if (texture0Loc_ != -1)
            {
                graphics_.SetTexture(0, material->texture0_.get());
            }

            if (texture1Loc_ != -1)
            {
                graphics_.SetTexture(1, material->texture1_.get());
            }

            if (texture2Loc_ != -1)
            {
                graphics_.SetTexture(2, material->texture2_.get());
            }


            if (activeMaterial_ != material || material->UniformsNeedUpdate())
            {
                if (materialLoc_.color_ != -1 && material_.color_ != material->color_)
                {
                    material_.color_ = material->color_;
                    glUniform4fv(materialLoc_.color_, 1, &material->color_[0]);
                }

                if (materialLoc_.ambient_ != -1 && material_.ambient_ != material->ambient_)
                {
                    material_.ambient_ = material->ambient_;
                    glUniform4fv(materialLoc_.ambient_, 1, &material->ambient_[0]);
                }

                if (materialLoc_.diffuse_ != -1 && material_.diffuse_ != material->diffuse_)
                {
                    material_.diffuse_ = material->diffuse_;
                    glUniform4fv(materialLoc_.diffuse_, 1, &material->diffuse_[0]);
                }

                if (materialLoc_.specular_ != -1 && material_.specular_ != material->specular_)
                {
                    material_.specular_ = material->specular_;
                    glUniform4fv(materialLoc_.specular_, 1, &material->specular_[0]);
                }

                if (materialLoc_.shininess_ != -1 && material_.shininess_ != material->shininess_)
                {
                    material_.shininess_ = material->shininess_;
                    glUniform1f(materialLoc_.shininess_, material->shininess_);
                }
            }
        }

        activeMaterial_ = material;
    }

    bool Program::SetSkeletonVariables(Skeleton* skeleton)
    {
        if (skeleton)
        {
            const std::vector<PNode>& bones = skeleton->GetBones();
            unsigned nBones = bones.size();
            if (nBones != nBones_)
            {
                CHECK_ASSERT(nBones > 0, __FILE__, __LINE__);
                TRACE_LOG("Invalidating shader since number of bones (in the shader) has changed. Before nBones = " << nBones_ << ", now is " << nBones << ".")
                Invalidate();
                nBones_ = nBones;
                return false;
            }

            PNode rootNode = skeleton->GetRoot();
            Matrix4 globalInverseModelMatrix(1);
            Node* parent = rootNode->GetParent();
            if (parent)
            {
                // In order to make all the bones relatives to the root's parent.
                // The model matrix and normal matrix for the active node is premultiplied in the shader (see Program::SetNodeVariables)
                // See in Transform.glsl: GetModelMatrix() and GetWorldNormal()
                globalInverseModelMatrix = parent->GetGlobalModelInvMatrix();
            }

            for (unsigned idx = 0; idx < nBones_; idx++)
            {
                const GLuint& boneLoc = bonesLoc_[idx];
                if (boneLoc != -1)
                {
                    Node* bone = bones[idx].get();
                    if (activeSkeleton_ != skeleton || bone->UniformsNeedUpdate())
                    {
                        // Be careful, bones don't have normal matrix so their scale must be uniform (sx == sy == sz)
                        CHECK_ASSERT(bone->IsScaleUniform(), __FILE__, __LINE__);

                        const Matrix4& m = bone->GetGlobalModelMatrix();
                        const Matrix4& offsetMatrix = bone->GetBoneOffsetMatrix();
                        if(graphics_.HasInstancedArrays())
                        {
                            // Using instancing: (See Graphics::UpdateBatchBuffer)
                            // we need to transpose the skin matrix since the model matrix is already transposed (uses rows instead of cols)
                            Matrix4 skinMatrix(glm::transpose(globalInverseModelMatrix * m * offsetMatrix));
                            glUniformMatrix4fv(boneLoc, 1, GL_FALSE, glm::value_ptr(skinMatrix));
                        }
                        else
                        {
                            Matrix4 skinMatrix(globalInverseModelMatrix * m * offsetMatrix);
                            glUniformMatrix4fv(boneLoc, 1, GL_FALSE, glm::value_ptr(skinMatrix));
                        }
                    }
                }
            }
        }
        activeSkeleton_ = skeleton;

        return true;
    }

    void Program::SetCameraVariables()
    {
        Camera* camera = Camera::GetActiveCamera();

        bool update_camera = (viewLoc_ != -1 || viewProjectionLoc_ != -1 || eyeWorldPosLoc_ != -1);

        update_camera = update_camera && (activeCamera_ != camera || (camera && camera->UniformsNeedUpdate()));
        update_camera = update_camera || neverUsed_;

        if (update_camera)
        {
            neverUsed_ = false;

            if (viewProjectionLoc_ != -1)
            {
                const Matrix4& m = Camera::GetMatViewProj();
                glUniformMatrix4fv(viewProjectionLoc_, 1, GL_FALSE, glm::value_ptr(m));
            }

            if (viewLoc_ != -1)
            {
                const Matrix4& m = Camera::GetViewMatrix();
                glUniformMatrix4fv(viewLoc_, 1, GL_FALSE, glm::value_ptr(m));
            }

            if (eyeWorldPosLoc_ != -1)
            {
                Vertex3 position(0);
                if (camera)
                    position = camera->GetGlobalPosition();
                glUniform3fv(eyeWorldPosLoc_, 1, &position[0]);
            }
        }

        activeCamera_ = camera;
    }

    void Program::SetBaseLightVariables(const BaseLightLoc& baseLoc, const Light* light)
    {
        if (baseLoc.ambient_ != -1)
        {
            const Color& ambient = light->GetAmbientColor();
            glUniform4fv(baseLoc.ambient_, 1, &ambient[0]);
        }

        if (baseLoc.diffuse_ != -1)
        {
            const Color& diffuse = light->GetDiffuseColor();
            glUniform4fv(baseLoc.diffuse_, 1, &diffuse[0]);
        }

        if (baseLoc.specular_ != -1)
        {
            const Color& specular = light->GetSpecularColor();
            glUniform4fv(baseLoc.specular_, 1, &specular[0]);
        }
    }

    bool Program::SetLightVariables(Scene* scene)
    {
        if (scene && HasLighting())
        {
            const Scene::Lights& dirLights = scene->GetLights(Light::DIRECTIONAL);

            if (nDirectionalLights_ != dirLights.size())
            {
                TRACE_LOG("Invalidating program due number of directionals light has changed!!!");
                Invalidate();
                return false;
            }

            const Scene::Lights& pointLigths = scene->GetLights(Light::POINT);

            if (nPointLights_ != pointLigths.size())
            {
                TRACE_LOG("Invalidating program due number of points light has changed!!!");
                Invalidate();
                return false;
            }

            const Scene::Lights& spotLigths = scene->GetLights(Light::SPOT);

            if (nSpotLights_ != spotLigths.size())
            {
                TRACE_LOG("Invalidating program due number of spot light has changed!!!");
                Invalidate();
                return false;
            }

            for (unsigned idx = 0; idx < nDirectionalLights_; idx++)
            {
                Light* light = dirLights[idx].get();

                if (activeDirectionalLights_[idx] != light || light->UniformsNeedUpdate())
                {
                    const DirectionalLightLoc& loc = directionalLightsLoc_[idx];

                    SetBaseLightVariables(loc.base_, light);

                    if (loc.enabled_ != -1)
                    {
                        glUniform1i(loc.enabled_, light->IsEnabled() ? 1 : 0);
                    }

                    if (loc.direction_ != -1)
                    {
                        const Vertex3& direction = light->GetLookAtDirection();
                        glUniform3fv(loc.direction_, 1, &direction[0]);
                    }
                }

                activeDirectionalLights_[idx] = light;
            }

            for (unsigned idx = 0; idx < nPointLights_; idx++)
            {
                Light* light = pointLigths[idx].get();

                if (activePointLights_[idx] != light || light->UniformsNeedUpdate())
                {
                    const PointLightLoc& loc = pointLightsLoc_[idx];

                    SetBaseLightVariables(loc.base_, light);

                    if (loc.enabled_ != -1)
                    {
                        glUniform1i(loc.enabled_, light->IsEnabled() ? 1 : 0);
                    }

                    if (loc.position_ != -1)
                    {
                        const Vertex3& position = light->GetGlobalPosition();
                        glUniform3fv(loc.position_, 1, &position[0]);
                    }

                    const Light::Attenuation& attenuation = light->GetAttenuation();

                    if (loc.atten_.constant_ != -1)
                    {
                        glUniform1f(loc.atten_.constant_, attenuation.constant);
                    }

                    if (loc.atten_.linear_  != -1)
                    {
                        glUniform1f(loc.atten_.linear_, attenuation.linear);
                    }

                    if (loc.atten_.quadratic_ != -1)
                    {
                        glUniform1f(loc.atten_.quadratic_, attenuation.quadratic);
                    }
                }

                activePointLights_[idx] = light;
            }

            for (unsigned idx = 0; idx < nSpotLights_; idx++)
            {
                Light* light = spotLigths[idx].get();

                if (activeSpotLights_[idx] != light || light->UniformsNeedUpdate())
                {
                    const SpotLightLoc& loc = spotLightsLoc_[idx];

                    SetBaseLightVariables(loc.point_.base_, light);

                    if (loc.point_.enabled_ != -1)
                    {
                        glUniform1i(loc.point_.enabled_, light->IsEnabled() ? 1 : 0);
                    }

                    if (loc.point_.position_ != -1)
                    {
                        const Vertex3& position = light->GetGlobalPosition();
                        glUniform3fv(loc.point_.position_, 1, &position[0]);
                    }

                    if (loc.point_.atten_.constant_ != -1)
                    {
                        const Light::Attenuation& attenuation = light->GetAttenuation();
                        glUniform1f(loc.point_.atten_.constant_, attenuation.constant);
                    }

                    if (loc.point_.atten_.linear_  != -1)
                    {
                        const Light::Attenuation& attenuation = light->GetAttenuation();
                        glUniform1f(loc.point_.atten_.linear_, attenuation.linear);
                    }

                    if (loc.point_.atten_.quadratic_ != -1)
                    {
                        const Light::Attenuation& attenuation = light->GetAttenuation();
                        glUniform1f(loc.point_.atten_.quadratic_, attenuation.quadratic);
                    }

                    if (loc.direction_ != -1)
                    {
                        const Vertex3& direction = light->GetLookAtDirection();
                        glUniform3fv(loc.direction_, 1, &direction[0]);
                    }

                    if (loc.cutOff_ != -1)
                    {
                        float cutOff = light->GetSpotCutOff();
                        glUniform1f(loc.cutOff_, glm::cos(glm::radians(cutOff)));
                    }
                }

                activeSpotLights_[idx] = light;
            }
        }

        return true;
    }

    void Program::SetVariables(Material* material, Mesh* mesh, Node* node)
    {
        if (SetSkeletonVariables(mesh->GetSkeleton().get()))
        {
            PScene scene = App::this_->GetCurrentScene();
            SetSceneVariables(scene.get());
            SetMaterialVariables(material);
            SetNodeVariables(node);
            SetCameraVariables();
            activeNode_ = node;
            if (SetLightVariables(scene.get()) && pExtraUniforms_)
                pExtraUniforms_->AssignValues();
        }
    }

    void Program::SetVariables(Material* material, Mesh* mesh)
    {
        if (SetSkeletonVariables(mesh->GetSkeleton().get()))
        {
            PScene scene = App::this_->GetCurrentScene();
            SetSceneVariables(scene.get());
            SetMaterialVariables(material);
            SetCameraVariables();
            if (SetLightVariables(scene.get()) && pExtraUniforms_)
                pExtraUniforms_->AssignValues();
        }
    }

    void Program::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Program");

        {
            std::stringstream ss;
            ss << name_;
            child.append_attribute("name") = ss.str().c_str();
        }

        child.append_attribute("flags") = flags_.to_string().c_str();

    }

    PProgram Program::CreateFrom(const pugi::xml_node& node)
    {
        std::string name = node.attribute("name").as_string();
        std::string flags = node.attribute("flags").as_string();
        PProgram program = App::this_->CreateProgram(name);
        program->SetFlags(flags);
        return program;
    }

    void Program::SetFlags(const ProgramFlags& flags)
    {
        if (flags_ != flags)
        {
            flags_ = flags;
            Invalidate();
        }
    }
}
