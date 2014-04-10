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
#include "EarthBehavior.h"
#include "CubeBehavior.h"

#include "NSG/Log.h"
#include "NSG/GLES2Texture.h"
#include "NSG/GLES2IMGUI.h"
#include "NSG/Resource.h"
#include "NSG/Constants.h"
#include "NSG/GLES2RoundedRectangleMesh.h"
#include "NSG/GLES2RectangleMesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>


extern void FSMExamples();
extern void FSMTest();
extern void TimedTaskTest();
extern void QueuedTaskTest();
extern void NodeTest();

using namespace NSG;


MyApp::MyApp() 
: x_angle_(0),
y_angle_(0),
x_(0),
y_(0),
buttonDown_(false),
buttonUp_(false),
selectedIndex_(0),
scale_(1,1,1),
aspectRatio_(1)
{
	NodeTest();
}

MyApp::~MyApp() 
{
	TRACE_LOG("MyApp::Destroy Begin");
	thread_.join();
	TRACE_LOG("MyApp::Destroy End");
}

void MyApp::InternalTask() 
{
	FSMExamples();
	FSMTest();
	TimedTaskTest();
	QueuedTaskTest();
}

int MyApp::GetFPS() const
{
	return 30;
}

void MyApp::Start() 
{
	TRACE_LOG("Start");

	thread_ = std::thread([this](){InternalTask();});	

	pCamera1_ = PGLES2Camera(new GLES2Camera());
	pCamera2_ = PGLES2Camera(new GLES2Camera());

    pEarthSceneNode_ = PSceneNode(new SceneNode());
    pEarthSceneNode_->SetBehavior(PBehavior(new EarthBehavior()));

    pCubeSceneNode_ = PSceneNode(new SceneNode());
    pCubeSceneNode_->SetBehavior(PBehavior(new CubeBehavior()));

	pTextNode0_ = PNode(new Node);
    pTextNode1_ = PNode(new Node());
    pTextNode1_->SetParent(pTextNode0_);

    pCamera2_->SetPosition(Vertex3(0,5,5));
    pCamera2_->SetLookAt(Vertex3(0,0,0));
    pCamera2_->SetViewportFactor(0.75f, 0.75f, 0.25f, 0.25f);

    pText1_ = PGLES2Text(new GLES2Text("font/FreeSans.ttf", 12, GL_DYNAMIC_DRAW));
    pText2_ = PGLES2Text(new GLES2Text("font/bluebold.ttf", 24, GL_STATIC_DRAW));
    pText3_ = PGLES2Text(new GLES2Text("font/FreeSans.ttf", 48, GL_STATIC_DRAW));

    pLight0_ = PGLES2Light(new GLES2Light());
    pLight0_->SetPosition(Vertex3(-1.0,  0.0,  5.0));
	//pLight0_->SetDirectional();

	camControlPoints_.push_back(Vertex3(-10.0f, 0.0f, 0.0f)); 
    camControlPoints_.push_back(Vertex3(0.0f, 0.0f, 10.0f));
	camControlPoints_.push_back(Vertex3(10.0f, 0.0f, 0.0f));
	camControlPoints_.push_back(Vertex3(0.0f, 0.0f, -10.0f)); 

	//PGLES2Texture pCellTexture = PGLES2Texture(new GLES2Texture("cell.png"));
	//PGLES2Texture pCellTexture = PGLES2Texture(new GLES2Texture("cube_example.png"));
	PGLES2Texture pCellTexture = PGLES2Texture(new GLES2Texture("Earthmap720x360_grid.jpg"));

	pSkin1_ = IMGUI::PSkin(new IMGUI::Skin(*IMGUI::pSkin));
	pSkin2_ = IMGUI::PSkin(new IMGUI::Skin(*IMGUI::pSkin));
	pSkin2_->drawBorder = false;
	pSkin2_->pMesh =  PGLES2RectangleMesh(new GLES2RectangleMesh(2, 2, GL_STATIC_DRAW));
	//pSkin2_->pMesh->EnableDepthTest(false);
	//pSkin2_->pActiveMaterial->SetMainTexture(pCellTexture);
	//pSkin1_->pActiveMaterial->SetMainTexture(pCellTexture);

	pRenderedTexture_ = PGLES2Texture (new GLES2Texture(GL_RGBA, GL_UNSIGNED_BYTE, 256, 256, nullptr));

	pSkin2_->pActiveMaterial->SetMainTexture(pRenderedTexture_);
	pSkin1_->pActiveMaterial->SetMainTexture(pRenderedTexture_);

    pRender2Texture_ = PGLES2Render2Texture(new GLES2Render2Texture(pRenderedTexture_));

}

void MyApp::Update() 
{
	//TRACE_LOG("MyApp::Update delta = " << Time::deltaTime);
    float deltaTime = App::GetPtrInstance()->GetDeltaTime();

	x_angle_ += glm::pi<float>()/10.0f * deltaTime;
	y_angle_ += glm::pi<float>()/10.0f * deltaTime;

	static float factor_scale = 1;
	scale_ -= deltaTime * 0.7f * factor_scale;

	if(scale_.x < -10 && factor_scale > 0)
		factor_scale *= -1;
	else if(scale_.x > 10 && factor_scale < 0)
		factor_scale *= -1;

	pTextNode0_->SetPosition(Vertex3(-pText1_->GetWidth()/2, 0, 0.2f));
    //pTextNode1_->SetPosition(Vertex3(200, 100, 0));
	pTextNode1_->SetOrientation(glm::angleAxis(x_angle_, Vertex3(0, 0, 1)));


	pLight0_->SetPosition(Vertex3(scale_.x,  2.0,  6.0));
	//pLight0_->SetOrientation(glm::angleAxis(x_angle_, Vertex3(1, 0, 0)));

    static float delta1 = 0;

	Vertex3 position = glm::catmullRom(
        camControlPoints_[0],
        camControlPoints_[1],
        camControlPoints_[2],
        camControlPoints_[3],
		delta1);

    pCamera1_->SetPosition(position);
    pCamera1_->SetLookAt(Vertex3(0));

    delta1 += deltaTime * 0.1f;

    if(delta1 > 1)
    {
    	delta1 = 0;
        Vertex3 p = camControlPoints_.front();
        camControlPoints_.pop_front();
        camControlPoints_.push_back(p);
    }

    //TRACE_LOG("position=" << position);
	

}

void MyApp::LateUpdate()
{
	//TRACE_LOG("MyApp::LateUpdate");
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
			IMGUI::pCurrentNode->SetOrientation(glm::angleAxis(x_angle_, Vertex3(0, 0, 1)));

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
	IMGUI::pCurrentNode->SetScale(Vertex3(aspectRatio_, 1, 1));

    IMGUIBeginHorizontal();

    //IMGUISpacer(80);

    static std::string str0 = "Abc";
	str0 = IMGUITextField(str0);
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

	pText3_->GetAtlas()->Show(pText3_->GetAtlas());

    pCamera1_->Activate();

    pLight0_->Render();

	pRender2Texture_->Begin();
    pCubeSceneNode_->Render(true);
    pEarthSceneNode_->Render(true);
    pRender2Texture_->End();

    std::stringstream ss;
    ss << "Mouse x=" << x_ << " y=" << y_;

    if(buttonDown_)
    {
    	ss << " BDown";
    }
    else if(buttonUp_)
    {
    	ss << " BUp";
    }

	ss << " S=" << std::hex << selectedIndex_;

	GLES2Camera::Deactivate();
	pText1_->SetText(ss.str());
	pText1_->Render(pTextNode1_, Color(1,1,1,1));

	pCamera1_->Activate();
}

void MyApp::ViewChanged(int32_t width, int32_t height) 
{
    //aspectRatio_ = (float)height/(float)width;
}

void MyApp::OnMouseMove(float x, float y)
{
	x_=x;
	y_=y;
}

void MyApp::OnMouseDown(float x, float y)
{
	x_=x;
	y_=y;
	
	buttonDown_ = true;
	buttonUp_ = false;

	selectedIndex_ = GetSelectedNode();
    
    TRACE_LOG("selected=" << selectedIndex_);
}

void MyApp::OnMouseUp()
{
	buttonDown_ = false;
	buttonUp_ = true;
}

