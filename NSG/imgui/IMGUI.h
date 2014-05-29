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
		void Begin();
		void End();
		bool InternalButton(GLushort id, const std::string& text);
		void InternalLabel(GLushort id, const std::string& text);
		std::string InternalTextField(GLushort id, const std::string& text, std::regex* pRegex);
        void OnMouseMove(float x, float y);
        void OnMouseDown(float x, float y);
        void OnMouseUp();
        void OnKey(int key, int action, int modifier);
        void OnChar(unsigned int character);
        void DoTick();
		void InternalBeginHorizontal(GLushort id);
		void InternalEndHorizontal();
		void InternalBeginVertical(GLushort id);
		void InternalEndVertical();
		void InternalSpacer(GLushort id, int percentage);
		bool IsStable();
	}
}

#define IMGUIButton(text) IMGUI::InternalButton(__LINE__, text)
#define IMGUILabel(text) IMGUI::InternalLabel(__LINE__, text)
#define IMGUITextField(text) IMGUI::InternalTextField(__LINE__, text, nullptr)
#define IMGUITextFieldWithPattern(text, pattern) IMGUI::InternalTextField(__LINE__, text, pattern)
#define IMGUIBeginHorizontal() IMGUI::InternalBeginHorizontal(__LINE__)
#define IMGUIEndHorizontal() IMGUI::InternalEndHorizontal()
#define IMGUIBeginVertical() IMGUI::InternalBeginVertical(__LINE__)
#define IMGUIEndVertical() IMGUI::InternalEndVertical()
#define IMGUISpacer(percentage) IMGUI::InternalSpacer(__LINE__, percentage)