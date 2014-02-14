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
#include <stdio.h>
#include <stdlib.h>
#include <string>

extern void FSMExamples();
extern void FSMTest();
extern void TimedTaskTest();
extern void QueuedTaskTest();

using namespace NSG;

namespace 
{
	const float kFovY = 45.0f;
	const float kZNear = 1.0f;
	const float kZFar = 10.0f;
}

App::App() 
: x_angle_(0),
y_angle_(0),
pCamera1_(new Camera(kFovY, kZNear, kZFar)),
pCamera2_(new Camera(kFovY, kZNear, kZFar))
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

const char kFragShaderSource[] = {
#include "shaders/gles2FragmentShader.h"
};

const char kVertexShaderSource[] = {
#include "shaders/gles2VertexShader.h"
};

void App::Start() 
{
	TRACE_LOG("Start");

	thread_ = std::thread([this](){InternalTask();});	

	pProgram_ = PGLES2Program(new GLES2Program(kVertexShaderSource, kFragShaderSource));

	pMesh_ = PBoxMesh(new BoxMesh(pProgram_, PGLES2Texture(new GLES2Texture("cube_example.png"))));

    pNode1_ = PNode(new Node);
    pNode2_ = PNode(new Node);

    pNode1_->SetPosition(Vertex3(-2, 0, 0));
    pNode2_->SetPosition(Vertex3(2, 0, 0));

    pCamera1_->SetBackgroundColor(Color(0, 0, 0, 1));
    pCamera2_->SetBackgroundColor(Color(0.3f, 0.3f, 0.4f, 1));

    pCamera1_->SetLookAt(Vertex3(0,5,5), Vertex3(0,0,0), Vertex3(0,1,0));
    pCamera2_->SetLookAt(Vertex3(0,5,5), Vertex3(0,0,0), Vertex3(0,1,0));
    pCamera2_->SetViewport(0.75f, 0.75f, 0.25f, 0.25f);
}

void App::Update(float delta) 
{
	//TRACE_LOG("App::Update delta = " << delta);
	x_angle_ += glm::pi<float>()/10.0f * delta;
	y_angle_ += glm::pi<float>()/10.0f * delta;
	pNode1_->SetRotation(glm::angleAxis(x_angle_, Vertex3(1, 0, 0)));
	pNode2_->SetRotation(glm::angleAxis(y_angle_, Vertex3(0, 1, 0)));
}

void App::LateUpdate()
{
	//TRACE_LOG("App::LateUpdate");
}

void App::RenderFrame() 
{
	//TRACE_LOG("App::RenderFrame");

    pCamera1_->Activate();

    pMesh_->Render(pNode1_);
    pMesh_->Render(pNode2_);

    pCamera2_->Activate(false);

    pMesh_->Render(pNode1_);
    pMesh_->Render(pNode2_);

}

void App::ViewChanged(int32_t width, int32_t height) 
{
	TRACE_LOG("App::ViewChanged width=" << width << " height=" << height);	
	pCamera1_->ViewChanged(width, height);
	pCamera2_->ViewChanged(width, height);
}
