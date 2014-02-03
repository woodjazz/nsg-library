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
#pragma once
#include "NSG/IApp.h"
#include <thread>
#include "NSG/NaClURLLoader.h"
//#include <NSG/GLES2Includes.h>
#include "NSG/GLES2Program.h"
#include "NSG/GLES2VertexBuffer.h"
#include "NSG/GLES2IndexBuffer.h"


using namespace NSG;
using namespace NSG::NaCl;

class NaClApp : public NSG::IApp 
{
public:
	NaClApp();
	~NaClApp();
	void Setup();
	bool Update();
	void Draw();
	void ViewChanged(int32_t width, int32_t height);
	void HandleMessage(const pp::Var& var_message);
private:
	void InternalTask();

	int32_t width_;
	int32_t height_;
	std::thread thread_;
	PNaClURLLoader pLoader_;
	PGLES2Program pProgram_;
	PGLES2IndexBuffer pIBuffer_;
	PGLES2VertexBuffer pVBuffer_;

	GLuint texture_;

	GLuint texture_loc_;
	GLuint position_loc_;
	GLuint texcoord_loc_;
	GLuint color_loc_;
	GLuint mvp_loc_;

	float x_angle_;
	float y_angle_;
	bool animating_;
};

