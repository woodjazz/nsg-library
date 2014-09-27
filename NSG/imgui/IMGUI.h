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
#pragma once
#include "Types.h"
#include "IMGUIStyle.h"
#include "IMGUIContext.h"
#include "IMGUISkin.h"
#include <regex>

namespace NSG
{
    namespace IMGUI
    {
        struct IWindow
        {
            bool hasTitle_;
            bool resizable_;
            std::string title_;
            IWindow() : hasTitle_(false), resizable_(false) {}
            virtual ~IWindow() {}
            virtual void StartGUIWindow() {}
            virtual void RenderGUIWindow() {}
            virtual void EndGUIWindow() {}
        };

        bool IsReady();
        void OnMouseMove(float x, float y);
		void OnMouseDown(int button, float x, float y);
        void OnMouseWheel(float x, float y);
		void OnMouseUp(int button, float x, float y);
        void OnKey(int key, int action, int modifier);
        void OnChar(unsigned int character);
        void DoTick();
        float Pixels2PercentageX(int pixels);
        float Pixels2PercentageY(int pixels);
    }

	bool IMGUIButton(const std::string& text, float percentageX = 100, float percentageY = 100, IMGUI::ButtonStyle& style = *IMGUI::Context::this_->pSkin_->buttonStyle_);
    bool IMGUICheckButton(bool pressed, const std::string& text, float percentageX = 100, float percentageY = 100, IMGUI::CheckButtonStyle& style = *IMGUI::Context::this_->pSkin_->checkButtonStyle_);
	float IMGUIVSlider(float value, float percentageX = 100, float percentageY = 100, float thumbPercentageX = 25, float thumbPercentageY = 25, IMGUI::SliderStyle& style = *IMGUI::Context::this_->pSkin_->vSliderStyle_);
	float IMGUIHSlider(float value, float percentageX = 100, float percentageY = 100, float thumbPercentageX = 25, float thumbPercentageY = 25, IMGUI::SliderStyle& style = *IMGUI::Context::this_->pSkin_->hSliderStyle_);
	float IMGUIVSliderThumb(float value, float percentageX = 100, float percentageY = 100, IMGUI::Style& style = *IMGUI::Context::this_->pSkin_->vThumbSliderStyle_);
	float IMGUIHSliderThumb(float value, float percentageX = 100, float percentageY = 100, IMGUI::Style& style = *IMGUI::Context::this_->pSkin_->hThumbSliderStyle_);
	void IMGUILabel(const std::string& text, float percentageX = 100, float percentageY = 100, IMGUI::LabelStyle& style = *IMGUI::Context::this_->pSkin_->labelStyle_);
	std::string IMGUITextField(const std::string& text, float percentageX = 100, float percentageY = 100, std::regex* pRegex = nullptr, IMGUI::TextStyle& style = *IMGUI::Context::this_->pSkin_->textStyle_);
	void IMGUIBeginHorizontal(float percentageX = 100, float percentageY = 100, IMGUI::AreaStyle& style = *IMGUI::Context::this_->pSkin_->areaStyle_);
	void IMGUIBeginVertical(float percentageX = 100, float percentageY = 100, IMGUI::AreaStyle& style = *IMGUI::Context::this_->pSkin_->areaStyle_);
    float IMGUIEndArea(float scroll = -1);
    void IMGUISpacer(float percentageX = 100, float percentageY = 100);
    float IMGUILine(IMGUI::LineStyle& style = *IMGUI::Context::this_->pSkin_->lineStyle_);
	IMGUI::PSkin& IMGUISkin();
    PNode& IMGUINode();
	void IMGUIWindow(IMGUI::IWindow* obj, float percentageX = 100, float percentageY = 100, IMGUI::PWindowStyle style = IMGUI::Context::this_->pSkin_->windowStyle_);
}

/*
#define IMGUICOUNTER __COUNTER__ + (int)(IMGUI::IdsTypes::IMGUI_FIRST_VALID_ID) //first positions are reserved
#define IMGUIButton(text, ...) IMGUI::Button_(IMGUICOUNTER, text, ##__VA_ARGS__ )
#define IMGUIVSlider(value, ...) IMGUI::VSlider_(IMGUICOUNTER, value, ##__VA_ARGS__ )
#define IMGUILabel(text,...) IMGUI::Label_(IMGUICOUNTER, text, ##__VA_ARGS__ )
#define IMGUITitle(text,...) IMGUI::Title_(IMGUICOUNTER, text, ##__VA_ARGS__ )
#define IMGUITextField(text,...) IMGUI::TextField_(IMGUICOUNTER, text, nullptr, ##__VA_ARGS__ )
#define IMGUITextFieldWithPattern(text, pattern, ...) IMGUI::TextField_(IMGUICOUNTER, text, pattern, ##__VA_ARGS__ )
#define IMGUIBeginHorizontal(...) IMGUI::BeginHorizontal_(IMGUICOUNTER, ##__VA_ARGS__ )
#define IMGUIBeginVertical(...) IMGUI::BeginVertical_(IMGUICOUNTER, ##__VA_ARGS__ )
#define IMGUIEndArea(...) IMGUI::EndArea_(##__VA_ARGS__ )
#define IMGUISpacer(...) IMGUI::Spacer_(IMGUICOUNTER, ##__VA_ARGS__ )
#define IMGUISkin() IMGUI::Skin_()
#define IMGUINode() IMGUI::Node_()
#define IMGUIWindow(obj, ...) IMGUI::Window_(IMGUICOUNTER, obj, ##__VA_ARGS__)
*/