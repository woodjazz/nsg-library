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
#include "SceneNode.h"

namespace NSG
{	
	class Light : public SceneNode
	{
	public:
		Light(const std::string& name);
		~Light();
		void SetEnergy(float energy);
		void SetColor(Color color);
		void EnableDiffuseColor(bool enable);
		void EnableSpecularColor(bool enable);
		void SetSpotCutOff(float spotCutOff); // angle in degrees
		float GetSpotCutOff() const { return spotCutOff_; }
		LightType GetType() const { return type_; }
		void SetType(LightType type);
		void Save(pugi::xml_node& node) const override;
		void Load(const pugi::xml_node& node) override;
		void FillShaderDefines(std::string& defines, PassType passType) const;
		static SignalLight::PSignal SignalBeingDestroy();
		bool IsVisible(const SceneNode* node) const;
		bool IsVisibleFromCurrentLightFace(const SceneNode* node) const;
		const Color& GetDiffuseColor() const { return diffuseColor_; }
		const Color& GetSpecularColor() const { return specularColor_; }
		void SetDistance(float distance);
		PFrustum GetFrustum() const;
		float GetInvRange() const { return invRange_; }
		void EnableShadows(bool enable) { shadows_ = enable; }
		bool DoShadows() const;
		const Matrix4& GetMatViewProj() const;
		FrameBuffer* GetShadowFrameBuffer() const { return shadowFrameBuffer_.get(); }
		PTexture GetShadowMap() const;
		void SetCurrentCubeShadowMapFace(TextureTarget target);
	private:
		void OnDirty() const override;
		void UpdateViewProjection() const;
		void UpdateFrustum() const;
		void CalculateColor();
		LightType type_;
		float energy_;
		Color color_;
		bool diffuse_;
		bool specular_;
        float spotCutOff_; // angle in degrees
        Color diffuseColor_; // calculated
        Color specularColor_; // calculated
        float distance_;
        float invRange_;
        bool shadows_;
        mutable int width_; // renderer window's width
		mutable int height_; // renderer window's height
        PFrameBuffer shadowFrameBuffer_;
        mutable bool frustumDirty_;
        mutable bool viewProjectionDirty_;
		mutable Matrix4 matView_;
		mutable Matrix4 matProjection_;
		mutable Matrix4 matViewProjection_;
        mutable PFrustum frustum_;
        Node dirPositiveX_;
        Node dirNegativeX_;
        Node dirPositiveY_;
        Node dirNegativeY_;
        Node dirPositiveZ_;
        Node dirNegativeZ_;
	};
}