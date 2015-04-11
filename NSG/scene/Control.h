/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "SceneNode.h"

namespace NSG
{
	class Control : public SceneNode
	{
	public:
		Control(const std::string& name);
		~Control();
		////////////////////////////////////////////////////////////////////////////////////
		/// Align control into screen (screen in normalized device coordinates (-1, 1)).
		/// This only makes sense when the camera used for the GUI layer is the default orthographic (see Scene::orthoCamera_)
		////////////////////////////////////////////////////////////////////////////////////
		void SetScreenAlignment(HorizontalAlignment hAlign, VerticalAlignment vAlign);
		void SetLeftScreenMargin(float margin);
		void SetRightScreenMargin(float margin);
		void SetTopScreenMargin(float margin);
		void SetBottomScreenMargin(float margin);
		////////////////////////////////////////////////////////////////////////////////////
		void SetWindow(Window* window);
		SignalButtonMouse::PSignal signalPush_;
		SignalButtonMouse::PSignal signalPop_;
		SignalButtonMouse::PSignal signalClicked_;
		SignalEmpty::PSignal signalMouseEnter_;
		SignalEmpty::PSignal signalMouseLeave_;
	private:
		void Align();
		void OnSceneSet() override;
		bool pushed_;
		bool mouseEntered_;
		SignalNodeMouseDown::PSlot slotNodeMouseDown_;
		SignalMouseUp::PSlot slotMouseUp_;
		SignalMouseMoved::PSlot slotMouseMoved_;
		HorizontalAlignment hAlign_;
		VerticalAlignment vAlign_;
		float leftMargin_;
		float rightMargin_;
		float topMargin_;
		float bottomMargin_;
		Window* window_;
		SignalWindow::PSlot slotWindowCreated_;
	};
}