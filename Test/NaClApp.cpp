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
#include "Log.h"
#include "NaCl3DInstance.h"
#include <GLES2/gl2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>

extern void FSMExamples();
extern void FSMTest();
extern void TimedTaskTest();
extern void QueuedTaskTest();

using namespace NSG::NaCl;

NaClApp::NaClApp() : width_(0), height_(0), loaded_(false)
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


void NaClApp::Setup() 
{
	thread_ = std::thread([this](){InternalTask();});	
	printf("Setup");
}

void NaClApp::Update() 
{
	if(pLoader_ == nullptr)
		return;

	if(pLoader_->IsDone())
	{
		TRACE_LOG("FILE LOADED!!! SIZE=" << pLoader_->GetData().size());
		pLoader_ = nullptr;
	}
}

void NaClApp::Draw() 
{
	glClearColor(0.5, 0.2, 0.2, 1);
	glClearDepthf(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void NaClApp::ViewChanged(int32_t width, int32_t height) 
{
	width_ = width;
	height_ = height;
	glViewport(0, 0, width_, height_);
	TRACE_LOG("ViewChanged width=" << width_ << " height=" << height_);
}
#endif
