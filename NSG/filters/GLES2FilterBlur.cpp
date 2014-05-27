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
#include "GLES2FilterBlur.h"
#include "GLES2Material.h"

static const char* fShader = STRINGIFY(

	varying vec2 v_texcoord;
	uniform sampler2D u_texture0;

	uniform vec2 u_texelSize;
	uniform int u_orientation;
	uniform int u_blurAmount;
	uniform float u_blurScale;
	uniform float u_blurStrength;


	/// Gets the Gaussian value in the first dimension.
	/// <param name="x">Distance from origin on the x-axis.</param>
	/// <param name="deviation">Standard deviation.</param>
	/// <returns>The gaussian value on the x-axis.</returns>
	float Gaussian(float x, float deviation)
	{
		return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation)));	
	}


	void main()
	{
		float halfBlur = float(u_blurAmount) * 0.5;
		vec4 colour = vec4(0.0);
		vec4 texColour = vec4(0.0);
		
		// Gaussian deviation
		float deviation = halfBlur * 0.35;
		deviation *= deviation;
		float strength = 1.0 - u_blurStrength;
		
		//if ( u_orientation == 0 )
		{
			// Horizontal blur
			for (int i = 0; i < 100; ++i)
			{
				if ( i >= u_blurAmount )
					break;
				
				float offset = float(i) - halfBlur;
				texColour = texture2D(u_texture0, v_texcoord + vec2(offset * u_texelSize.x * u_blurScale, 0.0)) * Gaussian(offset * strength, deviation);
				colour += texColour;
			}
		}
		//else
		{
			// Vertical blur
			for (int i = 0; i < 100; ++i)
			{
				if ( i >= u_blurAmount )
					break;
				
				float offset = float(i) - halfBlur;
				texColour = texture2D(u_texture0, v_texcoord + vec2(0.0, offset * u_texelSize.y * u_blurScale)) * Gaussian(offset * strength, deviation);
				colour += texColour;
			}
		}
		
		// Apply colour
		gl_FragColor = clamp(colour, 0.0, 1.0);
		gl_FragColor.w = 1.0;
	}

);

namespace NSG
{
	GLES2FilterBlur::GLES2FilterBlur(PTexture input, PTexture output)
	: GLES2Filter(input, output, fShader),
	texelSize_loc_(-1),
	orientation_loc_(-1),
	blurAmount_loc_(-1),
	blurScale_loc_(-1),
	blurStrength_loc_(-1),
	texelSize_(1, 1),
	orientation_(1),
	blurAmount_(2),
	blurScale_(4),
	blurStrength_(0.2f)
	{
        pMaterial_->GetProgram()->Set(this);
	}

	GLES2FilterBlur::~GLES2FilterBlur()
	{

	}

	void GLES2FilterBlur::SetLocations()
	{
		PGLES2Program pProgram = pMaterial_->GetProgram();

		texelSize_loc_ = pProgram->GetUniformLocation("u_texelSize");
		orientation_loc_ = pProgram->GetUniformLocation("u_orientation");
		blurAmount_loc_ = pProgram->GetUniformLocation("u_blurAmount");
		blurScale_loc_ = pProgram->GetUniformLocation("u_blurScale");
		blurStrength_loc_ = pProgram->GetUniformLocation("u_blurStrength");
	}

	void GLES2FilterBlur::AssignValues()
	{
		if(texelSize_loc_ != -1)
		{
			glUniform2fv(texelSize_loc_, 1, &texelSize_[0]);
		}

		if(orientation_loc_ != -1)
		{
			glUniform1i(orientation_loc_, orientation_);
		}

		if(blurAmount_loc_ != -1)
		{
			glUniform1i(blurAmount_loc_, blurAmount_);
		}

		if(blurScale_loc_ != -1)
		{
			glUniform1f(blurScale_loc_, blurScale_);
		}

		if(blurStrength_loc_ != -1)
		{
			glUniform1f(blurStrength_loc_, blurStrength_);
		}
	}

}
