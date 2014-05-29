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
#include "IMGUISkin.h"
#include "Technique.h"
#include "Pass.h"
#include "Pass2Stencil.h"
#include "Material.h"
#include "RoundedRectangleMesh.h"

static const char* vShader = STRINGIFY(
	uniform mat4 u_mvp;
	attribute vec4 a_position;
	attribute vec2 a_texcoord;
	varying vec4 v_position;
	varying vec2 v_texcoord;
	
	void main()
	{
		gl_Position = u_mvp * vec4(a_position.xyz, 1);
		v_position = a_position;
		v_texcoord = a_texcoord;
	}
);

static const char* fShader = STRINGIFY(
	struct Material
	{
		vec4 diffuse;
		float shininess;
	};
	uniform Material u_material;
	uniform sampler2D u_texture0;
	varying vec4 v_position;
	varying vec2 v_texcoord;

	void main()
	{
		float factor = u_material.shininess - abs(v_position.y);

        if(u_material.shininess < 0.0)
            factor = abs(v_position.y) + u_material.shininess;

		gl_FragColor = texture2D(u_texture0, v_texcoord) * u_material.diffuse * vec4(factor, factor, factor, 1.0);
	}
);

static const char* fShaderBorder = STRINGIFY(
	struct Material
	{
		vec4 diffuse;
	};

	uniform Material u_material;

	void main()
	{
		gl_FragColor = u_material.diffuse;
	}
);


namespace NSG 
{
	namespace IMGUI
	{
		Skin::Skin() 
		: alphaFactor_(1),
		fontSize_(18),
		textMaxLength_(std::numeric_limits<int>::max()),
        activeTechnique_(new Technique),
        normalTechnique_(new Technique),
        hotTechnique_(new Technique),
        labelTechnique_(new Technique)
		{
			PMaterial pActiveMaterial(new Material);
			pActiveMaterial->SetBlendMode(ALPHA);
			pActiveMaterial->EnableDepthTest(false);
            PProgram pProgram(new Program(vShader, fShader));
            pActiveMaterial->SetProgram(pProgram);
			pActiveMaterial->SetDiffuseColor(Color(1,0,0,0.7f));
			pActiveMaterial->SetShininess(1);

			PMaterial pNormalMaterial(new Material);
			pNormalMaterial->SetBlendMode(ALPHA);
			pNormalMaterial->EnableDepthTest(false);
            pNormalMaterial->SetProgram(pProgram);
			pNormalMaterial->SetDiffuseColor(Color(0,1,0,0.7f));
			pNormalMaterial->SetShininess(1);

			PMaterial pHotMaterial(new Material);
			pHotMaterial->SetBlendMode(ALPHA);
			pHotMaterial->EnableDepthTest(false);
			pHotMaterial->SetProgram(pProgram);
			pHotMaterial->SetDiffuseColor(Color(0,0,1,0.7f));
			pHotMaterial->SetShininess(1);

			PMaterial pBorderMaterial(new Material);
			PProgram pBorderProgram(new Program(vShader, fShaderBorder));
			pBorderMaterial->SetBlendMode(ALPHA);
			pBorderMaterial->EnableDepthTest(false);
			pBorderMaterial->SetProgram(pBorderProgram);
			pBorderMaterial->SetDiffuseColor(Color(1,1,1,1));

            PMesh pMesh(new RoundedRectangleMesh(0.5f, 2, 2, 64, GL_STATIC_DRAW));

			PPass activePass(new Pass);
			activePass->Set(pActiveMaterial);
			activePass->Add(nullptr, pMesh);

			PPass normalPass(new Pass);
			normalPass->Set(pNormalMaterial);
			normalPass->Add(nullptr, pMesh);

			PPass hotPass(new Pass);
			hotPass->Set(pHotMaterial);
			hotPass->Add(nullptr, pMesh);

			PPass borderPass(new Pass);
			borderPass->Set(pBorderMaterial);
			borderPass->Add(nullptr, pMesh);
			borderPass->SetDrawMode(Pass::WIREFRAME);

			PPass stencilPass(new Pass2Stencil);
			stencilPass->Add(nullptr, pMesh);

			activeTechnique_->Add(activePass);
			activeTechnique_->Add(borderPass);
			activeTechnique_->Add(stencilPass);

			normalTechnique_->Add(normalPass);
			normalTechnique_->Add(borderPass);
			normalTechnique_->Add(stencilPass);

			hotTechnique_->Add(hotPass);
			hotTechnique_->Add(borderPass);
			hotTechnique_->Add(stencilPass);

			PMaterial labelMaterial(new Material);
			labelMaterial->SetBlendMode(ALPHA);
			labelMaterial->EnableDepthTest(false);
            labelMaterial->SetProgram(pProgram);
			labelMaterial->SetDiffuseColor(Color(0,0,0,0.0f));
			labelMaterial->SetShininess(1);

			PPass labelPass(new Pass);
			labelPass->Set(labelMaterial);
			labelPass->Add(nullptr, pMesh);

			labelTechnique_->Add(labelPass);
			labelTechnique_->Add(stencilPass);
		}

		Skin::Skin(const Skin& obj)
		: alphaFactor_(obj.alphaFactor_),
		fontFile_(obj.fontFile_),
		fontSize_(obj.fontSize_),
		textMaxLength_(obj.textMaxLength_),
		activeTechnique_(obj.activeTechnique_),
		normalTechnique_(obj.normalTechnique_),
		hotTechnique_(obj.hotTechnique_),
		labelTechnique_(obj.labelTechnique_)
		{
		}
	}
}