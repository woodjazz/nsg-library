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

namespace NSG
{
	namespace IMGUI
	{
		class Object
		{
		public:
			Object(GLushort id, LayoutType type, bool isWindow, int percentageX, int percentageY, bool keepAspectRatio);
			~Object();
			bool IsReady() const;
			void Draw();
			bool Update();
			virtual void UpdateControl() {};
			bool HasFocus() const;
			bool IsActive() const;
			bool IsHot() const;
			static void DisableKeyboard();
			virtual void OnFocus(bool needsKeyboard);
			virtual void OnActive();
			virtual void OnHot();
			virtual void OnKey(int key) {}
			virtual void OnChar(unsigned int character) {}
			virtual PTechnique GetActiveTechnique() const;
			virtual PTechnique GetHotTechnique() const;
			virtual PTechnique GetNormalTechnique() const;
			PLayoutArea GetArea() const { return area_; }
            void FixCurrentTecnique();
            bool IsMouseInArea() const;
            bool IsMouseButtonPressedInArea() const;
		protected:
			GLushort id_;
			State& uistate_;
			Skin& skin_;
			LayoutManager& layoutManager_;
			PWindowManager currentWindowManager_;
			PLayoutArea area_;
			GLushort& lastTitleHit_;
			GLushort& lastSizerHit_;
			PNode node_;
			Vertex3 areaSize_;
			PTechnique currentTechnique_;

			const float& mouseDownX_;
			const float& mouseDownY_;
			const float& mousex_;
			const float& mousey_;
			const bool& mousedown_;
			const bool& mouseup_;
			const float& mouseRelX_;
			const float& mouseRelY_;
			GLushort& activeScrollArea_;

			size_t level_;
			LayoutType type_;
			bool isWindow_;
			bool drawn_;

			GLushort& hotitem_;
			GLushort& activeitem_;
			GLushort& kbditem_;
			GLushort& lastwidget_;
			bool& activeitem_needs_keyboard_;	
			std::pair<int32_t, int32_t> viewSize_;
			bool keepAspectRatio_;

		};
	}
}