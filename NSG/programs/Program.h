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
#include "GPUObject.h"
#include "Constants.h"
#include "FlagSet.h"
#include <string>

namespace NSG
{
    struct ExtraUniforms;
    class Program : public GPUObject
    {
    public:
        enum Flag
        {
            PER_VERTEX_LIGHTING = 1 << 0,
            PER_PIXEL_LIGHTING = 1 << 1,
            BLEND = 1 << 2,
            BLUR = 1 << 3,
            TEXT = 1 << 4,
            SHOW_TEXTURE = 1 << 5,
            STENCIL = 1 << 6,
            NORMAL_MAP = 1 << 7
        };

        typedef FlagSet<Flag> Flags;

        Program(const std::string& name = "", Flags flags = 0);
        virtual ~Program();
        void SetVertexShader(PResource resource);
        void SetFragmentShader(PResource resource);

        bool Initialize();
        void Set(ExtraUniforms* pExtraUniforms)
        {
            pExtraUniforms_ = pExtraUniforms;
        }
        GLuint GetAttributeLocation(const std::string& name);
        GLuint GetUniformLocation(const std::string& name);
        virtual bool IsValid() override;
        virtual void AllocateResources() override;
        virtual void ReleaseResources() override;
        GLuint GetAttPositionLoc() const
        {
            return att_positionLoc_;
        }
        GLuint GetAttTextCoordLoc() const
        {
			return att_texcoordLoc_;
        }
        GLuint GetAttNormalLoc() const
        {
            return att_normalLoc_;
        }
        GLuint GetAttColorLoc() const
        {
            return att_colorLoc_;
        }
        GLuint GetAttTangentLoc() const
        {
            return att_tangentLoc_;
        }
        GLuint GetAttModelMatrixLoc() const
        {
            return att_modelMatrixRow0Loc_;
        }
        GLuint GetAttNormalMatrixLoc() const
        {
            return att_normalMatrixCol0Loc_;
        }
        void SetVariables(Material* material, Node* node);
        void SetVariables(Material* material);
        GLuint GetId() const
        {
            return id_;
        }
        void Save(pugi::xml_node& node);
        static PProgram CreateFrom(const pugi::xml_node& node);
    private:
        void SetSceneVariables(Scene* scene);
        void SetCameraVariables();
        void SetNodeVariables(Node* node);
        void SetMaterialVariables(Material* material);
        void SetLightVariables(Scene* scene);

        Flags flags_;
        GLuint id_;
        ExtraUniforms* pExtraUniforms_;

        PVertexShader pVShader_;
        PFragmentShader pFShader_;

        /////////////////////////////////////
        // attributes
        /////////////////////////////////////
        GLuint att_texcoordLoc_;
        GLuint att_positionLoc_;
        GLuint att_normalLoc_;
        GLuint att_colorLoc_;
        GLuint att_tangentLoc_;
        GLuint att_modelMatrixRow0Loc_;
        GLuint att_normalMatrixCol0Loc_;
        /////////////////////////////////////


        /////////////////////////////////////
        // Uniforms
        /////////////////////////////////////
        GLuint modelLoc_;
        GLuint normalMatrixLoc_;
        GLuint viewLoc_;
        GLuint viewProjectionLoc_;
        GLuint sceneColorAmbientLoc_;
        GLuint eyeWorldPosLoc_;
        
        GLuint texture0Loc_;
        GLuint texture1Loc_;

        struct MaterialLoc
        {
            GLuint color_;
            GLuint ambient_;
            GLuint diffuse_;
            GLuint specular_;
            GLuint shininess_;
        };

        MaterialLoc materialLoc_;

        struct BaseLightLoc
        {
            GLuint diffuse_;
            GLuint specular_;
        };

        struct DirectionalLightLoc
        {
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
            BaseLightLoc base_;
            GLuint position_;
            AttenuationLoc atten_;
        };

        GLuint numPointLightsLoc_;
        static const int MAX_POINT_LIGHTS = 2;
        PointLightLoc pointLighsLoc_[MAX_POINT_LIGHTS];
        DirectionalLightLoc directionalLightLoc_;
        /////////////////////////////////////

        bool hasLights_;
        size_t nPointLights_;
        Camera* activeCamera_;
        bool neverUsed_;
        Material* activeMaterial_;
        Node* activeNode_;
        Scene* activeScene_;
        Color sceneColor_;

        Light* activePointLights_[MAX_POINT_LIGHTS];
        Matrix4 activeNodeGlobalModel_;
        struct MaterialProgram
        {
            Color color_;
            Color ambient_;
            Color diffuse_;
            Color specular_;
            float shininess_;
            MaterialProgram() : shininess_(0) {}
        } material_; //used to avoid setting the same uniform value twice

        std::string name_;
        PResource vertexShader_;
        PResource fragmentShader_;
        Graphics& graphics_;
    };
}
