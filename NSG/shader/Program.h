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
#pragma once

#include "Types.h"
#include "ResourceFile.h"
#include "Object.h"
#include "Constants.h"

namespace NSG
{
    struct ExtraUniforms;
    class Program : public Object
    {
    public:
		Program(Material* material);
        virtual ~Program();
        void SetVertexShader(PResource resource);
        void SetFragmentShader(PResource resource);
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
        void SetVariables(Mesh* mesh, Node* node);
        GLuint GetId() const { return id_; }
        void Save(pugi::xml_node& node);
        const ProgramFlags& GetFlags() const { return flags_; }
        void SetFlags(const ProgramFlags& flags);
        void EnableFlags(const ProgramFlags& flags);
        void DisableFlags(const ProgramFlags& flags);
        PProgram Clone(Material* material) const;
        Material* GetMaterial() const { return material_; }
    private:
        size_t GetNeededVarying() const;
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        void SetSceneVariables(Scene* scene);
        void SetCameraVariables();
        void SetNodeVariables(Node* node);
        void SetMaterialVariables();
		bool SetSkeletonVariables(Skeleton* skeleton);
        bool HasLighting() const;

        struct BaseLightLoc
        {
            GLuint ambient_;
            GLuint diffuse_;
            GLuint specular_;
        };

        void SetBaseLightVariables(const BaseLightLoc& baseLoc, const Light* light);
        bool SetLightVariables(Scene* scene);

        Material* material_;
        ProgramFlags flags_;
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
		std::vector<GLuint> bonesLoc_;
        GLuint modelLoc_;
        GLuint normalMatrixLoc_;
        GLuint viewLoc_;
        GLuint viewProjectionLoc_;
        GLuint sceneColorAmbientLoc_;
        GLuint eyeWorldPosLoc_;

        GLuint texture0Loc_;
        GLuint texture1Loc_;
        GLuint texture2Loc_;
        GLuint texture3Loc_;
        GLuint texture4Loc_;
        GLuint texture5Loc_;

        struct MaterialLoc
        {
            GLuint color_;
            GLuint ambient_;
            GLuint diffuse_;
            GLuint specular_;
            GLuint shininess_;
            GLuint parallaxScale_;
        };

        MaterialLoc materialLoc_;

        struct DirectionalLightLoc
        {
            GLuint enabled_;
            BaseLightLoc base_;
            GLuint direction_;
        };

        struct AttenuationLoc
        {
            GLuint constant_;
            GLuint linear_;
            GLuint quadratic_;
        };

        struct PointLightLoc
        {
            GLuint enabled_;
            BaseLightLoc base_;
            GLuint position_;
            AttenuationLoc atten_;
        };

        struct SpotLightLoc
        {
			GLuint enabled_;
            BaseLightLoc base_;
            GLuint position_;
            AttenuationLoc atten_;
            GLuint direction_;
            GLuint cutOff_;
        };

        std::vector<PointLightLoc> pointLightsLoc_;
        std::vector<DirectionalLightLoc> directionalLightsLoc_;
        std::vector<SpotLightLoc> spotLightsLoc_;

        GLuint blendMode_loc_;
        struct BlurFilterLoc
        {
            GLuint blurDir_;
            GLuint blurRadius_;
            GLuint sigma_;
        };
        BlurFilterLoc blurFilterLoc_;

        /////////////////////////////////////

		size_t nBones_;
        size_t nDirectionalLights_;
        size_t nPointLights_;
        size_t nSpotLights_;
        Camera* activeCamera_;
        bool viewVariablesNeverSet_;
        bool materialVariablesNeverSet_;
		Skeleton* activeSkeleton_;
        Node* activeNode_;
        Scene* activeScene_;
        Color sceneColor_;

        std::vector<const Light*> activeDirectionalLights_;
		std::vector<const Light*> activePointLights_;
		std::vector<const Light*> activeSpotLights_;

        std::string name_;
        PResource vertexShader_;
        PResource fragmentShader_;
        Graphics& graphics_;
        bool spotLightsReduced_;
        bool directionalLightsReduced_;
        bool pointLightsReduced_;
    };
}
