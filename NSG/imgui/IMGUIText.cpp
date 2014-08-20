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
#include "IMGUIStyle.h"
#include "IMGUILayoutManager.h"
#include "TextMesh.h"
#include "SceneNode.h"
#include "Technique.h"
#include "Pass.h"
#include "Material.h"
#include "Keys.h"
#include "Keyboard.h"
#include "Graphics.h"

namespace NSG
{
    namespace IMGUI
    {
        Text::Text(const std::string& text, std::regex* pRegex, float percentageX, float percentageY, TextStyle& style)
            : Object(LayoutType::CONTROL, percentageX, percentageY, style),
			textStyle_(style),
              currentText_(text),
              pTextMesh_(area_->textMesh0_),
              pCursorMesh_(area_->cursorMesh_),
              pRegex_(pRegex)
        {
			if (!pTextMesh_ || !pTextMesh_->Has(style.fontAtlasFile_))
            {
				pTextMesh_ = area_->textMesh0_ = PTextMesh(new TextMesh(style.fontAtlasFile_));
            }

			if (!pCursorMesh_ || !pCursorMesh_->Has(style.fontAtlasFile_))
            {
				pCursorMesh_ = area_->cursorMesh_ = PTextMesh(new TextMesh(style.fontAtlasFile_));
            }
        }

        Text::~Text()
        {
            lastwidget_ = id_;
        }

        bool Text::OnActive()
        {
            //Calculates cursor's position after click
            float mouseRelPosX(uistate_.mousex_);
            float textEndRelPosX = pTextMesh_->GetWidth();
            float mouseTotalX = mouseRelPosX  + area_->textOffsetX_;
            if (mouseTotalX > textEndRelPosX)
            {
                // mouse exceeded text length
                area_->cursor_character_position_ = currentText_.length();
            }
            else
            {
                area_->cursor_character_position_ = pTextMesh_->GetCharacterPositionForWidth(mouseTotalX);
                if (area_->cursor_character_position_ > 0)
                    --area_->cursor_character_position_;
                CHECK_ASSERT(area_->cursor_character_position_ <= currentText_.length(), __FILE__, __LINE__);
            }

            return Object::OnActive();
        }

        bool Text::OnFocus(bool needsKeyboard)
        {
            area_->cursor_character_position_ = currentText_.length();

            if (needsKeyboard && Keyboard::this_->Enable())
            {
            }

            return Object::OnFocus(needsKeyboard);
        }

        void Text::OnKey(int key)
        {
            switch (key)
            {
            case NSG_KEY_BACKSPACE:
                if (area_->cursor_character_position_ > 0)
                {
                    std::string::iterator it = currentText_.begin() + area_->cursor_character_position_ - 1;
                    currentText_.erase(it);
                    --area_->cursor_character_position_;
                }
                break;

            case NSG_KEY_DELETE:
                if (area_->cursor_character_position_ < currentText_.length())
                {
                    std::string::iterator it = currentText_.begin() + area_->cursor_character_position_;
                    currentText_.erase(it);
                }
                break;

            case NSG_KEY_RIGHT:
                if (area_->cursor_character_position_ < currentText_.length())
                    ++area_->cursor_character_position_;
                break;

            case NSG_KEY_LEFT:
                if (area_->cursor_character_position_ > 0)
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
                break;
            }
        }

        void Text::OnChar(unsigned int character)
        {
            if (character >= 32 && character < 256 && currentText_.size() < textStyle_.textMaxLength_)
            {
                std::string textCopy = currentText_;

                std::string::iterator it = currentText_.begin() + area_->cursor_character_position_;
                currentText_.insert(it, (char)character);
                ++area_->cursor_character_position_;

                if (pRegex_)
                {
                    if (!regex_match(currentText_, *pRegex_))
                    {
                        currentText_ = textCopy;
                        --area_->cursor_character_position_;
                    }
                }
            }
        }

        void Text::UpdateControl()
        {
            pTextMesh_->SetText(currentText_, LEFT_ALIGNMENT, MIDDLE_ALIGNMENT);
            pCursorMesh_->SetText("_", LEFT_ALIGNMENT, MIDDLE_ALIGNMENT);

            float cursorPositionInText = pTextMesh_->GetWidthForCharacterPosition(area_->cursor_character_position_);

            if (area_->textOffsetX_ + areaSize_.x < cursorPositionInText)
            {
                //If cursor moves to the right of the area then scroll
                area_->textOffsetX_ = pCursorMesh_->GetWidth() + cursorPositionInText - areaSize_.x;
            }
            else if (cursorPositionInText - area_->textOffsetX_ < 0)
            {
                // if cursor moves to the left of the area then scroll
                area_->textOffsetX_ -= areaSize_.x / 4;

                if (area_->textOffsetX_ < 0)
                    area_->textOffsetX_ = 0;
            }

            Node& textNode0 = *area_->controlNodes_.node0_;
            textNode0.SetParent(node_);

            if (pTextMesh_->GetTextHorizontalAlignment() == LEFT_ALIGNMENT)
                textNode0.SetPosition(Vertex3(-1, 0, 0)); //move text to the beginning of the current area
            if (pTextMesh_->GetTextHorizontalAlignment() == RIGHT_ALIGNMENT)
                textNode0.SetPosition(Vertex3(1, 0, 0)); //move text to the end of the current area

            if (pTextMesh_->GetTextVerticalAlignment() == BOTTOM_ALIGNMENT)
                textNode0.SetPosition(textNode0.GetPosition() + Vertex3(0, -1, 0)); //move text to the bottom of the current area
            else if (pTextMesh_->GetTextVerticalAlignment() == TOP_ALIGNMENT)
                textNode0.SetPosition(textNode0.GetPosition() + Vertex3(0, 1, 0)); //move text to the top of the current area
            else if (pTextMesh_->GetTextVerticalAlignment() == MIDDLE_ALIGNMENT)
                textNode0.SetPosition(textNode0.GetPosition() + Vertex3(0, -0.25f, 0));

            Node& textNode1 = *area_->controlNodes_.node1_;
            textNode1.SetParent(&textNode0);
            textNode1.SetInheritScale(false);
            textNode1.SetScale(Context::this_->pRootNode_->GetGlobalScale());

            Node& textNode2 = *area_->controlNodes_.node2_;
            textNode2.SetParent(&textNode1);
            textNode2.SetPosition(Vertex3(-area_->textOffsetX_, 0, 0));

            Graphics::this_->Set(&textNode2);
            Graphics::this_->Set(pTextMesh_.get());

            Pass pass;
            pass.EnableDepthTest(false);
            pass.EnableStencilTest(true);
            pass.SetProgram(pTextMesh_->GetProgram());
            size_t level = Context::this_->pLayoutManager_->GetNestingLevel();
            pass.SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            pass.SetStencilFunc(GL_EQUAL, level, ~GLuint(0));

            Material textMaterial;
            textMaterial.SetColor(textStyle_.textColor_);
			textMaterial.SetTexture0(pTextMesh_->GetTexture());
            Graphics::this_->Set(&textMaterial);
            
            pass.Render();

            // Render cursor if we have keyboard focus
            if (HasFocus() && layoutManager_.IsCurrentWindowActive() && (uistate_.tick_ < 15))
            {
                Node& cursorNode = *area_->controlNodes_.node3_;
                cursorNode.SetParent(&textNode2);
                cursorNode.SetPosition(Vertex3(cursorPositionInText, 0, 0));

                Graphics::this_->Set(&cursorNode);
                Graphics::this_->Set(pCursorMesh_.get());
                
                Pass pass;
                pass.EnableDepthTest(false);
                pass.EnableStencilTest(true);
                pass.SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
                pass.SetStencilFunc(GL_EQUAL, level, ~GLuint(0));
                pass.SetProgram(pCursorMesh_->GetProgram());
                pass.Render();
            }
        }


        std::string Text::Render()
        {
            Update();

            return currentText_;
        }
    }
}