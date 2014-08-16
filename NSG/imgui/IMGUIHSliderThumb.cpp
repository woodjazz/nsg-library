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
#include "IMGUIHSliderThumb.h"
#include "IMGUIContext.h"
#include "IMGUIState.h"
#include "IMGUISkin.h"
#include "IMGUILayoutManager.h"
#include "TextMesh.h"
#include "SceneNode.h"
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
        HSliderThumb::HSliderThumb(float value, float percentageX, float percentageY, Style& style)
            : Object(LayoutType::CONTROL, percentageX, percentageY, style),
              value_(value)
        {
        }

        HSliderThumb::~HSliderThumb()
        {
            lastwidget_ = id_;
        }

        void HSliderThumb::UpdateControl()
        {
            if (layoutManager_.IsCurrentWindowActive()) 
            {
				if (mousedown_)
				{
					Vertex3 parentGlobalScale = area_->parent_->pNode_->GetGlobalScale();
					Vertex3 globalScale = node_->GetGlobalScale();
					Node& node = *area_->controlNodes_.node0_;
					node.SetParent(node_);
					node.SetInheritScale(false);
					globalScale.x = parentGlobalScale.x; // In order to hit all the slider area: reset scale
					node.SetScale(globalScale);

					if ((!lastHit_ && node.IsPointInsideBB(Vertex3(mouseDownX_, mouseDownY_, 0))) || lastHit_ == id_)
					{
						lastHit_ = id_;
						Vertex3 globalPos = area_->parent_->pNode_->GetGlobalPosition();
						Vertex3 globalScale = area_->parent_->pNode_->GetGlobalScale();

	                    float xLeft = globalPos.x - globalScale.x;
	                    float xRight = globalPos.x + globalScale.x;

	                    float x1 = mousex_;
	                    if (x1 >= xLeft && x1 <= xRight)
	                    {
	                        float a2 = 1 / (xRight - xLeft);
	                        float x2 = a2 * x1 - a2 * xLeft;
	                        CHECK_ASSERT(x2 >= 0 && x2 <= 1, __FILE__, __LINE__);
	                        value_ = x2;
	                    }
					}
				}
				else if (HasFocus() && mouseRelX_ != 0)
				{
					Vertex3 position = node_->GetPosition();
					Vertex3 parentGlobalScale = area_->parent_->pNode_->GetGlobalScale();
					position.x += mouseRelX_ * parentGlobalScale.x;
					Vertex3 scale = node_->GetScale();
					position.x = std::min(position.x, 1.0f - scale.x);
					position.x = std::max(position.x, -1.0f + scale.x);
					value_ = (position.x-1+scale.x)/(2*(-1+scale.x));
				}
            }
        }

        float HSliderThumb::Render()
        {
            Update();

			Vertex3 position = node_->GetPosition();
			Vertex3 scale = node_->GetScale();
			position.x = 2 * (-1 + scale.x) * value_ + 1 - scale.x;
			position.x *= -1;
			node_->SetPosition(position);

            return value_;
        }
    }
}