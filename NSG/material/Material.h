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

#include "SharedPointers.h"
#include "GPUObject.h"
#include "Program.h"
#include "Types.h"
#include "UniformsUpdate.h"
#include <vector>

namespace NSG
{
	class UseMaterial;
	class Material : public GPUObject, UniformsUpdate
	{
	public:
		Material();
		~Material();
		void EnableCullFace(bool enable);
		void SetProgram(PProgram pProgram);
		PProgram GetProgram() const { return pProgram_; }
		void SetTexture0(PTexture pTexture);
		void SetTexture1(PTexture pTexture);
        PTexture GetTexture0() const { return pTexture0_; }
        PTexture GetTexture1() const { return pTexture1_; }
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
		void SetUniformValue(const char* name, int value);
		int GetUniformValue(const char* name) const;
		virtual bool IsValid() override;
		virtual void AllocateResources() override;
		virtual void ReleaseResources() override;
		void Use();
	private:
		PTexture pTexture0_;
		PTexture pTexture1_;
		PProgram pProgram_;

        Color ambient_;
        Color diffuse_;
        Color specular_;
        float shininess_;

        Color color_;

        bool enableCullFace_;

		friend class UseMaterial;
		friend class Program;
	};
}