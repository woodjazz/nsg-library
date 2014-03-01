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
#include "App.h"
#include "NSG/Log.h"
#include "NSG/GLES2Texture.h"
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

namespace 
{
	const float kFovY = 45.0f;
	const float kZNear = 0.1f;
	const float kZFar = 100.0f;
}

App::App() 
: x_angle_(0),
y_angle_(0),
pCamera1_(new Camera(kFovY, kZNear, kZFar)),
pCamera2_(new Camera(kFovY, kZNear, kZFar)),
width_(1),
height_(1),
x_(0),
y_(0),
buttonDown_(false),
buttonUp_(false),
selectedIndex_(0),
scale_(1,1,1)
{
}

App::~App() 
{
	thread_.join();
}

void App::InternalTask() 
{
	FSMExamples();
	FSMTest();
	TimedTaskTest();
	QueuedTaskTest();
}

int App::GetFPS() const
{
	return 30;
}

void App::Start() 
{
	TRACE_LOG("Start");

	thread_ = std::thread([this](){InternalTask();});	

	PResource pVResource(new Resource("shaders/DiffuseSpecularReflection.vert"));
	PResource pFResource(new Resource("shaders/Simple.frag"));
	PGLES2Program pDiffuseProgram(new GLES2Program(pVResource, pFResource));
	PGLES2Texture pTexture(new GLES2Texture("cube_example.png"));
	PGLES2Texture pEarthTexture(new GLES2Texture("Earthmap720x360_grid.jpg"));
	pMaterial2_ = PGLES2Material(new GLES2Material (pEarthTexture, pDiffuseProgram));
	pMaterial2_->SetDiffuseColor(Color(1.0f,1.0f,1.0f,1));
	pMaterial2_->SetSpecularColor(Color(1.0f,0.0f,0.0f,1));
	pMaterial2_->SetShininess(0.3f);
	PGLES2Material pMaterial3(new GLES2Material (pTexture, pDiffuseProgram));

	pSphereMesh_ = PSphereMesh(new SphereMesh(Color(0,0,0,1), 3, 32, pMaterial2_, GL_STATIC_DRAW));
	pMesh_ = PBoxMesh(new BoxMesh(Color(1,1,1,1), 1,1,1, 2,2,2, pMaterial3, GL_STATIC_DRAW));


    pNode1_ = PNode(new Node);
    pNode2_ = PNode(new Node);

	pTextNode0_ = PNode(new Node);
    pTextNode1_ = PNode(new Node(pTextNode0_));

    pNode1_->SetPosition(Vertex3(-5, 0, 0));
    pNode1_->SetScale(Vertex3(3,3,3));

    pNode2_->SetPosition(Vertex3(5, 0, 0));

    pCamera1_->SetLookAt(Vertex3(0,0,10), Vertex3(0,0,0));
    pCamera2_->SetLookAt(Vertex3(0,5,5), Vertex3(0,0,0));
    pCamera2_->SetViewport(0.75f, 0.75f, 0.25f, 0.25f);

    pText1_ = PText(new Text("font/FreeSans.ttf", 24, GL_DYNAMIC_DRAW));
    pText2_ = PText(new Text("font/bluebold.ttf", 24, GL_STATIC_DRAW));
    pText3_ = PText(new Text("font/FreeSans.ttf", 48, GL_STATIC_DRAW));

    pLight0_ = PLight(new Light());
    pLight0_->SetPosition(Vertex3(-1.0,  0.0,  5.0));
	//pLight0_->SetDirectional();

}

void App::Update(float delta) 
{
	//TRACE_LOG("App::Update delta = " << delta << "\n");
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

	pTextNode0_->SetPosition(Vertex3(-pText1_->GetWidth()/2, 0, 0));
	pTextNode1_->SetOrientation(glm::angleAxis(x_angle_, Vertex3(0, 0, 1)));

	//pMaterial2_->SetDiffuseColor(Color(1,1,1,scale_.x/10.0f));


	pLight0_->SetPosition(Vertex3(scale_.x,  2.0,  6.0));
	//pLight0_->SetOrientation(glm::angleAxis(x_angle_, Vertex3(1, 0, 0)));
    //pNode1_->SetLookAt(Vertex3(0,scale_.x*25,10));
}

void App::LateUpdate()
{
	//TRACE_LOG("App::LateUpdate\n");
}

void App::RenderFrame() 
{
	//TRACE_LOG("App::RenderFrame\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(1, 1, 1, 1);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    pCamera1_->Activate();

    pLight0_->Render();

    pMesh_->SetMode(Mesh::TRIANGLES);
    pSphereMesh_->SetMode(Mesh::TRIANGLES);

    pMesh_->Render(pNode1_);
    pSphereMesh_->Render(pNode2_);

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

	Camera::Deactivate();
	pText1_->Render(pTextNode1_, Color(0,0,0,1), ss.str());

    //pText1_->RenderText(Color(1,0,0,1), "(-1,-1)");

    //pText2_->RenderText(Color(0,1,0,1), "(-1,1)");

//	screenWidth = pText2_->GetWidth();
//	screenHeight = pText2_->GetHeight();

    //pText3_->RenderText(Color(0,0,1,1), "(1,-1)");
    //pText3_->RenderText(Color(0,0,1,1), "(1,1)");

    
    pCamera2_->Activate();

    pMesh_->SetMode(Mesh::LINES);
    pSphereMesh_->SetMode(Mesh::LINES);

    pMesh_->Render(pNode1_);
    pSphereMesh_->Render(pNode2_);

	pFrameColorSelection_->Begin(x_, y_);

    pCamera1_->Activate();

    pFrameColorSelection_->Render(0xFF00, pMesh_, pNode1_);
    pFrameColorSelection_->Render(0x12AB, pSphereMesh_, pNode2_);

    pCamera2_->Activate();

    pFrameColorSelection_->Render(0x0001, pMesh_, pNode1_);
    pFrameColorSelection_->Render(0x0002, pSphereMesh_, pNode2_);

    pFrameColorSelection_->End();
}

void App::ViewChanged(int32_t width, int32_t height) 
{
	glViewport(0, 0, width, height);

	width_ = width;
	height_ = height;

	pCamera1_->ViewChanged(width, height);
	pCamera2_->ViewChanged(width, height);

    if(!pFrameColorSelection_)
        pFrameColorSelection_ = PFrameColorSelection(new FrameColorSelection());

	pFrameColorSelection_->ViewChanged(width, height);
}

void App::OnMouseMove(double x, double y)
{
	x_=x;
	y_=y;
}

void App::OnMouseDown(double x, double y)
{
	x_=x;
	y_=y;
	
	buttonDown_ = true;
	buttonUp_ = false;

	selectedIndex_ = pFrameColorSelection_->GetSelected();
}

void App::OnMouseUp()
{
	buttonDown_ = false;
	buttonUp_ = true;
}

