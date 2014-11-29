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
#include "IMGUIState.h"
#include "Keys.h"
#include "Keyboard.h"
#include "Camera.h"
#include "IMGUIContext.h"
#include "IMGUILayoutManager.h"
#include "IMGUI.h"
#include "App.h"
#include <cstring>

namespace NSG
{
    namespace IMGUI
    {
        State::State()
            : mouseDownX_(-2),
              mouseDownY_(-2),
              mousex_(0),
              mousey_(0),
              mousedown_(false),
              mouseup_(false),
              mouseRelX_(0),
              mouseRelY_(0),
              activeitem_needs_keyboard_(false),

              keyentered_(0),
              keymod_(0),
              keyaction_(0),
              character_(0),
              lastHit_(static_cast<IdType>(IdsTypes::IMGUI_UNKNOWN_ID)),
              activeScrollArea_(static_cast<IdType>(IdsTypes::IMGUI_UNKNOWN_ID)),
              tick_(0),
              mouseRelDownX_(0),
              mouseRelDownY_(0)
        {
			slotMouseMoved_ = App::this_->signalMouseMoved_->Connect([&](float x, float y)
            {
                mousex_ = x;
                mousey_ = y;

                if (mousedown_)
                {
                    mouseRelX_ = mousex_ - mouseRelDownX_;
                    mouseRelY_ = mousey_ - mouseRelDownY_;
                }
            });


			slotMouseDown_ = App::this_->signalMouseDown_->Connect([&](int button, float x, float y)
			{
				activeScrollArea_ = static_cast<IdType>(IdsTypes::IMGUI_UNKNOWN_ID);
				mouseRelDownX_ = mouseDownX_ = x;
				mouseRelDownY_ = mouseDownY_ = y;
				mousex_ = x;
				mousey_ = y;
				mousedown_ = true;
				mouseup_ = false;
				Context::this_->pLayoutManager_->SetWindowFocus(x, y);
			});

			slotMouseUp_ = App::this_->signalMouseUp_->Connect([&](int button, float x, float y)
			{
				mousex_ = x;
				mousey_ = y;

				mousedown_ = false;
				mouseup_ = true;

				lastHit_ = static_cast<IdType>(IdsTypes::IMGUI_UNKNOWN_ID);
			});

			slotMouseWheel_ = App::this_->signalMouseWheel_->Connect([&](float x, float y)
			{
				mouseRelX_ = -x;
				mouseRelY_ = -y;
			});

			slotKey_ = App::this_->signalKey_->Connect([&](int key, int action, int modifier)
			{
				if (action == NSG_KEY_PRESS)
				{
					keyentered_ = key;
					keyaction_ = action;
					keymod_ = modifier;
				}
			});

			slotChar_ = App::this_->signalChar_->Connect([&](unsigned int character)
			{
				character_ = character;
			});
        }

        State::~State()
        {

        }


        MouseRelPosition State::GetMouseRelPosition() const
        {
            MouseRelPosition relPos = {mouseRelX_, mouseRelY_};
            return relPos;
        }

        void State::DoTick()
        {
            ++tick_;
            if (tick_ > 30)
            {
                tick_ = 0;
            }
        }

        void State::Begin()
        {
        }

        void State::End()
        {
            mouseup_ = false;
            mouseRelX_ = 0;
            mouseRelY_ = 0;

            mouseRelDownX_ = mousex_;
            mouseRelDownY_ = mousey_;

            // Clear the entered key
            keyentered_ = 0;
            character_ = 0;
        }

    }
}