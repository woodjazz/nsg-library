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

struct Statistics : public AppStatistics
{
    void StartGUIWindow()
    {
    }

    void EndGUIWindow()
    {
    }
};

struct Sample : App
{
    PScene scene_;
    Statistics statistics_;
    PCamera camera_;
    PSceneNode box_;
    PSceneNode sphere_;
    PTechnique technique_;
    PShowTexture showTexture_;
    PTexture blendedTexture_;
    BoxBehavior* boxBehavior_;
    SphereBehavior* sphereBehavior_;

	Sample()
	{
		AppConfiguration::this_->width_ = 50;
		AppConfiguration::this_->height_ = 50;
	}

	void Start()
	{
        scene_ = PScene(new Scene);
        
        camera_ = PCamera(new Camera);
        scene_->Add(camera_);
        camera_->SetPosition(Vertex3(0,0,10));
        camera_->Activate();

        box_ = PSceneNode(new SceneNode);
        scene_->Add(box_);
        boxBehavior_ = new BoxBehavior;
        box_->SetBehavior(PBehavior(boxBehavior_));
        
        sphere_ = PSceneNode(new SceneNode);
        scene_->Add(sphere_);
        sphereBehavior_ = new SphereBehavior;
        sphere_->SetBehavior(PBehavior(sphereBehavior_));

        showTexture_ = PShowTexture(new ShowTexture);

        scene_->Start();

        technique_ = PTechnique(new Technique);
        
        PPass normalPass(new Pass);
        normalPass->SetProgram(PProgram(new ProgramUnlit));
        normalPass->SetFrontFace(FrontFaceMode::CW);


        PPass depthPass(new Pass);
        depthPass->EnableColorBuffer(false);
        depthPass->SetProgram(PProgram(new ProgramWhiteColor));
        
        {
            //box passes

            PPass2Texture pass2Texture(new Pass2Texture(boxBehavior_->renderedTexture_, true, false));
            technique_->Add(pass2Texture);
            pass2Texture->Add(depthPass, sphereBehavior_->GetSceneNode(), nullptr, sphereBehavior_->mesh_);
            pass2Texture->Add(normalPass, boxBehavior_->GetSceneNode(), boxBehavior_->material_, boxBehavior_->mesh_);
            
            PPassFilter filterPass(new PassFilter(boxBehavior_->filter_));
            technique_->Add(filterPass);
        }

        {
            //sphere passes
    
            PPass2Texture pass2Texture(new Pass2Texture(sphereBehavior_->renderedTexture_, true, false));
            technique_->Add(pass2Texture);
            pass2Texture->Add(depthPass, boxBehavior_->GetSceneNode(), nullptr, boxBehavior_->mesh_);
            pass2Texture->Add(normalPass, sphereBehavior_->GetSceneNode(), sphereBehavior_->material_, sphereBehavior_->mesh_);

            PPassFilter filterPass(new PassFilter(sphereBehavior_->filter_));
            technique_->Add(filterPass);

            PPassFilter passBlend(new PassFilter(sphereBehavior_->blendFilter_));
            technique_->Add(passBlend);
        }


        blendedTexture_ = PTexture (new TextureMemory(GL_RGBA, 1024, 1024, nullptr));
        showTexture_->SetNormal(blendedTexture_);
        //showTexture_->SetNormal(sphereBehavior_->blendedTexture_);
        //showTexture_->SetNormal(boxBehavior_->filteredTexture_);
        //showTexture_->SetNormal(boxBehavior_->renderedTexture_);
        //showTexture_->SetNormal(sphereBehavior_->renderedTexture_);
        //showTexture_->SetNormal(sphereBehavior_->filteredTexture_);
        
        PFilter blendFilter(new FilterBlend(sphereBehavior_->blendedTexture_, boxBehavior_->filteredTexture_, blendedTexture_));
        PPassFilter passBlend(new PassFilter(blendFilter));
        technique_->Add(passBlend);
      
     }

    void Update()
    {
        scene_->Update();
    }


    void RenderFrame()
    {
        technique_->Render();
        showTexture_->Show();
    }

    void RenderGUIWindow()
    {
        IMGUIWindow(&statistics_, 25, 25);
    }
};

NSG_MAIN(Sample);
