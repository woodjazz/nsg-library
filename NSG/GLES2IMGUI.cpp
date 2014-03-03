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
#include "GLES2IMGUI.h"
#include "Types.h"
#include "GLES2PlaneMesh.h"
#include "Node.h"

using namespace NSG;

const char s_fragShaderSource[] = {
#include "shaders/gles2GUIFragmentShader.h"
};

const char s_vertexShaderSource[] = {
#include "shaders/gles2GUIVertexShader.h"
};

static PGLES2PlaneMesh s_pPlaneMesh;
static int32_t s_width = 0;
static int32_t s_height = 0;
static GLES2Camera* s_pCamera = nullptr;

namespace NSG 
{
	namespace IMGUI
	{
		void AllocateResources()
		{
			assert(s_pPlaneMesh == nullptr);
			float width = 1;
			float height = 1;
			int columns = 2;
			int rows = 2;
			PGLES2Program pProgram(new GLES2Program(s_vertexShaderSource, s_fragShaderSource));
			PGLES2Material pMaterial(new GLES2Material(nullptr, pProgram));
			pMaterial->SetDiffuseColor(Color(1,0,0,1));
			s_pPlaneMesh = PGLES2PlaneMesh(new GLES2PlaneMesh(width, height, columns, rows, pMaterial, GL_STATIC_DRAW));
		}

		void ReleaseResources()
		{
			s_pPlaneMesh = nullptr;
		}


		void Begin()
		{
			s_pCamera = GLES2Camera::GetActiveCamera();
			GLES2Camera::Deactivate();
			glViewport(0, 0, s_width, s_height);
		}

		void End()
		{
			if(s_pCamera)
				s_pCamera->Activate();
		}

		void DrawRect(float x, float y, float w, float h)
		{
			PNode pNode(new Node());
			pNode->SetPosition(Vertex3(x,y,0));
			pNode->SetScale(Vertex3(w, h, 1));
			s_pPlaneMesh->Render(pNode);
		}

		void ViewChanged(int32_t width, int32_t height)
		{
			s_width = width;
			s_height = height;
		}
	}
}