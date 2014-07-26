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
#include "IMGUIStyle.h"
#include "Technique.h"
#include "Pass.h"
#include "Material.h"
#include "IMGUIContext.h"
//#include "ProgramSimpleColor.h"
//#include "ProgramWhiteColor.h"

namespace NSG
{
    namespace IMGUI
    {
        Style::Style()
            : activeTechnique_(new Technique),
              normalTechnique_(new Technique),
              hotTechnique_(new Technique),
              enableActive_(true),
              enableHot_(true),
              enableFocus_(true)
        {
            Context& context = *Context::this_;

            PMaterial pActiveMaterial(new Material);
            pActiveMaterial->SetProgram(context.unlitProgram_);
            pActiveMaterial->SetColor(Color(1, 0, 0, 0.7f));

            PMaterial pNormalMaterial(new Material);
            pNormalMaterial->SetProgram(context.unlitProgram_);
            pNormalMaterial->SetColor(Color(0, 1, 0, 0.7f));

            PMaterial pHotMaterial(new Material);
            pHotMaterial->SetProgram(context.unlitProgram_);
            pHotMaterial->SetColor(Color(0, 0, 1, 0.7f));

            PPass activePass(new Pass);
            activePass->Set(pActiveMaterial);
            activePass->Add(nullptr, context.controlMesh_);
            activePass->EnableDepthTest(false);
            activePass->EnableStencilTest(true);

            PPass normalPass(new Pass);
            normalPass->Set(pNormalMaterial);
            normalPass->Add(nullptr, context.controlMesh_);
            normalPass->EnableDepthTest(false);
            normalPass->EnableStencilTest(true);

            PPass hotPass(new Pass);
            hotPass->Set(pHotMaterial);
            hotPass->Add(nullptr, context.controlMesh_);
            hotPass->EnableDepthTest(false);
            hotPass->EnableStencilTest(true);

            activeTechnique_->Add(activePass);
            normalTechnique_->Add(normalPass);
            hotTechnique_->Add(hotPass);
        }

        Style::Style(const Style& obj)
            : activeTechnique_(obj.activeTechnique_),
              normalTechnique_(obj.normalTechnique_),
              hotTechnique_(obj.hotTechnique_)
        {
        }

        LineStyle::LineStyle()
            : pixels_(1)
        {
            hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(1, 1, 1, 1));
            activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(1, 1, 1, 1));
            normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0.5f, 0.5f, 0.5f, 0.9f));
        }

        LineStyle::LineStyle(const LineStyle& obj)
            : Style(obj),
              pixels_(obj.pixels_)
        {
        }

        AreaStyle::AreaStyle()
            : showVScroll_(true),
              showHScroll_(true),
              vScrollTechnique_(new Technique),
              hScrollTechnique_(new Technique)
        {
            hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 0, 0));
            activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 0, 0));
            normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 0, 0));

            Context& context = *Context::this_;
            PMaterial material(new Material);
            material->SetProgram(context.unlitProgram_);
            material->SetColor(Color(0.3f, 0.3f, 0.3f, 0.9f));

            PPass pass(new Pass);
            pass->Set(material);
            pass->Add(nullptr, context.controlMesh_);
            pass->EnableDepthTest(false);
            pass->EnableStencilTest(true);

            vScrollTechnique_->Add(pass);
            hScrollTechnique_->Add(pass);
        }

        AreaStyle::AreaStyle(const AreaStyle& obj)
            : Style(obj),
              showVScroll_(obj.showVScroll_),
              showHScroll_(obj.showHScroll_),
              vScrollTechnique_(obj.vScrollTechnique_),
              hScrollTechnique_(obj.hScrollTechnique_)
        {
        }

        SliderStyle::SliderStyle()
            : thumbSliderStyle_(new Style)
        {
            hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0.2f, 0.2f, 1, 0.5f));
            activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(1, 0.2f, 0.2f, 0.5f));
            normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0.2f, 1, 0.2f, 0.5f));

            thumbSliderStyle_->hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0.3f, 0.3f, 1, 0.9f));
            thumbSliderStyle_->activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(1, 0.3f, 0.3f, 0.9f));
            thumbSliderStyle_->normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0.3f, 1, 0.3f, 0.9f));
        }

        SliderStyle::SliderStyle(const SliderStyle& obj)
            : AreaStyle(obj),
              thumbSliderStyle_(obj.thumbSliderStyle_)
        {
        }

        SizerStyle::SizerStyle()
        {
            hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0.2f, 0.2f, 0.2f, 0.9f));
            activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(1, 0.2f, 0.2f, 0.9f));
            normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0.2f, 0.2f, 0.2f, 0.5f));
        }

        SizerStyle::SizerStyle(const SizerStyle& obj)
            : Style(obj)
        {
        }


        LabelStyle::LabelStyle()
            : textColor_(1, 1, 1, 1)
        {
            hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 0, 0));
            activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 0, 0));
            normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 0, 0));
        }

        LabelStyle::LabelStyle(const LabelStyle& obj)
            : Style(obj),
              textColor_(obj.textColor_),
			  fontAtlasFile_(obj.fontAtlasFile_)
        {
        }

        ButtonStyle::ButtonStyle()
            : textColor_(1, 1, 1, 1)
        {
            hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 1, 0.7f));
            activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(1, 0, 0, 0.7f));
            normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 1, 0, 0.7f));
        }

        ButtonStyle::ButtonStyle(const ButtonStyle& obj)
            : Style(obj),
              textColor_(obj.textColor_),
			  fontAtlasFile_(obj.fontAtlasFile_)
        {
        }

        CheckButtonStyle::CheckButtonStyle()
        : pressedStyle_(new Style)
        {
            pressedStyle_->hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 0.5f, 0.9f));
            pressedStyle_->activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0.5f, 0, 0, 0.9f));
            pressedStyle_->normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0.5f, 0, 0.9f));
        }

        CheckButtonStyle::CheckButtonStyle(const CheckButtonStyle& obj)
        : ButtonStyle(obj),
        pressedStyle_(obj.pressedStyle_)
        {

        }


        TextStyle::TextStyle()
            : textColor_(1, 1, 1, 1),
              textMaxLength_(std::numeric_limits<int>::max())
        {
            hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 1, 0.7f));
            activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(1, 0, 0, 0.7f));
            normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 1, 0, 0.7f));
        }

        TextStyle::TextStyle(const TextStyle& obj)
            : Style(obj),
              textColor_(obj.textColor_),
			  fontAtlasFile_(obj.fontAtlasFile_),
              textMaxLength_(obj.textMaxLength_)
        {
        }

        TitleStyle::TitleStyle()
            : textColor_(0, 0, 0, 1),
              pixelsHeight_(25)
        {
            hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0.9f, 0.7f, 0.7f, 0.7f));
            activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0.7f, 0.7f, 0.7f, 1));
            normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0.7f, 0.7f, 0.7f, 0.7f));
        }

        TitleStyle::TitleStyle(const TitleStyle& obj)
            : Style(obj),
              pixelsHeight_(obj.pixelsHeight_),
			  fontAtlasFile_(obj.fontAtlasFile_)
        {
        }

        WindowStyle::WindowStyle()
            : sizerPixels_(8),
              titleStyle_(new TitleStyle),
              sizerLeftTopStyle_(new SizerStyle),
              sizerTopStyle_(new SizerStyle),
              sizerRightTopStyle_(new SizerStyle),
              sizerLeftStyle_(new SizerStyle),
              sizerRightStyle_(new SizerStyle),
              sizerLeftBottomStyle_(new SizerStyle),
              sizerBottomStyle_(new SizerStyle),
              sizerRightBottomStyle_(new SizerStyle)

        {
            hotTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 1, 0.6f));
            activeTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 1, 0.6f));
            normalTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0, 0, 1, 0.6f));
        }

        WindowStyle::WindowStyle(const WindowStyle& obj)
            : AreaStyle(obj),
			fontAtlasFile_(obj.fontAtlasFile_),
              sizerPixels_(obj.sizerPixels_),
              titleStyle_(obj.titleStyle_),
              sizerLeftTopStyle_(obj.sizerLeftTopStyle_),
              sizerTopStyle_(obj.sizerTopStyle_),
              sizerRightTopStyle_(obj.sizerRightTopStyle_),
              sizerLeftStyle_(obj.sizerLeftStyle_),
              sizerRightStyle_(obj.sizerRightStyle_),
              sizerLeftBottomStyle_(obj.sizerLeftBottomStyle_),
              sizerBottomStyle_(obj.sizerBottomStyle_),
              sizerRightBottomStyle_(obj.sizerRightBottomStyle_)
        {
        }


    }
}