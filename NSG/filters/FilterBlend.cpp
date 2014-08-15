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
#include "FilterBlend.h"
#include "GLES2Includes.h"
#include "Material.h"
#include "Types.h"

static const char* fShader = STRINGIFY(

	uniform sampler2D u_texture0;
	uniform sampler2D u_texture1;
	varying vec2 v_texcoord;
	uniform int u_blendMode;

	void main ()
	{
		vec4 dst = texture2D(u_texture0, v_texcoord); // rendered scene
		vec4 src = texture2D(u_texture1, v_texcoord); // for example the glow map 

		if ( u_blendMode == 0 )
		{
			// Additive blending (strong result, high overexposure)
			gl_FragColor = min(src + dst, 1.0);
		}
		else if ( u_blendMode == 1 )
		{
			// Screen blending (mild result, medium overexposure)
			gl_FragColor = clamp((src + dst) - (src * dst), 0.0, 1.0);
			gl_FragColor.w = 1.0;
		}
		else if ( u_blendMode == 2 )
		{
			// Softlight blending (light result, no overexposure)
			// Due to the nature of soft lighting, we need to bump the black region of the glowmap
			// to 0.5, otherwise the blended result will be dark (black soft lighting will darken
			// the image).
			src = (src * 0.5) + 0.5;

			float x = (src.x <= 0.5) ? (dst.x - (1.0 - 2.0 * src.x) * dst.x * (1.0 - dst.x)) : (((src.x > 0.5) && (dst.x <= 0.25)) ? (dst.x + (2.0 * src.x - 1.0) * (4.0 * dst.x * (4.0 * dst.x + 1.0) * (dst.x - 1.0) + 7.0 * dst.x)) : (dst.x + (2.0 * src.x - 1.0) * (sqrt(dst.x) - dst.x)));
			float y = (src.y <= 0.5) ? (dst.y - (1.0 - 2.0 * src.y) * dst.y * (1.0 - dst.y)) : (((src.y > 0.5) && (dst.y <= 0.25)) ? (dst.y + (2.0 * src.y - 1.0) * (4.0 * dst.y * (4.0 * dst.y + 1.0) * (dst.y - 1.0) + 7.0 * dst.y)) : (dst.y + (2.0 * src.y - 1.0) * (sqrt(dst.y) - dst.y)));
			float z = (src.z <= 0.5) ? (dst.z - (1.0 - 2.0 * src.z) * dst.z * (1.0 - dst.z)) : (((src.z > 0.5) && (dst.z <= 0.25)) ? (dst.z + (2.0 * src.z - 1.0) * (4.0 * dst.z * (4.0 * dst.z + 1.0) * (dst.z - 1.0) + 7.0 * dst.z)) : (dst.z + (2.0 * src.z - 1.0) * (sqrt(dst.z) - dst.z)));
			
			gl_FragColor = vec4(x, y, z, 1.0);
		}
		else
		{
			gl_FragColor = src;
		}
	}
);

namespace NSG
{
	FilterBlend::FilterBlend(PTexture input0, PTexture input1, PTexture output)
	: Filter(input0, output, fShader),
	blendMode_loc_(-1),
	blendMode_(0)
	{
		pMaterial_->SetTexture1(input1);
        pMaterial_->GetProgram()->Set(this);
	}

	FilterBlend::~FilterBlend()
	{

	}

	void FilterBlend::SetLocations()
	{
		PProgram program = pMaterial_->GetProgram();

		blendMode_loc_ = program->GetUniformLocation("u_blendMode");
	}

	void FilterBlend::AssignValues()
	{
		if(blendMode_loc_ != -1)
		{
			glUniform1i(blendMode_loc_, blendMode_);
		}
	}

}
