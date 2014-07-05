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
#include "IMGUIText.h"
#include "IMGUIContext.h"
#include "IMGUIState.h"
#include "IMGUISkin.h"
#include "IMGUILayoutManager.h"
#include "TextMesh.h"
#include "SceneNode.h"
#include "Technique.h"
#include "Pass.h"
#include "Material.h"
#include "Keys.h"
#include "Camera.h"
#include "Keyboard.h"

namespace NSG
{
	namespace IMGUI
	{
		Text::Text(GLushort id, const std::string& text, int maxLength, std::regex* pRegex, int percentageX, int percentageY)
        : Object(id, false, LayoutType::Control, percentageX, percentageY),
		currentText_(text),
		pTextMesh_(Context::this_->GetCurrentTextMesh(id, maxLength)),
        pCursorMesh_(Context::this_->GetCurrentTextMesh(-1, 1)),
		pRegex_(pRegex)
		{
		}

		Text::~Text()
		{

		}

		void Text::OnActive()
		{
	  		///////////////////////////////////////////////
	        //Calculates cursor's position after click
	        Vertex4 worldPos = node_->GetGlobalModelMatrix() * Vertex4(-1,0,0,1); //left border in world coords
	        Vertex3 screenPos = Context::this_->pCamera_->WorldToScreen(Vertex3(worldPos)); //left border in screen coords
	        float mouseRelPosX(uistate_.mousex_ - screenPos.x);
	        float textEndRelPosX = pTextMesh_->GetWidth();
	        float mouseTotalX = mouseRelPosX  + area_->textOffsetX_;
	        if(mouseTotalX > textEndRelPosX)
	        {
	        	// mouse exceeded text length
	            area_->cursor_character_position_ = currentText_.length();
	        }
	        else
	        {
	             area_->cursor_character_position_ = pTextMesh_->GetCharacterPositionForWidth(mouseTotalX);
                 if(area_->cursor_character_position_ > 0)
                     --area_->cursor_character_position_;
                 CHECK_ASSERT(area_->cursor_character_position_ <= currentText_.length(), __FILE__, __LINE__);
	        }
	        ///////////////////////////////////////////////

			if(Keyboard::this_->Enable())
			{
				Vertex3 position(0, screenPos.y, 0);
		  		Context::this_->pCamera_->SetPosition(position);
		  	}
		}

		void Text::OnFocus()
		{
			area_->cursor_character_position_ = currentText_.length();
		}

		void Text::OnKey(int key)
		{
			switch (key)
			{
			case NSG_KEY_BACKSPACE:
				if(area_->cursor_character_position_ > 0)
				{
	                std::string::iterator it = currentText_.begin() + area_->cursor_character_position_ - 1;
	                currentText_.erase(it);
	                --area_->cursor_character_position_;
				}
				break;   

	        case NSG_KEY_DELETE:
				if(area_->cursor_character_position_ < currentText_.length())
				{
	                std::string::iterator it = currentText_.begin() + area_->cursor_character_position_;
	                currentText_.erase(it);
				}
	            break;

	        case NSG_KEY_RIGHT:
	            if(area_->cursor_character_position_ < currentText_.length())
	                ++area_->cursor_character_position_;
	            break;

	        case NSG_KEY_LEFT:
	            if(area_->cursor_character_position_ > 0)
	                --area_->cursor_character_position_;
	            break;

	        case NSG_KEY_HOME:
	            area_->cursor_character_position_ = 0;
	            break;

	        case NSG_KEY_END:
	            area_->cursor_character_position_ = currentText_.length();
	            break;

	        case NSG_KEY_ENTER:
				Keyboard::this_->Disable();
				Context::this_->pCamera_->SetPosition(Vertex3(0,0,0));
				break;
			}
		}

		void Text::OnChar(unsigned int character)
		{
	        if (character >= 32 && character < 256 && currentText_.size() < Context::this_->pSkin_->textMaxLength_)
	        {
	        	std::string textCopy = currentText_;

	            std::string::iterator it = currentText_.begin() + area_->cursor_character_position_;
	            currentText_.insert(it, (char)character);
	            ++area_->cursor_character_position_;

	        	if(pRegex_)
	        	{
	        		if(!regex_match(currentText_, *pRegex_))
	        		{
	        			currentText_ = textCopy;
	        			--area_->cursor_character_position_;
	        		}
	        	}

                //pTextMesh_->SetText(currentText_);
	        }
		}

		void Text::UpdateControl()
		{
			pTextMesh_->SetText(currentText_, LEFT_ALIGNMENT, MIDDLE_ALIGNMENT);
			pCursorMesh_->SetText("_", LEFT_ALIGNMENT, MIDDLE_ALIGNMENT);

	        float cursorPositionInText = pTextMesh_->GetWidthForCharacterPosition(area_->cursor_character_position_);

	        if(area_->textOffsetX_ + areaSize_.x < cursorPositionInText)
			{
	            //If cursor moves to the right of the area then scroll
				area_->textOffsetX_ = pCursorMesh_->GetWidth() + cursorPositionInText - areaSize_.x;  
			}
	        else if(cursorPositionInText - area_->textOffsetX_ < 0) 
	        {
	            // if cursor moves to the left of the area then scroll
	            area_->textOffsetX_ -= areaSize_.x /4;

	            if(area_->textOffsetX_ < 0)
	                area_->textOffsetX_ = 0;
	        }
	        
	        Node textNode0;
	        textNode0.SetParent(node_);

            if(pTextMesh_->GetTextHorizontalAlignment() == LEFT_ALIGNMENT)
	            textNode0.SetPosition(Vertex3(-1, 0, 0)); //move text to the beginning of the current area
            if(pTextMesh_->GetTextHorizontalAlignment() == RIGHT_ALIGNMENT)
	            textNode0.SetPosition(Vertex3(1, 0, 0)); //move text to the end of the current area

            if(pTextMesh_->GetTextVerticalAlignment() == BOTTOM_ALIGNMENT)
	            textNode0.SetPosition(textNode0.GetPosition() + Vertex3(0, -1, 0)); //move text to the bottom of the current area
            else if(pTextMesh_->GetTextVerticalAlignment() == TOP_ALIGNMENT)
	            textNode0.SetPosition(textNode0.GetPosition() + Vertex3(0, 1, 0)); //move text to the top of the current area
            else if(pTextMesh_->GetTextVerticalAlignment() == MIDDLE_ALIGNMENT)
                textNode0.SetPosition(textNode0.GetPosition() + Vertex3(0, -0.25f, 0));

	        Node textNode1;
	        textNode1.SetParent(&textNode0);
	        textNode1.SetInheritScale(false);
	        textNode1.SetScale(Context::this_->pRootNode_->GetGlobalScale());

	        SceneNode textNode2;
	        textNode2.SetParent(&textNode1);
	        textNode2.SetPosition(Vertex3(-area_->textOffsetX_, 0, 0));

			Technique technique;
	        Pass pass;
	        technique.Add(&pass);
	        pass.Add(&textNode2, pTextMesh_);
	        Material textMaterial;
	        pass.EnableDepthTest(false);
	        pass.EnableStencilTest(true);
	        textMaterial.SetTexture0(pTextMesh_->GetAtlas());
	        textMaterial.SetProgram(pTextMesh_->GetProgram());
            size_t level = Context::this_->pLayoutManager_->GetNestingLevel();
            pass.SetStencilFunc(GL_EQUAL, level, ~GLuint(0));

            pass.Set(&textMaterial);
	        technique.Render();
	        
			// Render cursor if we have keyboard focus
			if(HasFocus() && (uistate_.tick_ < 15))
	        {
	            SceneNode cursorNode;
	            cursorNode.SetParent(&textNode2);
	            cursorNode.SetPosition(Vertex3(cursorPositionInText, 0, 0));

	            Technique technique;
	            Pass pass;
				pass.EnableDepthTest(false);
				pass.EnableStencilTest(true);
	            technique.Add(&pass);
	            pass.Add(&cursorNode, pCursorMesh_);
	            pass.Set(&textMaterial);
				pass.SetStencilFunc(GL_EQUAL, level, ~GLuint(0));
	            
	            technique.Render();
	        }
		}


		std::string Text::operator()()
		{
			Update();
				
			return currentText_;
		}	
	}
}