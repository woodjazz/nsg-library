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
#include "IMGUIState.h"
#include "GLES2RectangleMesh.h"
#include "GLES2CircleMesh.h"
#include "GLES2EllipseMesh.h"
#include "GLES2RoundedRectangleMesh.h"
#include "Technique.h"
#include "Pass.h"
#include "Node.h"
#include "Constants.h"
#include "Keys.h"
#include "Log.h"
#include "Keyboard.h"
#include "GLES2FrameColorSelection.h"
#include "GLES2Text.h"
#include "GLES2StencilMask.h"
#include "Camera.h"
#include "SharedPointers.h"
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
		PGLES2Text GetCurrentTextMesh(GLushort item)
		{
			return Context::this_->pTextManager_->GetTextMesh(item, Context::this_->pSkin_->fontFile, Context::this_->pSkin_->fontSize);
		}

		void Begin()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			Context::this_->pCamera_->Activate();
			Context::this_->state_->hotitem = 0;

			Context::this_->pCurrentNode_ = Context::this_->pRootNode_;
			Context::this_->pLayoutManager_->Begin();
			Context::this_->pLayoutManager_->BeginVertical(0);
		}

		void End()
		{
			Context::this_->pLayoutManager_->EndVertical();
			Context::this_->pLayoutManager_->End();
			
			if(!Context::this_->state_->activeitem_is_a_text_field)
			{
				if(Keyboard::this_->Disable())
                {
					Context::this_->pCamera_->SetPosition(Vertex3(0,0,0));
				}
			}

			if(!Context::this_->state_->mousedown)
			{
				Context::this_->state_->activeitem = GLushort(-1);
			}
			else
			{
				if(Context::this_->state_->activeitem == GLushort(-1))
				{
			  		//Context::this_->state_->activeitem = -1;
			  		Context::this_->state_->activeitem_is_a_text_field = false;
			  	}
			}

			// If no widget grabbed tab, clear focus
			if (Context::this_->state_->keyentered == NSG_KEY_TAB || !IsStable())
				Context::this_->state_->kbditem = 0;
	
			// Clear the entered key
			Context::this_->state_->keyentered = 0;	
			Context::this_->state_->character = 0;	
		}

		void DrawButton(PNode pNode, bool hasFocus)
		{
            State& uistate = *Context::this_->state_;

            float alphaFactor = uistate.currentTechnique->GetAlphaFactor();
            float shininessFactor = uistate.currentTechnique->GetShininessFactor();
			uistate.currentTechnique->SetAlphaFactor(Context::this_->pSkin_->alphaFactor);
			uistate.currentTechnique->SetShininessFactor(hasFocus ? 1 : -0.25f);
			uistate.currentTechnique->Render(pNode.get());
            uistate.currentTechnique->SetShininessFactor(shininessFactor);
            uistate.currentTechnique->SetAlphaFactor(alphaFactor);
		}

		bool Hit(GLushort id, PNode pNode)
		{
			if(!Context::this_->pFrameColorSelection_)
				return false;

            State& uistate = *Context::this_->state_;

            if(uistate.currentTechnique)
            {
                PGLES2Mesh currentMesh(uistate.currentTechnique->GetCurrentRenderedMesh());

                if(currentMesh)
                {
			        Context::this_->pFrameColorSelection_->Begin(Context::this_->state_->mousex, Context::this_->state_->mousey);
	    	        Context::this_->pFrameColorSelection_->Render(id, currentMesh.get(), pNode.get());
		            Context::this_->pFrameColorSelection_->End();

		            return id == Context::this_->pFrameColorSelection_->GetSelected();
                }
            }

            return false;
		}

		void InternalBeginHorizontal(GLushort id)
		{
			Context::this_->pLayoutManager_->BeginHorizontal(id);
		}

		void InternalEndHorizontal()
		{
			Context::this_->pLayoutManager_->EndHorizontal();
		}

		void InternalBeginVertical(GLushort id)
		{
			Context::this_->pLayoutManager_->BeginVertical(id);
		}

		void InternalEndVertical()
		{
			Context::this_->pLayoutManager_->EndVertical();
		}

		void InternalSpacer(GLushort id, int percentage)
		{
			Context::this_->pLayoutManager_->Spacer(id, percentage);
		}

		bool IsStable()
		{
			return Context::this_->pLayoutManager_->IsStable();
		}

        void DrawButton(GLushort id, PNode pNode, bool hasFocus)
        {
            State& uistate = *Context::this_->state_;

            CHECK_GL_STATUS(__FILE__, __LINE__);

			if(uistate.hotitem == id)
			{
				if(uistate.activeitem == id)
				{
                    // Button is both 'hot' and 'active'
                    uistate.currentTechnique = Context::this_->pSkin_->pActiveTechnique;
			  	}
				else
				{
					// Button is merely 'hot'
                    uistate.currentTechnique = Context::this_->pSkin_->pHotTechnique;
				}
			}
			else
			{
				// button is not hot, but it may be active    
                uistate.currentTechnique  = Context::this_->pSkin_->pNormalTechnique;
			}

            DrawButton(pNode, hasFocus);

            CHECK_GL_STATUS(__FILE__, __LINE__);

			Context::this_->pStencilMask_->Begin();
            Context::this_->pStencilMask_->Render(pNode.get(), uistate.currentTechnique->GetCurrentRenderedMesh().get());
			Context::this_->pStencilMask_->End();

            CHECK_GL_STATUS(__FILE__, __LINE__);
        }

		bool InternalButton(GLushort id, const std::string& text)
		{
            State& uistate = *Context::this_->state_;

            PNode pNode = Context::this_->pLayoutManager_->GetAreaForControl(id)->pNode_;
            CHECK_ASSERT(pNode, __FILE__, __LINE__);

			if(!IsStable())
				return false;

			// Check whether the button should be hot
			if (Hit(id, pNode))
			{
				Context::this_->state_->hotitem = id;

				if (uistate.activeitem == GLushort(-1) && uistate.mousedown)
				{
					Context::this_->state_->activeitem_is_a_text_field = false;
			  		Context::this_->state_->activeitem = id;
			  		Context::this_->state_->kbditem = id;
			  	}
			}
			// If no widget has keyboard focus, take it
			if (uistate.kbditem == GLushort(-1))
			{
				uistate.kbditem = id;
				uistate.activeitem_is_a_text_field = false;
			}

			// If we have keyboard focus, show it
			bool hasFocus = uistate.kbditem == id;

            DrawButton(id, pNode, hasFocus);

            {
                CHECK_GL_STATUS(__FILE__, __LINE__);

			
                PGLES2Text pTextMesh = GetCurrentTextMesh(id);
	            pTextMesh->SetText(text);

                CHECK_GL_STATUS(__FILE__, __LINE__);
				
				{
	                Node textNode;
	                textNode.EnableUpdate(false);
	                textNode.SetParent(pNode);
	                textNode.SetInheritScale(false);
                    textNode.SetScale(Context::this_->pRootNode_->GetGlobalScale());
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
                    textMaterial.SetDiffuseColor(Color(1,1,1,Context::this_->pSkin_->alphaFactor));
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
            State& uistate = *Context::this_->state_;

            PLayoutArea pArea = Context::this_->pLayoutManager_->GetAreaForControl(id);
			PNode pNode = pArea->pNode_;

			if(!IsStable())
				return text;

            Vertex4 areaSize = Context::this_->pCamera_->GetModelViewProjection(pNode.get()) * Vertex4(2,0,0,0);

			std::string currentText = text;

            PGLES2Text pTextMesh = GetCurrentTextMesh(id);
			PGLES2Text pCursorMesh = GetCurrentTextMesh(-1);
			pCursorMesh->SetText("_");

			// Check whether the button should be hot
			if (Hit(id, pNode))
			{
				uistate.hotitem = id;

				if (uistate.activeitem == GLushort(-1) && uistate.mousedown)
				{
			  		uistate.activeitem = id;
			  		uistate.kbditem = id;
			  		uistate.activeitem_is_a_text_field = true;

			  		///////////////////////////////////////////////
                    //Calculates cursor's position after click
                    Vertex4 worldPos = pNode->GetGlobalModelMatrix() * Vertex4(-1,0,0,1); //left border in world coords
                    Vertex3 screenPos = Context::this_->pCamera_->WorldToScreen(Vertex3(worldPos)); //left border in screen coords
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

					if(Keyboard::this_->Enable())
					{
						Vertex3 position(0, screenPos.y, 0);
				  		Context::this_->pCamera_->SetPosition(position);
				  	}
			  	}
			}

			// If no widget has keyboard focus, take it
			if (uistate.kbditem == GLushort(-1))
			{
				uistate.kbditem = id;
				pArea->cursor_character_position_ = currentText.length();
			}

			// If we have keyboard focus, show it
			bool hasFocus = uistate.kbditem == id;

            DrawButton(id, pNode, hasFocus);

			{
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
                textNode0.SetScale(Context::this_->pRootNode_->GetGlobalScale());
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
                textMaterial.SetDiffuseColor(Color(1,1,1,Context::this_->pSkin_->alphaFactor));
                textNode1.Render(true);
                
				// Render cursor if we have keyboard focus
				if(hasFocus && (uistate.tick < 15))
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
                    textMaterial.SetDiffuseColor(Color(1,0,0,Context::this_->pSkin_->alphaFactor));
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
					Keyboard::this_->Disable();
					Context::this_->pCamera_->SetPosition(Vertex3(0,0,0));
					break;
				}

	            if (uistate.character >= 32 && uistate.character < 256 && currentText.size() < Context::this_->pSkin_->textMaxLength)
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

        void OnMouseMove(float x, float y)
        {
            State& uistate = *Context::this_->state_;

        	uistate.mousex = x;
        	uistate.mousey = y;
        }

        void OnMouseDown(float x, float y)
        {
            State& uistate = *Context::this_->state_;

        	uistate.mousex = x;
        	uistate.mousey = y;
        	uistate.mousedown = true;
        }

        void OnMouseUp()
        {
            State& uistate = *Context::this_->state_;

        	uistate.mousedown = false;
        }

        void OnKey(int key, int action, int modifier)
        {
            if(action == NSG_KEY_PRESS)
            {
                State& uistate = *Context::this_->state_;

                uistate.keyentered = key;
                uistate.keyaction = action;
                uistate.keymod = modifier;
            }
        }

        void OnChar(unsigned int character)
        {
            State& uistate = *Context::this_->state_;

        	uistate.character = character;
        }


        void DoTick()
        {
            State& uistate = *Context::this_->state_;

        	++uistate.tick;
            if(uistate.tick > 30)
            {
                uistate.tick = 0;
            }
        }
	}
}

