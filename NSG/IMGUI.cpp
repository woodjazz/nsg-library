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
#include "IMGUI.h"
#include "IMGUILayoutManager.h"
#include "IMGUITextManager.h"
#include "GLES2RectangleMesh.h"
#include "GLES2CircleMesh.h"
#include "GLES2EllipseMesh.h"
#include "GLES2RoundedRectangleMesh.h"
#include "Node.h"
#include "Constants.h"
#include "Keys.h"
#include "Log.h"
#include "GLES2FrameColorSelection.h"
#include "GLES2Text.h"
#include "GLES2StencilMask.h"
#include "GLES2Camera.h"
#include "App.h"
#include "Check.h"
#include <map>
#include <list>
#include <set>
#include <cassert>

using namespace NSG;

namespace NSG 
{
	namespace IMGUI
	{
		PSkin pSkin;
		PGLES2Camera pCamera;
		PNode pCurrentNode;
		PGLES2FrameColorSelection pFrameColorSelection;		
        PNode pRootNode;
        PGLES2StencilMask pStencilMask;
		int tick = 0;

		PTextManager pTextManager;
		PLayoutManager pLayoutManager_;

		struct UIState
		{
			float mousex;
			float mousey;
			bool mousedown;

			GLushort hotitem;
			GLushort activeitem;

			GLushort kbditem;
  			int keyentered;
  			int keymod;
  			int keyaction;
  			unsigned int character;
  			GLushort lastwidget;	
  			bool activeitem_is_a_text_field;
		} uistate;

		PGLES2Text GetCurrentTextMesh(GLushort item)
		{
			return pTextManager->GetTextMesh(item, pSkin->fontFile, pSkin->fontSize);
		}

		void AllocateResources()
		{
			TRACE_LOG("IMGUI Allocating resources");
            
            CHECK_GL_STATUS(__FILE__, __LINE__);
			
            memset(&uistate, 0, sizeof(uistate));
			pSkin = PSkin(new Skin());
            
            CHECK_GL_STATUS(__FILE__, __LINE__);
			
            pCamera = PGLES2Camera(new GLES2Camera());
			pCamera->EnableOrtho();
            pCamera->SetFarClip(1000000);
            pCamera->SetNearClip(-1000000);
            pRootNode = PNode(new Node());
            pLayoutManager_ = PLayoutManager(new LayoutManager(pRootNode, pCurrentNode));
        	pTextManager = PTextManager(new TextManager());

            CHECK_GL_STATUS(__FILE__, __LINE__);

        	pStencilMask = PGLES2StencilMask(new GLES2StencilMask());

        	pFrameColorSelection = PGLES2FrameColorSelection(new GLES2FrameColorSelection(false));

            CHECK_GL_STATUS(__FILE__, __LINE__);
		}

		void ReleaseResources()
		{
			TRACE_LOG("IMGUI Releasing resources");
            CHECK_GL_STATUS(__FILE__, __LINE__);
			pFrameColorSelection = nullptr;
			pStencilMask = nullptr;
			pTextManager = nullptr;
			pRootNode = nullptr;
            pLayoutManager_ = nullptr;
            pCurrentNode = nullptr;
            pCamera = nullptr;
			pSkin = nullptr;
            CHECK_GL_STATUS(__FILE__, __LINE__);
		}

		void Begin()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			pCamera->Activate();
			uistate.hotitem = 0;

			pCurrentNode = pRootNode;
			pLayoutManager_->Begin();
			pLayoutManager_->BeginVertical(0);
		}

		void End()
		{
			pLayoutManager_->EndVertical();
			pLayoutManager_->End();
			
			if(!uistate.activeitem_is_a_text_field)
			{
				if(App::GetPtrInstance()->IsKeyboardVisible())
				{
					App::GetPtrInstance()->HideKeyboard();
					pCamera->SetPosition(Vertex3(0,0,0));
				}
			}

			if(!uistate.mousedown)
			{
				uistate.activeitem = 0;
			}
			else
			{
				if(uistate.activeitem == 0)
				{
			  		uistate.activeitem = -1;
			  		uistate.activeitem_is_a_text_field = false;
			  	}
			}

			// If no widget grabbed tab, clear focus
			if (uistate.keyentered == NSG_KEY_TAB || !pLayoutManager_->IsStable())
				uistate.kbditem = 0;
	
			// Clear the entered key
			uistate.keyentered = 0;	
			uistate.character = 0;	
		}

		void DrawButton(PGLES2Material pMaterial, PNode pNode, bool hasFocus)
		{
            if(pMaterial->IsReady())
            {
            	Color diffuse = pMaterial->GetDiffuseColor();
            	float shininess = pMaterial->GetShininess();

                pMaterial->SetDiffuseColor(diffuse * Color(1,1,1, pSkin->alphaFactor));
                pMaterial->SetShininess(hasFocus ? 1 : -0.25f);
                pMaterial->Render(pSkin->fillEnabled, pNode.get(), pSkin->pMesh.get());
                
                pMaterial->SetShininess(shininess);
                pMaterial->SetDiffuseColor(diffuse);
            }
		}

		bool Hit(GLushort id, PNode pNode)
		{
			if(!pFrameColorSelection)
				return false;

			pFrameColorSelection->Begin(uistate.mousex, uistate.mousey);
	    	pFrameColorSelection->Render(id, pSkin->pMesh.get(), pNode.get());
		    pFrameColorSelection->End();

		    return id == pFrameColorSelection->GetSelected();
		}

		void InternalBeginHorizontal(GLushort id)
		{
			pLayoutManager_->BeginHorizontal(id);
		}

		void InternalEndHorizontal()
		{
			pLayoutManager_->EndHorizontal();
		}

		void InternalBeginVertical(GLushort id)
		{
			pLayoutManager_->BeginVertical(id);
		}

		void InternalEndVertical()
		{
			pLayoutManager_->EndVertical();
		}

		void InternalSpacer(GLushort id, int percentage)
		{
			pLayoutManager_->Spacer(id, percentage);
		}

		bool InternalButton(GLushort id, const std::string& text)
		{
            PNode pNode = pLayoutManager_->GetAreaForControl(id)->pNode_;
            assert(pNode);

			if(!pLayoutManager_->IsStable())
				return false;

			// Check whether the button should be hot
			if (Hit(id, pNode))
			{
				uistate.hotitem = id;

				if (uistate.activeitem == 0 && uistate.mousedown)
				{
					uistate.activeitem_is_a_text_field = false;
			  		uistate.activeitem = id;
			  		uistate.kbditem = id;
			  	}
			}
			// If no widget has keyboard focus, take it
			if (uistate.kbditem == 0)
			{
				uistate.kbditem = id;
				uistate.activeitem_is_a_text_field = false;
			}

			// If we have keyboard focus, show it
			bool hasFocus = uistate.kbditem == id;

            CHECK_GL_STATUS(__FILE__, __LINE__);

			if(uistate.hotitem == id)
			{
				if(uistate.activeitem == id)
				{
			  		// Button is both 'hot' and 'active'
			  		DrawButton(pSkin->pActiveMaterial, pNode, hasFocus);
			  	}
				else
				{
					// Button is merely 'hot'
					DrawButton(pSkin->pHotMaterial, pNode, hasFocus);
				}
			}
			else
			{
				// button is not hot, but it may be active    
				DrawButton(pSkin->pNormalMaterial, pNode, hasFocus);
			}

            CHECK_GL_STATUS(__FILE__, __LINE__);

			if(pSkin->drawBorder)
			{
				bool fillEnabled = pSkin->fillEnabled;
				pSkin->fillEnabled = false;
				DrawButton(pSkin->pBorderMaterial, pNode, hasFocus);
				pSkin->fillEnabled = fillEnabled;
			}

			{
                CHECK_GL_STATUS(__FILE__, __LINE__);

				pStencilMask->Begin();
				pStencilMask->Render(pNode.get(), pSkin->pMesh.get());
				pStencilMask->End();
				PGLES2Text pTextMesh = GetCurrentTextMesh(id);
	            pTextMesh->SetText(text);

                CHECK_GL_STATUS(__FILE__, __LINE__);
				
				{
	                Node textNode;
	                textNode.EnableUpdate(false);
	                textNode.SetParent(pNode);
	                textNode.SetInheritScale(false);
                    textNode.SetScale(pRootNode->GetGlobalScale());
                    SceneNode textNode1;
                    textNode1.SetParent(&textNode);
                    textNode1.SetPosition(Vertex3(-pTextMesh->GetWidth()/2, -pTextMesh->GetHeight()/2, 0));
                    textNode.EnableUpdate(true);
                    textNode.Update(false);

                    textNode1.SetMesh(pTextMesh);
                    GLES2Material textMaterial;
                    textMaterial.SetTexture0(pTextMesh->GetAtlas());
                    textMaterial.SetProgram(pTextMesh->GetProgram());
                    textNode1.SetMaterial(&textMaterial);
                    textMaterial.SetDiffuseColor(Color(1,1,1,pSkin->alphaFactor));
                    textNode1.Render(true);
				}
			}

			bool enterKey = false;

			// If we have keyboard focus, we'll need to process the keys
			if (hasFocus)
			{
				switch (uistate.keyentered)
				{
				case NSG_KEY_TAB:
					// If tab is pressed, lose keyboard focus.
					// Next widget will grab the focus.
					uistate.kbditem = 0;
					// If shift was also pressed, we want to move focus
					// to the previous widget instead.
					if (uistate.keymod & NSG_KEY_MOD_SHIFT)
						uistate.kbditem = uistate.lastwidget;
					// Also clear the key so that next widget
					// won't process it
					uistate.keyentered = 0;
					break;

				case NSG_KEY_ENTER:
					// Had keyboard focus, received return,
					// so we'll act as if we were clicked.
					enterKey = true;
				  	break;
				}
			}

			uistate.lastwidget = id;

			// If button is hot and active, but mouse button is not
			// down, the user must have clicked the button.
			return enterKey || (uistate.mousedown == 0 && uistate.hotitem == id &&  uistate.activeitem == id);
		}		

		std::string InternalTextField(GLushort id, const std::string& text, std::regex* pRegex)
		{
            LayoutManager::PLayoutArea pArea = pLayoutManager_->GetAreaForControl(id);
			PNode pNode = pArea->pNode_;

			if(!pLayoutManager_->IsStable())
				return text;

            Vertex4 areaSize = pCamera->GetModelViewProjection(pNode.get()) * Vertex4(2,0,0,0);

			std::string currentText = text;

            PGLES2Text pTextMesh = GetCurrentTextMesh(id);
			PGLES2Text pCursorMesh = GetCurrentTextMesh(-1);
			pCursorMesh->SetText("_");

			// Check whether the button should be hot
			if (Hit(id, pNode))
			{
				uistate.hotitem = id;

				if (uistate.activeitem == 0 && uistate.mousedown)
				{
			  		uistate.activeitem = id;
			  		uistate.kbditem = id;
			  		uistate.activeitem_is_a_text_field = true;

			  		///////////////////////////////////////////////
                    //Calculates cursor's position after click
                    Vertex4 worldPos = pNode->GetGlobalModelMatrix() * Vertex4(-1,0,0,1); //left border in world coords
                    Vertex3 screenPos = pCamera->WorldToScreen(Vertex3(worldPos)); //left border in screen coords
                    float mouseRelPosX(uistate.mousex - screenPos.x);
                    float textEndRelPosX = pTextMesh->GetWidth();
                    float mouseTotalX = mouseRelPosX  + pArea->textOffsetX_;
                    if(mouseTotalX > textEndRelPosX)
                    {
                    	// mouse exceeded text length
                        pArea->cursor_character_position_ = currentText.length();
                    }
                    else
                    {
                         pArea->cursor_character_position_ = pTextMesh->GetCharacterPositionForWidth(mouseTotalX)-1;
                    }
                    ///////////////////////////////////////////////

					if(!App::GetPtrInstance()->IsKeyboardVisible())
					{
						if(App::GetPtrInstance()->ShowKeyboard())
						{
							Vertex3 position(0, screenPos.y, 0);
				  			pCamera->SetPosition(position);
				  		}
				  	}
			  	}
			}

			// If no widget has keyboard focus, take it
			if (uistate.kbditem == 0)
			{
				uistate.kbditem = id;
				pArea->cursor_character_position_ = currentText.length();
			}

			// If we have keyboard focus, show it
			bool hasFocus = uistate.kbditem == id;

			if(uistate.hotitem == id)
			{
				if(uistate.activeitem == id)
				{
			  		// Button is both 'hot' and 'active'
                    DrawButton(pSkin->pActiveMaterial, pNode, hasFocus);
			  	}
				else
				{
					// Button is merely 'hot'
					DrawButton(pSkin->pHotMaterial, pNode, hasFocus);
				}
			}
			else
			{
				// button is not hot, but it may be active    
				DrawButton(pSkin->pNormalMaterial, pNode, hasFocus);
			}

			if(pSkin->drawBorder)
			{
				bool fillEnabled = pSkin->fillEnabled;
				pSkin->fillEnabled = false;
				DrawButton(pSkin->pBorderMaterial, pNode, hasFocus);
				pSkin->fillEnabled = fillEnabled;
			}

			{
                /*
				pStencilMask->Begin();
				pStencilMask->Render(pNode.get(), pSkin->pMesh.get());
				pStencilMask->End();
                */
                
	            pTextMesh->SetText(currentText);
				
                float cursorPositionInText = pTextMesh->GetWidthForCharacterPosition(pArea->cursor_character_position_);

                if(pArea->textOffsetX_ + areaSize.x < cursorPositionInText)
				{
                    //If cursor moves to the right of the area then scroll
 					pArea->textOffsetX_ = pCursorMesh->GetWidth() + cursorPositionInText - areaSize.x;  
				}
                else if(cursorPositionInText - pArea->textOffsetX_ < 0) 
                {
                    // if cursor moves to the left of the area then scroll
                    pArea->textOffsetX_ -= areaSize.x /4;

                    if(pArea->textOffsetX_ < 0)
                        pArea->textOffsetX_ = 0;
                }
                
                Node textNode;
                textNode.EnableUpdate(false);
	            textNode.SetParent(pNode);
	            textNode.SetPosition(Vertex3(-1, 0, 0)); //move text to the beginning of the current area
                Node textNode0;
                textNode0.SetParent(&textNode);
	            textNode0.SetInheritScale(false);
                textNode0.SetScale(pRootNode->GetGlobalScale());
                SceneNode textNode1;
                textNode1.SetParent(&textNode0);
                textNode1.SetPosition(Vertex3(-pArea->textOffsetX_, 0, 0));
                textNode.EnableUpdate(true);
                textNode.Update(false);
                textNode1.SetMesh(pTextMesh);
                GLES2Material textMaterial;
                textMaterial.SetTexture0(pTextMesh->GetAtlas());
                textMaterial.SetProgram(pTextMesh->GetProgram());
                textNode1.SetMaterial(&textMaterial);
                textMaterial.SetDiffuseColor(Color(1,1,1,pSkin->alphaFactor));
                textNode1.Render(true);
                
				// Render cursor if we have keyboard focus
				if(hasFocus && (tick < 15))
                {
                    SceneNode cursorNode;
                    cursorNode.EnableUpdate(false);
                    cursorNode.SetParent(&textNode1);
                    cursorNode.SetPosition(Vertex3(cursorPositionInText, 0, 0));
                    cursorNode.EnableUpdate(true);
                    cursorNode.Update(false);

                    cursorNode.SetMesh(pCursorMesh);
                    GLES2Material textMaterial;
                    textMaterial.SetTexture0(pTextMesh->GetAtlas());
                    textMaterial.SetProgram(pTextMesh->GetProgram());
                    cursorNode.SetMaterial(&textMaterial);
                    textMaterial.SetDiffuseColor(Color(1,0,0,pSkin->alphaFactor));
                    cursorNode.Render(true);
                }
                
			}

			// If we have keyboard focus, we'll need to process the keys
			if (hasFocus)
			{
				switch (uistate.keyentered)
				{
				case NSG_KEY_TAB:
					// If tab is pressed, lose keyboard focus.
					// Next widget will grab the focus.
					uistate.kbditem = 0;
					// If shift was also pressed, we want to move focus
					// to the previous widget instead.
					if (uistate.keymod & NSG_KEY_MOD_SHIFT)
						uistate.kbditem = uistate.lastwidget;
					// Also clear the key so that next widget
					// won't process it
					uistate.keyentered = 0;
					break;
				case NSG_KEY_BACKSPACE:
					if(pArea->cursor_character_position_ > 0)
					{
                        std::string::iterator it = currentText.begin() + pArea->cursor_character_position_ - 1;
                        currentText.erase(it);
                        --pArea->cursor_character_position_;
					}
					break;   

                case NSG_KEY_DELETE:
					if(pArea->cursor_character_position_ < currentText.length())
					{
                        std::string::iterator it = currentText.begin() + pArea->cursor_character_position_;
                        currentText.erase(it);
					}
                    break;

                case NSG_KEY_RIGHT:
                    if(pArea->cursor_character_position_ < currentText.length())
                        ++pArea->cursor_character_position_;
                    break;

                case NSG_KEY_LEFT:
                    if(pArea->cursor_character_position_ > 0)
                        --pArea->cursor_character_position_;
                    break;

                case NSG_KEY_HOME:
                    pArea->cursor_character_position_ = 0;
                    break;

                case NSG_KEY_END:
                    pArea->cursor_character_position_ = currentText.length();
                    break;

                case NSG_KEY_ENTER:
					if(App::GetPtrInstance()->IsKeyboardVisible())
					{
						App::GetPtrInstance()->HideKeyboard();
						pCamera->SetPosition(Vertex3(0,0,0));
					}
					break;
				}

	            if (uistate.character >= 32 && uistate.character < 256 && currentText.size() < pSkin->textMaxLength)
	            {
	            	std::string textCopy = currentText;

                    std::string::iterator it = currentText.begin() + pArea->cursor_character_position_;
	                currentText.insert(it, (char)uistate.character);
                    ++pArea->cursor_character_position_;

	            	if(pRegex)
	            	{
	            		if(!regex_match(currentText, *pRegex))
	            		{
	            			currentText = textCopy;
	            			--pArea->cursor_character_position_;
	            		}
	            	}
	            }
			}

			// If button is hot and active, but mouse button is not
			// down, the user must have clicked the widget; give it 
			// keyboard focus.
			if (uistate.mousedown == 0 && uistate.hotitem == id && uistate.activeitem == id)
				uistate.kbditem = id;

			uistate.lastwidget = id;

			return currentText;
		}		


		void ViewChanged(int32_t width, int32_t height)
		{
        	if(pFrameColorSelection)
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

        void OnKey(int key, int action, int modifier)
        {
            if(action == NSG_KEY_PRESS)
            {
                uistate.keyentered = key;
                uistate.keyaction = action;
                uistate.keymod = modifier;
            }
        }

        void OnChar(unsigned int character)
        {
        	uistate.character = character;
        }


        void DoTick()
        {
        	++tick;
            if(tick > 30)
                tick = 0;
        }
	}
}

