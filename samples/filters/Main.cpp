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
    PScene scene_;
    PCamera camera_;
    PSceneNode box_;
    PSceneNode sphere_;
    PTechnique technique_;
    PShowTexture showTexture_;
    BoxBehavior* boxBehavior_;
    SphereBehavior* sphereBehavior_;

	Sample()
	{
        //AppConfiguration::this_->width_ = 30;
        //AppConfiguration::this_->height_ = 20;
        AppConfiguration::this_->showStatistics_ = true;
	}

	void Start(int argc, char* argv[]) override
	{
		scene_ = GetCurrentScene();
        
		camera_ = scene_->GetOrCreateChild<Camera>("camera");
        camera_->SetPosition(Vertex3(0,0,10));
        camera_->Activate();

		box_ = scene_->GetOrCreateChild<SceneNode>("node 1");
        boxBehavior_ = new BoxBehavior;
		box_->AddBehavior(PBehavior(boxBehavior_));
        
		sphere_ = scene_->GetOrCreateChild<SceneNode>("node 2");
        sphereBehavior_ = new SphereBehavior;
		sphere_->AddBehavior(PBehavior(sphereBehavior_));

        showTexture_ = PShowTexture(new ShowTexture);

        scene_->Start();

        technique_ = PTechnique(new Technique);
        
        PPass normalPass(new Pass);
		PProgram normalProgram(GetOrCreateProgram("program1"));
		normalProgram->SetFlags((int)ProgramFlag::UNLIT);
		normalPass->SetProgram(normalProgram);

        PPass depthPass(new Pass);
        depthPass->EnableColorBuffer(false);
		PProgram depthProgram(GetOrCreateProgram("program2"));
		depthProgram->SetFlags((int)ProgramFlag::STENCIL);
		depthPass->SetProgram(depthProgram);
        
		PFilter boxFilter;
        
        {
            //box passes
            PPass2Texture pass2Texture(new Pass2Texture(1024, 1024));	
			technique_->Add(pass2Texture);

			boxFilter = PFilter(new Filter("BoxFilter", pass2Texture->GetTexture(), 1024, 1024));
			PResourceMemory resource(new ResourceMemory(BoxBehavior::GetFS()));
			boxFilter->GetProgram()->SetFragmentShader(resource);
            
			pass2Texture->Add(depthPass, sphereBehavior_->sceneNode_, nullptr, sphereBehavior_->mesh_);
			pass2Texture->Add(normalPass, boxBehavior_->sceneNode_, boxBehavior_->material_, boxBehavior_->mesh_);
            
			boxBehavior_->SetFilterMaterial(boxFilter->GetMaterial());
			PPassFilter filterPass(new PassFilter(boxFilter));
            technique_->Add(filterPass);
        }

#if 1
		PFilter sphereBlendFilter;
        {
            //sphere passes
    
            PPass2Texture pass2Texture(new Pass2Texture(1024, 1024));
            technique_->Add(pass2Texture);
			pass2Texture->Add(depthPass, boxBehavior_->sceneNode_, nullptr, boxBehavior_->mesh_);
			pass2Texture->Add(normalPass, sphereBehavior_->sceneNode_, sphereBehavior_->material_, sphereBehavior_->mesh_);

			PFilter blurFilter(new FilterBlur(pass2Texture->GetTexture(), 16, 16));
			    
			PPassFilter filterPass(new PassFilter(blurFilter));
            technique_->Add(filterPass);


			sphereBlendFilter = PFilter(new FilterBlend(blurFilter->GetTexture(), pass2Texture->GetTexture(), 1024, 1024));

			PPassFilter passBlend(new PassFilter(sphereBlendFilter));
            technique_->Add(passBlend);

			//showTexture_->SetNormal(sphereBlendFilter->GetTexture());
        }

		PFilter blendFilter(new FilterBlend(sphereBlendFilter->GetTexture(), boxFilter->GetTexture(), 1024, 1024));
		PPassFilter passBlend(new PassFilter(blendFilter));
		technique_->Add(passBlend);
#endif

		showTexture_->SetNormal(blendFilter->GetTexture());
		//showTexture_->SetNormal(sphereBlendFilter->GetTexture());
        //showTexture_->SetNormal(boxBehavior_->filteredTexture_);
        //showTexture_->SetNormal(boxBehavior_->renderedTexture_);
        //showTexture_->SetNormal(sphereBehavior_->renderedTexture_);
        //showTexture_->SetNormal(sphereBehavior_->filteredTexture_);
        
      
     }

    void Update() override
    {
        scene_->Update();
    }


    void RenderFrame() override
    {
        technique_->Render();
        showTexture_->Show();
    }
};

NSG_MAIN(Sample);
