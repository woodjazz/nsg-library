/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#pragma once

#include "Types.h"
#include "ResourceFile.h"
#include "Object.h"
#include "Constants.h"
#include "MapAndVector.h"
#include <string>

namespace NSG
{
    struct ExtraUniforms;
    class Program : public Object
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
        void Set(Mesh* mesh);
        void Set(SceneNode* node);
        void Set(Material* material);
        void Set(const Light* light);
        void SetVariables(bool shadowPass);
        GLuint GetId() const { return id_; }
        Material* GetMaterial() const { return material_; }
        const std::string& GetName() const { return name_; }
		void SetNumberBones(size_t nBones);
		void SetNodeVariables();
    private:
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        void SetSceneVariables();
        void SetCameraVariables();
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
        void SetLightShadowVariables();

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
        GLuint lightViewProjectionLoc_;
        GLuint projectionLoc_;
        GLuint sceneColorAmbientLoc_;
        GLuint eyeWorldPosLoc_;
        GLuint u_uvTransformLoc_;

		GLuint textureLoc_[MaterialTexture::MAX_MAPS];

        struct MaterialLoc
        {
            GLuint color_;
            GLuint ambient_;
            GLuint diffuse_;
            GLuint specular_;
            GLuint shininess_;
        };

        MaterialLoc materialLoc_;

        struct DirectionalLightLoc
        {
            BaseLightLoc base_;
            GLuint direction_;
            GLuint position_;
        };

        struct PointLightLoc
        {
            BaseLightLoc base_;
            GLuint position_;
        };

        struct SpotLightLoc
        {
            BaseLightLoc base_;
            GLuint position_;
            GLuint direction_;
            GLuint cutOff_;
        };

        PointLightLoc pointLightLoc_;
        DirectionalLightLoc directionalLightLoc_;
        SpotLightLoc spotLightLoc_;
        GLuint lightInvRangeLoc_;
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


        /////////////////////////////////////

		size_t nBones_;
        Camera* activeCamera_;
        bool viewVariablesNeverSet_;
        bool materialVariablesNeverSet_;
		Skeleton* activeSkeleton_;
        Node* activeNode_;
        Scene* activeScene_;
        Color sceneColor_;

        Mesh* mesh_;
        SceneNode* node_;
        Material* material_;
        const Light* light_;
    };
}
