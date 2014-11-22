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
#include "IMGUITitle.h"
#include "IMGUIContext.h"
#include "IMGUISkin.h"
#include "IMGUIStyle.h"
#include "IMGUINode.h"
#include "TextMesh.h"
#include "Technique.h"
#include "Pass.h"
#include "Material.h"
#include "SceneNode.h"
#include "IMGUILayoutManager.h"
#include "Graphics.h"
#include "App.h"

namespace NSG
{
    namespace IMGUI
    {
        Title::Title(const std::string& text, TitleStyle& style)
            : Object(LayoutType::CONTROL, 100, Pixels2PercentageY(style.pixelsHeight_), style),
              titleStyle_(style),
              pTextMesh_(area_->textMesh0_)
        {
            if (!pTextMesh_ || !pTextMesh_->Has(style.fontAtlasFile_))
            {
                pTextMesh_ = area_->textMesh0_ = app_.CreateTextMesh(style.fontAtlasFile_);
            }

            pTextMesh_->SetText(text, LEFT_ALIGNMENT, MIDDLE_ALIGNMENT);
        }


        Title::~Title()
        {
        }

        Vertex3 Title::Render()
        {
            Update();

            return windowOffset_;
        }

        void Title::UpdateControl()
        {
            CHECK_GL_STATUS(__FILE__, __LINE__);

			AutoNode textNode0("textNode0");
			textNode0->SetParent(node_);

            if (pTextMesh_->GetTextHorizontalAlignment() == LEFT_ALIGNMENT)
                textNode0->SetPosition(Vertex3(-1, 0, 0)); //move text to the beginning of the current area
            if (pTextMesh_->GetTextHorizontalAlignment() == RIGHT_ALIGNMENT)
                textNode0->SetPosition(Vertex3(1, 0, 0)); //move text to the end of the current area

            if (pTextMesh_->GetTextVerticalAlignment() == BOTTOM_ALIGNMENT)
                textNode0->SetPosition(textNode0->GetPosition() + Vertex3(0, -1, 0)); //move text to the bottom of the current area
            else if (pTextMesh_->GetTextVerticalAlignment() == TOP_ALIGNMENT)
                textNode0->SetPosition(textNode0->GetPosition() + Vertex3(0, 1, 0)); //move text to the top of the current area
            else if (pTextMesh_->GetTextVerticalAlignment() == MIDDLE_ALIGNMENT)
                textNode0->SetPosition(textNode0->GetPosition() + Vertex3(0, -0.25f, 0));

			PNode textNode = std::make_shared<Node>("textNode");
            textNode->SetParent(textNode0.Get());
            textNode->SetInheritScale(false);
            textNode->SetScale(Context::this_->pRootNode_->GetGlobalScale());

			Graphics::this_->SetNode(textNode.get());
            Graphics::this_->Set(pTextMesh_.get());
            
            Pass pass;
            pass.EnableDepthTest(false);
            pass.EnableStencilTest(true);
            pass.SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            pass.SetStencilFunc(GL_EQUAL, level_, ~GLuint(0));
            pass.SetProgram(pTextMesh_->GetProgram());

			titleStyle_.textMaterial_->SetTexture0(pTextMesh_->GetTexture());

			Graphics::this_->Set(titleStyle_.textMaterial_.get());

            pass.Render();

            CHECK_GL_STATUS(__FILE__, __LINE__);

            if (layoutManager_.IsCurrentWindowActive() && mousedown_)
            {
                if ((!lastHit_ && node_->IsPointInsideBB(Vertex3(mouseDownX_, mouseDownY_, 0))) || lastHit_ == id_)
                {
                    lastHit_ = id_;
                    windowOffset_.x += mouseRelX_;
                    windowOffset_.y += mouseRelY_;

                }
            }
        }
    }
}