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
		bool Button_(GLushort id, const std::string& text, int percentage);
		void Label_(GLushort id, const std::string& text, int percentage);
		std::string TextField_(GLushort id, const std::string& text, std::regex* pRegex, int percentage);
		void BeginHorizontal_(GLushort id, int percentage);
		void EndHorizontal_();
		void BeginVertical_(GLushort id, int percentage);
		void EndVertical_();
		void Spacer_(GLushort id, int percentage);
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

#define IMGUIButton(text, percentage) IMGUI::Button_(__LINE__, text, percentage)
#define IMGUILabel(text, percentage) IMGUI::Label_(__LINE__, text, percentage)
#define IMGUITextField(text, percentage) IMGUI::TextField_(__LINE__, text, nullptr, percentage)
#define IMGUITextFieldWithPattern(text, pattern, percentage) IMGUI::TextField_(__LINE__, text, pattern, percentage)
#define IMGUIBeginHorizontal(percentage) IMGUI::BeginHorizontal_(__LINE__, percentage)
#define IMGUIEndHorizontal() IMGUI::EndHorizontal_()
#define IMGUIBeginVertical(percentage) IMGUI::BeginVertical_(__LINE__, percentage)
#define IMGUIEndVertical() IMGUI::EndVertical_()
#define IMGUISpacer(percentage) IMGUI::Spacer_(__LINE__, percentage)
#define IMGUISetPercentage(percentage) IMGUI::SetPercentage_(percentage)
#define IMGUISkin() IMGUI::Skin_()
#define IMGUINode() IMGUI::Node_()