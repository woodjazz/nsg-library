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
#include "IMGUIWindow.h"
#include "IMGUI.h"
#include "FrameColorSelection.h"
#include "IMGUITextManager.h"
#include "AppStatistics.h"
#include "App.h"
#include "Technique.h"
#include "Graphics.h"
#include "BoundingBox.h"
#include "Context.h"
#include <algorithm>

namespace NSG 
{
	namespace IMGUI
	{
		LayoutArea::LayoutArea(GLushort id, bool isReadOnly, LayoutArea* parent, PNode pNode, LayoutType type, int percentageX, int percentageY) 
		: id_(id), 
		percentageX_(percentageX), 
		percentageY_(percentageY), 
		pNode_(pNode), 
		type_(type), 
		isReadOnly_(isReadOnly), 
		textOffsetX_(0), 
		cursor_character_position_(0), 
		parent_(parent),
		childrenRoot_(new Node),
		isScrollable_(false),
		isXScrollable_(false),
		isYScrollable_(false),
		scrollFactorAreaX_(0),
		scrollFactorAreaY_(0),
		controlArea_(nullptr)
		{
			CHECK_ASSERT(percentageX > 0 && percentageY > 0, __FILE__, __LINE__);
			float scaleX = percentageX/100.0f;
			float scaleY = percentageY/100.0f;
			pNode->SetScale(Vertex3(scaleX, scaleY, 1));
			childrenRoot_->SetParent(pNode);
		}

		void LayoutArea::CalculateScrollAreaFactor()
		{
			scrollFactorAreaX_ = 0;
			scrollFactorAreaY_ = 0;
			/*
			float yPosition = controlArea_->GetTopPosition(); //y position to the top of current area
			float xPosition = controlArea_->GetLeftPosition(); //x position to the left of current area
			float areaYPercentage = 100 + (1 - yPosition) * 100;
			*/
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
			scrollFactorAreaY_ /= 100.0f;// areaYPercentage;

			isXScrollable_ = scrollFactorAreaX_ > 1;
			isYScrollable_ = scrollFactorAreaY_ > 1;

			isScrollable_ = isXScrollable_ || isYScrollable_;
		}

		bool LayoutArea::IsVisible() const
		{
			if(!parent_)
			{
				static BoundingBox boxParent(Vertex3(-1, -1, 0), Vertex3(1, 1, 0));
                BoundingBox box(*pNode_.get());
				return boxParent.IsInside(box) != OUTSIDE;
			}
			else
			{
				BoundingBox box(*pNode_.get());
				return BoundingBox(*parent_->pNode_.get()).IsInside(box) != OUTSIDE && parent_->IsVisible();
			}
		}

		bool LayoutArea::IsInside(const Vertex3& point) const
		{
			return pNode_->IsPointInsideBB(point);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		LayoutManager::WindowManager::WindowManager(IWindow* userWindow, GLushort id, PNode pRootNode, int percentageX, int percentageY)
		: id_(id),
		lastId_(0),
		layoutChanged_(false),
        visibleAreas_(0),
        newControlAdded_(false),
        isStable_(false),
        pRootNode_(pRootNode),
        visible_(false),
        percentageX_(percentageX),
        percentageY_(percentageY),
        userWindow_(userWindow),
		pTextManager_(new TextManager)
       	{

       	}
        
		size_t LayoutManager::WindowManager::GetNestingLevel() 
		{
			return nestedAreas_.size() + 1; 
		}

		PLayoutArea LayoutManager::WindowManager::InsertNewArea(GLushort id, bool isReadOnly, LayoutType type, int percentageX, int percentageY)
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
                CHECK_ASSERT(pCurrentArea->type_ != LayoutType::Control, __FILE__, __LINE__);
                PNode pNode(new Node());
                pNode->SetParent(pCurrentArea->childrenRoot_);
            	pArea = PLayoutArea(new LayoutArea(id, isReadOnly, pCurrentArea.get(), pNode, type, percentageX, percentageY));
			    pCurrentArea->children_.insert(pArea);
            }
            else
            {
				pArea = PLayoutArea(new LayoutArea(id, isReadOnly, nullptr, pRootNode_, type, percentageX, percentageY));
            }

			auto it = areas_.insert(WindowManager::AREAS::value_type(id, pArea));

            assert(it.second);

            if(++(it.first) != areas_.end())
            {
            	// The new area has been inserted in the middle => the layout has changed
                layoutChanged_ = true;
            }

            if(!nestedAreas_.empty())
            {
			    RecalculateLayout(pCurrentArea);
			    pCurrentArea->CalculateScrollAreaFactor();
			}

			return pArea;
		}

		void LayoutManager::WindowManager::Reset()
		{
			nestedAreas_.clear();
			areas_.clear();
			layoutChanged_ = false;
            visibleAreas_ = 0;
		}

		void LayoutManager::WindowManager::Begin(bool showTitle, bool showBorder)
		{
			lastId_ = 0;
			ClearStencilBuffer();
			BeginWindow(id_, showTitle, showBorder, percentageX_, percentageY_);
			
		}

		void LayoutManager::WindowManager::End()
		{
            EndWindow();

			if(layoutChanged_ || visibleAreas_ < areas_.size())
			{
				Reset();
				isStable_ = false;
			}
			else if(newControlAdded_)
			{
				newControlAdded_ = false;
				isStable_ = false;
			}
			else
			{
				isStable_ = true;
			}

            visibleAreas_ = 0;
		}

		PLayoutArea LayoutManager::WindowManager::GetArea(GLushort id) const
		{
			PLayoutArea pArea;

			auto it = areas_.find(id);

			if(it != areas_.end())
				pArea = it->second;

			return pArea;
		}

		PLayoutArea LayoutManager::WindowManager::GetAreaForControl(GLushort id, bool isReadOnly, LayoutType type, int percentageX, int percentageY)
		{
			PLayoutArea pArea = GetArea(id);
			
			if(!pArea)
			{
				CHECK_ASSERT(nestedAreas_.size() || type == LayoutType::Horizontal || type == LayoutType::Vertical, __FILE__, __LINE__);
				pArea = InsertNewArea(id, isReadOnly, type, percentageX, percentageY);
				newControlAdded_ = true;
			}

			CHECK_ASSERT(pArea, __FILE__, __LINE__);

            ++visibleAreas_;

            Context::this_->pCurrentNode_ = pArea->pNode_;
            
			return pArea;
		}

		void LayoutManager::WindowManager::InsertArea(PArea area)
		{
			CHECK_ASSERT(area->GetArea()->type_ == LayoutType::Horizontal || area->GetArea()->type_ == LayoutType::Vertical, __FILE__, __LINE__);
			nestedAreas_.push_back(area);
		}

		void LayoutManager::WindowManager::BeginHorizontalArea(GLushort id, int percentageX, int percentageY)
		{
			PArea obj(new Area(id, false, LayoutType::Horizontal, percentageX, percentageY));
			(*obj)();	
			InsertArea(obj);
		}

		void LayoutManager::WindowManager::BeginWindow(GLushort id, bool showTitle, bool showBorder, int percentageX, int percentageY)
		{
            IMGUI::Window* window = new IMGUI::Window(userWindow_, id, showTitle, showBorder, percentageX, percentageY);
			CHECK_ASSERT(window->GetArea()->type_ == LayoutType::Vertical, __FILE__, __LINE__);
            (*window)();	
			InsertArea(PArea(window));
		}

		void LayoutManager::WindowManager::EndWindow()
		{
			nestedAreas_.pop_back();
		}

		void LayoutManager::WindowManager::BeginVerticalArea(GLushort id, int percentageX, int percentageY)
		{
			PArea obj(new Area(id, false, LayoutType::Vertical, percentageX, percentageY));
			(*obj)();	
			InsertArea(obj);
		}

		float LayoutManager::WindowManager::EndArea(float scroll)
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

		void LayoutManager::WindowManager::Spacer(GLushort id, int percentageX, int percentageY)
		{
			GetAreaForControl(id, true, LayoutType::Spacer, percentageX, percentageY);
		}

		bool LayoutManager::WindowManager::IsReady() const 
		{
			return !newControlAdded_ && isStable_ && visible_;
		}

		void LayoutManager::WindowManager::RecalculateLayout(PLayoutArea pCurrentArea)
		{
            if(pCurrentArea->children_.empty())
                return;

			auto it = pCurrentArea->children_.begin();
            float yPosition = pCurrentArea->controlArea_->GetTopPosition(); //y position to the top of current area
            float xPosition = pCurrentArea->controlArea_->GetLeftPosition(); //x position to the left of current area

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

		GLushort LayoutManager::WindowManager::GetValidId(GLushort id)
		{
			id += id_;

			if (lastId_ >= id)
				id = ++lastId_;
			else
				lastId_ = id;

			return id;
		}

		PTextMesh LayoutManager::WindowManager::GetCurrentTextMesh(GLushort item, int maxLength)
		{
			return pTextManager_->GetTextMesh(item, maxLength, Context::this_->pSkin_->fontFile_, Context::this_->pSkin_->fontSize_);
		}

		void LayoutManager::WindowManager::Invalidate()
		{
			layoutChanged_ = true;
			pTextManager_->Invalidate();
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		LayoutManager::LayoutManager(PNode pRootNode) 
		: focusedUserWindow_(nullptr),
		focusHasChanged_(false)
		{
			Window(IMGUI_FIRST_VERTICAL_AREA_ID, App::this_, 100, 100); // Create main window
		}

		void LayoutManager::RenderUserWindow()
		{
			IWindow* userWindow = currentWindowManager_->userWindow_;

			userWindow->StartGUIWindow();

			if (userWindow == App::this_)
				currentWindowManager_->Begin(false, false);
			else
				currentWindowManager_->Begin(true, true);

			if (AppStatistics::this_)
				AppStatistics::this_->Collect(true);

			userWindow->RenderGUIWindow();

			if (AppStatistics::this_)
				AppStatistics::this_->Collect(false);

			currentWindowManager_->End();

			userWindow->EndGUIWindow();
		}

		void LayoutManager::Render()
		{
			focusHasChanged_ = false;
			auto it = windowsSequence_.begin();
			while(it != windowsSequence_.end())
			{
				IWindow* userWindow = *(it++);
				auto it1 = windowManagers_.begin();
				while (it1 != windowManagers_.end())
				{
					currentWindowManager_ = it1->second;
					currentWindowManager_->visible_ = userWindow == currentWindowManager_->userWindow_;
					RenderUserWindow();
					if (focusHasChanged_ || !currentWindowManager_->isStable_)
						return;
					++it1;
				}
			}

			if (AppStatistics::this_)
				AppStatistics::this_->Collect(true);
		}

		void LayoutManager::Window(GLushort id, IMGUI::IWindow* obj, int percentageX, int percentageY)
		{
			auto it = windowManagers_.find(obj);
			if(it == windowManagers_.end())
			{
				PNode rootNode(new Node);
                PWindowManager windowManager(new WindowManager(obj, id, rootNode, percentageX, percentageY));
				auto result = windowManagers_.insert(WindowManagers::value_type(obj, windowManager));
				CHECK_ASSERT(result.second == true, __FILE__, __LINE__);
				windowsSequence_.push_back(obj);
			}
		}

		void LayoutManager::Invalidate()
		{
			auto it = windowManagers_.begin();
			while(it != windowManagers_.end())
			{
				PWindowManager windowManager = it->second;
				windowManager->Invalidate();
				++it;
			}
		}

		bool LayoutManager::IsFirstOnTopOfSecond(IWindow* first, IWindow* second) const
		{
			bool firstFound = false;
			auto it = windowsSequence_.begin();
			while (it != windowsSequence_.end())
			{
				IWindow* userWindow = *(it++);
				if (userWindow == first)
					firstFound = true;
				else if (userWindow == second && firstFound)
					return false;
			}
			return true;
		}

		void LayoutManager::SetWindowFocus(float x, float y)
		{
			auto it = windowsSequence_.rbegin();
			while (it != windowsSequence_.rend())
			{
				IWindow* userWindow = *(it++);
				PWindowManager windowManager = windowManagers_.find(userWindow)->second;
				PLayoutArea area = windowManager->areas_.find(windowManager->id_)->second;
				if(area->IsInside(Vertex3(x, y, 0)))
				{
					focusedUserWindow_ = userWindow;
					focusHasChanged_ = true;

					if (userWindow != App::this_) // check it is not the main window
					{
						windowsSequence_.remove(userWindow);
						windowsSequence_.push_back(userWindow); // move window to the end of the list (will be the last one rendered)
					}

					break;
				}
			}

			CHECK_ASSERT(focusedUserWindow_ != nullptr, __FILE__, __LINE__);
		}

		PLayoutArea LayoutManager::GetAreaForControl(GLushort id, bool isReadOnly, LayoutType type, int percentageX, int percentageY)
		{
			return currentWindowManager_->GetAreaForControl(id, isReadOnly, type, percentageX, percentageY);
		}

		void LayoutManager::BeginHorizontalArea(GLushort id, int percentageX, int percentageY)
		{
			currentWindowManager_->BeginHorizontalArea(id, percentageX, percentageY);
		}

		void LayoutManager::BeginVerticalArea(GLushort id, int percentageX, int percentageY)
		{
			currentWindowManager_->BeginVerticalArea(id, percentageX, percentageY);
		}

		float LayoutManager::EndArea(float scroll)
		{
			return currentWindowManager_->EndArea(scroll);
		}

		void LayoutManager::Spacer(GLushort id, int percentageX, int percentageY)
		{
			currentWindowManager_->Spacer(id, percentageX, percentageY);
		}

		size_t LayoutManager::GetNestingLevel()
		{
			return currentWindowManager_->GetNestingLevel();
		}

		bool LayoutManager::IsReady() const 
		{
			return currentWindowManager_ && currentWindowManager_->IsReady();
		}

		GLushort LayoutManager::GetValidId(GLushort id)
		{
			if (currentWindowManager_)
				return currentWindowManager_->GetValidId(id);

			return 0;
		}

		PTextMesh LayoutManager::GetCurrentTextMesh(GLushort item, int maxLength)
		{
			if (currentWindowManager_)
				return currentWindowManager_->GetCurrentTextMesh(item, maxLength);

			return nullptr;
		}

		bool LayoutManager::IsCurrentWindowActive() const
		{
			return currentWindowManager_ && currentWindowManager_->userWindow_ == focusedUserWindow_;
		}

	}
}