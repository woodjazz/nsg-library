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
            hotitem_(-1),
			activeitem_(-1),
			kbditem_(-1),
            keyentered_(0),
            keymod_(0),
            keyaction_(0),
            character_(0),
            lastwidget_(0),
            activeitem_needs_keyboard_(false),
            lastScrollHit_(-1),
            tick_(0),
            mouseRelDownX_(0),
            mouseRelDownY_(0)
		{
		}

		State::~State()
		{
			
		}

        void State::OnMouseMove(float x, float y)
        {
        	mousex_ = x;
        	mousey_ = y;

            if(mousedown_)
            {
                mouseRelX_ = mousex_ - mouseRelDownX_;
                mouseRelY_ = mousey_ - mouseRelDownY_;
            }
        }

        void State::OnMouseDown(float x, float y)
        {
            mouseRelDownX_ = mouseDownX_ = x;
            mouseRelDownY_ = mouseDownY_ = y;
        	mousex_ = x;
        	mousey_ = y;
        	mousedown_ = true;
            mouseup_ = false;
        }

        void State::OnMouseUp(float x, float y)
        {
            mousex_ = x;
            mousey_ = y;

        	mousedown_ = false;
            mouseup_ = true;
        }

        void State::OnMouseWheel(float x, float y)
        {
            mouseRelX_ = -x;
            mouseRelY_ = -y;
        }

        MouseRelPosition State::GetMouseRelPosition() const
        {
            MouseRelPosition relPos = {mouseRelX_, mouseRelY_};
            return relPos;
        }

        void State::OnKey(int key, int action, int modifier)
        {
            if(action == NSG_KEY_PRESS)
            {
                keyentered_ = key;
                keyaction_ = action;
                keymod_ = modifier;
            }
        }

        void State::OnChar(unsigned int character)
        {
        	character_ = character;
        }

        void State::DoTick()
        {
        	++tick_;
            if(tick_ > 30)
            {
                tick_ = 0;
            }
        }

        void State::Begin()
        {
            hotitem_ = 0;
        }

        void State::End()
        {
            mouseup_ = false;
            mouseRelX_ = 0;
            mouseRelY_ = 0;

            mouseRelDownX_ = mousex_;
            mouseRelDownY_ = mousey_;
            
            if(!activeitem_needs_keyboard_)
            {
                if(Keyboard::this_->Disable())
                {
                    Context::this_->pCamera_->SetPosition(Vertex3(0,0,0));
                }
            }

            if(!mousedown_)
            {
                activeitem_ = GLushort(-1);
            }
            else
            {
                if(activeitem_ == GLushort(-1))
                {
                    activeitem_needs_keyboard_ = false;
                }
            }

            // If no widget grabbed tab, clear focus
            if (!Context::this_->IsStable())
                kbditem_ = 0;
            else if(keyentered_ == NSG_KEY_TAB && !keymod_)
                kbditem_ = 0;
    
            // Clear the entered key
            keyentered_ = 0;    
            character_ = 0; 
        }

	}
}