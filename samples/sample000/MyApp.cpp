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
	TRACE_LOG("MyApp::Destroy Begin");
	TRACE_LOG("MyApp::Destroy End");
}

int MyApp::GetFPS() const
{
	return 30;
}

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


void MyApp::Start() 
{
	TRACE_LOG("Start");

    CHECK_GL_STATUS(__FILE__, __LINE__);

	pCamera1_ = PGLES2Camera(new GLES2Camera());
	pCamera1_->SetBehavior(PBehavior(new CameraBehavior()));

	pCamera2_ = PGLES2Camera(new GLES2Camera());

    pEarthSceneNode_ = PSceneNode(new SceneNode());
    pEarthSceneNode_->SetBehavior(PBehavior(new EarthBehavior()));

    pCubeSceneNode_ = PSceneNode(new SceneNode());
    pCubeSceneNode_->SetBehavior(PBehavior(new CubeBehavior()));

    pTextSceneNode_ = PSceneNode(new SceneNode());
    pTextSceneNode_->SetBehavior(PBehavior(new TextBehavior()));

    pCamera2_->SetPosition(Vertex3(0,5,5));
    pCamera2_->SetLookAt(Vertex3(0,0,0));
    pCamera2_->SetViewportFactor(0.75f, 0.75f, 0.25f, 0.25f);

    pLight0_ = PGLES2Light(new GLES2Light());
    pLight0_->SetBehavior(PBehavior(new LightBehavior()));

	//PGLES2Texture pCellTexture = PGLES2Texture(new GLES2Texture("cell.png"));
	//PGLES2Texture pCellTexture = PGLES2Texture(new GLES2Texture("cube_example.png"));
	PGLES2Texture pCellTexture = PGLES2Texture(new GLES2Texture("Earthmap720x360_grid.jpg"));

	pSkin1_ = IMGUI::PSkin(new IMGUI::Skin(*IMGUI::pSkin));
	pSkin2_ = IMGUI::PSkin(new IMGUI::Skin(*IMGUI::pSkin));
	pSkin2_->drawBorder = false;
	pSkin2_->pMesh =  PGLES2RectangleMesh(new GLES2RectangleMesh(2, 2, GL_STATIC_DRAW));
	//pSkin2_->pMesh->EnableDepthTest(false);
	//pSkin2_->pActiveMaterial->SetMainTexture(pCellTexture);
	//pSkin1_->pActiveMaterial->SetTexture0(pCellTexture);

	pRenderedTexture_ = PGLES2Texture (new GLES2Texture(GL_RGBA, 1024, 1024, nullptr));

	pSkin2_->pActiveMaterial->SetTexture0(pCellTexture);
	pSkin1_->pNormalMaterial->SetTexture0(pRenderedTexture_);


    pRender2Texture_ = PGLES2Render2Texture(new GLES2Render2Texture(pRenderedTexture_, true));

    //pModel_ = PModel(new Model("cube.dae"));
    pModel_ = PModel(new Model("duck.dae"));
    //pModel_ = PModel(new Model("spider.obj"));
    pModel_->SetBehavior(PBehavior(new ModelBehavior()));

    pFilteredTexture_ = PGLES2Texture (new GLES2Texture(GL_RGBA, 16, 16, nullptr));

    //pFilter_ = PGLES2Filter(new GLES2Filter(pRenderedTexture_, pFilteredTexture_, fShader));
    pFilter_ = PGLES2Filter(new GLES2FilterBlur(pRenderedTexture_, pFilteredTexture_));

    pBlendedTexture_ = PGLES2Texture (new GLES2Texture(GL_RGBA, 1024, 1024, nullptr));

    pFilterBlend_ = PGLES2Filter(new GLES2FilterBlend(pFilteredTexture_, pRenderedTexture_, pBlendedTexture_));
}

void MyApp::Update() 
{
	//TRACE_LOG("MyApp::Update delta = " << Time::deltaTime);
//    float deltaTime = App::GetPtrInstance()->GetDeltaTime();

}

void MyApp::TestIMGUI2()
{
	IMGUI::pSkin->fontSize = 18;

	Vertex3 position = IMGUI::pCurrentNode->GetGlobalPosition();
	Vertex3 size = IMGUI::pCurrentNode->GetGlobalScale();
    
    
    IMGUI::pCamera->SetNearClip(0.1f);
    IMGUI::pCamera->DisableOrtho();
    IMGUI::pCamera->SetPosition(position + Vertex3(0,0,size.x * 2));
    IMGUI::pCamera->SetLookAt(Vertex3(0,0,0));


    IMGUI::pCurrentNode->SetOrientation(glm::angleAxis(PI/4, Vertex3(0, 1, 0)));

	IMGUIBeginHorizontal();
	{
		static std::string str0 = "Abc";
		str0 = IMGUITextField(str0);

        static bool enabled = false;
		if(IMGUIButton("Button 1"))
		{
            enabled = true;
		}

        if(enabled)
        {
			//IMGUI::pCurrentNode->SetOrientation(glm::angleAxis(x_angle_, Vertex3(0, 0, 1)));

            if(IMGUIButton("Button asas1"))
            {
            	enabled = false;
           	}
        }

		static std::string str1 = "xyz";
		str1 = IMGUITextField(str1);

        IMGUIBeginVertical();
		{
			IMGUISpacer(20);

            IMGUIBeginVertical();
			{
                IMGUIButton("Button 2");
                IMGUIButton("Button 3");
    			static std::string str2 = "012";
	    		str2 = IMGUITextField(str2);
            }
            IMGUIEndVertical();

            static std::string str3 = "jkl";
			str3 = IMGUITextField(str3);


			IMGUIBeginHorizontal();
			{
				IMGUIButton("B552");
				
				static std::string str0 = "cccc";
				str0 = IMGUITextField(str0);

				static std::string str1 = "dddd";
				str1 = IMGUITextField(str1);
			}
			IMGUIEndHorizontal();

            IMGUIButton("Button 52");
		}
		IMGUIEndVertical();
		
		//IMGUISpacer(50);

	}
	IMGUIEndHorizontal();
}

void MyApp::TestIMGUI4()
{
	IMGUI::pSkin = pSkin2_;
	IMGUI::pSkin->fontSize = 18;
	//IMGUI::pCurrentNode->SetScale(Vertex3(aspectRatio_, 1, 1));

    IMGUIBeginHorizontal();

    //IMGUISpacer(80);

    static std::string str0 = "Abc";
	str0 = IMGUITextField(str0);
	//IMGUIButton("Button 52");
    //IMGUISpacer(25);

    

    //IMGUIButton("Button 1");

    //IMGUISpacer(25);
    
    IMGUIEndHorizontal();
}

void MyApp::Menu1()
{
    static float delta = -1;
    static Vertex3 camControlPoint0(-3, 3, 0);
    static Vertex3 camControlPoint1(0, 2, 0);
    static Vertex3 camControlPoint2(0, 0, 0);
    static Vertex3 camControlPoint3(3, -2, 0);

    static bool menu = false;
    static float alpha = IMGUI::pSkin->alphaFactor;

    if(delta > 1) delta = 1;

	Vertex3 position = glm::catmullRom(
        camControlPoint0,
        camControlPoint1,
        camControlPoint2,
        camControlPoint3,
		delta);

    if(!menu)
    {
    	IMGUI::pSkin = pSkin1_;
        menu = false;
        IMGUI::pCurrentNode->SetPosition(Vertex3(0,0,0));
	    IMGUIBeginHorizontal();
	    {
	    	
		    IMGUIBeginVertical();
		    {
			    IMGUISpacer(80);
			    if(IMGUIButton("Menu"))
			    {
			    	IMGUI::pSkin->alphaFactor = alpha;
                    menu = true;
				    delta = 0;
			    }
		    }
		    IMGUIEndVertical();
		    IMGUISpacer(80);
		    
	    }
	    IMGUIEndHorizontal();
	    
        if(IMGUI::pSkin->pNormalMaterial->GetDiffuseColor().w < alpha)
	    	IMGUI::pSkin->alphaFactor += 0.01f;
	    else
	    	IMGUI::pSkin->alphaFactor = 1;
		
    }
    else
	{
		IMGUI::pSkin = pSkin2_;
		IMGUI::pCurrentNode->SetPosition(position);
		IMGUISpacer(20);
		static std::string str = "0123";
		str = IMGUITextField(str);
		IMGUISpacer(20);
        static bool exit = false;
        IMGUI::pSkin = pSkin1_;
		if(IMGUIButton("Exit"))
		{
            exit = true;
		}

        if(exit)
        {
            delta -= App::GetPtrInstance()->GetDeltaTime();
        }
        else
        {
            delta += App::GetPtrInstance()->GetDeltaTime();
        }

        if(delta < 0)
        {
            menu = exit = false;
            IMGUI::pSkin->alphaFactor = 0;
        }
	}
}

void MyApp::RenderGUIFrame()
{
	//TestIMGUI2();
    //TestIMGUI4();
    Menu1();
}

void MyApp::RenderFrame() 
{
	//TRACE_LOG("MyApp::RenderFrame");
    pCamera1_->Activate();

	pRender2Texture_->Begin();
    //pCubeSceneNode_->Render(true);
    //pEarthSceneNode_->Render(true);
    //pModel_->GetSceneNode()->Render(true);
    pRender2Texture_->End();

	float deltaTime = App::GetPtrInstance()->GetDeltaTime();
	float move = deltaTime * TWO_PI * 0.25f;  // 1/4 of a wave cycle per second
    pFilter_->GetMaterial()->SetShininess(move);
    pFilter_->Render();

    pFilterBlend_->Render();

    //pRenderedTexture_->Show(pRenderedTexture_);
    //pFilteredTexture_->Show(pFilteredTexture_);
    
    //pBlendedTexture_->Show(pBlendedTexture_);
    

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //pRenderedTexture_->Show(pRenderedTexture_);
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

