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
#include "Render2TextureBehavior.h"
#define ENABLED 1

template<> Render2TextureBehavior* Singleton<Render2TextureBehavior>::this_ = nullptr;

Render2TextureBehavior::Render2TextureBehavior()
{
}
	
Render2TextureBehavior::~Render2TextureBehavior()
{
}

void Render2TextureBehavior::Start()
{
#if ENABLED    
    pRenderedTexture_ = PTexture(new TextureMemory(GL_RGBA, 1024, 1024, nullptr));
    PTechnique technique(new Technique);
    pSceneNode_->Set(technique);
    pass_ = PPass2Texture(new Pass2Texture(pRenderedTexture_, true, false));
    technique->Add(pass_);

    pFilteredTexture_ = PTexture(new TextureMemory(GL_RGBA, 16, 16, nullptr));
    PFilter blurFilter(new FilterBlur(pRenderedTexture_, pFilteredTexture_));
    PPassFilter passBlur(new PassFilter(blurFilter));
    technique->Add(passBlur);

    pBlendedTexture_ = PTexture (new TextureMemory(GL_RGBA, 1024, 1024, nullptr));
    PFilter blendFilter(new FilterBlend(pFilteredTexture_, pRenderedTexture_, pBlendedTexture_));
    PPassFilter passBlend(new PassFilter(blendFilter));
    technique->Add(passBlend);

    showTexture_ = PShowTexture(new ShowTexture);
    showTexture_->SetNormal(pBlendedTexture_);
#endif    
}

void Render2TextureBehavior::AddPass(PPass pass)
{
#if ENABLED 
    pass_->Add(pass);
#endif
}

void Render2TextureBehavior::Update()
{
}

void Render2TextureBehavior::Render()
{
#if ENABLED     
	pSceneNode_->Render();
    showTexture_->Show();
#endif    
}

