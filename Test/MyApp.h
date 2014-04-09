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
#pragma once
#include <thread>
#include <deque>
#include "NSG/App.h"
#include "NSG/GLES2BoxMesh.h"
#include "NSG/GLES2SphereMesh.h"
#include "NSG/GLES2PlaneMesh.h"
#include "NSG/Node.h"
#include "NSG/GLES2Camera.h"
#include "NSG/GLES2Text.h"
#include "NSG/GLES2Texture.h"
#include "NSG/GLES2Light.h"
#include "NSG/GLES2IMGUI.h"
#include "NSG/GLES2Render2Texture.h"

using namespace NSG;

class MyApp : public NSG::App 
{
public:
	MyApp();
	~MyApp();
	int GetFPS() const;
	void Start();
	void Update();
	void LateUpdate();
	void RenderFrame();
	void RenderGUIFrame();
	void ViewChanged(int32_t width, int32_t height);
	void OnMouseMove(float x, float y);
    void OnMouseDown(float x, float y);
    void OnMouseUp();

private:
	void InternalTask();
	void TestIMGUI2();
    void TestIMGUI4();
    void Menu1();
	std::thread thread_;
	PGLES2Light pLight0_;
	PGLES2Material pMaterial2_;
	PGLES2BoxMesh pMesh_;
	PGLES2SphereMesh pSphereMesh_;
	PNode pNode1_;
	PNode pNode2_;
	PNode pNode3_;
	PNode pTextNode0_;
	PNode pTextNode1_;
	PGLES2Camera pCamera1_;
	PGLES2Camera pCamera2_;
	float x_angle_;
	float y_angle_;
	PGLES2Text pText1_;
	PGLES2Text pText2_;
	PGLES2Text pText3_;
	float x_;
	float y_;
	bool buttonDown_;
	bool buttonUp_;
	GLushort selectedIndex_;
	Vertex3 scale_;
    std::deque<Vertex3> camControlPoints_;
    IMGUI::PSkin pSkin1_;
    IMGUI::PSkin pSkin2_;
    float aspectRatio_;
    PGLES2Texture pRenderedTexture_;
    PGLES2Render2Texture pRender2Texture_;
};

