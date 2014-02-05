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

#define GLM_FORCE_RADIANS
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

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
	const float kCameraZ = -4.0f;
	const float kXAngleDelta = 2.0f;
	const float kYAngleDelta = 0.5f;
}

App::App() 
: width_(0), 
height_(0),
x_angle_(0),
y_angle_(0)
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

#if NACL
void App::HandleMessage(const pp::Var& var_message)
{
	TRACE_LOG("App::HandleMessage");

	if(var_message.is_string())
	{
		std::string message = var_message.AsString();
	}
}
#endif

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

	pMeshObject_ = PMeshObject(new MeshObject(pProgram_, PGLES2Texture(new GLES2Texture("cube_example.png"))));

	mvp_loc_ = pProgram_->GetUniformLocation("u_mvp");

	pMeshObject_->AddVertexData(VertexData(Vertex3(-1.0, -1.0, +1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(1.0, 0.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(+1.0, -1.0, +1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(0.0, 0.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(+1.0, +1.0, +1.0), Vertex3(0.5, 0.0, 0.0), Vertex2(0.0, 1.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(-1.0, +1.0, +1.0), Vertex3(0.5, 0.0, 0.0), Vertex2(1.0, 1.0)));

	pMeshObject_->AddVertexData(VertexData(Vertex3(+1.0, -1.0, -1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(1.0, 0.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(+1.0, +1.0, -1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(0.0, 0.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(+1.0, +1.0, +1.0), Vertex3(0.0, 0.5, 0.0), Vertex2(0.0, 1.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(+1.0, -1.0, +1.0), Vertex3(0.0, 0.5, 0.0), Vertex2(1.0, 1.0)));

	pMeshObject_->AddVertexData(VertexData(Vertex3(-1.0, +1.0, -1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(1.0, 0.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(-1.0, +1.0, +1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(0.0, 0.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(+1.0, +1.0, +1.0), Vertex3(0.0, 0.0, 0.5), Vertex2(0.0, 1.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(+1.0, +1.0, -1.0), Vertex3(0.0, 0.0, 0.5), Vertex2(1.0, 1.0)));

	pMeshObject_->AddVertexData(VertexData(Vertex3(+1.0, +1.0, -1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(1.0, 1.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(-1.0, +1.0, -1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(0.0, 1.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(-1.0, -1.0, -1.0), Vertex3(1.0, 0.0, 0.0), Vertex2(0.0, 0.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(+1.0, -1.0, -1.0), Vertex3(1.0, 0.0, 0.0), Vertex2(1.0, 0.0)));

	pMeshObject_->AddVertexData(VertexData(Vertex3(-1.0, +1.0, +1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(1.0, 1.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(-1.0, -1.0, +1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(0.0, 1.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(-1.0, -1.0, -1.0), Vertex3(0.0, 1.0, 0.0), Vertex2(0.0, 0.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(-1.0, +1.0, -1.0), Vertex3(0.0, 1.0, 0.0), Vertex2(1.0, 0.0)));

	pMeshObject_->AddVertexData(VertexData(Vertex3(+1.0, -1.0, +1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(1.0, 1.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(+1.0, -1.0, -1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(0.0, 1.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(-1.0, -1.0, -1.0), Vertex3(0.0, 0.0, 1.0), Vertex2(0.0, 0.0)));
    pMeshObject_->AddVertexData(VertexData(Vertex3(-1.0, -1.0, +1.0), Vertex3(0.0, 0.0, 1.0), Vertex2(1.0, 0.0)));


	const IndexType kCubeIndexes[36] = {
		2,  1,  0,  3,  2,  0,
		6,  5,  4,  7,  6,  4,
		10,  9,  8, 11, 10,  8,
		14, 13, 12, 15, 14, 12,
		18, 17, 16, 19, 18, 16,
		22, 21, 20, 23, 22, 20,
	};

    Indexes indexes;
    indexes.insert(indexes.end(), &kCubeIndexes[0], &kCubeIndexes[sizeof(kCubeIndexes)/sizeof(IndexType)]);
    pMeshObject_->SetIndices(indexes);
    pMeshObject_->Redo();

    pMeshObject_->SetPosition(Vertex3(0, 0, 0));
}

void App::Update(float delta) 
{
	//TRACE_LOG("Update delta = " << delta);
	x_angle_ += glm::pi<float>()/10.0f * delta;
	y_angle_ += glm::pi<float>() * delta;
	pMeshObject_->SetRotation(glm::angleAxis(x_angle_, Vertex3(1, 0, 0)));
}

void App::LateUpdate()
{
	//TRACE_LOG("LateUpdate");
}

void App::RenderFrame() 
{
	glClearColor(0.5f, 0.2f, 0.2f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Creates Projection Matrix.
	const float aspect_ratio = static_cast<float>(width_) / height_;
	glm::mat4 matProjection = glm::perspective(kFovY, aspect_ratio, kZNear, kZFar);

	// Camera matrix
	glm::mat4 matView = glm::lookAt(
    	glm::vec3(0,3,3), // Camera position, in World Space
    	glm::vec3(0,0,0), // looks at
    	glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	);	

	glm::mat4 matModelViewProjection = matProjection * matView * pMeshObject_->GetModelView();

	glUniformMatrix4fv(mvp_loc_, 1, GL_FALSE, glm::value_ptr(matModelViewProjection));

    pMeshObject_->Render();
}

void App::ViewChanged(int32_t width, int32_t height) 
{
	width_ = width;
	height_ = height;
	glViewport(0, 0, width_, height_);
	TRACE_LOG("ViewChanged width=" << width_ << " height=" << height_);
}
