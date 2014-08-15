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

#include "GLES2Includes.h"
#include "VertexShader.h"
#include "FragmentShader.h"
#include "Resource.h"
#include "GPUObject.h"
#include "SharedPointers.h"
#include "Constants.h"
#include <memory>
#include <string>
#include <vector>

namespace NSG
{
    struct ExtraUniforms;
    class UseProgram;
    class Mesh;
    class Program : public GPUObject
    {
    public:
        Program(PResource pRVShader, PResource pRFShader);
        Program(const char* vShader, const char* fShader);
        virtual ~Program();
        bool Initialize();
        void Set(ExtraUniforms* pExtraUniforms)
        {
            pExtraUniforms_ = pExtraUniforms;
        }
        GLuint GetAttributeLocation(const std::string& name);
        GLuint GetUniformLocation(const std::string& name);
        virtual bool IsValid();
        virtual void AllocateResources();
        virtual void ReleaseResources();
        GLuint GetAttPositionLoc() const
        {
            return att_position_loc_;
        }
        GLuint GetAttTextCoordLoc() const
        {
            return att_texcoord_loc_;
        }
        GLuint GetAttNormalLoc() const
        {
            return att_normal_loc_;
        }
        GLuint GetAttColorLoc() const
        {
            return att_color_loc_;
        }
        void Use(Material* material, Node* node);
        GLuint GetId() const
        {
            return id_;
        }
    private:
        void SetSceneVariables();
        void Use(Node* node);
        void Use(Material* material);
        operator const GLuint() const
        {
            return id_;
        }
        GLuint id_;
        PVertexShader pVShader_;
        PFragmentShader pFShader_;
        PResource pRVShader_;
        PResource pRFShader_;
        const char* vShader_;
        const char* fShader_;

        ExtraUniforms* pExtraUniforms_;
        GLuint color_scene_ambient_loc_;
        GLuint color_ambient_loc_;
        GLuint color_diffuse_loc_;
        GLuint color_specular_loc_;
        GLuint shininess_loc_;
        GLuint texture0_loc_;
        GLuint texture1_loc_;
        GLuint color_loc_;

        GLuint att_texcoord_loc_;
        GLuint att_position_loc_;
        GLuint att_normal_loc_;
        GLuint att_color_loc_;

        GLuint model_inv_transp_loc_;
        GLuint v_inv_loc_;
        GLuint mvp_loc_;
        GLuint m_loc_;
        GLuint numOfLights_loc_;

        struct LightLoc
        {
            GLuint type_loc;
            GLuint position_loc;
            GLuint diffuse_loc;
            GLuint specular_loc;
            GLuint constantAttenuation_loc;
            GLuint linearAttenuation_loc;
            GLuint quadraticAttenuation_loc;
            GLuint spotCutoff_loc;
            GLuint spotExponent_loc;
            GLuint spotDirection_loc;
        };

        typedef std::vector<LightLoc> LightsLoc;
        LightsLoc lightsLoc_;
        bool hasLights_;
        size_t nLights_;
        Camera* activeCamera_;
        bool nullCameraSet_;
        Material* activeMaterial_;
        Node* activeNode_;
        Scene* activeScene_;
        Light* activeLights_[MAX_LIGHTS];

        friend class UseProgram;
    };
}
