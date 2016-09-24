/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "Light.h"
#include "Camera.h"
#include "ShadowCamera.h"
#include "Mesh.h"
#include "SceneNode.h"
#include "Scene.h"
#include "Skeleton.h"
#include "Material.h"
#include "RenderingContext.h"
#include "Constants.h"
#include "Util.h"
#include "Material.h"
#include "Renderer.h"
#include "Pass.h"
#include "VertexArrayObj.h"
#include "StringConverter.h"
#include "pugixml.hpp"
#include "Maths.h"
#include "autogenerated/Common_glsl.inl"
#include "autogenerated/Samplers_glsl.inl"
#include "autogenerated/Transforms_glsl.inl"
#include "autogenerated/Ambient_glsl.inl"
#include "autogenerated/Shadows_glsl.inl"
#include "autogenerated/Lighting_glsl.inl"
#include "autogenerated/PostProcess_glsl.inl"
#include "autogenerated/VS_glsl.inl"
#include "autogenerated/FS_glsl.inl"
#include <stdlib.h>
#include <stdio.h>
#if !defined(IS_TARGET_APPLE)
#include <malloc.h>
#endif
#include <assert.h>
#include <cstring>
#include <string>
#include <algorithm>
#include <sstream>

namespace NSG
{
    template<> std::map<std::string, PProgram> StrongFactory<std::string, Program>::objsMap_ = std::map<std::string, PProgram> {};

    Program::Program(const std::string& defines)
        : Object(GetUniqueName("Program")),
          defines_(defines),
          id_(0),
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
          projectionLoc_(-1),
          sceneColorAmbientLoc_(-1),
          u_sceneHorizonColorLoc_(-1),
          eyeWorldPosLoc_(-1),
          u_fogMinIntensityLoc_(-1),
          u_fogStartLoc_(-1),
          u_fogEndLoc_(-1),
          u_fogHeightLoc_(-1),
          lightDiffuseColorLoc_(-1),
          lightSpecularColorLoc_(-1),
          lightInvRangeLoc_(-1),
          lightPositionLoc_(-1),
          lightDirectionLoc_(-1),
          lightCutOffLoc_(-1),
          shadowCameraZFarLoc_(-1),
          shadowMapInvSize_(-1),
          shadowColor_(-1),
          shadowBias_(-1),
          blendMode_loc_(-1),
          activeSkeleton_(nullptr),
          activeNode_(nullptr),
          activeMaterial_(nullptr),
          activeLight_(nullptr),
          activeCamera_(nullptr),
          sceneColor_(-1),
          skeleton_(nullptr),
          node_(nullptr),
          material_(nullptr),
          light_(nullptr),
          camera_(nullptr),
          scene_(nullptr)
    {
        memset(&textureLoc_, -1, sizeof(textureLoc_));
        memset(&u_uvTransformLoc_, -1, sizeof(u_uvTransformLoc_));
        memset(&materialLoc_, -1, sizeof(materialLoc_));
        memset(&blurFilterLoc_, -1, sizeof(blurFilterLoc_));
        memset(&wavesFilterLoc_, -1, sizeof(wavesFilterLoc_));
        memset(&shockWaveFilterLoc_, -1, sizeof(shockWaveFilterLoc_));        
        memset(&lightViewLoc_, -1, sizeof(lightViewLoc_));
        memset(&lightProjectionLoc_, -1, sizeof(lightProjectionLoc_));
        memset(&lightViewProjectionLoc_, -1, sizeof(lightViewProjectionLoc_));

    }

    Program::~Program()
    {
        Invalidate();
    }

    void Program::ConfigureShaders(std::string& vertexShader, std::string& fragmentShader)
    {
        std::string preDefines;

        #ifdef GL_ES_VERSION_2_0
        preDefines = "#version 100\n#define GLES2\n";
        #else
        preDefines = "#version 120\n";
        #endif

        std::string::size_type pos0 = 0;
        std::string::size_type pos1 = defines_.find('\n', 0);
        while (pos1 != std::string::npos)
        {
            preDefines += "#define " + defines_.substr(pos0, pos1 - pos0 + 1);
            pos0 = pos1 + 1;
            pos1 = defines_.find('\n', pos0);
        } ;

        LOGI("Shader variation:\n%s", defines_.c_str());

        {
            std::string buffer = preDefines + "#define COMPILEVS\n";
            buffer += COMMON_GLSL;
            buffer += TRANSFORMS_GLSL;
            buffer += LIGHTING_GLSL;
            buffer += VS_GLSL;
            vertexShader = buffer;
        }

        {
            std::string fBuffer = preDefines + "#define COMPILEFS\n";
            fBuffer += COMMON_GLSL;
            fBuffer += SAMPLERS_GLSL;
            fBuffer += TRANSFORMS_GLSL;
            fBuffer += AMBIENT_GLSL;
            fBuffer += SHADOWS_GLSL;
            fBuffer += LIGHTING_GLSL;
            fBuffer += POSTPROCESS_GLSL;
            fBuffer += FS_GLSL;
            fragmentShader = fBuffer;
        }
    }

    bool Program::IsValid()
    {
        return !material_ || material_->IsReady();
    }

    void Program::AllocateResources()
    {
        graphics_ = RenderingContext::GetSharedPtr();
        std::string vShader;
        std::string fShader;
        ConfigureShaders(vShader, fShader);
        pVShader_ = PVertexShader(new VertexShader(vShader.c_str()));
        pFShader_ = PFragmentShader(new FragmentShader(fShader.c_str()));
        if (Initialize())
        {
            graphics_.lock()->SetProgram(this);
            SetUniformLocations();
        }
    }

    void Program::ReleaseResources()
    {
        if (pVShader_)
            glDetachShader(id_, pVShader_->GetId());
        if (pFShader_)
            glDetachShader(id_, pFShader_->GetId());

        pVShader_ = nullptr;
        pFShader_ = nullptr;
        glDeleteProgram(id_);

		auto ctx = graphics_.lock();
        if (ctx && ctx->GetProgram() == this)
			ctx->SetProgram(nullptr);

        activeSkeleton_ = nullptr;
        activeNode_ = nullptr;
        activeMaterial_ = nullptr;
        activeLight_ = nullptr;
        activeCamera_ = nullptr;
        sceneColor_ = ColorRGB(-1);

        bonesBaseLoc_.clear();
    }

    bool Program::ShaderCompiles(GLenum type, const std::string& buffer) const
    {
        CHECK_GL_STATUS();
        GLuint id = glCreateShader(type);
        const char* source = buffer.c_str();
        glShaderSource(id, 1, &source, nullptr);
        glCompileShader(id);
        GLint compile_status = GL_FALSE;
        glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status);
        if (compile_status != GL_TRUE)
        {
            GLint logLength = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
            if (logLength > 0)
            {
                std::string log;
                log.resize(logLength);
                glGetShaderInfoLog(id, logLength, &logLength, &log[0]);
                LOGE("%s", log.c_str());
            }
        }
        glDeleteShader(id);
        //glReleaseShaderCompiler(); // fails on osx
        CHECK_GL_STATUS();
        LOGI("Checking %s shader for material %s: %s", (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT"), name_.c_str(), (compile_status == GL_TRUE ? "IS OK" : "HAS FAILED"));

        return compile_status == GL_TRUE;
    }

    void Program::SetUniformLocations()
    {
        CHECK_GL_STATUS();

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
        projectionLoc_  = GetUniformLocation("u_projection");
        sceneColorAmbientLoc_ = GetUniformLocation("u_sceneAmbientColor");
        u_sceneHorizonColorLoc_ = GetUniformLocation("u_sceneHorizonColor");
        eyeWorldPosLoc_ = GetUniformLocation("u_eyeWorldPos");
        u_fogMinIntensityLoc_ = GetUniformLocation("u_fogMinIntensity");
        u_fogStartLoc_ = GetUniformLocation("u_fogStart");
        u_fogEndLoc_ = GetUniformLocation("u_fogEnd");
        u_fogHeightLoc_ = GetUniformLocation("u_fogHeight");

        for (size_t index = 0; index < MaterialTexture::MAX_MAPS; index++)
        {
            textureLoc_[index] = GetUniformLocation("u_texture" + ToString(index));
            u_uvTransformLoc_[index] = GetUniformLocation("u_uvTransform" + ToString(index));
        }
        materialLoc_.diffuseColor_ = GetUniformLocation("u_material.diffuseColor");
        materialLoc_.diffuseIntensity_ = GetUniformLocation("u_material.diffuseIntensity");
        materialLoc_.specularColor_ = GetUniformLocation("u_material.specularColor");
        materialLoc_.specularIntensity_ = GetUniformLocation("u_material.specularIntensity");
        materialLoc_.ambientIntensity_ = GetUniformLocation("u_material.ambientIntensity");
        materialLoc_.shininess_ = GetUniformLocation("u_material.shininess");
        materialLoc_.emitIntensity_ = GetUniformLocation("u_material.emitIntensity");

        for (size_t i = 0; i < MAX_BONES; i++)
        {
            GLuint boneLoc = GetUniformLocation("u_bones[" + ToString(i) + "]");
            if (boneLoc != -1)
                bonesBaseLoc_.push_back(boneLoc);
        }

        lightDiffuseColorLoc_ = GetUniformLocation("u_lightDiffuseColor");
        lightSpecularColorLoc_ = GetUniformLocation("u_lightSpecularColor");
        lightInvRangeLoc_ = GetUniformLocation("u_lightInvRange");
        lightPositionLoc_ = GetUniformLocation("u_lightPosition");

        for (int i = 0; i < MAX_SPLITS; i++)
        {
            lightViewLoc_[i] = GetUniformLocation("u_lightView[" + ToString(i) + "]");
            lightProjectionLoc_[i] = GetUniformLocation("u_lightProjection[" + ToString(i) + "]");
            lightViewProjectionLoc_[i] = GetUniformLocation("u_lightViewProjection[" + ToString(i) + "]");
        }

        lightDirectionLoc_ = GetUniformLocation("u_lightDirection");
        lightCutOffLoc_ = GetUniformLocation("u_lightCutOff");
        shadowCameraZFarLoc_ = GetUniformLocation("u_shadowCameraZFar");
        shadowMapInvSize_ = GetUniformLocation("u_shadowMapInvSize");
        shadowColor_ = GetUniformLocation("u_shadowColor");
        shadowBias_ = GetUniformLocation("u_shadowBias");

        blendMode_loc_ = GetUniformLocation("u_blendMode");
        blurFilterLoc_.blurDir_ = GetUniformLocation("u_blurDir");
        blurFilterLoc_.blurRadius_ = GetUniformLocation("u_blurRadius");
        blurFilterLoc_.sigma_ = GetUniformLocation("u_sigma");
        wavesFilterLoc_.factor_ = GetUniformLocation("u_waveFactor");
        wavesFilterLoc_.offset_ = GetUniformLocation("u_waveOffset");
        shockWaveFilterLoc_.center_ = GetUniformLocation("u_shockWaveCenter");
        shockWaveFilterLoc_.time_ = GetUniformLocation("u_shockWaveTime");
        shockWaveFilterLoc_.params_ = GetUniformLocation("u_shockWaveParams");

        for (int index = 0; index < MaterialTexture::MAX_MAPS; index++)
        {
            if (textureLoc_[index] != -1)
                glUniform1i(textureLoc_[index], index); //set fixed locations for samplers
        }

        CHECK_GL_STATUS();
    }

    bool Program::Initialize()
    {
        CHECK_GL_STATUS();
        LOGI("Creating program for material %s", name_.c_str());
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
        glAttachShader(id_, pVShader_->GetId());
        glAttachShader(id_, pFShader_->GetId());
        glLinkProgram(id_);
        GLint link_status = GL_FALSE;
        glGetProgramiv(id_, GL_LINK_STATUS, &link_status);
        if (link_status != GL_TRUE)
        {
            GLint logLength = 0;
            glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &logLength);
            if (logLength > 0)
            {
                std::string log;
                log.resize(logLength);
                glGetProgramInfoLog(id_, logLength, &logLength, &log[0]);
                LOGE("Program creation failed: %s", log.c_str());
                //LOGI("VS: %s", pVShader_->GetSource());
                //LOGI("FS: %s" << pFShader_->GetSource());
            }
            LOGE("Linking program for material %s HAS FAILED", name_.c_str());
            return false;
        }
        LOGI("Program for material %s OK.", name_.c_str());
        CHECK_GL_STATUS();
        return true;
    }

    GLuint Program::GetAttributeLocation(const std::string& name)
    {
        return glGetAttribLocation(id_, name.c_str());
    }

        GLuint Program::GetUniformLocation(const std::string& name)
    {
        return glGetUniformLocation(id_, name.c_str());
    }

    void Program::SetSceneVariables()
    {
        if (sceneColorAmbientLoc_ != -1)
        {
            if (scene_)
            {
				if (scene_->UniformsNeedUpdate())
                    glUniform3fv(sceneColorAmbientLoc_, 1, &scene_->GetAmbientColor()[0]);
            }
            else if (sceneColor_ == ColorRGB(-1))
            {
                sceneColor_ = ColorRGB(0);
                glUniform3fv(sceneColorAmbientLoc_, 1, &sceneColor_[0]);
            }
        }

		if (scene_)
        {
            if (u_sceneHorizonColorLoc_)
				glUniform3fv(u_sceneHorizonColorLoc_, 1, &scene_->GetHorizonColor()[0]);

            if (u_fogMinIntensityLoc_ != -1)
                glUniform1f(u_fogMinIntensityLoc_, scene_->GetFogMinIntensity());

            if (u_fogStartLoc_ != -1)
            {
                auto start = scene_->GetFogStart();
                if(camera_)
                    start = std::max(camera_->GetZNear(), start);
                glUniform1f(u_fogStartLoc_, start);
            }
            if (u_fogEndLoc_ != -1)
            {
                auto end = scene_->GetFogStart() + scene_->GetFogDepth();
                if(camera_)
                    end = std::min(camera_->GetZFar(), end);
                glUniform1f(u_fogEndLoc_, end);
            }
            if (u_fogHeightLoc_ != -1)
                glUniform1f(u_fogHeightLoc_, scene_->GetFogHeight());
        }
    }

    void Program::SetNodeVariables()
    {
        if (node_ && (activeNode_ != node_ || node_->UniformsNeedUpdate()))
        {
            if (modelLoc_ != -1)
            {
                const Matrix4& m = node_->GetGlobalModelMatrix();
                glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, m.GetPointer());
            }

            if (normalMatrixLoc_ != -1)
            {
                const Matrix3& m = node_->GetGlobalModelInvTranspMatrix();
                glUniformMatrix3fv(normalMatrixLoc_, 1, GL_FALSE, m.GetPointer());
            }
        }
        else if (!node_)
        {
            if (modelLoc_ != -1)
            {
                static const Matrix4 m(1);
                glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, m.GetPointer());
            }

            if (normalMatrixLoc_ != -1)
            {
                static const Matrix4 m(1);
                glUniformMatrix3fv(normalMatrixLoc_, 1, GL_FALSE, m.GetPointer());
            }
        }

    }

    void Program::SetMaterialVariables()
    {
        if (material_)
        {
            for (int index = 0; index < MaterialTexture::SHADOW_MAP0; index++)
            {
                if (textureLoc_[index] != -1)
                {
                    MaterialTexture type = (MaterialTexture)index;
                    auto texture = material_->GetTexture(type).get();
                    graphics_.lock()->SetTexture(index, texture);

                    if (u_uvTransformLoc_[index] != -1)
                        glUniform4fv(u_uvTransformLoc_[index], 1, &texture->GetUVTransform()[0]);
                }
            }

            if (activeMaterial_ != material_ || material_->UniformsNeedUpdate())
            {
                if (materialLoc_.diffuseColor_ != -1)
                    glUniform4fv(materialLoc_.diffuseColor_, 1, &material_->diffuseColor_[0]);

                if (materialLoc_.diffuseIntensity_ != -1)
                    glUniform1f(materialLoc_.diffuseIntensity_, material_->diffuseIntensity_);

                if (materialLoc_.specularColor_ != -1)
                    glUniform4fv(materialLoc_.specularColor_, 1, &material_->specularColor_[0]);

                if (materialLoc_.specularIntensity_ != -1)
                    glUniform1f(materialLoc_.specularIntensity_, material_->specularIntensity_);

                if (materialLoc_.ambientIntensity_ != -1)
                    glUniform1f(materialLoc_.ambientIntensity_, material_->ambientIntensity_);

                if (materialLoc_.shininess_ != -1)
                    glUniform1f(materialLoc_.shininess_, material_->shininess_);

                if (materialLoc_.emitIntensity_ != -1)
                    glUniform1f(materialLoc_.emitIntensity_, material_->emitIntensity_);

                if (blendMode_loc_ != -1)
                    glUniform1i(blendMode_loc_, (int)material_->GetFilterBlendMode());

                if (blurFilterLoc_.blurDir_ != -1)
                    glUniform2fv(blurFilterLoc_.blurDir_, 1, &material_->blurFilter_.blurDir_[0]);

                if (blurFilterLoc_.blurRadius_ != -1)
                    glUniform2fv(blurFilterLoc_.blurRadius_, 1, &material_->blurFilter_.blurRadius_[0]);

                if (blurFilterLoc_.sigma_ != -1)
                    glUniform1f(blurFilterLoc_.sigma_, material_->blurFilter_.sigma_);

                if (wavesFilterLoc_.factor_ != -1)
                    glUniform1f(wavesFilterLoc_.factor_, material_->waveFilter_.factor_);

                if (wavesFilterLoc_.offset_ != -1)
                    glUniform1f(wavesFilterLoc_.offset_, material_->waveFilter_.offset_);

                if (shockWaveFilterLoc_.center_ != -1)
                    glUniform2fv(shockWaveFilterLoc_.center_, 1, &material_->shockWaveFilter_.center_[0]);

                if (shockWaveFilterLoc_.time_ != -1)
                    glUniform1f(shockWaveFilterLoc_.time_, material_->shockWaveFilter_.time_);

                if (shockWaveFilterLoc_.params_ != -1)
                    glUniform3fv(shockWaveFilterLoc_.params_, 1, &material_->shockWaveFilter_.params_[0]);
            }
        }
    }

    void Program::SetSkeletonVariables()
    {
        if (skeleton_)
        {
            const std::vector<std::string>& names = skeleton_->GetShaderOrder();
            size_t nBones = names.size();
            PNode armatureNode = node_->GetArmature();
            CHECK_ASSERT(graphics_.lock()->GetMesh()->HasDeformBones());
            CHECK_ASSERT(armatureNode);
            Matrix4 globalInverseModelMatrix(1);
            // In order to make all the bones relatives to the armature.
            // The model matrix and normal matrix for the active node is premultiplied in the shader (see Program::SetNodeVariables)
            // See in Transform.glsl: GetModelMatrix() and GetWorldNormal()
            globalInverseModelMatrix = armatureNode->GetGlobalModelInvMatrix();
            CHECK_GL_STATUS();
            for (unsigned idx = 0; idx < nBones; idx++)
            {
                GLuint boneLoc = bonesBaseLoc_[idx];
                std::string boneName = names[idx];
                auto bone = armatureNode->GetChild<Node>(boneName, true);
                // Be careful, bones don't have normal matrix so their scale must be uniform (sx == sy == sz)
                //CHECK_ASSERT(bone->IsScaleUniform());
                const Matrix4& m = bone->GetGlobalModelMatrix();
                const Matrix4& offsetMatrix = skeleton_->GetBoneOffsetMatrix(boneName);
                Matrix4 boneMatrix(globalInverseModelMatrix * m * offsetMatrix);
                glUniformMatrix4fv(boneLoc, 1, GL_FALSE, boneMatrix.GetPointer());
            }
            CHECK_GL_STATUS();
        }
        activeSkeleton_ = skeleton_;
    }

    Matrix4 Program::AdjustProjection(const Matrix4& m) const
    {
        auto slopeScaledBias = material_->GetSlopeScaledBias() * light_->GetSlopeScaledBias();
        graphics_.lock()->SetSlopeScaledBias(slopeScaledBias);

        Matrix4 m1(m);
        // Add constant depth bias to the projection matrix
        // OpenGL constant bias is unreliable and dependant on depth buffer bitdepth.
        auto constantBias = 2.f * material_->GetBias() * light_->GetBias();
        m1[2][2] += m1[2][3] * constantBias;
        m1[3][2] += m1[3][3] * constantBias;
        return m1;
    }

    void Program::SetCameraVariables(bool shadowPass)
    {
        if (camera_ && (camera_ != activeCamera_ || camera_->UniformsNeedUpdate()))
        {
            if (viewProjectionLoc_ != -1)
            {
                if (shadowPass)
                {
                    auto m = AdjustProjection(camera_->GetProjection()) * camera_->GetView();
                    glUniformMatrix4fv(viewProjectionLoc_, 1, GL_FALSE, m.GetPointer());
                }
                else
                {
                    auto& m = camera_->GetViewProjection();
                    glUniformMatrix4fv(viewProjectionLoc_, 1, GL_FALSE, m.GetPointer());
                }
            }

            if (viewLoc_ != -1)
            {
                auto& m = camera_->GetView();
                glUniformMatrix4fv(viewLoc_, 1, GL_FALSE, m.GetPointer());
            }

            if (projectionLoc_ != -1)
            {
                if (shadowPass)
                {
                    auto m = AdjustProjection(camera_->GetProjection());
                    glUniformMatrix4fv(projectionLoc_, 1, GL_FALSE, m.GetPointer());
                }
                else
                {
                    auto& m = camera_->GetProjection();
                    glUniformMatrix4fv(projectionLoc_, 1, GL_FALSE, m.GetPointer());
                }
            }

            if (eyeWorldPosLoc_ != -1)
            {
                auto& position = camera_->GetGlobalPosition();
                glUniform3fv(eyeWorldPosLoc_, 1, &position[0]);
            }
        }
    }

    void Program::SetLightShadowVariables(bool shadowPass)
    {
        if (light_)
        {
            if (activeLight_ != light_ || light_->UniformsNeedUpdate())
            {
                if (lightDirectionLoc_ != -1)
                {
                    const Vertex3& direction = light_->GetLookAtDirection();
                    glUniform3fv(lightDirectionLoc_, 1, &direction[0]);
                }
            }

            if (lightInvRangeLoc_ != -1)
            {
                // lightInvRangeLoc_ only used for point and spot lights
                CHECK_ASSERT(light_->GetType() != LightType::DIRECTIONAL);
                glUniform1f(lightInvRangeLoc_, light_->GetInvRange());
            }

            if (shadowCameraZFarLoc_ != -1)
            {
                auto shadowSplits = light_->GetShadowSplits();
                Vector4 shadowCameraZFarSplits;
                bool uniformsNeedUpdate = camera_->UniformsNeedUpdate();
                for (int i = 0; i < shadowSplits; i++)
                {
                    auto shadowCamera = light_->GetShadowCamera(i);
                    uniformsNeedUpdate |= shadowCamera->UniformsNeedUpdate();
                    if (shadowCamera->IsDisabled())
                        shadowCameraZFarSplits[i] = 0;
                    else
                        shadowCameraZFarSplits[i] = shadowCamera->GetFarSplit();
                }

                if (uniformsNeedUpdate)
                {
                    glUniform4fv(shadowCameraZFarLoc_, 1, &shadowCameraZFarSplits[0]);
                    //LOGI("zFar = %f %f %f %f", shadowCameraZFarSplits[0], shadowCameraZFarSplits[1], shadowCameraZFarSplits[2], shadowCameraZFarSplits[3]);
                }
            }
        }
    }

    void Program::SetLightVariables()
    {
        if (light_)
        {
            auto shadowSplits = light_->GetShadowSplits();

            if (light_->DoShadows())
            {
                if (shadowColor_ != -1)
                {
                    const Color& color = light_->GetShadowColor();
                    glUniform4fv(shadowColor_, 1, &color[0]);
                }

#if 0
                if (shadowBias_ != -1)
                {
                    auto bias = material_->GetBias() * light_->GetBias();
                    glUniform1f(shadowBias_, bias);
                }
#endif
                if (shadowMapInvSize_ != -1)
                {
                    Vector4 shadowMapsInvSize;
                    for (int i = 0; i < shadowSplits; i++)
                    {
                        auto shadowMap = light_->GetShadowMap(i);
                        float width = (float)shadowMap->GetWidth();
                        //CHECK_ASSERT(width > 0);
                        shadowMapsInvSize[i] = 1.f / width;
                    }
                    glUniform4fv(shadowMapInvSize_, 1, &shadowMapsInvSize[0]);
                }

                for (int i = 0; i < shadowSplits; i++)
                {
                    if (lightViewLoc_[i] != -1)
                    {
                        auto shadowCamera = light_->GetShadowCamera(i);
                        auto& m = shadowCamera->GetView();
                        glUniformMatrix4fv(lightViewLoc_[i], 1, GL_FALSE, m.GetPointer());
                    }

                    if (lightProjectionLoc_[i] != -1)
                    {
                        auto shadowCamera = light_->GetShadowCamera(i);
                        auto& m = shadowCamera->GetProjection();
                        glUniformMatrix4fv(lightProjectionLoc_[i], 1, GL_FALSE, m.GetPointer());
                    }

                    if (lightViewProjectionLoc_[i] != -1)
                    {
                        auto shadowCamera = light_->GetShadowCamera(i);
                        auto& m = shadowCamera->GetViewProjection();
                        glUniformMatrix4fv(lightViewProjectionLoc_[i], 1, GL_FALSE, m.GetPointer());
                    }

                    int index = (int)MaterialTexture::SHADOW_MAP0 + i;
                    if (textureLoc_[index] != -1)
                    {
                        auto shadowMap = light_->GetShadowMap(i).get();
                        graphics_.lock()->SetTexture(index, shadowMap);
                    }
                }
            }

            if (lightPositionLoc_ != -1)
            {
                auto& position = light_->GetGlobalPosition();
                glUniform3fv(lightPositionLoc_, 1, &position[0]);
            }

            if (activeLight_ != light_ || light_->UniformsNeedUpdate())
            {
                if (lightDiffuseColorLoc_ != -1)
                {
                    Color diffuse = Color(light_->GetDiffuseColor(), 1);
                    glUniform4fv(lightDiffuseColorLoc_, 1, &diffuse[0]);
                }

                if (lightSpecularColorLoc_ != -1)
                {
                    Color specular = Color(light_->GetSpecularColor(), 1);
                    glUniform4fv(lightSpecularColorLoc_, 1, &specular[0]);
                }

                if (lightCutOffLoc_ != -1)
                {
                    float cutOff = light_->GetSpotCutOff() * 0.5f;
                    float value = Cos(Radians(cutOff));
                    glUniform1f(lightCutOffLoc_, value);
                }
            }
        }
    }

    void Program::SetVariables(bool shadowPass)
    {
        if (shadowPass)
        {
            SetSkeletonVariables();
            SetNodeVariables();
            SetLightShadowVariables(true);
            SetCameraVariables(true);
        }
        else
        {
            SetSkeletonVariables();
            SetSceneVariables();
            SetMaterialVariables();
            SetNodeVariables();
            SetCameraVariables(false);
            SetLightVariables();
            SetLightShadowVariables(false);
        }

        activeNode_ = node_;
        activeMaterial_ = material_;
        activeLight_ = light_;
        activeCamera_ = camera_;
    }

    void Program::SetSkeleton(const Skeleton* skeleton)
    {
        if (skeleton_ != skeleton)
        {
            skeleton_ = skeleton;
            Invalidate();
        }
    }

    void Program::Set(SceneNode* node)
    {
        if (node_ != node)
        {
            if (node)
            {
                node->SetUniformsNeedUpdate();
                auto armature = node->GetArmature();
                if (armature)
                    SetSkeleton(armature->GetSkeleton().get());
                else
                    SetSkeleton(nullptr);
            }
            node_ = node;
        }
    }

    void Program::Set(Material* material)
    {
        if (material_ != material)
        {
            material_ = material;
            Invalidate();
        }
    }

    void Program::Set(const Light* light)
    {
        if (light_ != light)
        {
            light_ = light;
            if (light)
                light->SetUniformsNeedUpdate();
        }
    }

    void Program::Set(const Camera* camera)
    {
        if (camera_ != camera)
        {
            camera_ = camera;
            if (camera)
                camera->SetUniformsNeedUpdate();
        }
    }

	void Program::Set(const Scene* scene)
	{
		if (scene_ != scene)
		{
			scene_ = scene;
			if (scene)
				scene->SetUniformsNeedUpdate();
		}
	}

    std::string Program::GetShaderVariation(const Pass* pass, const Scene* scene, const Camera* camera, const Mesh* mesh, const Material* material, const Light* light, const SceneNode* sceneNode)
    {
        bool allowInstancing = sceneNode == nullptr;
        #if defined(IS_TARGET_MOBILE)
        std::string defines = "IS_TARGET_MOBILE\n";
        #elif defined(IS_TARGET_WEB)
        std::string defines = "IS_TARGET_WEB\n";
        #else
        std::string defines = "IS_TARGET_DESKTOP\n";
        #endif
        auto passType = pass->GetType();

        if (scene)
            scene->FillShaderDefines(defines, passType);

        if (camera)
        {
            camera->FillShaderDefines(defines, passType);
            defines += "SPLITS" + ToString(camera->GetMaxShadowSplits()) + "\n";
        }

        if (material)
            material->FillShaderDefines(defines, passType, mesh, allowInstancing);

        if (sceneNode)
            sceneNode->FillShaderDefines(defines);

        if (light)
            light->FillShaderDefines(defines, passType, material);


        return defines;
    }

}
