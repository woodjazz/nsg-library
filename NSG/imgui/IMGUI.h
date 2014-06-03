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
		void EndHorizontal_();
		void BeginVertical_(GLushort id, int percentageX = 0, int percentageY = 0);
		void EndVertical_();
		void Spacer_(GLushort id, int percentageX = 0, int percentageY = 0);
		PSkin& Skin_();
		PNode& Node_();
		
		bool IsStable();
		void Begin();
		void End();
        void OnMouseMove(float x, float y);
        void OnMouseDown(float x, float y);
        void OnMouseUp();
        void OnKey(int key, int action, int modifier);
        void OnChar(unsigned int character);
        void DoTick();
	}
}

#define IMGUICOUNTER __COUNTER__ + 1
#define IMGUIButton(text, ...) IMGUI::Button_(IMGUICOUNTER, text, ##__VA_ARGS__ )
#define IMGUILabel(text, ...) IMGUI::Label_(IMGUICOUNTER, text, ##__VA_ARGS__ )
#define IMGUITextField(text, ...) IMGUI::TextField_(IMGUICOUNTER, text, nullptr, ##__VA_ARGS__ )
#define IMGUITextFieldWithPattern(text, pattern, ...) IMGUI::TextField_(IMGUICOUNTER, text, pattern, ##__VA_ARGS__ )
#define IMGUIBeginHorizontal(...) IMGUI::BeginHorizontal_(IMGUICOUNTER, ##__VA_ARGS__ )
#define IMGUIEndHorizontal() IMGUI::EndHorizontal_()
#define IMGUIBeginVertical(...) IMGUI::BeginVertical_(IMGUICOUNTER, ##__VA_ARGS__ )
#define IMGUIEndVertical() IMGUI::EndVertical_()
#define IMGUISpacer(...) IMGUI::Spacer_(IMGUICOUNTER, ##__VA_ARGS__ )
#define IMGUISkin() IMGUI::Skin_()
#define IMGUINode() IMGUI::Node_()