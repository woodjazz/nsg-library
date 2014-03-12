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
#include "NSG/Log.h"
#include "NSG/GLES2Texture.h"
#include "NSG/GLES2IMGUI.h"
#include "NSG/Resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

extern void FSMExamples();
extern void FSMTest();
extern void TimedTaskTest();
extern void QueuedTaskTest();

using namespace NSG;


MyApp::MyApp() 
: x_angle_(0),
y_angle_(0),
x_(0),
y_(0),
buttonDown_(false),
buttonUp_(false),
selectedIndex_(0),
scale_(1,1,1)
{
}

MyApp::~MyApp() 
{
	thread_.join();
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

    PResource pVResource(new Resource("shaders/DiffuseSpecularReflection.vert"));
	PResource pFResource(new Resource("shaders/Simple.frag"));
	PGLES2Program pDiffuseProgram(new GLES2Program(pVResource, pFResource));
	PGLES2Texture pTexture(new GLES2Texture("cube_example.png"));
	PGLES2Texture pEarthTexture(new GLES2Texture("Earthmap720x360_grid.jpg"));
	pMaterial2_ = PGLES2Material(new GLES2Material ());
    //pMaterial2_->SetProgram(pDiffuseProgram);
	pMaterial2_->SetMainTexture(pEarthTexture);
	pMaterial2_->SetDiffuseColor(Color(1.0f,1.0f,1.0f,1));
	pMaterial2_->SetSpecularColor(Color(1.0f,0.0f,0.0f,1));
	pMaterial2_->SetShininess(0.3f);

	PGLES2Material pMaterial3(new GLES2Material ());
    pMaterial3->SetProgram(pDiffuseProgram);
	pMaterial3->SetMainTexture(pTexture);

	pSphereMesh_ = PGLES2SphereMesh(new GLES2SphereMesh(3, 32, pMaterial2_, GL_STATIC_DRAW));
	pMesh_ = PGLES2BoxMesh(new GLES2BoxMesh(1,1,1, 2,2,2, pMaterial3, GL_STATIC_DRAW));

    pNode1_ = PNode(new Node);
    pNode2_ = PNode(new Node);
    pNode3_ = PNode(new Node);

	pTextNode0_ = PNode(new Node);
    pTextNode1_ = PNode(new Node(pTextNode0_));

    pNode1_->SetPosition(Vertex3(-5, 0, 0));
    pNode1_->SetScale(Vertex3(3,3,3));

    pNode2_->SetPosition(Vertex3(5, 0, 0));
    //pNode2_->SetScale(Vertex3(30,30,30));

    pCamera1_->SetPosition(Vertex3(0,0,10));
    pCamera1_->SetLookAt(Vertex3(0,0,0));

    pCamera2_->SetPosition(Vertex3(0,5,5));
    pCamera2_->SetLookAt(Vertex3(0,0,0));
    pCamera2_->SetViewport(0.75f, 0.75f, 0.25f, 0.25f);

    pText1_ = PGLES2Text(new GLES2Text("font/FreeSans.ttf", 12, GL_DYNAMIC_DRAW));
    pText2_ = PGLES2Text(new GLES2Text("font/bluebold.ttf", 24, GL_STATIC_DRAW));
    pText3_ = PGLES2Text(new GLES2Text("font/FreeSans.ttf", 48, GL_STATIC_DRAW));

    pLight0_ = PGLES2Light(new GLES2Light());
    pLight0_->SetPosition(Vertex3(-1.0,  0.0,  5.0));
	//pLight0_->SetDirectional();

}

void MyApp::Update(float delta) 
{
	//TRACE_LOG("MyApp::Update delta = " << delta << "\n");
	x_angle_ += glm::pi<float>()/10.0f * delta;
	y_angle_ += glm::pi<float>()/10.0f * delta;
	pNode1_->SetOrientation(glm::angleAxis(x_angle_, Vertex3(1, 0, 0)) * glm::angleAxis(y_angle_, Vertex3(0, 0, 1)));
	pNode2_->SetOrientation(glm::angleAxis(y_angle_, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle_, Vertex3(0, 1, 0)));
	//pNode1_->SetScale(scale_);


	static float factor_scale = 1;
	scale_ -= delta * 0.7f * factor_scale;

	if(scale_.x < -10 && factor_scale > 0)
		factor_scale *= -1;
	else if(scale_.x > 10 && factor_scale < 0)
		factor_scale *= -1;

	pTextNode0_->SetPosition(Vertex3(-pText1_->GetWidth()/2, 0, 0.2f));
    //pTextNode1_->SetPosition(Vertex3(200, 100, 0));
	pTextNode1_->SetOrientation(glm::angleAxis(x_angle_, Vertex3(0, 0, 1)));

	//pMaterial2_->SetDiffuseColor(Color(1,1,1,scale_.x/10.0f));


	pLight0_->SetPosition(Vertex3(scale_.x,  2.0,  6.0));
	//pLight0_->SetOrientation(glm::angleAxis(x_angle_, Vertex3(1, 0, 0)));
    //pNode1_->SetLookAt(Vertex3(0,scale_.x*25,10));
}

void MyApp::LateUpdate()
{
	//TRACE_LOG("MyApp::LateUpdate\n");
}

void MyApp::RenderGUIFrame()
{

	IMGUI::SetActiveColor(Color(0,0,1,0.9f));
	
	IMGUI::SetButtonType(IMGUI::Rectangle);

	IMGUI::SetPosition(Vertex3(0,0,0));
	IMGUI::SetSize(IMGUI::ConvertPixels2ScreenCoords(Vertex3(100, 100, 1)));

	IMGUI::Fill(false);

	IMGUI::SetFont("font/FreeSans.ttf", 24);
	
	if(IMGUIButton("Button 1"))
	{
		TRACE_LOG("Button 1 pressed\n");
	}

	IMGUI::SetButtonType(IMGUI::Circle);

    Vertex3 position = IMGUI::GetPosition();
    position.x += IMGUI::GetSize().x;
	IMGUI::SetPosition(position);	
    
    if(IMGUIButton("Button 2"))
	{
		TRACE_LOG("Button 2 pressed\n");
	}

	IMGUI::SetFont("font/FreeSans.ttf", 64);

	IMGUI::SetButtonType(IMGUI::Ellipse);

	IMGUI::SetPosition(IMGUI::ConvertPixels2ScreenCoords(Vertex3(-200, 200, 0)));	
	IMGUI::SetSize(IMGUI::ConvertPixels2ScreenCoords(Vertex3(100, 50, 1)));

    if(IMGUIButton("Button 3"))
	{
		TRACE_LOG("Button 3 pressed\n");
	}

    position = IMGUI::GetPosition();
    position.y -= IMGUI::GetSize().y;
	IMGUI::SetPosition(position);

	IMGUI::SetButtonType(IMGUI::RoundedRectangle);

	IMGUI::Fill(true);
    
    if(IMGUIButton("Button 4"))
	{
		TRACE_LOG("Button 4 pressed\n");
	}

	IMGUI::SetPosition(IMGUI::ConvertPixels2ScreenCoords(Vertex3(100, 150, 0)));	
	IMGUI::SetSize(IMGUI::ConvertPixels2ScreenCoords(Vertex3(300, 300, 1)));

    if(IMGUIButton("Button 5"))
	{
		TRACE_LOG("Button 5 pressed\n");
	}

	IMGUI::SetPosition(IMGUI::ConvertPixels2ScreenCoords(Vertex3(100, -150, 0)));	
	IMGUI::SetSize(IMGUI::ConvertPixels2ScreenCoords(Vertex3(300, 100, 1)));

	static std::string str0 = "Abc";
	str0 = IMGUITextField(str0);

    IMGUI::SetFont("font/FreeSans.ttf", 18);
	IMGUI::SetButtonType(IMGUI::Rectangle);
	IMGUI::SetPosition(IMGUI::ConvertPixels2ScreenCoords(Vertex3(100, -250, 0)));	
	IMGUI::SetSize(IMGUI::ConvertPixels2ScreenCoords(Vertex3(300, 100, 1)));

	static std::string str1 = "XYZ";
	str1 = IMGUITextField(str1);

}

void MyApp::RenderFrame() 
{
	//TRACE_LOG("MyApp::RenderFrame\n");

	//pText1_->ShowAtlas();

    pCamera1_->Activate();

    //pLight0_->Render();

    pMesh_->SetMode(GL_TRIANGLES);
    pSphereMesh_->SetMode(GL_TRIANGLES);

    //pMesh_->Render(pNode1_);
    //pSphereMesh_->Render(pNode2_);

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
	//pText1_->Render(pTextNode1_, Color(1,1,1,1));

    //pText1_->RenderText(Color(1,0,0,1), "(-1,-1)");

    //pText2_->RenderText(Color(0,1,0,1), "(-1,1)");

//	screenWidth = pText2_->GetWidth();
//	screenHeight = pText2_->GetHeight();

    //pText3_->RenderText(Color(0,0,1,1), "(1,-1)");
    //pText3_->RenderText(Color(0,0,1,1), "(1,1)");

    
    pCamera2_->Activate();

    pMesh_->SetMode(GL_LINES);
    pSphereMesh_->SetMode(GL_LINES);

    //pMesh_->Render(pNode1_);
    //pSphereMesh_->Render(pNode2_);
}

void MyApp::ViewChanged(int32_t width, int32_t height) 
{
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
    
    TRACE_LOG("selected=" << selectedIndex_ << "\n");
}

void MyApp::OnMouseUp()
{
	buttonDown_ = false;
	buttonUp_ = true;
}

