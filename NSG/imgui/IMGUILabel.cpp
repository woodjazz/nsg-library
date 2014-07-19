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
#include "IMGUILabel.h"
#include "IMGUIContext.h"
#include "IMGUISkin.h"
#include "IMGUIStyle.h"
#include "TextMesh.h"
#include "SceneNode.h"
#include "Technique.h"
#include "Pass.h"
#include "Material.h"
#include "Graphics.h"
#include "IMGUILayoutManager.h"

namespace NSG
{
	namespace IMGUI
	{
		Label::Label(const std::string& text, float percentageX, float percentageY, Style& style)
			: Object(LayoutType::CONTROL, percentageX, percentageY, style),
			pTextMesh_(area_->textMesh_)
		{
			if (!pTextMesh_ || !pTextMesh_->Has(style.fontFile_, style.fontSize_))
			{
				pTextMesh_ = area_->textMesh_ = PTextMesh(new TextMesh(style.fontFile_, style.fontSize_, GL_STREAM_DRAW));
			}
			
			pTextMesh_->SetText(text, LEFT_ALIGNMENT, MIDDLE_ALIGNMENT);
		}

		Label::~Label()
		{
		}

		void Label::Render()
		{
			Update();
		}		

		void Label::UpdateControl()
		{
            CHECK_GL_STATUS(__FILE__, __LINE__);

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

            SceneNode textNode;
            textNode.SetParent(&textNode0);
            textNode.SetInheritScale(false);
            textNode.SetScale(Context::this_->pRootNode_->GetGlobalScale());

            Technique technique;
            Pass pass;
            technique.Add(&pass);
            pass.Add(&textNode, pTextMesh_);
            pass.EnableDepthTest(false);
            pass.EnableStencilTest(true);
            pass.SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            pass.SetStencilFunc(GL_EQUAL, level_, ~GLuint(0));

            Material textMaterial;
            textMaterial.SetTexture0(pTextMesh_->GetAtlas());
            textMaterial.SetProgram(pTextMesh_->GetProgram());
            
            pass.Set(&textMaterial);

            technique.Render();

            CHECK_GL_STATUS(__FILE__, __LINE__);
		}

	}
}