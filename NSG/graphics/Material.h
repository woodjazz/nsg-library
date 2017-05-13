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
#include "UniformsUpdate.h"
#include "Util.h"
#include "Vector2.h"
#include "Vector3.h"
#include "WeakFactory.h"
namespace NSG {
struct BlurFilter {
    Vector2 blurDir_;
    /// This should usually be equal to 1.0f / texture_pixel_width for a
    /// horizontal blur, and
    /// 1.0f / texture_pixel_height for a vertical blur.
    Vector2 blurRadius_; // calculated in Material::IsValid()
    float sigma_;
    BlurFilter();
    bool operator!=(const BlurFilter& obj) const;
};

struct WaveFilter {
    float factor_;
    float offset_;
    WaveFilter();
    bool operator!=(const WaveFilter& obj) const;
};

struct ShockWaveFilter {
    Vector2 center_;
    float time_;
    Vector3 params_;
    ShockWaveFilter();
    bool operator!=(const ShockWaveFilter& obj) const;
};

class Material : public Object,
                 public std::enable_shared_from_this<Material>,
                 public WeakFactory<std::string, Material>,
                 UniformsUpdate {
public:
    Material(const std::string& name = GetUniqueName("Material"));
    ~Material();
    PMaterial Clone(const std::string& name = GetUniqueName("Clone"));
    void SetFontAtlas(PFontAtlas fontAtlas);
    PFontAtlas GetFontAtlas() const { return fontAtlas_; }
    void SetTextMap(PTexture texture);
    bool SetTexture(PTexture texture);
    bool SetTexture(MaterialTexture index, PTexture texture);
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
    void SetFilterShockWave(const ShockWaveFilter& data);
    const BlurFilter& GetFilterBlur() const { return blurFilter_; }
    const WaveFilter& GetWaveFilter() const { return waveFilter_; }
    const ShockWaveFilter& GetShockWaveFilter() const {
        return shockWaveFilter_;
    }
    PTexture GetTextureWith(PResource resource) const;
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
    static PTexture GetTextureWithResource(PResource resource);
    static void SaveMaterials(pugi::xml_node& node);
    bool HasLightMap() const;
    void FillShaderDefines(std::string& defines, PassType passType,
                           const Mesh* mesh, bool allowInstancing) const;
    void SetRenderPass(RenderPass pass) { renderPass_ = pass; }
    RenderPass GetRenderPass() const { return renderPass_; }
    void SetBillboardType(BillboardType type) { billboardType_ = type; }
    BillboardType GetBillboardType() const { return billboardType_; }
    void FlipYTextureCoords(bool enable) { flipYTextureCoords_ = enable; }
    bool IsYFlipped() const { return flipYTextureCoords_; }
    void SetShadeless(bool shadeless); // If true, makes this material
                                       // insensitive to light (but AMBIENT pass
                                       // applies)
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
    void SetSlopeScaledBias(float slopeScaledBias) {
        slopeScaledBias_ = slopeScaledBias;
    }
    float GetSlopeScaledBias() const { return slopeScaledBias_; }
    bool HasTextures() const;

private:
    PTexture ShowPreview(FrameBuffer* frameBuffer, Scene* scene);
    void SetupBlur();
    bool IsValid() override;
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
    ShockWaveFilter shockWaveFilter_;
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
    float shadowBias_; // factor to multiply shadow buffer bias with (see
                       // Light::shadowBias_)
    float slopeScaledBias_;
    PFontAtlas fontAtlas_;
    friend class Program;
};
}
