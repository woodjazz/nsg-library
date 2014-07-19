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
#include "IMGUISkin.h"
#include "IMGUIStyle.h"
#include "Technique.h"
#include "Pass.h"
#include "Material.h"
#include "PlaneMesh.h"
#include "CircleMesh.h"
#include "RoundedRectangleMesh.h"
#include "RectangleMesh.h"
#include "ProgramSimpleColor.h"
#include "ProgramUnlit.h"
#include "ProgramWhiteColor.h"

namespace NSG
{
    namespace IMGUI
    {
        Skin::Skin()
            : mainWindowStyle_(new Style),
              windowStyle_(new Style),
              areaStyle_(new Style),
              labelStyle_(new Style),
              buttonStyle_(new Style),
			  textStyle_(new Style),
              areaSliderStyle_(new Style),
              titleStyle_(new Style),
              sizerLeftTopStyle_(new Style),
              sizerTopStyle_(new Style),
              sizerRightTopStyle_(new Style),
              sizerLeftStyle_(new Style),
              sizerRightStyle_(new Style),
              sizerLeftBottomStyle_(new Style),
              sizerBottomStyle_(new Style),
              sizerRightBottomStyle_(new Style),
              vSliderStyle_(new Style),
              vThumbSliderStyle_(new Style),
              hSliderStyle_(new Style),
              hThumbSliderStyle_(new Style),
              stencilTechnique_(new Technique)
        {
			{
				mainWindowStyle_->hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 0, 0));
				mainWindowStyle_->activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 0, 0));
				mainWindowStyle_->normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 0, 0));
			}

			{
				windowStyle_->hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 1, 0.6f));
				windowStyle_->activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 1, 0.6f));
				windowStyle_->normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 1, 0.6f));
			}

			{
				areaStyle_->hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 0, 0));
				areaStyle_->activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 0, 0));
				areaStyle_->normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 0, 0));
			}

			{
				labelStyle_->hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 0, 0));
				labelStyle_->activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 0, 0));
				labelStyle_->normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 0, 0));
			}

			{
				buttonStyle_->hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 1, 0.7f));
				buttonStyle_->activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(1, 0, 0, 0.7f));
				buttonStyle_->normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 1, 0, 0.7f));
			}

			{
				textStyle_->hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 1, 0.7f));
				textStyle_->activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(1, 0, 0, 0.7f));
				textStyle_->normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 1, 0, 0.7f));
			}

			{
				areaSliderStyle_->hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 1, 0.7f));
				areaSliderStyle_->activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(1, 0, 0, 0.7f));
				areaSliderStyle_->normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 1, 0, 0.7f));
			}

			{
				titleStyle_->hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 1, 0.7f));
				titleStyle_->activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(1, 0, 0, 0.7f));
				titleStyle_->normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 1, 0, 0.7f));
			}

			PMesh areaMesh(new PlaneMesh(2, 2, 2, 2, GL_STATIC_DRAW));
            PProgram program(new ProgramWhiteColor);
            PMaterial material(new Material);
            material->SetColor(Color(1, 0, 1, 0.7f));
            material->SetProgram(program);
            PPass pass(new Pass);
            pass->Set(material);
            pass->Add(nullptr, areaMesh);
            pass->EnableDepthTest(false);
            pass->EnableDepthBuffer(false);
            pass->EnableStencilTest(true);
            pass->EnableColorBuffer(false);
            pass->SetBlendMode(BLEND_NONE);
            stencilTechnique_->Add(pass);
        }
    }
}