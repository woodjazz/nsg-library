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
#include "IMGUIButton.h"
#include "IMGUICheckButton.h"
#include "IMGUILabel.h"
#include "IMGUITitle.h"
#include "IMGUILine.h"
#include "IMGUIText.h"
#include "IMGUILayoutManager.h"
#include "IMGUIState.h"
#include "IMGUIContext.h"
#include "IMGUIVSliderThumb.h"
#include "IMGUIHSliderThumb.h"
#include "App.h"

using namespace NSG;

namespace NSG
{
    void IMGUIBeginHorizontal(float percentageX, float percentageY, IMGUI::AreaStyle& style)
    {
		IMGUI::Context::this_->pLayoutManager_->BeginHorizontalArea(percentageX, percentageY, style);
    }

    void IMGUIBeginVertical(float percentageX, float percentageY, IMGUI::AreaStyle& style)
    {
		IMGUI::Context::this_->pLayoutManager_->BeginVerticalArea(percentageX, percentageY, style);
    }

    float IMGUIEndArea(float scroll)
    {
		return IMGUI::Context::this_->pLayoutManager_->EndArea(scroll);
    }

    void IMGUISpacer(float percentageX, float percentageY)
    {
		IMGUI::Context::this_->pLayoutManager_->Spacer(percentageX, percentageY);
    }

    float IMGUILine(IMGUI::LineStyle& style)
    {
        IMGUI::Line obj(style);
        return obj.Render();
    }

	bool IMGUIButton(const std::string& text, float percentageX, float percentageY, IMGUI::ButtonStyle& style)
    {
		IMGUI::Button obj(text, false, CENTER_ALIGNMENT, MIDDLE_ALIGNMENT, percentageX, percentageY, style);
        return obj.Render();
    }

    bool IMGUICheckButton(bool pressed, const std::string& text, float percentageX, float percentageY, IMGUI::CheckButtonStyle& style)
    {
        IMGUI::CheckButton obj(pressed, text, CENTER_ALIGNMENT, MIDDLE_ALIGNMENT, percentageX, percentageY, style);
        return obj.Render();
    }

	float IMGUIVSlider(float value, float percentageX, float percentageY, float thumbPercentageX, float thumbPercentageY, IMGUI::SliderStyle& style)
    {
		IMGUIBeginHorizontal(percentageX, percentageY, style);
        float percentageHSpace = 0.5f * (100 - thumbPercentageX);
        IMGUISpacer(percentageHSpace);
		value = IMGUIVSliderThumb(value, thumbPercentageX, thumbPercentageY, *style.thumbSliderStyle_);
        IMGUISpacer(percentageHSpace);
        IMGUIEndArea();
        return value;
    }

	float IMGUIHSlider(float value, float percentageX, float percentageY, float thumbPercentageX, float thumbPercentageY, IMGUI::SliderStyle& style)
    {
		IMGUIBeginVertical(percentageX, percentageY, style);
        float percentageVSpace = 0.5f * (100 - thumbPercentageY);
        IMGUISpacer(100, percentageVSpace);
		value = IMGUIHSliderThumb(value, thumbPercentageX, thumbPercentageY, *style.thumbSliderStyle_);
        IMGUISpacer(100, percentageVSpace);
        IMGUIEndArea();
        return value;
    }


	float IMGUIVSliderThumb(float value, float percentageX, float percentageY, IMGUI::Style& style)
    {
		IMGUI::VSliderThumb obj(value, percentageX, percentageY, style);

        float result = obj.Render();

        return result;
    }

	float IMGUIHSliderThumb(float value, float percentageX, float percentageY, IMGUI::Style& style)
    {
        IMGUI::HSliderThumb obj(value, percentageX, percentageY, style);

        float result = obj.Render();

        return result;
    }

	void IMGUILabel(const std::string& text, float percentageX, float percentageY, IMGUI::LabelStyle& style)
    {
		IMGUI::Label obj(text, percentageX, percentageY, style);

        obj.Render();
    }

	std::string IMGUITextField(const std::string& text, float percentageX, float percentageY, std::regex* pRegex, IMGUI::TextStyle& style)
    {
		IMGUI::Text obj(text, pRegex, percentageX, percentageY, style);

        std::string result = obj.Render();

        return result;
    }

	IMGUI::PSkin& IMGUISkin()
    {
		return IMGUI::Context::this_->pSkin_;
    }

    PNode& IMGUINode()
    {
        return IMGUI::Context::this_->pCurrentNode_;
    }

	void IMGUIWindow(IMGUI::IWindow* obj, float percentageX, float percentageY, IMGUI::PWindowStyle style)
    {
		IMGUI::Context::this_->pLayoutManager_->Window(obj, percentageX, percentageY, style);
    }

    namespace IMGUI
    {
        bool IsReady()
        {
            return Context::this_->IsReady();
        }

        void OnMouseMove(float x, float y)
        {
            Context::this_->state_->OnMouseMove(x, y);
        }

        void OnMouseDown(float x, float y)
        {
            Context::this_->state_->OnMouseDown(x, y);
        }

        void OnMouseUp(float x, float y)
        {
            Context::this_->state_->OnMouseUp(x, y);
        }

        void OnMouseWheel(float x, float y)
        {
            Context::this_->state_->OnMouseWheel(x, y);
        }

        void OnKey(int key, int action, int modifier)
        {
            Context::this_->state_->OnKey(key, action, modifier);
        }

        void OnChar(unsigned int character)
        {
            Context::this_->state_->OnChar(character);
        }

        void DoTick()
        {
            Context::this_->state_->DoTick();
        }

        float Pixels2PercentageX(int pixels)
        {
            PNode node = IMGUINode();
            Vertex3 globalScale = node->GetGlobalScale();
            float xPixelsNodeSize = globalScale.x * (float)App::this_->GetViewSize().first;
            float percentage = 100 * pixels / xPixelsNodeSize;
            return percentage;
        }

        float Pixels2PercentageY(int pixels)
        {
            PNode node = IMGUINode();
            Vertex3 globalScale = node->GetGlobalScale();
            float yPixelsNodeSize = globalScale.y * (float)App::this_->GetViewSize().second;
            float percentage = 100 * pixels / yPixelsNodeSize;
            return percentage;
        }

    }
}

