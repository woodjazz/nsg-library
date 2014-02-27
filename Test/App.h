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
#include "NSG/IApp.h"
#include "NSG/BoxMesh.h"
#include "NSG/SphereMesh.h"
#include "NSG/Node.h"
#include "NSG/Camera.h"
#include "NSG/Text.h"
#include "NSG/FrameColorSelection.h"

using namespace NSG;

class App : public NSG::IApp 
{
public:
	App();
	~App();
	void Start();
	void Update(float delta);
	void LateUpdate();
	void RenderFrame();
	void ViewChanged(int32_t width, int32_t height);
	void OnMouseMove(double x, double y);
    void OnMouseDown(double x, double y);
    void OnMouseUp();

private:
	void InternalTask();
	std::thread thread_;
	PGLES2Material pMaterial2_;
	PBoxMesh pMesh_;
	PSphereMesh pSphereMesh_;
	PNode pNode1_;
	PNode pNode2_;
	PNode pTextNode0_;
	PNode pTextNode1_;
	PCamera pCamera1_;
	PCamera pCamera2_;
	float x_angle_;
	float y_angle_;
	PText pText1_;
	PText pText2_;
	PText pText3_;
	int32_t width_;
	int32_t height_;
	double x_;
	double y_;
	bool buttonDown_;
	bool buttonUp_;
	GLushort selectedIndex_;
	PFrameColorSelection pFrameColorSelection_;
	Vertex3 scale_;
};

