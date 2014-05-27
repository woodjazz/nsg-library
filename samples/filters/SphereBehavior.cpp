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
#include "SphereBehavior.h"
extern void AddPass(PPass pass);

void SphereBehavior::Start()
{
    pSceneNode_->SetPosition(Vertex3(5, 0, 0));
    //pSceneNode_->SetScale(Vertex3(3,3,3));

	PGLES2SphereMesh mesh(new GLES2SphereMesh(3, 32, GL_STATIC_DRAW));

	PGLES2Material material(new GLES2Material);
    PTexture texture(new TextureFile("Earth.jpg"));
	material->SetTexture0(texture);

    renderedTexture_ = PTexture(new TextureMemory(GL_RGBA, 1024, 1024, nullptr));
    PPass2Texture pass0(new Pass2Texture(renderedTexture_, true));
    AddPass(pass0);

    PPass pass00(new Pass);
    pass00->Add(mesh);
    pass00->Set(material);
    pass00->SetNode(pSceneNode_);
    pass0->Add(pass00);


    filteredTexture_ = PTexture(new TextureMemory(GL_RGBA, 16, 16, nullptr));
    filter_ = PGLES2Filter(new GLES2FilterBlur(renderedTexture_, filteredTexture_));
    PPassFilter pass1(new PassFilter(filter_));
    AddPass(pass1);

    blendedTexture_ = PTexture (new TextureMemory(GL_RGBA, 1024, 1024, nullptr));
    PGLES2Filter blendFilter(new GLES2FilterBlend(filteredTexture_, renderedTexture_, blendedTexture_));
    PPassFilter passBlend(new PassFilter(blendFilter));
    AddPass(passBlend);
}

void SphereBehavior::Update()
{
    float deltaTime = App::this_->GetDeltaTime();

	static float y_angle = 0;

    y_angle += glm::pi<float>()/10.0f * deltaTime;

	pSceneNode_->SetOrientation(glm::angleAxis(y_angle, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle, Vertex3(0, 1, 0)));
}


