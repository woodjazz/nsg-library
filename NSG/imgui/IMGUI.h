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
#include <regex>
#include "Types.h"

namespace NSG 
{
	namespace IMGUI
	{
		bool Button_(GLushort id, const std::string& text, int percentageX = 0, int percentageY = 0);
		void Label_(GLushort id, const std::string& text, int percentageX = 0, int percentageY = 0);
		std::string TextField_(GLushort id, const std::string& text, std::regex* pRegex, int percentageX = 0, int percentageY = 0);
		void BeginHorizontal_(GLushort id, int percentageX = 0, int percentageY = 0);
		void BeginVertical_(GLushort id, int percentageX = 0, int percentageY = 0);
		float EndArea_(float scroll = -1);
		void Spacer_(GLushort id, int percentageX = 0, int percentageY = 0);
		PSkin& Skin_();
		PNode& Node_();
		
		bool IsStable();
		void Begin();
		void End();
        void OnMouseMove(float x, float y);
        void OnMouseDown(float x, float y);
        void OnMouseWheel(float x, float y);
        void OnMouseUp(float x, float y);
        void OnKey(int key, int action, int modifier);
        void OnChar(unsigned int character);
        void DoTick();
	}
}

#define IMGUI_UNKNOWN_ID 0
#define IMGUI_FIRST_VERTICAL_AREA_ID 1
#define IMGUI_VERTICAL_SLIDER_ID 2
#define IMGUI_HORIZONTAL_SLIDER_ID 3
#define IMGUICOUNTER __COUNTER__ + 10 //first 10th positions are reserved
#define IMGUIButton(text, ...) IMGUI::Button_(IMGUICOUNTER, text, ##__VA_ARGS__ )
#define IMGUILabel(text, ...) IMGUI::Label_(IMGUICOUNTER, text, ##__VA_ARGS__ )
#define IMGUITextField(text, ...) IMGUI::TextField_(IMGUICOUNTER, text, nullptr, ##__VA_ARGS__ )
#define IMGUITextFieldWithPattern(text, pattern, ...) IMGUI::TextField_(IMGUICOUNTER, text, pattern, ##__VA_ARGS__ )
#define IMGUIBeginHorizontal(...) IMGUI::BeginHorizontal_(IMGUICOUNTER, ##__VA_ARGS__ )
#define IMGUIBeginVertical(...) IMGUI::BeginVertical_(IMGUICOUNTER, ##__VA_ARGS__ )
#define IMGUIEndArea(scroll) IMGUI::EndArea_(scroll)
#define IMGUISpacer(...) IMGUI::Spacer_(IMGUICOUNTER, ##__VA_ARGS__ )
#define IMGUISkin() IMGUI::Skin_()
#define IMGUINode() IMGUI::Node_()