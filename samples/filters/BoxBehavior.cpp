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
#include "BoxBehavior.h"
extern void AddPass(PPass pass);

static const char* fShader = STRINGIFY(
    uniform sampler2D u_texture0;
    varying vec2 v_texcoord;

    struct Material
    {
        float shininess;
    };

    uniform Material u_material;

    void main()
    {
        vec2 texcoord = v_texcoord;
        texcoord.x += sin(texcoord.y * 4.0 * 2.0 * 3.14159 + u_material.shininess) / 100.0;
        gl_FragColor = texture2D(u_texture0, texcoord);
    }
);

void BoxBehavior::Start()
{
	PGLES2BoxMesh pMesh(new GLES2BoxMesh(1,1,1, 2,2,2, GL_STATIC_DRAW));

	PGLES2Material material(new GLES2Material);
    PTexture texture(new TextureFile("cube.png"));
	material->SetTexture0(texture);

    renderedTexture_ = PTexture(new TextureMemory(GL_RGBA, 1024, 1024, nullptr));
    PPass2Texture pass0(new Pass2Texture(renderedTexture_, true));
    AddPass(pass0);

    PPass pass00(new Pass);
    pass00->Add(pMesh);
    pass00->Set(material);
    pass00->SetNode(pSceneNode_);
    pass0->Add(pass00);


    pSceneNode_->SetPosition(Vertex3(-5, 0, 0));
    pSceneNode_->SetScale(Vertex3(3,3,3));

    filteredTexture_ = PTexture(new TextureMemory(GL_RGBA, 1024, 1024, nullptr));
    filter_ = PGLES2Filter(new GLES2Filter(renderedTexture_, filteredTexture_, fShader));
    PPassFilter pass1(new PassFilter(filter_));
    AddPass(pass1);
  
}

void BoxBehavior::Update()
{
    float deltaTime = App::this_->GetDeltaTime();
    static float move = -1;
    move += deltaTime * TWO_PI * 0.25f;  // 1/4 of a wave cycle per second
    filter_->GetMaterial()->SetShininess(move);
}


