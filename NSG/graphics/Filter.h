/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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

#include "Types.h"

namespace NSG
{
	class Filter
	{
	public:
		Filter(const std::string& name, PTexture input);
		~Filter();
		void Draw();
		PMaterial GetMaterial() const { return pMaterial_; }
		PTexture GetTexture() const;
		void SetInputTexture(PTexture input);
		PTexture GetInputTexture() const;
		void SetWindow(Window* window);
		PFrameBuffer GetFrameBuffer() const { return frameBuffer_; }
	protected:
        PMaterial pMaterial_; 
		PMesh pMesh_;
		PFrameBuffer frameBuffer_;
		std::string name_;
		PProgram program_;
		PSceneNode node_;
		PGraphics graphics_;
	};
}