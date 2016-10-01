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
#include "SceneNode.h"
#include "Constants.h"

namespace NSG
{
    class Light : public SceneNode
    {
    public:
        Light(const std::string& name);
        ~Light();
        void SetEnergy(float energy);
        float GetEnergy() const;
        void SetColor(ColorRGB color);
        const ColorRGB& GetColor() const;
        void EnableDiffuseColor(bool enable);
        void EnableSpecularColor(bool enable);
        void SetSpotCutOff(float spotCutOff); // angle in degrees
        float GetSpotCutOff() const { return spotCutOff_; } // angle in degrees
        LightType GetType() const { return type_; }
        void SetType(LightType type);
        void Save(pugi::xml_node& node) const override;
        void Load(const pugi::xml_node& node) override;
        void FillShaderDefines(std::string& defines, PassType passType, const Material* material) const;
        static SignalLight::PSignal SignalBeingDestroy();
        SignalLight::PSignal SignalSetType() { return signalSetType_; }
        const ColorRGB& GetDiffuseColor() const { return diffuseColor_; }
        const ColorRGB& GetSpecularColor() const { return specularColor_; }
        void SetShadowColor(Color color);
        const Color& GetShadowColor() const { return shadowColor_; }
        void SetDistance(float distance);
        float GetDistance() const { return distance_; }
        void EnableShadows(bool enable) { shadows_ = enable; }
        bool DoShadows() const;
        PTexture GetShadowMap(int idx) const;
        float GetShadowClipStart() const { return shadowClipStart_; }
        float GetShadowClipEnd() const { return shadowClipEnd_; }
        void SetShadowClipStart(float value);
        void SetShadowClipEnd(float value);
        void SetOnlyShadow(bool onlyShadow) { onlyShadow_ = onlyShadow; }
        bool GetOnlyShadow() const { return onlyShadow_; }
        void SetBias(float shadowBias) { shadowBias_ = shadowBias; }
        float GetBias() const { return shadowBias_; }
        void SetSlopeScaledBias(float slopeScaledBias) { slopeScaledBias_ = slopeScaledBias; }
        float GetSlopeScaledBias() const { return slopeScaledBias_; }
        ShadowCamera* GetShadowCamera(int idx) const;
        bool HasSpecularColor() const;
        void SetShadowSplits(int splits) const { shadowSplits_ = splits; }
        int GetShadowSplits() const {return shadowSplits_; }
        float GetInvRange() const { return invRange_; }
		bool IsDiffuseEnabled() const { return diffuse_; }
        bool IsSpecularEnabled() const { return specular_; }
        FrameBuffer* GetShadowFrameBuffer(int idx) const;
    private:
        void CalculateColor();
        void CalculateRange();
    private:
        LightType type_;
        float energy_;
        ColorRGB color_;
        bool diffuse_;
        bool specular_;
        float spotCutOff_; // angle in degrees
        ColorRGB diffuseColor_; // calculated
        ColorRGB specularColor_; // calculated
        Color shadowColor_;
        float distance_;
        bool shadows_;
        float shadowClipStart_;
        float shadowClipEnd_;
        bool onlyShadow_;
        PFrameBuffer shadowFrameBuffer_[MAX_SPLITS];
        // Bias is used to add a slight offset distance between an object and the shadows cast by it.
        float shadowBias_; // final bias is multiplied by material bias (See Material::shadowBias_)
        float slopeScaledBias_;
        PShadowCamera shadowCamera_[MAX_SPLITS];
        mutable int shadowSplits_; //Calculated in the shadow pass
        float invRange_;
        SignalLight::PSignal signalSetType_;
    };
}
