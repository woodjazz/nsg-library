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
#include "IMGUIStyle.h"
#include "IMGUILabel.h"
#include "IMGUIState.h"
#include "IMGUITitle.h"
#include "IMGUISizer.h"
#include "IMGUILayoutManager.h"
#include "FrameColorSelection.h"
#include "Node.h"
#include "Technique.h"
#include "Pass.h"
#include "Material.h"
#include "App.h"

namespace NSG
{
	namespace IMGUI
	{
		static const int TITLE_WIDTH = 20; //pixels 
		static const int SIZER_WIDTH = 5; //pixels 

		Window::Window(IWindow* userWindow, float percentageX, float percentageY)
			: Area(LayoutType::WINDOW, percentageX, percentageY, userWindow->GetStyle()),
        userWindow_(userWindow),
		sizerSizeX_(Pixels2PercentageX(SIZER_WIDTH)),
		longSizerSizeX_(100 - 2 * sizerSizeX_),
		sizerSizeY_(Pixels2PercentageY(SIZER_WIDTH)),
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

		void Window::BeginRenderUserWindow()
		{
			if (userWindow_ == App::this_)
				return;

			if(userWindow_->resizable_)
			{
				layoutManager_.BeginHorizontalArea(100, sizerSizeY_, *skin_.areaStyle_);
				{
					{
						Sizer obj(SizerType::LEFT_TOP_SIZER, sizerSizeX_, 100, *skin_.sizerLeftTopStyle_);
						obj.Render();
					}

					{
						Sizer obj(SizerType::TOP_SIZER, longSizerSizeX_, 100, *skin_.sizerTopStyle_);
						obj.Render();
					}

					{
						Sizer obj(SizerType::RIGHT_TOP_SIZER, sizerSizeX_, 100, *skin_.sizerRightTopStyle_);
						obj.Render();
					}
				}
				layoutManager_.EndArea(-1);

				layoutManager_.BeginHorizontalArea(100, longSizerSizeY_, *skin_.areaStyle_);
				{
					{
						Sizer obj(SizerType::LEFT_SIZER, sizerSizeX_, 100, *skin_.sizerLeftStyle_);
						obj.Render();
					}

					layoutManager_.BeginVerticalArea(longSizerSizeX_, 100, *skin_.areaStyle_);

					if(userWindow_->hasTitle_)
					{
						float titlePercentage = Pixels2PercentageY(TITLE_WIDTH);
						{
							Title obj(userWindow_->title_, 100, titlePercentage, *skin_.titleStyle_);
							obj.Render();				
						}

						layoutManager_.BeginVerticalArea(100, 100 - titlePercentage, *skin_.areaStyle_);
					}
				}
			}
			else if(userWindow_->hasTitle_)
			{
				float titlePercentage = Pixels2PercentageY(TITLE_WIDTH);
				{
					Title obj(userWindow_->title_, 100, titlePercentage, *skin_.titleStyle_);
					obj.Render();				
				}

				layoutManager_.BeginVerticalArea(100, 100 - titlePercentage, *skin_.areaStyle_);
			}
		}

		void Window::EndRenderUserWindow()
		{
			if (userWindow_ == App::this_)
				return;

			if(userWindow_->resizable_)
			{
				if(userWindow_->hasTitle_)
					layoutManager_.EndArea(-1); // Vertical

				layoutManager_.EndArea(-1); // Vertical

				{
					Sizer obj(SizerType::RIGHT_SIZER, sizerSizeX_, 100, *skin_.sizerRightStyle_);
					obj.Render();
				}
				
				layoutManager_.EndArea(-1); // Horizontal

				layoutManager_.BeginHorizontalArea(100, sizerSizeY_, *skin_.areaStyle_);
				{
					{
						Sizer obj(SizerType::LEFT_BOTTOM_SIZER, sizerSizeX_, 100, *skin_.sizerLeftBottomStyle_);
						obj.Render();
					}

					{
						Sizer obj(SizerType::BOTTOM_SIZER, longSizerSizeX_, 100, *skin_.sizerBottomStyle_);
						obj.Render();
					}

					{
						Sizer obj(SizerType::RIGHT_BOTTOM_SIZER, sizerSizeX_, 100, *skin_.sizerRightBottomStyle_);
						obj.Render();
					}
				}
				layoutManager_.EndArea(-1); // Horizontal
			}
			else if(userWindow_->hasTitle_)
			{
				layoutManager_.EndArea(-1); // Vertical
			}
		}
	}
}
