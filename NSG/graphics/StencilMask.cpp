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
#include "StencilMask.h"
#include "Node.h"
#include "Check.h"
#include "Mesh.h"
#include "Program.h"
#include "Camera.h"
#include "Context.h"

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

	StencilMask::StencilMask() 
	: pProgram_(new Program(vShader, fShader)),
	clearBuffer_(true)
	{
	}

	StencilMask::~StencilMask()
	{
		Context::this_->Remove(this);
	}

	void StencilMask::ClearBuffer(bool clear)
	{
		clearBuffer_ = clear;
	}

	bool StencilMask::IsValid()
	{
		return pProgram_->IsReady();
	}

	void StencilMask::AllocateResources()
	{
	}

	void StencilMask::ReleaseResources()
	{
		enabled_ = false;
	}

	static const GLint RefValue = 0x01;
	static const GLint Mask = 0xFF;

	void StencilMask::Begin()
	{
		if(IsReady())
		{
	        CHECK_GL_STATUS(__FILE__, __LINE__);

			glGetBooleanv(GL_COLOR_WRITEMASK, save_color_mask_);
		  	glGetBooleanv(GL_DEPTH_WRITEMASK, &save_depth_mask_);
		  	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		  	glDepthMask(GL_FALSE);

		  	glEnable(GL_STENCIL_TEST);
			glStencilMaskSeparate(GL_FRONT_AND_BACK, Mask); //Enables writting in the stencil buffer
		  	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP); //Put RefValue in the stencil buffer when test fails

		  	if(clearBuffer_)
            {
		  		glStencilFunc(GL_NEVER, RefValue, Mask); //When buffer is initially cleared then always fail the test (and writes in the stencil buffer)
                glClear(GL_STENCIL_BUFFER_BIT);
            }
		  	else
            {
		  		glStencilFunc(GL_NOTEQUAL, RefValue, Mask); //If stencil buffer already contains RefValue then  fail the test if equal
            }

	        CHECK_GL_STATUS(__FILE__, __LINE__);

			enabled_ = true;

	    }
	}

	void StencilMask::Render(Node* pNode, Mesh* pMesh)
	{
		if(IsReady() && pMesh->IsReady() && enabled_)
		{
			//Draw in stencil buffer
	        CHECK_GL_STATUS(__FILE__, __LINE__);

	        UseProgram useProgram(*pProgram_);
            pProgram_->Use(pNode);

			GLuint positionLoc = pProgram_->GetPositionLoc();
			GLuint texcoordLoc = pProgram_->GetTextCoordLoc();
			GLuint normalLoc = pProgram_->GetNormalLoc();
			GLuint colorLoc = pProgram_->GetColorLoc();

			pMesh->Render(true, positionLoc, texcoordLoc, normalLoc, colorLoc);

	        CHECK_GL_STATUS(__FILE__, __LINE__);
	    }
	}

	void StencilMask::Render(const std::vector<MeshNode>& meshNodes)
	{
		if(IsReady() && enabled_)
		{
	        CHECK_GL_STATUS(__FILE__, __LINE__);

	        UseProgram useProgram(*pProgram_);

			GLuint positionLoc = pProgram_->GetPositionLoc();
			GLuint texcoordLoc = pProgram_->GetTextCoordLoc();
			GLuint normalLoc = pProgram_->GetNormalLoc();
			GLuint colorLoc = pProgram_->GetColorLoc();

			auto it = meshNodes.begin();
			while(it != meshNodes.end())
			{
                pProgram_->Use(it->first.get());

				if(it->second->IsReady())
				{
					it->second->Render(true, positionLoc, texcoordLoc, normalLoc, colorLoc);
				}

				++it;
			}

			CHECK_GL_STATUS(__FILE__, __LINE__);
		}
	}

	void StencilMask::End()
	{
		if(IsReady() && enabled_)
		{
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	  		glDepthMask(GL_TRUE);
			glStencilMaskSeparate(GL_FRONT_AND_BACK, 0x00); //Disables writting in the stencil buffer
			glStencilFunc(GL_EQUAL, RefValue, Mask); // draw only where stencil's value is RefValue
	        glDisable(GL_STENCIL_TEST); //remember to enable when (and where) stencil buffer has to be used
			glColorMask(save_color_mask_[0], save_color_mask_[1], save_color_mask_[2], save_color_mask_[3]);
		  	glDepthMask(save_depth_mask_);

	        CHECK_GL_STATUS(__FILE__, __LINE__);

			enabled_ = false;
        }
	}
}
