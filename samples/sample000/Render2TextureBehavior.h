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
#include "NSG.h"
using namespace NSG;
class Render2TextureBehavior : public Behavior, public Singleton<Render2TextureBehavior>
{
public:
	Render2TextureBehavior();
	~Render2TextureBehavior();
	void AddPass(PPass pass);

	void Start();
	void Update();
	void Render();
	//void Render2Select();
private:
	PTexture pRenderedTexture_;
    PTexture pFilteredTexture_;
    PTexture pBlendedTexture_;
	PShowTexture showTexture_;
	PPass2Texture pass_;
};