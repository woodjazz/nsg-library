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
including commercial NaClApplications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
NaClAppreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#ifdef NACL
#include "NaClApp.h"
#include "NSG/Log.h"
#include "NSG/NaCl3DInstance.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>

#define GLM_FORCE_RADIANS
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "soil/SOIL.h"

extern void FSMExamples();
extern void FSMTest();
extern void TimedTaskTest();
extern void QueuedTaskTest();

namespace 
{
	const float kFovY = 45.0f;
	const float kZNear = 1.0f;
	const float kZFar = 10.0f;
	const float kCameraZ = -4.0f;
	const float kXAngleDelta = 2.0f;
	const float kYAngleDelta = 0.5f;

	struct Vertex {
		float loc[3];
		float color[3];
		float tex[2];
	};

	const Vertex kCubeVerts[24] = {
		// +Z (red arrow, black tip)
		{{-1.0, -1.0, +1.0}, {0.0, 0.0, 0.0}, {1.0, 0.0}},
		{{+1.0, -1.0, +1.0}, {0.0, 0.0, 0.0}, {0.0, 0.0}},
		{{+1.0, +1.0, +1.0}, {0.5, 0.0, 0.0}, {0.0, 1.0}},
		{{-1.0, +1.0, +1.0}, {0.5, 0.0, 0.0}, {1.0, 1.0}},

		// +X (green arrow, black tip)
		{{+1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}, {1.0, 0.0}},
		{{+1.0, +1.0, -1.0}, {0.0, 0.0, 0.0}, {0.0, 0.0}},
		{{+1.0, +1.0, +1.0}, {0.0, 0.5, 0.0}, {0.0, 1.0}},
		{{+1.0, -1.0, +1.0}, {0.0, 0.5, 0.0}, {1.0, 1.0}},

		// +Y (blue arrow, black tip)
		{{-1.0, +1.0, -1.0}, {0.0, 0.0, 0.0}, {1.0, 0.0}},
		{{-1.0, +1.0, +1.0}, {0.0, 0.0, 0.0}, {0.0, 0.0}},
		{{+1.0, +1.0, +1.0}, {0.0, 0.0, 0.5}, {0.0, 1.0}},
		{{+1.0, +1.0, -1.0}, {0.0, 0.0, 0.5}, {1.0, 1.0}},

		// -Z (red arrow, red tip)
		{{+1.0, +1.0, -1.0}, {0.0, 0.0, 0.0}, {1.0, 1.0}},
		{{-1.0, +1.0, -1.0}, {0.0, 0.0, 0.0}, {0.0, 1.0}},
		{{-1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}, {0.0, 0.0}},
		{{+1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}, {1.0, 0.0}},

		// -X (green arrow, green tip)
		{{-1.0, +1.0, +1.0}, {0.0, 0.0, 0.0}, {1.0, 1.0}},
		{{-1.0, -1.0, +1.0}, {0.0, 0.0, 0.0}, {0.0, 1.0}},
		{{-1.0, -1.0, -1.0}, {0.0, 1.0, 0.0}, {0.0, 0.0}},
		{{-1.0, +1.0, -1.0}, {0.0, 1.0, 0.0}, {1.0, 0.0}},

		// -Y (blue arrow, blue tip)
		{{+1.0, -1.0, +1.0}, {0.0, 0.0, 0.0}, {1.0, 1.0}},
		{{+1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}, {0.0, 1.0}},
		{{-1.0, -1.0, -1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0}},
		{{-1.0, -1.0, +1.0}, {0.0, 0.0, 1.0}, {1.0, 0.0}},
	};

	const GLubyte kCubeIndexes[36] = {
		2,  1,  0,  3,  2,  0,
		6,  5,  4,  7,  6,  4,
		10,  9,  8, 11, 10,  8,
		14, 13, 12, 15, 14, 12,
		18, 17, 16, 19, 18, 16,
		22, 21, 20, 23, 22, 20,
	};
}

NaClApp::NaClApp() 
: width_(0), 
height_(0), 
texture_(0), 
texture_loc_(0),
position_loc_(0),
color_loc_(0),
mvp_loc_(0),
x_angle_(0),
y_angle_(0),
animating_(true)
{
}

NaClApp::~NaClApp() 
{
	thread_.join();
}

void NaClApp::InternalTask() 
{
	FSMExamples();
	FSMTest();
	TimedTaskTest();
	QueuedTaskTest();
}

void NaClApp::HandleMessage(const pp::Var& var_message)
{
	TRACE_LOG("NaClApp::HandleMessage");

	if(var_message.is_string())
	{
		std::string message = var_message.AsString();
		pLoader_ = NaClURLLoader::Create(NaCl3DInstance::GetInstance(), message);
	}
}

const char kFragShaderSource[] = {
#include "shaders/gles2FragmentShader.h"
};

const char kVertexShaderSource[] = {
#include "shaders/gles2VertexShader.h"
};

void NaClApp::Setup() 
{
	TRACE_LOG("Setup");

	thread_ = std::thread([this](){InternalTask();});	

	pProgram_ = PGLES2Program(new GLES2Program(kVertexShaderSource, kFragShaderSource));

	texture_loc_ = pProgram_->GetUniformLocation("u_texture");
	position_loc_ = pProgram_->GetAttributeLocation("a_position");
	texcoord_loc_ = pProgram_->GetAttributeLocation("a_texcoord");
	color_loc_ = pProgram_->GetAttributeLocation("a_color");
	mvp_loc_ = pProgram_->GetUniformLocation("u_mvp");

	pVBuffer_ = PGLES2VertexBuffer(new GLES2VertexBuffer(sizeof(kCubeVerts), &kCubeVerts[0]));
	pIBuffer_ = PGLES2IndexBuffer(new GLES2IndexBuffer(sizeof(kCubeIndexes), &kCubeIndexes[0]));
}

bool NaClApp::Update() 
{
	static bool resources_loaded = false;

	if(pLoader_ && pLoader_->IsDone())
	{
		TRACE_LOG("FILE LOADED!!! SIZE=" << pLoader_->GetData().size());

        int img_width, img_height;
        unsigned char* img = SOIL_load_image_from_memory((const unsigned char *const)pLoader_->GetData().c_str(), pLoader_->GetData().size(), &img_width, &img_height, NULL, 0);
        glGenTextures(1, &texture_);
        glBindTexture(GL_TEXTURE_2D, texture_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGB,
			img_width,
			img_height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			img);

		pLoader_ = nullptr;

		SOIL_free_image_data(img);

		resources_loaded = true;
	}

	return resources_loaded;
}

void NaClApp::Draw() 
{
	if (animating_) {
		x_angle_ = fmod(360.0f + x_angle_ + kXAngleDelta, 360.0f);
		y_angle_ = fmod(360.0f + y_angle_ + kYAngleDelta, 360.0f);
	}

	glClearColor(0.5, 0.2, 0.2, 1);
	glClearDepthf(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//set what program to use
	pProgram_->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glUniform1i(texture_loc_, 0);

	glm::mat4 matRotateY;
	glm::mat4 matRotateX;

	// Creates matrix rotations to X and Y.
	matRotateX = glm::rotate(matRotateX, x_angle_, glm::vec3(1.0f, 0.0f, 0.0f));
	matRotateY = glm::rotate(matRotateY, y_angle_, glm::vec3(0.0f, 1.0f, 0.0f));

	// Translates the matrix, by going back in the Z axis.
	// Translation can be done without new matrices, look in Utils.h file.
	glm::mat4 matTranslate;
	matTranslate = glm::translate(matTranslate, glm::vec3(0.0f, 0.0f, kCameraZ));

	// This order produces a Local rotation (premultiply).
	// To see a Global rotation just the the order of this multiplication.
	// Like this matrixMultiply(matRotateX, matRotateY, matModelView);
	glm::mat4 matModelView = matTranslate * matRotateX * matRotateY;


	// Creates Projection Matrix.
	const float aspect_ratio = static_cast<float>(width_) / height_;
	glm::mat4 matProjection = glm::perspective(kFovY, aspect_ratio, kZNear, kZFar);

	// Multiplies the Projection by the ModelView to create the ModelViewProjection matrix.
	glm::mat4 matModelViewProjection = matProjection * matModelView;

	glUniformMatrix4fv(mvp_loc_, 1, GL_FALSE, glm::value_ptr(matModelViewProjection));

	//define the attributes of the vertex
	pVBuffer_->Bind();

	glVertexAttribPointer(position_loc_,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, loc)));
	glEnableVertexAttribArray(position_loc_);

	glVertexAttribPointer(color_loc_,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, color)));
	glEnableVertexAttribArray(color_loc_);

	glVertexAttribPointer(texcoord_loc_,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, tex)));
	glEnableVertexAttribArray(texcoord_loc_);

	pIBuffer_->Bind();

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);

}

void NaClApp::ViewChanged(int32_t width, int32_t height) 
{
	width_ = width;
	height_ = height;
	glViewport(0, 0, width_, height_);
	TRACE_LOG("ViewChanged width=" << width_ << " height=" << height_);
}
#endif
