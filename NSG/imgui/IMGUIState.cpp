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
#include <cstring>

namespace NSG 
{
	namespace IMGUI
	{
		State::State()
            : mousex_(0),
            mousey_(0),
            mousedown_(false),
            hotitem_(-1),
			activeitem_(-1),
			kbditem_(-1),
            keyentered_(0),
            keymod_(0),
            keyaction_(0),
            character_(0),
            lastwidget_(0),
            activeitem_needs_keyboard_(false),
            tick_(0)
		{
		}

		State::~State()
		{
			
		}

        void State::OnMouseMove(float x, float y)
        {
        	mousex_ = x;
        	mousey_ = y;
        }

        void State::OnMouseDown(float x, float y)
        {
        	mousex_ = x;
        	mousey_ = y;
        	mousedown_ = true;
        }

        void State::OnMouseUp()
        {
        	mousedown_ = false;
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
	}
}