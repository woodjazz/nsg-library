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
#include "Object.h"
#include "Batch.h"
#include "Util.h"
#include "MapAndVector.h"
#include "UniformsUpdate.h"
namespace NSG
{
    class Material : public Object, UniformsUpdate
    {
    public:
        Material(const std::string& name = GetUniqueName("Material"));
        ~Material();
        PMaterial Clone(const std::string& name = GetUniqueName("Clone"));
        bool SetTexture(PTexture texture);
        void SetTextMap(PTexture texture);
        PTexture GetTexture(MaterialTexture index) const;
        void SetDiffuseColor(Color color);
        Color GetDiffuseColor() const { return diffuseColor_; }
        void SetDiffuseIntensity(float intensity);
        float GetDiffuseIntensity() const { return diffuseIntensity_; }
        void SetSpecularColor(Color color);
        Color GetSpecularColor() const { return specularColor_; }
        void SetSpecularIntensity(float intensity);
        float GetSpecularIntensity() const { return specularIntensity_; }
        void SetAmbientIntensity(float intensity);
        float GetAmbientIntensity() const { return ambientIntensity_; }
        void SetShininess(float shininess);
        float GetShininess() const { return shininess_; }
        void SetUniformValue(const char* name, int value);
        int GetUniformValue(const char* name) const;
        void Save(pugi::xml_node& node);
        void SetSerializable(bool serializable) { serializable_ = serializable; }
        bool IsSerializable() const { return serializable_; }
        void SetFilterBlendMode(BlendFilterMode mode);
        BlendFilterMode GetFilterBlendMode() const { return blendFilterMode_; }
        void SetFilterBlur(const BlurFilter& data);
        void SetFilterWave(const WaveFilter& data);
        const BlurFilter& GetFilterBlur() const { return blurFilter_; }
        const WaveFilter& GetWaveFilter() const { return waveFilter_; }
        PTexture GetTextureWith(PResource resource) const;
        PInstanceBuffer GetInstanceBuffer() const { return instanceBuffer_; }
        bool IsTransparent() const;
        bool IsLighted() const;
        void SetFillMode(FillMode fillMode) { fillMode_ = fillMode; }
        FillMode GetFillMode() const { return fillMode_; }
        bool IsBatched();
        void UpdateBatchBuffer(const Batch& batch);
        void BachedNodeHasChanged();
        static void Clear();
        static PMaterial Create(const std::string& name = GetUniqueName("Material"));
        static PMaterial GetOrCreate(const std::string& name = GetUniqueName("Material"));
        static PMaterial Get(const std::string& name);
        static std::vector<PMaterial> GetMaterials();
        static PTexture GetTextureWithResource(PResource resource);
        static std::vector<PMaterial> LoadMaterials(PResource resource, const pugi::xml_node& node);
        static void SaveMaterials(pugi::xml_node& node);
        void Set(PResourceXMLNode xmlResource);
        void SetUVTransform(const Vector4& uvTransform);
        void SetBlendMode(BLEND_MODE mode) { blendMode_ = mode; }
        BLEND_MODE GetBlendMode() const { return blendMode_; }
        bool HasLightMap() const;
        void FillShaderDefines(std::string& defines, PassType passType, const Light* light, const Mesh* mesh);
        void SetRenderPass(RenderPass pass) { renderPass_ = pass; }
        RenderPass GetRenderPass() const { return renderPass_; }
        void SetBillboardType(BillboardType type) { billboardType_ = type; }
        void FlipYTextureCoords(bool enable) { flipYTextureCoords_ = enable; }
        void SetShadeless(bool shadeless); // If true, makes this material insensitive to light (but AMBIENT pass applies)
        bool IsShadeless() const { return shadeless_; }
        void SetCullFaceMode(CullFaceMode mode) { cullFaceMode_ = mode; }
        CullFaceMode GetCullFaceMode() const { return cullFaceMode_; }
        void SetFriction(float friction);
        float GetFriction() const { return friction_; }
        SignalEmpty::PSignal SigPhysicsSet() { return signalPhysicsSet_; }
        void CastShadow(bool shadow) { castShadow_ = shadow; }
        bool CastShadow() const { return castShadow_; }
        void ReceiveShadows(bool shadow) { receiveShadows_ = shadow; }
        bool ReceiveShadows() const { return receiveShadows_; }
        bool IsShadowCaster() const { return !shadeless_ && castShadow_; }
        bool HasSpecularColor() const;
    private:
        void LoadFrom(PResource resource, const pugi::xml_node& node) override;
        void SetupBlur();
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        PTexture texture_[MaterialTexture::MAX_MAPS];
        Color diffuseColor_;
        float diffuseIntensity_;
        Color specularColor_;
        float specularIntensity_;
        float ambientIntensity_;
        float shininess_;
        Vector4 uvTransform_; // uv transform for texture 0
        bool serializable_;
        BlendFilterMode blendFilterMode_;
        BlurFilter blurFilter_;
        WaveFilter waveFilter_;
        PInstanceBuffer instanceBuffer_;
        Batch lastBatch_;
        bool isBatched_;
        FillMode fillMode_;
        BLEND_MODE blendMode_;
        RenderPass renderPass_;
        BillboardType billboardType_;
        bool flipYTextureCoords_;
        PResourceXMLNode xmlResource_;
        bool shadeless_;
        CullFaceMode cullFaceMode_;
        float friction_; // rigidbody friction
        SignalEmpty::PSignal signalPhysicsSet_;
        bool castShadow_;
        bool receiveShadows_;
        static MapAndVector<std::string, Material> materials_;
        friend class Program;
    };
}