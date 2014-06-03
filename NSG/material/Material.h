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
#include <vector>

namespace NSG
{
	class UseMaterial;
	class Material : public GPUObject
	{
	public:
		Material();
		~Material();
		void SetBlendMode(BLEND_MODE mode);
		void EnableColorBuffer(bool enable);
		void EnableDepthBuffer(bool enable);
		void EnableDepthTest(bool enable);
		void EnableCullFace(bool enable);
		void EnableStencilTest(bool enable);
		void SetStencilMask(GLuint mask);
		void SetStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
		void SetStencilFunc(GLenum func, GLint ref, GLuint mask);
		void SetProgram(PProgram pProgram);
		PProgram GetProgram() const { return pProgram_; }
		void SetTexture0(PTexture pTexture);
		void SetTexture1(PTexture pTexture);
        PTexture GetTexture0() const { return pTexture0_; }
        PTexture GetTexture1() const { return pTexture1_; }
		void SetDiffuseColor(Color diffuse) { diffuse_ = diffuse; }
		Color GetDiffuseColor() const { return diffuse_; }
		void SetSpecularColor(Color specular) { specular_ = specular; }
		Color GetSpecularColor() const { return specular_; }
		void SetAmbientColor(Color ambient) { ambient_ = ambient; }
		Color GetAmbientColor() const { return ambient_; }
		void SetShininess(float shininess) { shininess_=shininess; }
		float GetShininess() const { return shininess_; }
		void SetUniformValue(const char* name, int value);
		int GetUniformValue(const char* name) const;
		void Render(bool solid, Node* pNode, Mesh* pMesh);
		void Render(bool solid, const std::vector<MeshNode>& meshNodes);

		virtual bool IsValid();
		virtual void AllocateResources();
		virtual void ReleaseResources();

	private:
		void Use();
		
		PTexture pTexture0_;
		PTexture pTexture1_;
		PProgram pProgram_;

        Color ambient_;
        Color diffuse_;
        Color specular_;
        float shininess_;

        BLEND_MODE blendMode_;
        bool enableDepthTest_;
        bool enableCullFace_;
        bool enableStencilTest_;
        GLuint stencilMask_;
        GLenum sfailStencilOp_;
        GLenum dpfailStencilOp_;
        GLenum dppassStencilOp_;
        GLenum stencilFunc_;
        GLint stencilRefValue_;
        GLuint stencilMaskValue_;
        bool enableColorBuffer_;
        bool enableDepthBuffer_;

		friend class UseMaterial;
		friend class Program;
	};
}