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
#include "TextBehavior.h"
#include <string>
#include <sstream>


TextBehavior::TextBehavior()
{

}
	
TextBehavior::~TextBehavior()
{

}

void TextBehavior::Start()
{
	PNode pParent(new Node);
	pSceneNode_->SetParent(pParent);

    pText_ = PGLES2Text(new GLES2Text("font/FreeSans.ttf", 12, GL_DYNAMIC_DRAW));
    pSceneNode_->SetMesh(pText_);

    PGLES2Material pMaterial(new GLES2Material());
    pMaterial->SetTexture0(pText_->GetAtlas());
    pMaterial->SetProgram(pText_->GetProgram());
    pSceneNode_->SetMaterial(pMaterial);
}

void TextBehavior::Update()
{
//    float deltaTime = App::GetPtrInstance()->GetDeltaTime();

	pSceneNode_->GetParent()->SetPosition(Vertex3(-pText_->GetWidth()/2, 0, 0.2f));

}

void TextBehavior::Render()
{
	GLES2Camera* pCamera = GLES2Camera::Deactivate();

	pSceneNode_->Render(true);

	GLES2Camera::Activate(pCamera);
}

void TextBehavior::OnMouseDown(float x, float y)
{
	GLushort id = pApp_->GetSelectedNode();

    std::stringstream ss;
    ss << "Selected Id=" << std::hex << id;

    pText_->SetText(ss.str());
}

