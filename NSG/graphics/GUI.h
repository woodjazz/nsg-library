/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "Pass.h"
#include "Util.h"
#include "StrongFactory.h"
#include "Object.h"
#include <string>
#include <functional>

struct ImDrawData;
struct ImGuiContext;
namespace NSG
{
	class GUI : public Object, public StrongFactory<std::string, GUI>
	{
	public:
		GUI(const std::string& name = GetUniqueName("GUI"));
		~GUI();
		void Render(PWindow window, std::function<void(void)> callback);
		void SetArea(const Rect& area);
	private:
		bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
		void Setup();
		void InternalDraw(ImDrawData* draw_data);
        PRenderingContext context_;
		PTexture2D fontTexture_;
		SignalKey::PSlot slotKey_;
		SignalText::PSlot slotText_;
		SignalMouseMoved::PSlot slotMouseMoved_;
		SignalMouseButtonInt::PSlot slotMouseDown_;
		SignalMouseButtonInt::PSlot slotMouseUp_;
		SignalFloatFloat::PSlot slotMouseWheel_;
		SignalMultiGesture::PSlot slotMultiGesture_;
		SignalEmpty::PSlot slotTextureAllocated_;
		SignalEmpty::PSlot slotTextureReleased_;
		Pass pass_;
		PProgram program_;
		PVertexBuffer vBuffer_;
		PIndexBuffer iBuffer_;
		PCamera camera_;
		PWeakWindow window_;
		ImGuiContext* state_;
		bool configured_;
		//Area related to the window where the GUI is painted.
		//Needed to adjust the mouse' coordinates
		Rect area_;
	};
}
