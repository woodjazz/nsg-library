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
	blurDirLoc_(-1),
	blurRadiusLoc_(-1),
	sigmaLoc_(-1),
	blurDir_(1, 0),
	blurRadius_(2, 2),
	sigma_(0.1f)
	{
        pass_->GetProgram()->Set(this);
	}

	FilterBlur::~FilterBlur()
	{

	}

	void FilterBlur::SetLocations()
	{
		PProgram program = pass_->GetProgram();

		blurDirLoc_ = program->GetUniformLocation("u_blurDir");
		blurRadiusLoc_ = program->GetUniformLocation("u_blurRadius");
		sigmaLoc_ = program->GetUniformLocation("u_sigma");
	}

	void FilterBlur::AssignValues()
	{
		if(blurDirLoc_ != -1)
		{
			glUniform2fv(blurDirLoc_, 1, &blurDir_[0]);
		}

		if (blurRadiusLoc_ != -1)
		{
			glUniform2fv(blurRadiusLoc_, 1, &blurRadius_[0]);
		}

		if(sigmaLoc_ != -1)
		{
			glUniform1f(sigmaLoc_, sigma_);
		}
	}

}
