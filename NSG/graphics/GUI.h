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

struct ImDrawData;
namespace NSG
{
	class GUI
	{
	public:
		GUI(Window* mainWindow);
		~GUI();
		void Render(Window* window);
		static GUI* GetPtr();
	private:
		void Setup(Window* mainWindow);
		static void Draw(ImDrawData* draw_data);
		void InternalDraw(ImDrawData* draw_data);
		Graphics* graphics_;
		PTexture2D fontTexture_;
		SignalKey::PSlot slotKey_;
		SignalText::PSlot slotText_;
		SignalMouseMoved::PSlot slotMouseMoved_;
		SignalMouseButton::PSlot slotMouseDown_;
		SignalMouseButton::PSlot slotMouseUp_;
		SignalFloatFloat::PSlot slotMouseWheel_;
		SignalEmpty::PSlot slotTextureAllocated_;
		SignalEmpty::PSlot slotTextureReleased_;
		PPass pass_;
		PProgram program_;
		PVertexBuffer vBuffer_;
		PIndexBuffer iBuffer_;
		PCamera camera_;
		Window* window_;
	};
}