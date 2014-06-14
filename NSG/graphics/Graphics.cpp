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
#include "Graphics.h"
#include "GLES2Includes.h"

namespace NSG
{
	void ClearAllBuffers()
	{
        glClearColor(0, 0, 0, 0);
        glClearDepth(1);
        glClearStencil(0);

        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);
        glStencilMask(~GLuint(0));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void ClearBuffers(bool color, bool depth, bool stencil)
	{
		GLbitfield mask(0);
		if(color)
		{
			mask |= GL_COLOR_BUFFER_BIT;
			glClearColor(0, 0, 0, 1);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		}

		if(depth)
		{
			mask |= GL_DEPTH_BUFFER_BIT;
			glClearDepth(1);
			glDepthMask(GL_TRUE);
		}

		if(stencil)
		{
			glClearStencil(0);
			glStencilMask(~GLuint(0));
		}

		glClear(mask);
	}

	void ClearStencilBuffer(GLint value)
	{
		glClearStencil(value);
		glStencilMask(~GLuint(0));
		glClear(GL_STENCIL_BUFFER_BIT);
	}
}