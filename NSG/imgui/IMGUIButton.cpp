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
#include "IMGUIButton.h"
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
#include "Graphics.h"
#include "Keys.h"

namespace NSG
{
    namespace IMGUI
    {
        Button::Button(const std::string& text, bool pressed, HorizontalAlignment hAlign, VerticalAlignment vAlign, float percentageX, float percentageY, ButtonStyle& style)
            : Object(LayoutType::CONTROL, percentageX, percentageY, style),
              currentText_(text),
              pTextMesh_(area_->textMesh0_),
              pressed_(pressed),
              buttonStyle_(style)
        {
            if (!pTextMesh_ || !pTextMesh_->Has(style.fontAtlasFile_))
            {
				pTextMesh_ = area_->textMesh0_ = PTextMesh(new TextMesh(style.fontAtlasFile_));
            }

            pTextMesh_->SetText(currentText_, hAlign, vAlign);
        }

        Button::~Button()
        {
            lastwidget_ = id_;
        }

        void Button::OnKey(int key)
        {
            if (key == NSG_KEY_ENTER)
				pressed_ = !pressed_;
        }

        void Button::UpdateControl()
        {
			if (uistate_.mouseup_ && layoutManager_.IsCurrentWindowActive() && IsMouseButtonPressedInArea() && IsMouseInArea())
				pressed_ = !pressed_;

            CHECK_GL_STATUS(__FILE__, __LINE__);

            Node& textNode0 = *area_->controlNodes_.node0_;
            textNode0.SetParent(node_);

            Vertex3 position;

            if (pTextMesh_->GetTextHorizontalAlignment() == LEFT_ALIGNMENT)
            {
                position = Vertex3(-1, 0, 0); //move text to the beginning of the current area
            }
            else if (pTextMesh_->GetTextHorizontalAlignment() == RIGHT_ALIGNMENT)
            {
                position = Vertex3(1, 0, 0); //move text to the end of the current area
            }

            if (pTextMesh_->GetTextVerticalAlignment() == BOTTOM_ALIGNMENT)
            {
                textNode0.SetPosition(position + Vertex3(0, -1, 0)); //move text to the bottom of the current area
            }
            else if (pTextMesh_->GetTextVerticalAlignment() == TOP_ALIGNMENT)
            {
                textNode0.SetPosition(position + Vertex3(0, 1, 0)); //move text to the top of the current area
            }
            else if (pTextMesh_->GetTextVerticalAlignment() == MIDDLE_ALIGNMENT)
            {
                textNode0.SetPosition(position + Vertex3(0, -0.25f, 0));
            }

            Node& textNode = *area_->controlNodes_.node1_;
            textNode.SetParent(&textNode0);
            textNode.SetInheritScale(false);
            textNode.SetScale(Context::this_->pRootNode_->GetGlobalScale());

            Graphics::this_->Set(&textNode);
            Graphics::this_->Set(pTextMesh_.get());
            Pass pass;
            pass.EnableDepthTest(false);
            pass.EnableStencilTest(true);
            pass.SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            pass.SetStencilFunc(GL_EQUAL, level_, ~GLuint(0));
            pass.SetProgram(pTextMesh_->GetProgram());

            Material textMaterial;
            textMaterial.SetColor(buttonStyle_.textColor_);
            textMaterial.SetTexture0(pTextMesh_->GetTexture());
            
            Graphics::this_->Set(&textMaterial);

            pass.Render();

            CHECK_GL_STATUS(__FILE__, __LINE__);
        }

        bool Button::Render()
        {
			Update();

            return pressed_;
        }
    }
}