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
		void SetAmbientColor(Color ambient);
		const Color& GetAmbientColor() const { return ambient_; }
		void SetDiffuseColor(Color diffuse);
		const Color& GetDiffuseColor() const { return diffuse_; }
		void SetSpecularColor(Color specular);
		const Color& GetSpecularColor() const { return specular_; }
		void SetAttenuation(float constant, float linear, float quadratic);
		struct Attenuation
		{
		    float constant;
		    float linear;
		    float quadratic;
		};
		const Attenuation& GetAttenuation() const { return attenuation_; }
		void SetSpotCutOff(float spotCutOff); // angle in degrees
		float GetSpotCutOff() const { return spotCutOff_; }
		LightType GetType() const { return type_; }
		void SetType(LightType type);
		void Save(pugi::xml_node& node) const override;
		void Load(const pugi::xml_node& node) override;
	private:
		LightType type_;
		Color ambient_;
        Color diffuse_;
        Color specular_;
        Attenuation attenuation_;
        float spotCutOff_; // angle in degrees
	};
}