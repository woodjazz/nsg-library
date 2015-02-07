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
#include "Pass.h"
#include "Program.h"
#include "Util.h"
#include "Technique.h"
namespace NSG
{
	FilterBlend::FilterBlend(PTexture input0, PTexture input1, int output_width, int output_height)
		: Filter(GetUniqueName("FilterBlend"), input0, output_width, output_height, (int)ProgramFlag::BLEND)
	{
		pMaterial_->SetTexture(1, input1);
	}

	FilterBlend::~FilterBlend()
	{
	}
}
