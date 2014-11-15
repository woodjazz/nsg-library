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
#include "GPUObject.h"
#include "UniformsUpdate.h"
namespace NSG
{
	class Material : public GPUObject, UniformsUpdate
	{
	public:
		Material(const std::string& name);
		~Material();
		void SetName(const std::string& name) {name_ = name;}
		const std::string& GetName() const { return name_;  }
		bool SetTexture0(PTexture texture); 
		bool SetTexture1(PTexture texture); 
		bool SetTexture2(PTexture texture);
		bool SetTexture3(PTexture texture);
		bool SetTexture4(PTexture texture);
		bool SetTexture5(PTexture texture);
		void SetDiffuseMap(PTexture texture);
		void SetNormalMap(PTexture texture);
		void SetLightMap(PTexture texture);
		void SetSpecularMap(PTexture texture);
		void SetAOMap(PTexture texture);
		void SetDisplacementMap(PTexture texture);
        PTexture GetTexture0() const { return texture0_; }
        PTexture GetTexture1() const { return texture1_; }
        PTexture GetTexture2() const { return texture2_; }
        PTexture GetTexture3() const { return texture3_; }
        PTexture GetTexture4() const { return texture4_; }
        PTexture GetDiffuseMap() const { return texture0_; }
        PTexture GetNormalMap() const { return texture1_; }
        PTexture GetLightMap() const { return texture2_; }
        PTexture GetSpecularMap() const { return texture3_; }
        PTexture GetAOMap() const { return texture4_; }
		PTexture GetDisplacementMap() const { return texture5_; }
        void SetColor(Color color);
        Color GetColor() const { return color_; }
		void SetDiffuseColor(Color diffuse);
		Color GetDiffuseColor() const { return diffuse_; }
		void SetSpecularColor(Color specular);
		Color GetSpecularColor() const { return specular_; }
		void SetAmbientColor(Color ambient);
		Color GetAmbientColor() const { return ambient_; }
		void SetShininess(float shininess);
		float GetShininess() const { return shininess_; }
		void SetParallaxScale(float parallaxScale);
		float GetParallaxScale() const { return parallaxScale_; }
		void SetUniformValue(const char* name, int value);
		int GetUniformValue(const char* name) const;
		virtual bool IsValid() override;
		void SetTechnique(PTechnique technique) { technique_ = technique; }
		PTechnique GetTechnique() const { return technique_; }
		void Save(pugi::xml_node& node);
		void Load(const pugi::xml_node& node);
		void SetSerializable(bool serializable) { serializable_ = serializable; }
		bool IsSerializable() const { return serializable_; }
	private:
		PTexture texture0_; //difusse map
		PTexture texture1_; //normal map
		PTexture texture2_; //light map
		PTexture texture3_; //specular map
		PTexture texture4_; //AO map
		PTexture texture5_; //Displacement map
        Color ambient_;
        Color diffuse_;
        Color specular_;
        float shininess_;
        float parallaxScale_; //used with displacement map
        Color color_;
        PTechnique technique_;
        std::string name_;
        bool serializable_;
        friend class Program;
	};
}