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
#include "IMGUILine.h"
#include "IMGUISkin.h"
#include "IMGUILayoutManager.h"
#include "IMGUI.h"
#include "App.h"

namespace NSG
{
    namespace IMGUI
    {
        Line::Line(LineStyle& style)
            : Object(LayoutType::CONTROL, 100, 100, style)
        {
            if (area_->parent_->type_ == LayoutType::VERTICAL)
            {
                percentage_ = area_->percentageY_ = Pixels2PercentageY(style.pixels_);
                area_->percentageX_ = 100;
            }
            else
            {
                CHECK_ASSERT(area_->parent_->type_ == LayoutType::HORIZONTAL, __FILE__, __LINE__);
                area_->percentageY_ = 100;
                percentage_ = area_->percentageX_ = Pixels2PercentageX(style.pixels_);
            }

			layoutManager_.GetCurrentWindowManager()->RecalculateLayout(area_->parent_);
        }

        Line::~Line()
        {
        }

        float Line::Render()
        {
            Update();
            return percentage_;
        }
    }
}