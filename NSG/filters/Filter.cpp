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
#include "Filter.h"
#include "PlaneMesh.h"
#include "Material.h"
#include "Camera.h"
#include "Check.h"
#include "Technique.h"
#include "Pass.h"
#include "Render2Texture.h"

static const char* vShader = STRINGIFY(
	attribute vec4 a_position;
	attribute vec2 a_texcoord;
	varying vec2 v_texcoord;

	void main()
	{
		gl_Position = a_position;
		v_texcoord = a_texcoord;
	}
);

namespace NSG
{
	Filter::Filter(PTexture input, PTexture output, const char* fragment)
    : pMaterial_(new Material ()),
    pMesh_(new PlaneMesh(2, 2, 2, 2, GL_STATIC_DRAW)),
    technique_(new Technique)
	{
		PProgram pProgram(new Program(vShader, fragment));
		pMaterial_->SetProgram(pProgram);
		pMaterial_->SetTexture0(input);

		pRender2Texture_ = PRender2Texture(new Render2Texture(output, true, false));

		PPass pass(new Pass);
		pass->Set(pMaterial_);
		pass->Add(nullptr, pMesh_);
		technique_->Add(pass);
	}

	Filter::~Filter()
	{

	}

	void Filter::Render()
	{
		if(pMesh_->IsReady() && pMaterial_->IsReady() && pRender2Texture_->IsReady())
		{
            CHECK_GL_STATUS(__FILE__, __LINE__);

			Camera* pCurrent = Camera::Deactivate();

			pRender2Texture_->Begin();

			technique_->Render();

			pRender2Texture_->End();

			Camera::Activate(pCurrent);

			CHECK_GL_STATUS(__FILE__, __LINE__);
		}
	}
}
