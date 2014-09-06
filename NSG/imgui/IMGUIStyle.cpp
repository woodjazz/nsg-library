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

namespace NSG
{
    namespace IMGUI
    {
        Style::Style()
            : enableActive_(true),
              enableHot_(true),
              enableFocus_(true),
              normalMaterial_(new Material("normal")),
              activeMaterial_(new Material("active")),
              hotMaterial_(new Material("hot")),
              pass_(new Pass)
        {
            Context& context = *Context::this_;

            pass_->EnableDepthTest(false);
            pass_->EnableStencilTest(true);
            pass_->SetProgram(context.unlitProgram_);
            
            activeMaterial_->SetColor(Color(1, 0, 0, 0.7f));
            normalMaterial_->SetColor(Color(0, 1, 0, 0.7f));
            hotMaterial_->SetColor(Color(0, 0, 1, 0.7f));
        }

        Style::Style(const Style& obj)
            : enableActive_(obj.enableActive_),
              enableHot_(obj.enableHot_),
              enableFocus_(obj.enableFocus_),
              normalMaterial_(obj.normalMaterial_),
              activeMaterial_(obj.activeMaterial_),
              hotMaterial_(obj.hotMaterial_),
              pass_(obj.pass_)
        {
        }

        LineStyle::LineStyle()
            : pixels_(1)
        {
            hotMaterial_->SetColor(Color(1, 1, 1, 1));
            activeMaterial_->SetColor(Color(1, 1, 1, 1));
            normalMaterial_->SetColor(Color(0.5f, 0.5f, 0.5f, 0.9f));
        }

        LineStyle::LineStyle(const LineStyle& obj)
            : Style(obj),
              pixels_(obj.pixels_)
        {
        }

        AreaStyle::AreaStyle()
            : showVScroll_(true),
              showHScroll_(true),
              scrollMaterial_(new Material("area")),
              scrollPass_(new Pass)
        {
            hotMaterial_->SetColor(Color(0, 0, 0, 0));
            activeMaterial_->SetColor(Color(0, 0, 0, 0));
            normalMaterial_->SetColor(Color(0, 0, 0, 0));
            
            scrollMaterial_->SetColor(Color(0.3f, 0.3f, 0.3f, 0.9f));

            Context& context = *Context::this_;
            scrollPass_->SetProgram(context.unlitProgram_);
            scrollPass_->EnableDepthTest(false);
            scrollPass_->EnableStencilTest(true);
        }

        AreaStyle::AreaStyle(const AreaStyle& obj)
            : Style(obj),
              showVScroll_(obj.showVScroll_),
              showHScroll_(obj.showHScroll_),
              scrollMaterial_(obj.scrollMaterial_),
              scrollPass_(obj.scrollPass_)
        {
        }

        SliderStyle::SliderStyle()
            : thumbSliderStyle_(new Style)
        {
            hotMaterial_->SetColor(Color(0.2f, 0.2f, 1, 0.5f));
            activeMaterial_->SetColor(Color(1, 0.2f, 0.2f, 0.5f));
            normalMaterial_->SetColor(Color(0.2f, 1, 0.2f, 0.5f));

            thumbSliderStyle_->hotMaterial_->SetColor(Color(0.3f, 0.3f, 1, 0.9f));
            thumbSliderStyle_->activeMaterial_->SetColor(Color(1, 0.3f, 0.3f, 0.9f));
            thumbSliderStyle_->normalMaterial_->SetColor(Color(0.3f, 1, 0.3f, 0.9f));
        }

        SliderStyle::SliderStyle(const SliderStyle& obj)
            : AreaStyle(obj),
              thumbSliderStyle_(obj.thumbSliderStyle_)
        {
        }

        SizerStyle::SizerStyle()
        {
            hotMaterial_->SetColor(Color(0.2f, 0.2f, 0.2f, 0.9f));
            activeMaterial_->SetColor(Color(1, 0.2f, 0.2f, 0.9f));
            normalMaterial_->SetColor(Color(0.2f, 0.2f, 0.2f, 0.5f));
        }

        SizerStyle::SizerStyle(const SizerStyle& obj)
            : Style(obj)
        {
        }


        LabelStyle::LabelStyle()
            : textColor_(1, 1, 1, 1)
        {
            hotMaterial_->SetColor(Color(0, 0, 0, 0));
            activeMaterial_->SetColor(Color(0, 0, 0, 0));
            normalMaterial_->SetColor(Color(0, 0, 0, 0));
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
            hotMaterial_->SetColor(Color(0, 0, 1, 0.7f));
            activeMaterial_->SetColor(Color(1, 0, 0, 0.7f));
            normalMaterial_->SetColor(Color(0, 1, 0, 0.7f));
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
            pressedStyle_->hotMaterial_->SetColor(Color(0, 0, 0.5f, 0.9f));
            pressedStyle_->activeMaterial_->SetColor(Color(0.5f, 0, 0, 0.9f));
            pressedStyle_->normalMaterial_->SetColor(Color(0, 0.5f, 0, 0.9f));
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
            hotMaterial_->SetColor(Color(0, 0, 1, 0.7f));
            activeMaterial_->SetColor(Color(1, 0, 0, 0.7f));
            normalMaterial_->SetColor(Color(0, 1, 0, 0.7f));
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
            hotMaterial_->SetColor(Color(0.9f, 0.7f, 0.7f, 0.7f));
            activeMaterial_->SetColor(Color(0.7f, 0.7f, 0.7f, 1));
            normalMaterial_->SetColor(Color(0.7f, 0.7f, 0.7f, 0.7f));
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
            hotMaterial_->SetColor(Color(0, 0, 1, 0.6f));
            activeMaterial_->SetColor(Color(0, 0, 1, 0.6f));
            normalMaterial_->SetColor(Color(0, 0, 1, 0.6f));
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