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
#include "IMGUILayoutManager.h"
#include "IMGUIContext.h"
#include "IMGUIState.h"
#include "IMGUISkin.h"
#include "Technique.h"
#include <algorithm>

namespace NSG 
{
	namespace IMGUI
	{
		LayoutManager::LayoutManager(PNode pRootNode, PNode pCurrentNode) 
		: layoutChanged_(false),
        visibleAreas_(0),
        newControlAdded_(false),
        isStable_(false),
        pRootNode_(pRootNode),
        pCurrentNode_(pCurrentNode)
		{
		}

		PLayoutArea LayoutManager::InsertNewArea(GLushort id, LayoutArea::Type type, int percentage)
		{
            PLayoutArea pCurrentArea;

			PLayoutArea pArea;

            if(!nestedAreas_.empty())
            {
            	pCurrentArea = nestedAreas_.back();
                PNode pNode(new Node());
                pNode->SetParent(pCurrentArea->pNode_);
            	pArea = PLayoutArea(new LayoutArea(id, pNode, type, percentage));
			    pCurrentArea->children_.insert(pArea);
            }
            else
            {
            	pArea = PLayoutArea(new LayoutArea(id, pRootNode_, type, percentage));
            }

			auto it = areas_.insert(AREAS::value_type(id, pArea));

            assert(it.second);

            if(++(it.first) != areas_.end())
            {
            	// The new area has been inserted in the middle => the layout has changed
                layoutChanged_ = true;
            }

            if(!nestedAreas_.empty())
            {
            	pCurrentArea->pNode_->EnableUpdate(false);
			    RecalculateLayout(pCurrentArea);
			}

			return pArea;
		}

		void LayoutManager::Reset()
		{
			nestedAreas_.clear();
			areas_.clear();
			layoutChanged_ = false;
            visibleAreas_ = 0;
		}

		void LayoutManager::Begin()
		{
		}

		void LayoutManager::End()
		{
			if(layoutChanged_ || visibleAreas_ < areas_.size())
			{
				Reset();
				isStable_ = false;
			}
			else if(newControlAdded_)
			{
				pRootNode_->EnableUpdate(true);
				pRootNode_->Update(false);
				newControlAdded_ = false;
				isStable_ = false;
			}
			else
			{
				isStable_ = true;
			}

            visibleAreas_ = 0;
		}

		PLayoutArea LayoutManager::GetAreaForControl(GLushort id, LayoutArea::Type type, int percentage)
		{
			PLayoutArea pArea;
			auto it = areas_.find(id);
			if(it != areas_.end())
			{
				pArea = it->second;
			}
			else if(nestedAreas_.size() || type != LayoutArea::Control)
			{
				pArea = InsertNewArea(id, type, percentage);
				newControlAdded_ = true;
			}

            ++visibleAreas_;

            pCurrentNode_ = pArea->pNode_;
            
			return pArea;
		}

		void LayoutManager::BeginHorizontal(GLushort id, int percentage)
		{
            PLayoutArea area = GetAreaForControl(id, LayoutArea::Horizontal, percentage);
            PNode node = area->pNode_;
            
            State& uistate = *Context::this_->state_;

            uistate.currentTechnique_ = Context::this_->pSkin_->areaTechnique_;
            uistate.currentTechnique_->Set(node);

			uistate.currentTechnique_->Render();


			nestedAreas_.push_back(area);
		}

		void LayoutManager::EndHorizontal()
		{
			nestedAreas_.pop_back();
		}

		void LayoutManager::BeginVertical(GLushort id, int percentage)
		{
            PLayoutArea area = GetAreaForControl(id, LayoutArea::Vertical, percentage);
            PNode node = area->pNode_;
            
            State& uistate = *Context::this_->state_;

            uistate.currentTechnique_ = Context::this_->pSkin_->areaTechnique_;
            uistate.currentTechnique_->Set(node);

			uistate.currentTechnique_->Render();


			nestedAreas_.push_back(area);

		}

		void LayoutManager::EndVertical()
		{
			nestedAreas_.pop_back();
		}

		void LayoutManager::Spacer(GLushort id, int percentage)
		{
			GetAreaForControl(id, LayoutArea::Control, percentage);
		}

		void LayoutManager::RecalculateLayout(PLayoutArea pCurrentArea)
		{
            if(pCurrentArea->children_.empty())
                return;

            float currentAreaPercentage = 100;
			if(pCurrentArea->percentage_ > 0)
				currentAreaPercentage = pCurrentArea->percentage_;

			int nControls = pCurrentArea->children_.size();

			float remainingPercentage = currentAreaPercentage; // percentage to be equally distributed in the controls with 0 percentage

			{
				auto it = pCurrentArea->children_.begin();
				while(it != pCurrentArea->children_.end())
				{
					PLayoutArea pArea = *it;
					
					if(pArea->percentage_)
					{
						remainingPercentage -= currentAreaPercentage * pArea->percentage_ / 100.0f;
						--nControls;
					}

					++it;
				}
			}

			remainingPercentage /= 100.0f;

            currentAreaPercentage /= 100.0f;

			//assert(remainingPercentage >= 0 && "Total percentage for spacer exceeds 100%");

			auto it = pCurrentArea->children_.begin();
			if(pCurrentArea->type_ == LayoutArea::Vertical)
			{
                float yPosition = 1; //position to the top of current area
				while(it != pCurrentArea->children_.end())
				{
					float scaleY(1);
					PLayoutArea pArea = *it;

					if(pArea->percentage_)
					{
						scaleY = pArea->percentage_ / 100.0f * currentAreaPercentage;
					}
                    else
                    {
                        CHECK_ASSERT(nControls && "At least should be a control with 0 percentage!!!", __FILE__, __LINE__);
                        scaleY = remainingPercentage/nControls;
                    }

                    if(pArea->type_ != LayoutArea::Control && scaleY > 1) //Only controls can scale up (areas have to fit in the screen)
                        scaleY = 1;

					float step = scaleY;
                    pArea->pNode_->SetPosition(Vertex3(0, yPosition - step, 0));
					pArea->pNode_->SetScale(Vertex3(1, scaleY, 1));
                    yPosition -= 2*step;
					++it;
				}
			}
			else
			{
				assert(pCurrentArea->type_ == LayoutArea::Horizontal);
                float xPosition = -1; //position to the left of current area
				while(it != pCurrentArea->children_.end())
				{
					float scaleX(1);
					PLayoutArea pArea = *it;
					if(pArea->percentage_)
					{
						scaleX = pArea->percentage_ / 100.0f * currentAreaPercentage;
					}
                    else
                    {
                        CHECK_ASSERT(nControls && "At least should be a control with 0 percentage!!!", __FILE__, __LINE__);
                        scaleX = remainingPercentage/nControls;
                    }

                    if(pArea->type_ != LayoutArea::Control && scaleX > 1) //Only controls can scale up (areas have to fit in the screen)
                        scaleX = 1;

                    float step = scaleX;
                    pArea->pNode_->SetPosition(Vertex3(xPosition + step, 0, 0));
					pArea->pNode_->SetScale(Vertex3(scaleX, 1, 1));
                    xPosition += 2*step;
					++it;
				}
			}
		}

		bool LayoutManager::IsStable() const 
		{
			return isStable_;
		}
	}
}