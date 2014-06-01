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
including commercial MyApplications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
MyAppreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "MyApp.h"
#include "ModelBehavior.h"
#include "CameraBehavior.h"
#include "EarthBehavior.h"
#include "CubeBehavior.h"
#include "TextBehavior.h"
#include "LightBehavior.h"
#include "Render2TextureBehavior.h"
#include "Check.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

using namespace NSG;

MyApp::MyApp() 
{
	
}

MyApp::~MyApp() 
{
}

int MyApp::GetFPS() const
{
	return 30;
}

void MyApp::Start() 
{
	TRACE_LOG("Start");

    CHECK_GL_STATUS(__FILE__, __LINE__);

	pCamera1_ = PCamera(new Camera());
	pCamera1_->SetBehavior(PBehavior(new CameraBehavior()));

	pCamera2_ = PCamera(new Camera());

    render2TextureSceneNode_ = PSceneNode(new SceneNode());
    Render2TextureBehavior* pRender2TextureBehavior = new Render2TextureBehavior;
    PBehavior render2TextureBehavior(pRender2TextureBehavior);
    render2TextureSceneNode_->SetBehavior(render2TextureBehavior);

    pEarthSceneNode_ = PSceneNode(new SceneNode());
    pEarthSceneNode_->SetBehavior(PBehavior(new EarthBehavior()));

    pCubeSceneNode_ = PSceneNode(new SceneNode());
    pCubeSceneNode_->SetBehavior(PBehavior(new CubeBehavior()));

    pTextSceneNode_ = PSceneNode(new SceneNode());
    pTextSceneNode_->SetBehavior(PBehavior(new TextBehavior()));

    pCamera2_->SetPosition(Vertex3(0,5,5));
    pCamera2_->SetLookAt(Vertex3(0,0,0));
    pCamera2_->SetViewportFactor(0.75f, 0.75f, 0.25f, 0.25f);

    pLight0_ = PLight(new Light());
    pLight0_->SetBehavior(PBehavior(new LightBehavior()));

	//PTextMeshure pCellTexture = PTextMeshure(new TextMeshure("cell.png"));
	//PTextMeshure pCellTexture = PTextMeshure(new TextMeshure("cube_example.png"));
	PTexture pCellTexture(new TextureFile("Earthmap720x360_grid.jpg"));

	IMGUI::Context::this_->pSkin_->fontFile_ = "font/FreeSans.ttf";
	pSkin1_ = IMGUI::PSkin(new IMGUI::Skin(*IMGUI::Context::this_->pSkin_));
	pSkin2_ = IMGUI::PSkin(new IMGUI::Skin(*IMGUI::Context::this_->pSkin_));
	//pSkin2_->pActiveTechnique->GetPass(0)->SetMesh(PRectangleMesh(new RectangleMesh(2, 2, GL_STATIC_DRAW)));
	//pSkin2_->pMesh->EnableDepthTest(false);
	//pSkin2_->pActiveMaterial->SetMainTexture(pCellTexture);
	//pSkin1_->pActiveMaterial->SetTexture0(pCellTexture);

	//pSkin2_->pActiveTechnique->GetPass(0)->GetMaterial()->SetTexture0(pCellTexture);
	pSkin2_->normalTechnique_->GetPass(0)->GetMaterial()->SetTexture0(pCellTexture);
	//pSkin2_->pHotTechnique->GetPass(0)->GetMaterial()->SetTexture0(pRenderedTexture_);


    //pModel_ = PModel(new Model("cube.dae"));
    pModel_ = PModel(new Model("duck.dae"));
    //pModel_ = PModel(new Model("spider.obj"));
    pModel_->SetBehavior(PBehavior(new ModelBehavior()));

    Behavior::StartAll();
   
}

void MyApp::Update() 
{
	//TRACE_LOG("MyApp::Update delta = " << Time::deltaTime);
//    float deltaTime = App::this_->GetDeltaTime();

	Behavior::UpdateAll();

}

void MyApp::TestIMGUI2()
{
	IMGUI::Context::this_->pSkin_->fontSize_ = 18;

	Vertex3 position = IMGUI::Context::this_->pCurrentNode_->GetGlobalPosition();
	Vertex3 size = IMGUI::Context::this_->pCurrentNode_->GetGlobalScale();
    
    
    IMGUI::Context::this_->pCamera_->SetNearClip(0.1f);
    IMGUI::Context::this_->pCamera_->DisableOrtho();
    IMGUI::Context::this_->pCamera_->SetPosition(position + Vertex3(0,0,size.x * 2));
    IMGUI::Context::this_->pCamera_->SetLookAt(Vertex3(0,0,0));


    IMGUI::Context::this_->pCurrentNode_->SetOrientation(glm::angleAxis(PI/4, Vertex3(0, 1, 0)));

	IMGUIBeginHorizontal(0);
	{
		static std::string str0 = "Abc";
		str0 = IMGUITextField(str0, 0);

        static bool enabled = false;
		if(IMGUIButton("Button 1", 0))
		{
            enabled = true;
		}

        if(enabled)
        {
			//IMGUI::pCurrentNode->SetOrientation(glm::angleAxis(x_angle_, Vertex3(0, 0, 1)));

            if(IMGUIButton("Button asas1", 0))
            {
            	enabled = false;
           	}
        }

		static std::string str1 = "xyz";
		str1 = IMGUITextField(str1, 0);

        IMGUIBeginVertical(0);
		{
			IMGUISpacer(20);

            IMGUIBeginVertical(0);
			{
                IMGUIButton("Button 2", 0);
                IMGUIButton("Button 3", 0);
    			static std::string str2 = "012";
	    		str2 = IMGUITextField(str2, 0);
            }
            IMGUIEndVertical();

            static std::string str3 = "jkl";
			str3 = IMGUITextField(str3, 0);


			IMGUIBeginHorizontal(0);
			{
				IMGUIButton("B552", 0);
				
				static std::string str0 = "cccc";
				str0 = IMGUITextField(str0, 0);

				static std::string str1 = "dddd";
				str1 = IMGUITextField(str1, 0);
			}
			IMGUIEndHorizontal();

            IMGUIButton("Button 52", 0);
		}
		IMGUIEndVertical();
		
		//IMGUISpacer(50);

	}
	IMGUIEndHorizontal();
}

void MyApp::TestIMGUI4()
{
	IMGUI::Context::this_->pSkin_ = pSkin2_;
	IMGUI::Context::this_->pSkin_->fontSize_ = 18;
	//IMGUI::pCurrentNode->SetScale(Vertex3(aspectRatio_, 1, 1));

    IMGUIBeginVertical(200);

    //IMGUIBeginHorizontal();

    //IMGUISpacer(80);

    static std::string str0 = "Abc";
	str0 = IMGUITextField(str0, 25);
    //IMGUISetPercentage(70);
    IMGUIButton("sdjfhsdjkfh", 0);
	//IMGUIButton("Button 52");
    //IMGUISpacer(25);

    

    //IMGUIButton("Button 1");

    //IMGUISpacer(70);
    
    //IMGUIEndHorizontal();

    IMGUIEndVertical();
}


void MyApp::RenderGUIFrame()
{
	//TestIMGUI2();
    TestIMGUI4();
}

void MyApp::RenderFrame() 
{
	//TRACE_LOG("MyApp::RenderFrame");
    pCamera1_->Activate();
/*
	float deltaTime = App::this_->GetDeltaTime();
	float move = deltaTime * TWO_PI * 0.25f;  // 1/4 of a wave cycle per second
    pFilter_->GetMaterial()->SetShininess(move);
    pFilter_->Render();

    pFilterBlend_->Render();
*/
    //pRenderedTexture_->Show(pRenderedTexture_);
    //pFilteredTexture_->Show(pFilteredTexture_);
    
    //pBlendedTexture_->Show(pBlendedTexture_);
    

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //showTexture_->Show();

    //Behavior::RenderAll();
}

void MyApp::Render2Select()
{
	Behavior::Render2SelectAll();
}

void MyApp::ViewChanged(int32_t width, int32_t height) 
{
    //aspectRatio_ = (float)height/(float)width;
    //InvalidateContext();
}

void MyApp::OnMouseMove(float x, float y)
{
}

void MyApp::OnMouseDown(float x, float y)
{
}

void MyApp::OnMouseUp()
{
	
}

