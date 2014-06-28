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
#include "ShowTexture.h"
#include "Types.h"
#include "Material.h"
#include "PlaneMesh.h"
#include "Context.h"
#include "Camera.h"
#include "Pass.h"

static const char* vShader = STRINGIFY(
	attribute vec4 a_position;
	attribute vec2 a_texcoord;
	varying vec2 v_texcoord;

	void main()
	{
		gl_Position = a_position;
		v_texcoord = vec2(a_texcoord.x, 1.0 - a_texcoord.y);
	}
);

static const char* fShader = STRINGIFY(
	uniform sampler2D u_texture0;
	varying vec2 v_texcoord;
	void main()
	{
		gl_FragColor = texture2D(u_texture0, v_texcoord);
	}
);

static const char* fFontShader = STRINGIFY(
	uniform sampler2D u_texture0;
	varying vec2 v_texcoord;
	void main()
	{
        gl_FragColor = vec4(1, 1, 1, texture2D(u_texture0, v_texcoord).a);
	}
);


namespace NSG
{
	ShowTexture::ShowTexture()
	: material_(new Material),
    pass_(new Pass),
	mesh_(new PlaneMesh(2, 2, 2, 2, GL_STATIC_DRAW))
	{
		pass_->EnableDepthTest(false);
        pass_->Set(material_);
        pass_->Add(nullptr, mesh_);
	}

	ShowTexture::~ShowTexture()
	{
        pass_ = nullptr;
		Context::this_->Remove(this);
	}

	bool ShowTexture::IsValid()
	{
		return material_->IsReady() && mesh_->IsReady();
	}

	void ShowTexture::AllocateResources()
	{

	}

	void ShowTexture::ReleaseResources()
	{

	}

	void ShowTexture::SetNormal(PTexture texture)
	{
		PProgram pProgram(new Program(vShader, fShader));
		material_->SetProgram(pProgram);
		material_->SetTexture0(texture);
	}

	void ShowTexture::SetFont(PTexture texture)
	{
		PProgram pProgram(new Program(vShader, fFontShader));
		material_->SetProgram(pProgram);
		material_->SetTexture0(texture);
	}

	void ShowTexture::Show()
	{
		if(IsReady())
		{
			CHECK_GL_STATUS(__FILE__, __LINE__);

			Camera* pCurrent = Camera::Deactivate();

			pass_->Render();

			Camera::Activate(pCurrent);

			CHECK_GL_STATUS(__FILE__, __LINE__);
		}

	}
}