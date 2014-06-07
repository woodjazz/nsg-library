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
#include "IMGUIArea.h"
#include "Technique.h"
#include "Graphics.h"
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

		PLayoutArea LayoutManager::InsertNewArea(GLushort id, bool isReadOnly, LayoutType type, int percentageX, int percentageY)
		{
            PLayoutArea pCurrentArea;
			PLayoutArea pArea;

            if(!nestedAreas_.empty())
            {
            	pCurrentArea = nestedAreas_.back();
                PNode pNode(new Node());
                pNode->SetParent(pCurrentArea->childrenRoot_);
            	pArea = PLayoutArea(new LayoutArea(id, isReadOnly, pCurrentArea.get(), pNode, type, percentageX, percentageY));
                CHECK_ASSERT(pCurrentArea->type_ != LayoutType::Control, __FILE__, __LINE__);
			    pCurrentArea->children_.insert(pArea);
            }
            else
            {
				pArea = PLayoutArea(new LayoutArea(id, isReadOnly, nullptr, pRootNode_, type, percentageX, percentageY));
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
			BeginVertical(0);
		}

		void LayoutManager::End()
		{
            EndVertical();

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

		PLayoutArea LayoutManager::GetArea(GLushort id) const
		{
			PLayoutArea pArea;

			auto it = areas_.find(id);

			if(it != areas_.end())
				pArea = it->second;

			return pArea;
		}

		PLayoutArea LayoutManager::GetAreaForControl(GLushort id, bool isReadOnly, LayoutType type, int percentageX, int percentageY)
		{
			PLayoutArea pArea = GetArea(id);
			
			if(!pArea)
			{
				CHECK_ASSERT(nestedAreas_.size() || type != LayoutType::Control, __FILE__, __LINE__);
				pArea = InsertNewArea(id, isReadOnly, type, percentageX, percentageY);
				newControlAdded_ = true;
			}

			CHECK_ASSERT(pArea, __FILE__, __LINE__);

            ++visibleAreas_;

            pCurrentNode_ = pArea->pNode_;
            
			return pArea;
		}

		void LayoutManager::BeginHorizontal(GLushort id, int percentageX, int percentageY)
		{
			Area obj(id, LayoutType::Horizontal, percentageX, percentageY);
			obj();			
			nestedAreas_.push_back(obj.GetArea());
		}

		void LayoutManager::EndHorizontal()
		{
			nestedAreas_.pop_back();
		}

		void LayoutManager::BeginVertical(GLushort id, int percentageX, int percentageY)
		{
			Area obj(id, LayoutType::Vertical, percentageX, percentageY);
			obj();			
			nestedAreas_.push_back(obj.GetArea());
		}

		void LayoutManager::EndVertical()
		{
			nestedAreas_.pop_back();
		}

		void LayoutManager::Spacer(GLushort id, int percentageX, int percentageY)
		{
			GetAreaForControl(id, true, LayoutType::Control, percentageX, percentageY);
		}

		void LayoutManager::RecalculateLayout(PLayoutArea pCurrentArea)
		{
            if(pCurrentArea->children_.empty())
                return;

            float currentAreaPercentageX = 100;
			float currentAreaPercentageY = 100;			

			if(pCurrentArea->percentageX_ > 0)
				currentAreaPercentageX = pCurrentArea->percentageX_;

			if(pCurrentArea->percentageY_ > 0)
				currentAreaPercentageY = pCurrentArea->percentageY_;

			int nEquallyDistributedControlsX = pCurrentArea->children_.size();
			int nEquallyDistributedControlsY = nEquallyDistributedControlsX;

			float remainingPercentageX = 100; // percentage of X to be equally distributed in the controls with 0 percentage
			float remainingPercentageY = 100; // percentage of Y to be equally distributed in the controls with 0 percentage

            {
				auto it = pCurrentArea->children_.begin();
				while(it != pCurrentArea->children_.end())
				{
					PLayoutArea pArea = *(it++);

                    if(pArea->percentageX_ || pArea->percentageY_)
                    {
					    if(pArea->percentageX_)
					    {
                            --nEquallyDistributedControlsX;
                            remainingPercentageX -= pArea->percentageX_;
					    }
					
                        if(pArea->percentageY_)
					    {
                            --nEquallyDistributedControlsY;
						    remainingPercentageY -= pArea->percentageY_;
					    }
                    }
				}
			}

            remainingPercentageX /= 100.0f;
			remainingPercentageY /= 100.0f;

            currentAreaPercentageX /= 100.0f;
            currentAreaPercentageY /= 100.0f;

			//assert(remainingPercentage >= 0 && "Total percentage for spacer exceeds 100%");

			auto it = pCurrentArea->children_.begin();
            float yPosition = 1; //y position to the top of current area
            float xPosition = -1; //x position to the left of current area
			while(it != pCurrentArea->children_.end())
			{
				PLayoutArea pArea = *(it++);
				float scaleY(1);
				float scaleX(1);
				Vertex3 position(0);

				if(pArea->percentageY_)
				{
					scaleY = pArea->percentageY_ / 100.0f;// * currentAreaPercentageY;
				}
                else if(pCurrentArea->type_ == LayoutType::Vertical)
                {
                    CHECK_ASSERT(nEquallyDistributedControlsY && "At least should be a control with 0 percentage!!!", __FILE__, __LINE__);
                    scaleY = remainingPercentageY/nEquallyDistributedControlsY;
                }

                //if(pArea->type_ != LayoutType::Control && scaleY > 1) //Only controls can scale up (areas have to fit in the screen)
                //    scaleY = 1;

				if(pArea->percentageX_)
				{
					scaleX = pArea->percentageX_ / 100.0f;// * currentAreaPercentageX;
				}
                else if(pCurrentArea->type_ == LayoutType::Horizontal)
                {
                    CHECK_ASSERT(nEquallyDistributedControlsX && "At least should be a control with 0 percentage!!!", __FILE__, __LINE__);
                    scaleX = remainingPercentageX/nEquallyDistributedControlsX;
                }

                //if(pArea->type_ != LayoutType::Control && scaleX > 1) //Only controls can scale up (areas have to fit in the screen)
                //    scaleX = 1;

                float stepY = scaleY;
                float stepX = scaleX;

				if(pCurrentArea->type_ == LayoutType::Vertical)
				{
					position.x = xPosition + stepX;
					position.y = yPosition - stepY;
	                yPosition -= 2*stepY;

	            }
	            else
	            {
	            	CHECK_ASSERT(pCurrentArea->type_ == LayoutType::Horizontal, __FILE__, __LINE__);
                    position.x = xPosition + stepX;
                    position.y = yPosition - stepY;
                    xPosition += 2*stepX;
	            }

	            pArea->pNode_->SetPosition(position);
	            pArea->pNode_->SetScale(Vertex3(scaleX, scaleY, 1));
			}
		}

		bool LayoutManager::IsStable() const 
		{
			return !newControlAdded_ && isStable_;
		}
	}
}