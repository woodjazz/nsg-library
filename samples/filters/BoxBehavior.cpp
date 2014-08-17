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
	controlPoints_.push_back(Vertex3(-5.0f, 0.0f, 0.0f)); 
    controlPoints_.push_back(Vertex3(0.0f, 0.0f, 5.0f));
	controlPoints_.push_back(Vertex3(5.0f, 0.0f, 0.0f));
	controlPoints_.push_back(Vertex3(0.0f, 0.0f, -5.0f)); 

	mesh_ = PBoxMesh(new BoxMesh(1,1,1, 2,2,2));

	material_ = PMaterial(new Material);
    PProgram unlit(new ProgramUnlit);
    material_->SetProgram(unlit);
    PTexture texture(new TextureFile("data/cube.png"));
	material_->SetTexture0(texture);

    renderedTexture_ = PTexture(new TextureMemory(GL_RGBA, 1024, 1024, nullptr));

    filteredTexture_ = PTexture(new TextureMemory(GL_RGBA, 1024, 1024, nullptr));
    filter_ = PFilter(new Filter(renderedTexture_, filteredTexture_, fShader));

    pSceneNode_->SetScale(Vertex3(3,3,3));
}

void BoxBehavior::Update()
{
    float deltaTime = App::this_->GetDeltaTime();

    {
        static float delta1 = 0;

	    Vertex3 position = glm::catmullRom(
            controlPoints_[0],
            controlPoints_[1],
            controlPoints_[2],
            controlPoints_[3],
		    delta1);

        pSceneNode_->SetPosition(position);

        delta1 += deltaTime * 0.3f;

        if(delta1 > 1)
        {
    	    delta1 = 0;
            Vertex3 p = controlPoints_.front();
            controlPoints_.pop_front();
            controlPoints_.push_back(p);
        }
    }

    {
        static float move = -1;
        move += deltaTime * TWO_PI * 0.25f;  // 1/4 of a wave cycle per second
        filter_->GetMaterial()->SetShininess(move);
    }
}

void BoxBehavior::Render()
{
    //technique_->Render();
}


