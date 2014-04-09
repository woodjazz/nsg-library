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
#include "GLES2StencilMask.h"
#include "Node.h"
#include "Check.h"
#include "GLES2Mesh.h"

#define STRINGIFY(S) #S

namespace NSG
{
	static const char* vShader = STRINGIFY(
		uniform mat4 u_mvp;
		attribute vec4 a_position;
		void main()
		{
			gl_Position = u_mvp * a_position;
		}
	);

	static const char* fShader = STRINGIFY(
		void main()
		{
			gl_FragColor = vec4(0,0,0,1);
		}
	);

	GLES2StencilMask::GLES2StencilMask()
	{
		PGLES2Program pProgram(new GLES2Program(vShader, fShader));
		pMaterial_ = PGLES2Material(new GLES2Material()); 
        pMaterial_->SetProgram(pProgram);
	}

	GLES2StencilMask::~GLES2StencilMask()
	{
	}

	void GLES2StencilMask::Begin()
	{
		glGetBooleanv(GL_COLOR_WRITEMASK, save_color_mask_);
	  	glGetBooleanv(GL_DEPTH_WRITEMASK, &save_depth_mask_);
        glEnable(GL_STENCIL_TEST);
	  	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	  	glDepthMask(GL_FALSE);
	  	glStencilFunc(GL_NEVER, 1, 0xFF);
	  	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
	  	glStencilMaskSeparate(GL_FRONT_AND_BACK, 0xFF);
	  	glClear(GL_STENCIL_BUFFER_BIT);

        CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);
	}

	void GLES2StencilMask::Render(Node* pNode, GLES2Mesh* pMesh)
	{
        CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);

		GLenum mode = pMesh->GetMode();
		
		if(mode == GL_LINE_LOOP)
			pMesh->SetMode(GL_TRIANGLE_FAN);
		else if(mode != GL_TRIANGLE_FAN)
			pMesh->SetMode(GL_TRIANGLES);

		pMesh->Render(pNode);
		
		pMesh->SetMode(mode);

        CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);
	}

	void GLES2StencilMask::End()
	{
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  		glDepthMask(GL_TRUE);
		glStencilMaskSeparate(GL_FRONT_AND_BACK, 0x00);
		glStencilFunc(GL_EQUAL, 1, 0xFF); // draw only where stencil's value is 1
        glDisable(GL_STENCIL_TEST);
		glColorMask(save_color_mask_[0], save_color_mask_[1], save_color_mask_[2], save_color_mask_[3]);
	  	glDepthMask(save_depth_mask_);

        CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);
	}
}
