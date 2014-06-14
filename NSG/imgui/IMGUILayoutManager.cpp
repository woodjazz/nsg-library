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
#include "IMGUI.h"
#include "Technique.h"
#include "Graphics.h"
#include <algorithm>

namespace NSG 
{
	namespace IMGUI
	{
		LayoutArea::LayoutArea(GLushort id, bool isReadOnly, LayoutArea* parent, PNode pNode, LayoutType type, int percentageX, int percentageY) 
		: id_(id), percentageX_(percentageX), percentageY_(percentageY), pNode_(pNode), type_(type), isReadOnly_(isReadOnly), textOffsetX_(0), 
		cursor_character_position_(0), parent_(parent),
		childrenRoot_(new Node),
		isScrollable_(false),
		scrollFactorAreaX_(0),
		scrollFactorAreaY_(0)
		{
			CHECK_ASSERT(percentageX > 0 && percentageY > 0, __FILE__, __LINE__);
			childrenRoot_->SetParent(pNode);
		}

		void LayoutArea::CalculateScrollAreaFactor()
		{
			scrollFactorAreaX_ = 0;
			scrollFactorAreaY_ = 0;

			if(type_ == LayoutType::Vertical)
			{
	            auto it = children_.begin();
	            while(it != children_.end())
	            {
	                PLayoutArea child = *(it++);
	                scrollFactorAreaX_ = std::max<float>(scrollFactorAreaX_, child->percentageX_);
	                scrollFactorAreaY_ += child->percentageY_;
	            }
	        }
	        else
	        {
	            auto it = children_.begin();
	            while(it != children_.end())
	            {
	                PLayoutArea child = *(it++);
	                scrollFactorAreaX_ += child->percentageX_;
	                scrollFactorAreaY_ = std::max<float>(scrollFactorAreaY_, child->percentageY_);
	            }
	        }

            scrollFactorAreaX_ /= 100.0f;
            scrollFactorAreaY_ /= 100.0f;

	        isScrollable_ = scrollFactorAreaX_ > 1 || scrollFactorAreaY_ > 1; 
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

			if(percentageX == 0)
				percentageX = 100;

			if(percentageY == 0)
				percentageY = 100;

            if(!nestedAreas_.empty())
            {
            	pCurrentArea = nestedAreas_.back()->GetArea();
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
			    pCurrentArea->CalculateScrollAreaFactor();
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
			BeginVerticalArea(IMGUI_FIRST_VERTICAL_AREA_ID);
		}

		void LayoutManager::End()
		{
            EndArea(-1);

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

		void LayoutManager::BeginHorizontalArea(GLushort id, int percentageX, int percentageY)
		{
			PArea obj(new Area(id, LayoutType::Horizontal, percentageX, percentageY));
			(*obj)();			
			nestedAreas_.push_back(obj);
		}

		void LayoutManager::BeginVerticalArea(GLushort id, int percentageX, int percentageY)
		{
			PArea obj(new Area(id, LayoutType::Vertical, percentageX, percentageY));
			(*obj)();			
			nestedAreas_.push_back(obj);
		}

		float LayoutManager::EndArea(float scroll)
		{
			PArea obj = nestedAreas_.back();

			if(scroll >= 0 && scroll <= 1)
			{
				obj->SetScroll(scroll);
			}

			scroll = obj->GetScroll();

			nestedAreas_.pop_back();

			return scroll;
		}

		void LayoutManager::Spacer(GLushort id, int percentageX, int percentageY)
		{
			GetAreaForControl(id, true, LayoutType::Control, percentageX, percentageY);
		}

		void LayoutManager::RecalculateLayout(PLayoutArea pCurrentArea)
		{
            if(pCurrentArea->children_.empty())
                return;

			auto it = pCurrentArea->children_.begin();
            float yPosition = 1; //y position to the top of current area
            float xPosition = -1; //x position to the left of current area
			while(it != pCurrentArea->children_.end())
			{
				PLayoutArea pArea = *(it++);
				float scaleY(1);
				float scaleX(1);
				Vertex3 position(0);

                scaleY = pArea->percentageY_ / 100.0f;
			    scaleX = pArea->percentageX_ / 100.0f;

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