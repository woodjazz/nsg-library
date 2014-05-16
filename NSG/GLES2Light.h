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
#include <memory>
#include <vector>
#include "SceneNode.h"

namespace NSG
{
	class GLES2Light : public SceneNode
	{
	public:
		GLES2Light();
		~GLES2Light();
		void SetDiffuseColor(Color diffuse) { diffuse_ = diffuse; }
		const Color& GetDiffuseColor() const { return diffuse_; }
		void SetSpecularColor(Color specular) { specular_ = specular; }
		const Color& GetSpecularColor() const { return specular_; }
		void SetAttenuation(float constant, float linear, float quadratic);
		struct Attenuation
		{
		    float constant;
		    float linear;
		    float quadratic;
		};
		const Attenuation& GetAttenuation() const { return attenuation_; }
		void SetPoint();
		void SetDirectional();
		void SetSpotLight(float spotCutOff, float exponent);
		float GetSpotCutOff() const { return spotCutOff_; }
		float GetSpotExponent() const { return exponent_; }
		enum Type {POINT, DIRECTIONAL, SPOT};
		Type GetType() const { return type_; }
		typedef std::vector<GLES2Light*> Lights;
		static const Lights& GetLights();
	private:
		Type type_;
        Color diffuse_;
        Color specular_;
        Attenuation attenuation_;
        float spotCutOff_;
        float exponent_;
	};
}