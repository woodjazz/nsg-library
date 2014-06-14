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

#include "ProgramUnlit.h"
#include "Types.h"

static const char* vShader = STRINGIFY(
    uniform mat4 u_mvp;
	attribute vec2 a_texcoord;
	attribute vec4 a_position;
	attribute vec4 a_color;

    varying vec4 v_vertex_color;
	varying vec2 v_texcoord;

	void main()
	{
		gl_Position = u_mvp * a_position;
		v_texcoord = a_texcoord;
		v_vertex_color = a_color;
	}
);

static const char* fShader = STRINGIFY(
	varying vec4 v_vertex_color;
	varying vec2 v_texcoord;
	uniform sampler2D u_texture0;
	uniform vec4 u_color;
	void main()
	{
		gl_FragColor = texture2D(u_texture0, v_texcoord) * v_vertex_color * u_color;
	}
);

namespace NSG 
{
	ProgramUnlit::ProgramUnlit()
	: Program(vShader, fShader)
	{
	}

	ProgramUnlit::~ProgramUnlit()
	{
		
	}
}
