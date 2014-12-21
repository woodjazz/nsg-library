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
#include "IMGUIVSliderThumb.h"
#include "IMGUIContext.h"
#include "IMGUIState.h"
#include "IMGUISkin.h"
#include "IMGUILayoutManager.h"
#include "TextMesh.h"
#include "Node.h"
#include "Technique.h"
#include "Pass.h"
#include "Material.h"
#include "Graphics.h"
#include "Keys.h"
#include <algorithm>

namespace NSG
{
    namespace IMGUI
    {
        VSliderThumb::VSliderThumb(float value, float percentageX, float percentageY, Style& style)
            : Object(LayoutType::CONTROL, percentageX, percentageY, style),
              value_(value)
        {
        }

        VSliderThumb::~VSliderThumb()
        {
            lastwidget_ = id_;
        }

        void VSliderThumb::UpdateControl()
        {
            if (layoutManager_.IsCurrentWindowActive()) 
            {
				if (mousedown_)
				{
					if ((!lastHit_ && area_->parent_->pNode_->IsPointInsideBB(Vertex3(mouseDownX_, mouseDownY_, 0))) || lastHit_ == id_)
					{
						lastHit_ = id_;

						Vertex3 thumbGlobalPos = node_->GetGlobalPosition();
						Vertex3 thumbGlobalScale = node_->GetGlobalScale();
						float yThumbTop = thumbGlobalPos.y + thumbGlobalScale.y;
						float yThumbBottom = thumbGlobalPos.y - thumbGlobalScale.y;
						float yThumbCenter = yThumbBottom + (yThumbTop - yThumbBottom) / 2;

						float centerOffset = yThumbCenter - mousey_;
						if (mouseRelY_ * centerOffset < 0)
							value_ = glm::clamp<float>(value_ + centerOffset, 0, 1);
						else if(!mouseRelY_ && (mousey_ > yThumbTop || mousey_ < yThumbBottom))
							value_ = glm::clamp<float>(value_ + centerOffset, 0, 1);
					}
				}
				else if (HasFocus() && mouseRelY_ != 0)
				{
					Vertex3 position = node_->GetPosition();
					Vertex3 parentGlobalScale = area_->parent_->pNode_->GetGlobalScale();
					position.y += mouseRelY_ * parentGlobalScale.y;
					Vertex3 scale = node_->GetScale();
					position.y = std::min(position.y, 1.0f - scale.y);
					position.y = std::max(position.y, -1.0f + scale.y);
					value_ = (position.y-1+scale.y)/(2*(-1+scale.y));
				}
            }
        }

        float VSliderThumb::Render()
        {
            Update();

			Vertex3 position = node_->GetPosition();
			Vertex3 scale = node_->GetScale();
			position.y = 2 * (-1 + scale.y) * value_ + 1 - scale.y;
			node_->SetPosition(position);

            return value_;
        }
      
    }
}