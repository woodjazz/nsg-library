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
#include "FilterBlur.h"
#include "Material.h"
#include "Pass.h"
#include "Program.h"

namespace NSG
{
	FilterBlur::FilterBlur(PTexture input, int output_width, int output_height)
		: Filter("FilterBlur", input, output_width, output_height, Program::BLUR),
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
        pass_->GetProgram()->Set(this);
	}

	FilterBlur::~FilterBlur()
	{

	}

	void FilterBlur::SetLocations()
	{
		PProgram program = pass_->GetProgram();

		texelSize_loc_ = program->GetUniformLocation("u_texelSize");
		orientation_loc_ = program->GetUniformLocation("u_orientation");
		blurAmount_loc_ = program->GetUniformLocation("u_blurAmount");
		blurScale_loc_ = program->GetUniformLocation("u_blurScale");
		blurStrength_loc_ = program->GetUniformLocation("u_blurStrength");
	}

	void FilterBlur::AssignValues()
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
