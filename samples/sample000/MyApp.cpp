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
	AppConfiguration::this_->width_ = 640;
	AppConfiguration::this_->height_ = 480;
	AppConfiguration::this_->showStatistics_ = true;
}

MyApp::~MyApp() 
{
}

void MyApp::Start() 
{
	TRACE_LOG("Start");

    scene_ = PScene(new Scene);

	pCamera1_ = scene_->CreateCamera();
    pCamera1_->Activate();
	pCamera1_->SetBehavior(PBehavior(new CameraBehavior()));

	render2TextureSceneNode_ = scene_->CreateSceneNode();
    render2TextureSceneNode_->SetBehavior(PBehavior(new Render2TextureBehavior));

	pEarthSceneNode_ = scene_->CreateSceneNode();
    pEarthSceneNode_->SetBehavior(PBehavior(new EarthBehavior()));

	pCubeSceneNode_ = scene_->CreateSceneNode();
    pCubeSceneNode_->SetBehavior(PBehavior(new CubeBehavior()));

	pLight0_ = scene_->CreateLight();
    pLight0_->SetBehavior(PBehavior(new LightBehavior()));

    scene_->Start();
   
}

void MyApp::Update() 
{
	scene_->Update();
}

void MyApp::RenderFrame() 
{
    scene_->Render();
}

