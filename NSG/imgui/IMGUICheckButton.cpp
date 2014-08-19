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
#include "IMGUICheckButton.h"
#include "IMGUIStyle.h"
#include "Technique.h"
#include "Graphics.h"

namespace NSG
{
    namespace IMGUI
    {
        CheckButton::CheckButton(bool pressed, const std::string& text, HorizontalAlignment hAlign, VerticalAlignment vAlign, float percentageX, float percentageY, CheckButtonStyle& style)
            : Button(text, pressed, hAlign, vAlign, percentageX, percentageY, style),
			checkButtonStyle_(style)
        {
        }

        CheckButton::~CheckButton()
        {
            lastwidget_ = id_;
        }

        void CheckButton::FixCurrentTechnique()
        {
            if(pressed_)
            {
                if (IsActive()) 
					currentMaterial_ = checkButtonStyle_.pressedStyle_->activeMaterial_;
                else if (IsHot())
					currentMaterial_ = checkButtonStyle_.pressedStyle_->hotMaterial_;
                else
					currentMaterial_ = checkButtonStyle_.pressedStyle_->normalMaterial_;
            }
            else
            {
				Button::FixCurrentTechnique();
            }
        }
   }
}