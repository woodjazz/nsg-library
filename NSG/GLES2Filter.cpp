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
#include "GLES2Filter.h"
#include "GLES2PlaneMesh.h"
#include "GLES2Camera.h"
#include "Check.h"

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
	GLES2Filter::GLES2Filter(PGLES2Texture input, PGLES2Texture output, const char* fragment)
    : pMaterial_(new GLES2Material ()),
    pMesh_(new GLES2PlaneMesh(2, 2, 2, 2, GL_STATIC_DRAW)),
    input_(input)
	{
		PGLES2Program pProgram(new GLES2Program(vShader, fragment));
		pMaterial_->SetProgram(pProgram);
		pMaterial_->SetTexture0(input);

		pRender2Texture_ = PGLES2Render2Texture(new GLES2Render2Texture(output, true));
	}

	GLES2Filter::~GLES2Filter()
	{

	}

	void GLES2Filter::Render()
	{
		if(input_->IsReady())
		{
			CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);

			if(pMaterial_->IsReady())
			{
				GLES2Camera* pCurrent = GLES2Camera::Deactivate();

				pRender2Texture_->Begin();

				pMaterial_->Render(true, nullptr, pMesh_.get());

				pRender2Texture_->End();

				GLES2Camera::Activate(pCurrent);
			}

			CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);
		}
	}
}
