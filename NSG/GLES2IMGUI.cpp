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
#include "GLES2PlaneMesh.h"
#include "Node.h"
#include "GLES2FrameColorSelection.h"
#include "GLES2Text.h"
#include <map>

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
		class TextManager
		{
		public:
			TextManager(const std::string& fontFile, int fontSize)
			: fontFile_(fontFile),
			fontSize_(fontSize)
			{
			}

			PGLES2Text GetTextMesh(GLushort item)
			{
				Key k(item);

				auto it = textMap_.find(k);

				if(it == textMap_.end())
				{
					PGLES2Text pTextMesh(new GLES2Text(fontFile_.c_str(), fontSize_, GL_STATIC_DRAW));

					textMap_.insert(TextMap::value_type(k, pTextMesh));

					return pTextMesh;
				}

				return it->second;
			}
		private:
			typedef GLushort Key;
			typedef std::map<Key, PGLES2Text> TextMap;
			TextMap textMap_;
			std::string fontFile_;
			int fontSize_;
		};

		TextManager textManager("font/FreeSans.ttf", 18);

		struct UIState
		{
			float pixelSizeX;
			float pixelSizeY;

			float mousex;
			float mousey;
			bool mousedown;

			GLushort hotitem;
			GLushort activeitem;
		};

		UIState uistate = {0,0,0,0,false,0,0};

		PGLES2FrameColorSelection pFrameColorSelection;		

		void AllocateResources()
		{
			assert(s_pPlaneMesh == nullptr);
			float width = 1;
			float height = 1;
			int columns = 2;
			int rows = 2;
			PGLES2Program pProgram(new GLES2Program(s_vertexShaderSource, s_fragShaderSource));
			PGLES2Material pMaterial(new GLES2Material(pProgram));
			pMaterial->SetDiffuseColor(Color(1,0,0,1));
			s_pPlaneMesh = PGLES2PlaneMesh(new GLES2PlaneMesh(width, height, columns, rows, pMaterial, GL_STATIC_DRAW));
		}

		void ReleaseResources()
		{
			s_pPlaneMesh = nullptr;
		}


		void Begin()
		{
			glDisable(GL_DEPTH_TEST);
			s_pCamera = GLES2Camera::GetActiveCamera();
			GLES2Camera::Deactivate();
			glViewport(0, 0, s_width, s_height);
			uistate.hotitem = 0;
		}

		void End()
		{
			if(s_pCamera)
				s_pCamera->Activate();

			if(uistate.mousedown == 0)
			{
				uistate.activeitem = 0;
			}
			else
			{
				if(uistate.activeitem == 0)
			  		uistate.activeitem = -1;
			}			
		}

		Color BORDER_COLOR(1,1,1,1);
		Color ACTIVE_HOT_COLOR(0,1,0,1);
		Color ACTIVE_COLOR(0.1f,0.1f,0.1f,1);
		Color HOT_COLOR(1,0,0,1);

		void DrawButton(Color color, float x, float y, float w, float h)
		{
			Node node;
			Node* pNode = &node;
			pNode->SetPosition(Vertex3(x, y, 0));
			pNode->SetScale(Vertex3(w, h, 1));

			/*s_pPlaneMesh->GetMaterial()->SetDiffuseColor(BORDER_COLOR);
			s_pPlaneMesh->Render(pNode);

			static float OffsetX = uistate.pixelSizeX * 2;
			static float OffsetY = uistate.pixelSizeY * 2;

			pNode->SetPosition(Vertex3(x, y, 0));
			pNode->SetScale(Vertex3(w-OffsetX, h-OffsetY, 1));
			*/
			s_pPlaneMesh->GetMaterial()->SetDiffuseColor(color);
			s_pPlaneMesh->Render(pNode);
		}

		bool RegionHit(GLushort id, float x, float y, float w, float h)
		{
			if(!pFrameColorSelection)
				return false;

			Node node;
			Node* pNode = &node;
			pNode->SetPosition(Vertex3(x, y, 0));
			pNode->SetScale(Vertex3(w, h, 1));

			pFrameColorSelection->Begin(uistate.mousex, uistate.mousey);
		    pFrameColorSelection->Render(id, s_pPlaneMesh, pNode);
		    pFrameColorSelection->End();

		    return id == pFrameColorSelection->GetSelected();
		}

		bool Button(GLushort id, float x, float y, const std::string& text)
		{
			PGLES2Text pTextMesh = textManager.GetTextMesh(id);

			float w = uistate.pixelSizeX * 64;
			float h = uistate.pixelSizeY * 48;

			// Check whether the button should be hot
			if (RegionHit(id, x, y, w, h))
			{
				uistate.hotitem = id;

				if (uistate.activeitem == 0 && uistate.mousedown)
				{
			  		uistate.activeitem = id;
			  	}
			}

			if(uistate.hotitem == id)
			{
				if(uistate.activeitem == id)
				{
			  		// Button is both 'hot' and 'active'
			  		DrawButton(ACTIVE_HOT_COLOR, x, y, w, h);
			  	}
				else
				{
					// Button is merely 'hot'
					DrawButton(HOT_COLOR, x, y, w, h);
				}
			}
			else
			{
				// button is not hot, but it may be active    
				DrawButton(ACTIVE_COLOR, x, y, w, h);
			}

			{
				static PNode pNode0(new Node());
				pNode0->SetPosition(Vertex3(-pTextMesh->GetWidth()/2, -pTextMesh->GetHeight()/2, 0));

				Node node(pNode0);
				Node* pNode = &node;
				pNode->SetPosition(Vertex3(x, y, 0));
				//pNode->SetScale(Vertex3(w, h, 1));

				pTextMesh->Render(pNode, Color(1,1,1,1), text);
			}

			// If button is hot and active, but mouse button is not
			// down, the user must have clicked the button.
			return uistate.mousedown == 0 && uistate.hotitem == id &&  uistate.activeitem == id;
		}		

		void ViewChanged(int32_t width, int32_t height)
		{
			s_width = width;
			s_height = height;
			uistate.pixelSizeX = 2/(float)width;
			uistate.pixelSizeY = 2/(float)height;

			if(!pFrameColorSelection)
        		pFrameColorSelection = PGLES2FrameColorSelection(new GLES2FrameColorSelection());

        	pFrameColorSelection->ViewChanged(width, height);
		}

        void OnMouseMove(float x, float y)
        {
        	uistate.mousex = x;
        	uistate.mousey = y;
        }

        void OnMouseDown(float x, float y)
        {
        	uistate.mousex = x;
        	uistate.mousey = y;
        	uistate.mousedown = true;
        }

        void OnMouseUp()
        {
        	uistate.mousedown = false;
        }
	}
}
