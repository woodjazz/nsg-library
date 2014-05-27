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
#include "SphereBehavior.h"
#include "NSG.h"
using namespace NSG;

struct Sample : App
{
    PCamera camera_;
    PSceneNode box_;
    PSceneNode sphere_;
    PTechnique technique_;
    PShowTexture showTexture_;
    PTexture blendedTexture_;
    BoxBehavior* boxBehavior_;
    SphereBehavior* sphereBehavior_;

	void Start()
	{
        camera_ = PCamera(new Camera);
        camera_->SetPosition(Vertex3(0,0,10));
        camera_->Activate();

        box_ = PSceneNode(new SceneNode);
        boxBehavior_ = new BoxBehavior;
        box_->SetBehavior(PBehavior(boxBehavior_));
        
        sphere_ = PSceneNode(new SceneNode);
        sphereBehavior_ = new SphereBehavior;
        sphere_->SetBehavior(PBehavior(sphereBehavior_));

        technique_ = PTechnique(new Technique);

        showTexture_ = PShowTexture(new ShowTexture);
	}

    void LastStart() 
    {
        blendedTexture_ = PTexture (new TextureMemory(GL_RGBA, 1024, 1024, nullptr));
        showTexture_->SetNormal(blendedTexture_);
        PGLES2Filter blendFilter(new GLES2FilterBlend(sphereBehavior_->blendedTexture_, boxBehavior_->filteredTexture_, blendedTexture_));
        PPassFilter passBlend(new PassFilter(blendFilter));
        technique_->Add(passBlend);

    }

    void RenderFrame()
    {
        technique_->Render();
        showTexture_->Show();
    }
};

void AddPass(PPass pass)
{
    static_cast<Sample*>(Sample::this_)->technique_->Add(pass);
}

NSG_MAIN(Sample);
