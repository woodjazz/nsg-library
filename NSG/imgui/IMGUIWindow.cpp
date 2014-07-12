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
#include "IMGUIWindow.h"
#include "IMGUI.h"
#include "IMGUIContext.h"
#include "IMGUISkin.h"
#include "IMGUILabel.h"
#include "IMGUIState.h"
#include "IMGUITitle.h"
#include "IMGUISizer.h"
#include "IMGUILayoutManager.h"
#include "FrameColorSelection.h"
#include "Node.h"
#include "Technique.h"
#include "Pass.h"
#include "App.h"
#include "Material.h"

namespace NSG
{
	namespace IMGUI
	{
		static const float TITLE_WIDTH = 30; //pixels 
		static const float SIZER_WIDTH = 10; //pixels 

		Window::Window(IWindow* userWindow, GLushort id, int percentageX, int percentageY)
			: Area(id, true, LayoutType::VERTICAL, percentageX, percentageY),
        userWindow_(userWindow),
		sizerSizeX_(PIXELS2PERCENTAGEX(SIZER_WIDTH)),
		longSizerSizeX_(100 - 2 * sizerSizeX_),
		sizerSizeY_(PIXELS2PERCENTAGEY(SIZER_WIDTH)),
		longSizerSizeY_(100 - 2 * sizerSizeY_)
		{
		}

	    Window::~Window()
	    {
	    }

	    void Window::UpdateControl()
	    {
			Area::UpdateControl();
	    }

		void Window::Render()
		{
			Update();
		}

		PTechnique Window::GetNormalTechnique() const
		{
			if(userWindow_ == App::this_)
				return skin_.mainWindowTechnique_;

			return skin_.windowTechnique_;
		}

		void Window::BeginRenderUserWindow()
		{
			if (userWindow_ == App::this_)
				return;

			layoutManager_.BeginHorizontalArea(id_ + IMGUI_WINDOW_AREA_ID0, 100, sizerSizeY_);
			{
				{
					Sizer obj(id_ + IMGUI_WINDOW_AREA_ID1, LEFT_TOP_SIZER, sizerSizeX_, 100);
					obj.Render();
				}

				{
					Sizer obj(id_ + IMGUI_WINDOW_AREA_ID2, TOP_SIZER, longSizerSizeX_, 100);
					obj.Render();
				}

				{
					Sizer obj(id_ + IMGUI_WINDOW_AREA_ID3, RIGHT_TOP_SIZER, sizerSizeX_, 100);
					obj.Render();
				}
			}
			layoutManager_.EndArea(-1);

			layoutManager_.BeginHorizontalArea(id_ + IMGUI_WINDOW_AREA_ID4, 100, longSizerSizeY_);
			{
				{
					Sizer obj(id_ + IMGUI_WINDOW_AREA_ID5, LEFT_SIZER, sizerSizeX_, 100);
					obj.Render();
				}

				layoutManager_.BeginVerticalArea(id_ + IMGUI_WINDOW_AREA_ID6, longSizerSizeX_, 100);

				float titlePercentage = PIXELS2PERCENTAGEY(TITLE_WIDTH);
				{
					Title obj(id_ + IMGUI_WINDOW_AREA_ID7, "Title", 10, 100, titlePercentage);
					obj.Render();				
				}

				layoutManager_.BeginVerticalArea(id_ + IMGUI_WINDOW_AREA_ID8, 100, 100 - titlePercentage);

			}
		}

		void Window::EndRenderUserWindow()
		{
			if (userWindow_ == App::this_)
				return;

			layoutManager_.EndArea(-1); // Vertical

			layoutManager_.EndArea(-1); // Vertical

			{
				Sizer obj(layoutManager_.GetValidId(id_ + IMGUI_WINDOW_AREA_ID9), RIGHT_SIZER, sizerSizeX_, 100);
				obj.Render();
			}
			
			layoutManager_.EndArea(-1); // Horizontal

			layoutManager_.BeginHorizontalArea(layoutManager_.GetValidId(id_ + IMGUI_WINDOW_AREA_IDA), 100, sizerSizeY_);
			{
				{
					Sizer obj(layoutManager_.GetValidId(id_ + IMGUI_WINDOW_AREA_IDB), LEFT_BOTTOM_SIZER, sizerSizeX_, 100);
					obj.Render();
				}

				{
					Sizer obj(layoutManager_.GetValidId(id_ + IMGUI_WINDOW_AREA_IDC), BOTTOM_SIZER, longSizerSizeX_, 100);
					obj.Render();
				}

				{
					Sizer obj(layoutManager_.GetValidId(id_ + IMGUI_WINDOW_AREA_IDD), RIGHT_BOTTOM_SIZER, sizerSizeX_, 100);
					obj.Render();
				}
			}
			layoutManager_.EndArea(-1); // Horizontal
		}
	}
}
