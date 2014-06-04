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

#include "ProgramSimpleColor.h"
#include "Types.h"

static const char* vShader = STRINGIFY(
    uniform mat4 u_mvp;
    attribute vec4 a_position;
    
    void main() 
    {
        gl_Position = u_mvp * a_position;
    }
);

static const char* fShader = STRINGIFY(
    uniform vec4 u_color;
    void main()
    {
        gl_FragColor = u_color;
    }
);

namespace NSG 
{
	ProgramSimpleColor::ProgramSimpleColor()
	: Program(vShader, fShader)
	{
	}

	ProgramSimpleColor::~ProgramSimpleColor()
	{
		
	}
}
