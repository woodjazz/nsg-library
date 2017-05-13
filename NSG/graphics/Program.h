/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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

#include "Color.h"
#include "Object.h"
#include "ResourceFile.h"
#include "ShadowCamera.h"
#include "StrongFactory.h"
#include "Types.h"
#include <string>

namespace NSG {
struct ExtraUniforms;
class Program : public Object, public StrongFactory<std::string, Program> {
public:
    Program(const std::string& defines);
    virtual ~Program();
    bool Initialize();
    GLint GetAttributeLocation(const std::string& name);
    GLint GetUniformLocation(const std::string& name);
    GLint GetAttPositionLoc() const { return att_positionLoc_; }
    GLint GetAttTextCoordLoc0() const { return att_texcoordLoc0_; }
    GLint GetAttTextCoordLoc1() const { return att_texcoordLoc1_; }
    GLint GetAttNormalLoc() const { return att_normalLoc_; }
    GLint GetAttColorLoc() const { return att_colorLoc_; }
    GLint GetAttTangentLoc() const { return att_tangentLoc_; }
    GLint GetAttBonesIDLoc() const { return att_bonesIDLoc_; }
    GLint GetAttBonesWeightLoc() const { return att_bonesWeightLoc_; }
    GLint GetAttModelMatrixLoc() const { return att_modelMatrixRow0Loc_; }
    GLint GetAttNormalMatrixLoc() const { return att_normalMatrixCol0Loc_; }
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
    static std::string GetShaderVariation(const Pass* pass, const Scene* scene,
                                          const Camera* camera,
                                          const Mesh* mesh,
                                          const Material* material,
                                          const Light* light,
                                          const SceneNode* sceneNode);

private:
    bool ReduceShaderComplexity();
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
    void ConfigureShaders(std::string& vertexShader,
                          std::string& fragmentShader);

    struct BaseLightLoc {
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
    GLint att_texcoordLoc0_;
    GLint att_texcoordLoc1_;
    GLint att_positionLoc_;
    GLint att_normalLoc_;
    GLint att_colorLoc_;
    GLint att_tangentLoc_;
    GLint att_bonesIDLoc_;
    GLint att_bonesWeightLoc_;
    GLint att_modelMatrixRow0Loc_;
    GLint att_normalMatrixCol0Loc_;
    /////////////////////////////////////

    /////////////////////////////////////
    // Uniforms
    /////////////////////////////////////
    std::vector<GLuint> bonesBaseLoc_;
    GLint modelLoc_;
    GLint normalMatrixLoc_;
    GLint viewLoc_;
    GLint viewProjectionLoc_;
    GLint lightViewLoc_[ShadowCamera::MAX_SPLITS];
    GLint lightProjectionLoc_[ShadowCamera::MAX_SPLITS];
    GLint lightViewProjectionLoc_[ShadowCamera::MAX_SPLITS];
    GLint projectionLoc_;
    GLint sceneColorAmbientLoc_;
    GLint u_sceneHorizonColorLoc_;
    GLint eyeWorldPosLoc_;

    GLint u_fogMinIntensityLoc_;
    GLint u_fogStartLoc_;
    GLint u_fogEndLoc_;
    GLint u_fogHeightLoc_;

    GLint textureLoc_[MaterialTexture::MAX_MAPS];
    GLint u_uvTransformLoc_[MaterialTexture::MAX_MAPS];

    struct MaterialLoc {
        GLint diffuseColor_;
        GLint diffuseIntensity_;
        GLint specularColor_;
        GLint specularIntensity_;
        GLint ambientIntensity_;
        GLint shininess_;
        GLint emitIntensity_;
    };

    MaterialLoc materialLoc_;

    GLint lightDiffuseColorLoc_;
    GLint lightSpecularColorLoc_;
    GLint lightInvRangeLoc_;
    GLint lightPositionLoc_;
    GLint lightDirectionLoc_;
    GLint lightCutOffLoc_;
    GLint shadowCameraZFarLoc_;
    GLint shadowMapInvSize_;
    GLint shadowColor_;
    GLint shadowBias_;

    GLint blendMode_loc_;

    struct BlurFilterLoc {
        GLint blurDir_;
        GLint blurRadius_;
        GLint sigma_;
    } blurFilterLoc_;

    struct WavesFilterLoc {
        GLint factor_;
        GLint offset_;
    } wavesFilterLoc_;

    struct ShockWaveFilterLoc {
        GLint center_;
        GLint time_;
        GLint params_;
    } shockWaveFilterLoc_;

    /////////////////////////////////////

    const Skeleton* activeSkeleton_;
    Node* activeNode_;
    Material* activeMaterial_;
    const Light* activeLight_;
    const Camera* activeCamera_;
    Color sceneColor_;
    const Skeleton* skeleton_;
    SceneNode* node_;
    Material* material_;
    const Light* light_;
    const Camera* camera_;
    const Scene* scene_;
};
}
