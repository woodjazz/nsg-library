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
		bool Button_(GLushort id, const std::string& text, int maxLength, int percentageX = 0, int percentageY = 0);
		void Label_(GLushort id, const std::string& text, int maxLength, int percentageX = 0, int percentageY = 0);
		std::string TextField_(GLushort id, const std::string& text, int maxLength, std::regex* pRegex, int percentageX = 0, int percentageY = 0);
		void BeginHorizontal_(GLushort id, int percentageX = 0, int percentageY = 0);
		void BeginVertical_(GLushort id, int percentageX = 0, int percentageY = 0);
		float EndArea_(float scroll = -1);
		void Spacer_(GLushort id, int percentageX = 0, int percentageY = 0);
		PSkin& Skin_();
		PNode& Node_();

        struct IWindow
        {
        	virtual ~IWindow() {};
        	virtual void StartWindow() {};
        	virtual void RenderWindow() = 0;
        	virtual void EndWindow() {};
        };
		
		void Window_(GLushort id, IWindow* obj, int percentageX = 0, int percentageY = 0);
		
		bool IsStable();
        void OnMouseMove(float x, float y);
        void OnMouseDown(float x, float y);
        void OnMouseWheel(float x, float y);
        void OnMouseUp(float x, float y);
        void OnKey(int key, int action, int modifier);
        void OnChar(unsigned int character);
        void DoTick();
	}
}

#define IMGUICOUNTER __COUNTER__ + IMGUI_FIRST_VALID_ID //first positions are reserved
#define IMGUIButton(maxLength, text, ...) IMGUI::Button_(IMGUICOUNTER, text, maxLength, ##__VA_ARGS__ )
#define IMGUILabel(maxLength, text,...) IMGUI::Label_(IMGUICOUNTER, text, maxLength, ##__VA_ARGS__ )
#define IMGUITextField(maxLength, text,...) IMGUI::TextField_(IMGUICOUNTER, text, maxLength, nullptr, ##__VA_ARGS__ )
#define IMGUITextFieldWithPattern(maxLength, text, pattern, ...) IMGUI::TextField_(IMGUICOUNTER, text, maxLength, pattern, ##__VA_ARGS__ )
#define IMGUIBeginHorizontal(...) IMGUI::BeginHorizontal_(IMGUICOUNTER, ##__VA_ARGS__ )
#define IMGUIBeginVertical(...) IMGUI::BeginVertical_(IMGUICOUNTER, ##__VA_ARGS__ )
#define IMGUIEndArea(scroll) IMGUI::EndArea_(scroll)
#define IMGUISpacer(...) IMGUI::Spacer_(IMGUICOUNTER, ##__VA_ARGS__ )
#define IMGUISkin() IMGUI::Skin_()
#define IMGUINode() IMGUI::Node_()
#define IMGUIWindow(obj, ...) IMGUI::Window_(IMGUICOUNTER, obj, ##__VA_ARGS__)