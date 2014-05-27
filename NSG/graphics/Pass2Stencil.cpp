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
#include "Pass2Stencil.h"
#include "GLES2StencilMask.h"
#include "GLES2Mesh.h"

namespace NSG
{
	Pass2Stencil::Pass2Stencil() 
	: stencilMask_(new GLES2StencilMask),
	clearBuffer_(true)
	{

	}

	Pass2Stencil::~Pass2Stencil()
	{

	}

	void Pass2Stencil::ClearBuffer(bool clear)
	{

	}

	void Pass2Stencil::Render()
	{
		stencilMask_->ClearBuffer(clearBuffer_);
		stencilMask_->Begin();
		stencilMask_->Render(node_, meshes_);
		stencilMask_->End();
	}
}