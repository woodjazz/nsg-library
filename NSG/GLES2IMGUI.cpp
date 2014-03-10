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
#include "GLES2RectangleMesh.h"
#include "GLES2CircleMesh.h"
#include "GLES2EllipseMesh.h"
#include "GLES2Camera.h"
#include "GLES2RoundedRectangleMesh.h"
#include "Node.h"
#include "Constants.h"
#include "GLES2FrameColorSelection.h"
#include "GLES2Text.h"
#include "GLES2StencilMask.h"
#include <map>

using namespace NSG;

#define STRINGIFY(S) #S

static const char* vShader = STRINGIFY(
	uniform mat4 u_mvp;
	attribute vec4 a_position;
	varying vec4 v_position;
	
	void main()
	{
		gl_Position = u_mvp * vec4(a_position.xyz, 1);
		v_position = a_position;
	}
);

static const char* fShader = STRINGIFY(
	struct Material
	{
		vec4 diffuse;
	};
	uniform Material u_material;
	varying vec4 v_position;
	void main()
	{
		float factor = 1.0 - abs(v_position.y);
		gl_FragColor = u_material.diffuse * vec4(factor, factor, factor, 1);
	}
);

static PGLES2RectangleMesh s_pRectangleMesh;
static PGLES2CircleMesh s_pCircleMesh;
static PGLES2EllipseMesh s_pEllipseMesh;
static PGLES2RoundedRectangleMesh s_pRoundedRectangle;
static GLES2Camera* s_pCamera = nullptr;

namespace NSG 
{
	namespace IMGUI
	{
		PGLES2Camera pCamera;
		ButtonType buttonType = RoundedRectangle;
		PGLES2Mesh pButtonMesh = s_pRoundedRectangle;
		PNode pCurrentNode(new Node());
        PNode pRenderNode(new Node(pCurrentNode));
        PNode pRenderTextNode(new Node());
		Vertex3 currentSize;
		std::string currentFontFile("font/FreeSans.ttf");
		int currentFontSize = 18;
		bool fillEnabled = true;
		Color activeColor(0,1,0,0.7f);
		Color normalColor(1,1,1,0.7f);
		Color hotColor(1,0,0,0.7f);

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

		typedef std::shared_ptr<TextManager> PTextManager;
		typedef std::pair<std::string, int> TextManagerKey;
		typedef std::map<TextManagerKey, PTextManager> TextManagerMap;
		TextManagerMap textManagers;

		PGLES2Text GetCurrentTextMesh(GLushort item)
		{
			TextManagerKey k(currentFontFile, currentFontSize);
			
			auto it = textManagers.find(k);

			if(it != textManagers.end())
			{
				return it->second->GetTextMesh(item);
			}
			else
			{
				PTextManager pTextManager(new TextManager(currentFontFile, currentFontSize));
				textManagers.insert(TextManagerMap::value_type(k, pTextManager));
				return pTextManager->GetTextMesh(item);
			}
		}

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
			assert(s_pRectangleMesh == nullptr);
			PGLES2Program pProgram(new GLES2Program(vShader, fShader));
			PGLES2Material pMaterial(new GLES2Material());
            pMaterial->SetProgram(pProgram);
			pMaterial->SetDiffuseColor(Color(1,0,0,1));
			s_pRectangleMesh = PGLES2RectangleMesh(new GLES2RectangleMesh(1, 1, pMaterial, GL_STATIC_DRAW));
			s_pCircleMesh = PGLES2CircleMesh(new GLES2CircleMesh(0.5f, 64, pMaterial, GL_STATIC_DRAW));
			s_pEllipseMesh = PGLES2EllipseMesh(new GLES2EllipseMesh(1.0f, 1.0f, 64, pMaterial, GL_STATIC_DRAW));
			s_pRoundedRectangle = PGLES2RoundedRectangleMesh(new GLES2RoundedRectangleMesh(0.25f, 1, 0.5f, 64, pMaterial, GL_STATIC_DRAW));
            pCamera = PGLES2Camera(new GLES2Camera());
			pCamera->EnableOrtho();
            pCamera->SetFarClip(1);
            pCamera->SetNearClip(-1);
		}

		void ReleaseResources()
		{
            pCamera = nullptr;
			s_pRectangleMesh = nullptr;
			s_pCircleMesh = nullptr;
			s_pEllipseMesh = nullptr;
			s_pRoundedRectangle = nullptr;
			pButtonMesh = nullptr;
		}


		GLboolean isBlendEnabled = false;
		void Begin()
		{
			isBlendEnabled = glIsEnabled(GL_BLEND);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

			glDisable(GL_DEPTH_TEST);
			s_pCamera = GLES2Camera::GetActiveCamera();
			pCamera->Activate();
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

			if(!isBlendEnabled)
				glDisable(GL_BLEND);
			
		}

		void DrawButton(Color color)
		{
            pButtonMesh->SetFilled(fillEnabled);
			pButtonMesh->GetMaterial()->SetDiffuseColor(color);
			pButtonMesh->Render(pRenderNode);
		}

		bool Hit(GLushort id)
		{
			if(!pFrameColorSelection)
				return false;

			pFrameColorSelection->Begin(uistate.mousex, uistate.mousey);
	    	pFrameColorSelection->Render(id, pButtonMesh, pRenderNode);
		    pFrameColorSelection->End();

		    return id == pFrameColorSelection->GetSelected();
		}

		void SetButtonType(ButtonType type)
		{
			buttonType = type;

			switch(buttonType)
			{
				case Rectangle:
					pButtonMesh = s_pRectangleMesh;
					break;
				case Circle:
					pButtonMesh = s_pCircleMesh;
					break;
				case Ellipse:
					pButtonMesh = s_pEllipseMesh;
					break;
				case RoundedRectangle:
					pButtonMesh = s_pRoundedRectangle;
					break;
				default:
					assert(false);
			}
		}

		Vertex3 ConvertPixels2ScreenCoords(const Vertex3& pixels)
		{
			Vertex3 screenCoords(pixels);
			screenCoords.x *= uistate.pixelSizeX;
			screenCoords.y *= uistate.pixelSizeY;
			return screenCoords;
		}

		Vertex3 ConvertScreenCoords2Pixels(const Vertex3& screenCoords)
		{
			Vertex3 pixels(screenCoords);
			pixels.x /= uistate.pixelSizeX;
			pixels.y /= uistate.pixelSizeY;
			return pixels;
		}

		void SetPosition(const Vertex3& position)
		{
			pCurrentNode->SetPosition(position);
		}

		const Vertex3& GetPosition()
		{
			return pCurrentNode->GetPosition();
		}

		void SetSize(const Vertex3& size)
		{
			currentSize = size;
		}

		const Vertex3& GetSize()
		{
			return pCurrentNode->GetScale();
		}

		void Fill(bool enable)
		{
			fillEnabled = enable;
		}

		void SetNormalColor(Color color)
		{
			normalColor = color;
		}

		void SetHotColor(Color color)
		{
			hotColor = color;
		}

		void SetActiveColor(Color color)
		{
			activeColor = color;
		}


		void SetFont(const std::string& fontFile, int fontSize)
		{
			currentFontFile = fontFile;
			currentFontSize = fontSize;
		}

		bool Button(GLushort id, const std::string& text)
		{
			pCurrentNode->SetScale(currentSize);

			// Check whether the button should be hot
			if (Hit(id))
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
			  		DrawButton(activeColor);
			  	}
				else
				{
					// Button is merely 'hot'
					DrawButton(hotColor);
				}
			}
			else
			{
				// button is not hot, but it may be active    
				DrawButton(normalColor);
			}

			{
				GLES2StencilMask stencilMask;
				stencilMask.Begin();
				stencilMask.Render(pRenderNode.get(), pButtonMesh.get());
				stencilMask.End();
				PGLES2Text pTextMesh = GetCurrentTextMesh(id);
	            pTextMesh->SetText(text);
				
				static PNode pNode0(new Node());
				pNode0->SetPosition(Vertex3(-pTextMesh->GetWidth()/2, -pTextMesh->GetHeight()/2, 0));

                static PNode pNode(new Node(pNode0));
                pNode->SetPosition(pCurrentNode->GetPosition());

                pRenderTextNode->SetParent(pNode);

				pTextMesh->Render(pRenderTextNode, Color(1,1,1,1));
			}

			// If button is hot and active, but mouse button is not
			// down, the user must have clicked the button.
			return uistate.mousedown == 0 && uistate.hotitem == id &&  uistate.activeitem == id;
		}		

		void ViewChanged(int32_t width, int32_t height)
		{
			//////////////////////////////////////////////////////////////////////////////////////////////////
			//Since we are using screen coordinates then  we have to recalculate the origin of coordinates 
			//and the scale to match the ortographic projection
			Vertex3 coordinates_origin(width/2, height/2, 0);
			Vertex3 coordinates_scale(width/2, height/2, 1);

            pRenderNode->SetPosition(coordinates_origin);
            pRenderNode->SetScale(coordinates_scale);
            pRenderTextNode->SetPosition(coordinates_origin);
            pRenderTextNode->SetScale(coordinates_scale);
            //////////////////////////////////////////////////////////////////////////////////////////////////
            
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

