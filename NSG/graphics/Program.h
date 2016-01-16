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
#pragma once

#include "Types.h"
#include "ResourceFile.h"
#include "Object.h"
#include "Constants.h"
#include "StrongFactory.h"
#include <string>

namespace NSG
{
    struct ExtraUniforms;
    class Program : public Object, public StrongFactory<std::string, Program>
    {
    public:
		Program(const std::string& defines);
        virtual ~Program();
        bool Initialize();
        GLuint GetAttributeLocation(const std::string& name);
        GLuint GetUniformLocation(const std::string& name);
        GLuint GetAttPositionLoc() const { return att_positionLoc_; }
        GLuint GetAttTextCoordLoc0() const { return att_texcoordLoc0_; }
        GLuint GetAttTextCoordLoc1() const { return att_texcoordLoc1_; }
        GLuint GetAttNormalLoc() const { return att_normalLoc_; }
        GLuint GetAttColorLoc() const { return att_colorLoc_; }
        GLuint GetAttTangentLoc() const { return att_tangentLoc_; }
        GLuint GetAttBonesIDLoc() const { return att_bonesIDLoc_; }
        GLuint GetAttBonesWeightLoc() const { return att_bonesWeightLoc_; }
        GLuint GetAttModelMatrixLoc() const { return att_modelMatrixRow0Loc_; }
        GLuint GetAttNormalMatrixLoc() const { return att_normalMatrixCol0Loc_; }
		void SetSkeleton(const Skeleton* skeleton);
        void Set(SceneNode* node);
        void Set(Material* material);
        void Set(const Light* light);
        void Set(const Camera* camera);
		void Set(const Scene* scene);
        void SetVariables(bool shadowPass);
        GLuint GetId() const { return id_; }
        Material* GetMaterial() const { return material_; }
        const std::string& GetName() const { return name_; }
        void SetNodeVariables();
		static std::string GetShaderVariation(const Pass* pass, const Scene* scene, const Camera* camera, const Mesh* mesh, const Material* material, const Light* light, const SceneNode* sceneNode);
    private:
        Matrix4 AdjustProjection(const Matrix4& m) const;
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        void SetSceneVariables();
		void SetCameraVariables(bool shadowPass);
        void SetMaterialVariables();
        void SetSkeletonVariables();
        void SetUniformLocations();
        bool ShaderCompiles(GLenum type, const std::string& buffer) const;
        void ConfigureShaders(std::string& vertexShader, std::string& fragmentShader);

        struct BaseLightLoc
        {
            GLuint diffuse_;
            GLuint specular_;
        };

        void SetBaseLightVariables(const BaseLightLoc& baseLoc);
        void SetLightVariables();
		void SetLightShadowVariables(bool shadowPass);

        std::string defines_;
        GLuint id_;

        PVertexShader pVShader_;
        PFragmentShader pFShader_;

        /////////////////////////////////////
        // attributes
        /////////////////////////////////////
        GLuint att_texcoordLoc0_;
        GLuint att_texcoordLoc1_;
        GLuint att_positionLoc_;
        GLuint att_normalLoc_;
        GLuint att_colorLoc_;
        GLuint att_tangentLoc_;
        GLuint att_bonesIDLoc_;
        GLuint att_bonesWeightLoc_;
        GLuint att_modelMatrixRow0Loc_;
        GLuint att_normalMatrixCol0Loc_;
        /////////////////////////////////////


        /////////////////////////////////////
        // Uniforms
        /////////////////////////////////////
        std::vector<GLuint> bonesBaseLoc_;
        GLuint modelLoc_;
        GLuint normalMatrixLoc_;
        GLuint viewLoc_;
        GLuint viewProjectionLoc_;
		GLuint lightViewLoc_[MAX_SPLITS];
		GLuint lightProjectionLoc_[MAX_SPLITS];
		GLuint lightViewProjectionLoc_[MAX_SPLITS];
        GLuint projectionLoc_;
        GLuint sceneColorAmbientLoc_;
        GLuint u_sceneHorizonColorLoc_;
        GLuint eyeWorldPosLoc_;

        GLuint u_fogMinIntensityLoc_;
        GLuint u_fogStartLoc_;
        GLuint u_fogEndLoc_;
        GLuint u_fogHeightLoc_;

        GLuint textureLoc_[MaterialTexture::MAX_MAPS];
        GLuint u_uvTransformLoc_[MaterialTexture::MAX_MAPS];
        

        struct MaterialLoc
        {
            GLuint diffuseColor_;
            GLuint diffuseIntensity_;
            GLuint specularColor_;
            GLuint specularIntensity_;
            GLuint ambientIntensity_;
            GLuint shininess_;
            GLuint emitIntensity_;
        };

        MaterialLoc materialLoc_;

        GLuint lightDiffuseColorLoc_;
        GLuint lightSpecularColorLoc_;
        GLuint lightInvRangeLoc_;
        GLuint lightPositionLoc_;
        GLuint lightDirectionLoc_;
        GLuint lightCutOffLoc_;
        GLuint shadowCameraZFarLoc_;
        GLuint shadowMapInvSize_;
        GLuint shadowColor_;
        GLuint shadowBias_;

        GLuint blendMode_loc_;

        struct BlurFilterLoc
        {
            GLuint blurDir_;
            GLuint blurRadius_;
            GLuint sigma_;
        } blurFilterLoc_;

        struct WavesFilterLoc
        {
            GLuint factor_;
            GLuint offset_;
        } wavesFilterLoc_;

        struct ShockWaveFilterLoc
        {
            GLuint center_;
            GLuint time_;
            GLuint params_;
        } shockWaveFilterLoc_;

        /////////////////////////////////////

        const Skeleton* activeSkeleton_;
        Node* activeNode_;
        Material* activeMaterial_;
        const Light* activeLight_;
        const Camera* activeCamera_;
        ColorRGB sceneColor_;
        const Skeleton* skeleton_;
        SceneNode* node_;
        Material* material_;
        const Light* light_;
		const Camera* camera_;
		const Scene* scene_;
        RenderingContext* graphics_;
    };
}
