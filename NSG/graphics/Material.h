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
#include "WeakFactory.h"
#include "UniformsUpdate.h"
namespace NSG
{
	class Material : public Object, public std::enable_shared_from_this<Material>, public WeakFactory<std::string, Material>, UniformsUpdate
    {
    public:
        Material(const std::string& name = GetUniqueName("Material"));
        ~Material();
        PMaterial Clone(const std::string& name = GetUniqueName("Clone"));
        bool SetTexture(PTexture texture);
        void SetTextMap(PTexture texture);
        PTexture GetTexture(MaterialTexture index) const;
        void SetDiffuseColor(ColorRGB color);
		Color GetDiffuseColor() const { return diffuseColor_; }
        void SetDiffuseIntensity(float intensity);
        float GetDiffuseIntensity() const { return diffuseIntensity_; }
		void SetSpecularColor(ColorRGB color);
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
        void SetAlpha(float alpha);
		float GetAlpha() const { return alpha_; }
		void SetAlphaForSpecular(float alphaForSpecular);
        void SetEmitIntensity(float emitIntensity);
        float GetEmitIntensity() const { return emitIntensity_; }
        bool IsTransparent() const;
        void EnableTransparent(bool enable);
        bool IsLighted() const;
        void SetFillMode(FillMode fillMode) { fillMode_ = fillMode; }
        FillMode GetFillMode() const { return fillMode_; }
        bool IsBatched() const;
        void UpdateBatchBuffer(const Batch& batch);
        void BachedNodeHasChanged();
        static PTexture GetTextureWithResource(PResource resource);
        static void SaveMaterials(pugi::xml_node& node);
        bool HasLightMap() const;
		void FillShaderDefines(std::string& defines, PassType passType, const Mesh* mesh, bool allowInstancing) const;
        void SetRenderPass(RenderPass pass) { renderPass_ = pass; }
        RenderPass GetRenderPass() const { return renderPass_; }
        void SetBillboardType(BillboardType type) { billboardType_ = type; }
		BillboardType GetBillboardType() const { return billboardType_;  }
        void FlipYTextureCoords(bool enable) { flipYTextureCoords_ = enable; }
		bool IsYFlipped() const { return flipYTextureCoords_; }
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
		void Load(const pugi::xml_node& node) override;
        void SetBias(float shadowBias) { shadowBias_ = shadowBias; }
        float GetBias() const { return shadowBias_; }
        void SetSlopeScaledBias(float slopeScaledBias) { slopeScaledBias_ = slopeScaledBias; }
        float GetSlopeScaledBias() const { return slopeScaledBias_; }
		bool HasTextures() const;
        void ShowGUIProperties(Editor* editor);
    private:
		PTexture ShowPreview(FrameBuffer* frameBuffer, Scene* scene);
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
        bool serializable_;
        BlendFilterMode blendFilterMode_;
        BlurFilter blurFilter_;
        WaveFilter waveFilter_;
        PInstanceBuffer instanceBuffer_;
        Batch lastBatch_;
        bool isBatched_;
        FillMode fillMode_;
        float alpha_;
		float alphaForSpecular_;
        bool isTransparent_;
        float emitIntensity_;
        RenderPass renderPass_;
        BillboardType billboardType_;
        bool flipYTextureCoords_;
        bool shadeless_;
        CullFaceMode cullFaceMode_;
        float friction_; // rigidbody friction
        SignalEmpty::PSignal signalPhysicsSet_;
        bool castShadow_;
        bool receiveShadows_;
        float shadowBias_; // factor to multiply shadow buffer bias with (see Light::shadowBias_)
        float slopeScaledBias_;
        friend class Program;
    };
}