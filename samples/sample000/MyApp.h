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
#include "NSG.h"
using namespace NSG;


class MyApp : public NSG::App 
{
public:
	MyApp();
	~MyApp();
	int GetFPS() const;
	void Start();
	void Update();
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
	PGLES2Light pLight0_;
	PGLES2Camera pCamera1_;
	PGLES2Camera pCamera2_;
    IMGUI::PSkin pSkin1_;
    IMGUI::PSkin pSkin2_;
    PGLES2Texture pRenderedTexture_;
    PGLES2Render2Texture pRender2Texture_;
    PSceneNode pEarthSceneNode_;
    PSceneNode pCubeSceneNode_;
    PSceneNode pTextSceneNode_;
    PModel pModel_;
    PGLES2Filter pFilter_;
    PGLES2Filter pFilterBlend_;
    PGLES2Texture pFilteredTexture_;
    PGLES2Texture pBlendedTexture_;
};

