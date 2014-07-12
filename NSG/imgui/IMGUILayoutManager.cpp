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
#include "Keys.h"
#include <algorithm>

namespace NSG 
{
	namespace IMGUI
	{
		LayoutArea::LayoutArea(GLushort id, LayoutArea* parent, PNode pNode, LayoutType type, int percentageX, int percentageY, bool keepAspectRatio) 
		: id_(id), 
		percentageX_(percentageX), 
		percentageY_(percentageY), 
		keepAspectRatio_(keepAspectRatio),
		pNode_(pNode), 
		type_(type), 
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
			childrenRoot_->SetParent(pNode_);
		}

		void LayoutArea::CalculateScrollAreaFactor()
		{
			scrollFactorAreaX_ = 0;
			scrollFactorAreaY_ = 0;

			if(type_ == LayoutType::VERTICAL)
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

		WindowManager::WindowManager(IWindow* userWindow, GLushort id, PNode pRootNode, int percentageX, int percentageY, bool keepAspectRatio)
		: uistate_(*Context::this_->state_),
		id_(id),
		lastId_(0),
		layoutChanged_(false),
        visibleAreas_(0),
        newControlAdded_(false),
        isStable_(false),
        pRootNode_(pRootNode),
        visible_(false),
        percentageX_(percentageX),
        percentageY_(percentageY),
		keepAspectRatio_(keepAspectRatio),
        userWindow_(userWindow),
		pTextManager_(new TextManager),
        hotitem_(-1),
		activeitem_(-1),
		kbditem_(-1),
        lastwidget_(0),
        currentWindow_(nullptr)
       	{

       	}
        
		size_t WindowManager::GetNestingLevel() 
		{
			return nestedAreas_.size() + 1; 
		}

		PLayoutArea WindowManager::InsertNewArea(GLushort id, LayoutType type, int percentageX, int percentageY, bool keepAspectRatio)
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
				CHECK_ASSERT(pCurrentArea->type_ != LayoutType::CONTROL, __FILE__, __LINE__);
                PNode pNode(new Node());
                pNode->SetParent(pCurrentArea->childrenRoot_);
            	pArea = PLayoutArea(new LayoutArea(id, pCurrentArea.get(), pNode, type, percentageX, percentageY, keepAspectRatio));
			    pCurrentArea->children_.insert(pArea);
            }
            else
            {
				pArea = PLayoutArea(new LayoutArea(id, nullptr, pRootNode_, type, percentageX, percentageY, keepAspectRatio));
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

		void WindowManager::Reset()
		{
			nestedAreas_.clear();
			areas_.clear();
			layoutChanged_ = false;
            visibleAreas_ = 0;
		}

		void WindowManager::Begin()
		{
			hotitem_ = 0;
			lastId_ = 0;
			ClearStencilBuffer();
			BeginWindow(id_, percentageX_, percentageY_, keepAspectRatio_);
			
		}

		void WindowManager::End()
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

            if(!uistate_.mousedown_)
            {
                activeitem_ = GLushort(-1);
            }

            if (!IsStable())
                kbditem_ = 0; //layout changed
            else if(uistate_.keyentered_ == NSG_KEY_TAB && !uistate_.keymod_) //clear focus
                kbditem_ = 0;

		}

		PLayoutArea WindowManager::GetArea(GLushort id) const
		{
			PLayoutArea pArea;

			auto it = areas_.find(id);

			if(it != areas_.end())
				pArea = it->second;

			return pArea;
		}

		PLayoutArea WindowManager::GetAreaForControl(GLushort id, LayoutType type, int percentageX, int percentageY, bool keepAspectRatio)
		{
			PLayoutArea pArea = GetArea(id);
			
			if(!pArea)
			{
				CHECK_ASSERT(nestedAreas_.size() || type == LayoutType::HORIZONTAL || type == LayoutType::VERTICAL, __FILE__, __LINE__);
				pArea = InsertNewArea(id, type, percentageX, percentageY, keepAspectRatio);
				newControlAdded_ = true;
			}

			CHECK_ASSERT(pArea, __FILE__, __LINE__);

            ++visibleAreas_;

            Context::this_->pCurrentNode_ = pArea->pNode_;
            
			return pArea;
		}

		void WindowManager::InsertArea(PArea area)
		{
			CHECK_ASSERT(area->GetArea()->type_ == LayoutType::HORIZONTAL || area->GetArea()->type_ == LayoutType::VERTICAL, __FILE__, __LINE__);
			nestedAreas_.push_back(area);
		}

		void WindowManager::BeginHorizontalArea(GLushort id, int percentageX, int percentageY, bool keepAspectRatio)
		{
			PArea obj(new Area(id, false, LayoutType::HORIZONTAL, percentageX, percentageY, keepAspectRatio));
			InsertArea(obj);
			obj->Render();	
			
		}

		void WindowManager::BeginWindow(GLushort id, int percentageX, int percentageY, bool keepAspectRatio)
		{
			CHECK_ASSERT(currentWindow_ == nullptr, __FILE__, __LINE__);
			currentWindow_ = new IMGUI::Window(userWindow_, id, percentageX, percentageY, keepAspectRatio);
			CHECK_ASSERT(currentWindow_->GetArea()->type_ == LayoutType::VERTICAL, __FILE__, __LINE__);
			InsertArea(PArea(currentWindow_));
            currentWindow_->Render();	
			
		}

		void WindowManager::EndWindow()
		{
			nestedAreas_.pop_back();
			currentWindow_ = nullptr;
		}

		void WindowManager::BeginVerticalArea(GLushort id, int percentageX, int percentageY, bool keepAspectRatio)
		{
			PArea obj(new Area(id, false, LayoutType::VERTICAL, percentageX, percentageY, keepAspectRatio));
			InsertArea(obj);
			obj->Render();	
			
		}

		float WindowManager::EndArea(float scroll)
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

		void WindowManager::Spacer(GLushort id, int percentageX, int percentageY, bool keepAspectRatio)
		{
			GetAreaForControl(id, LayoutType::SPACER, percentageX, percentageY, keepAspectRatio);
		}

		bool WindowManager::IsReady() const 
		{
			return IsStable() && visible_;
		}

		bool WindowManager::IsStable() const
		{
			return !newControlAdded_ && isStable_;
		}

		void WindowManager::RecalculateLayout(PLayoutArea pCurrentArea)
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

				if (pCurrentArea->type_ == LayoutType::VERTICAL)
				{
					position.x = xPosition + stepX;
					position.y = yPosition - stepY;
	                yPosition -= 2*stepY;

	            }
	            else
	            {
					CHECK_ASSERT(pCurrentArea->type_ == LayoutType::HORIZONTAL, __FILE__, __LINE__);
                    position.x = xPosition + stepX;
                    position.y = yPosition - stepY;
                    xPosition += 2*stepX;
	            }

				pArea->pNode_->SetPosition(position);
				//pArea->pNode_->SetInheritScale(!pArea->keepAspectRatio_);
				pArea->pNode_->SetScale(Vertex3(scaleX, scaleY, 1));
			}
		}

		GLushort WindowManager::GetValidId(GLushort id)
		{
			id += id_;

			if (lastId_ >= id)
				id = ++lastId_;
			else
				lastId_ = id;

			return id;
		}

		PTextMesh WindowManager::GetCurrentTextMesh(GLushort item, int maxLength)
		{
			return pTextManager_->GetTextMesh(item, maxLength, Context::this_->pSkin_->fontFile_, Context::this_->pSkin_->fontSize_);
		}

		void WindowManager::Invalidate()
		{
			layoutChanged_ = true;
			pTextManager_->Invalidate();
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		LayoutManager::LayoutManager(PNode pRootNode) 
		: focusedUserWindow_(App::this_),
		focusHasChanged_(false)
		{
			Window(IMGUI_MAIN_WINDOW_ID, App::this_, 100, 100, false); // Create main window
		}

		void LayoutManager::RenderUserWindow()
		{
			IWindow* userWindow = currentWindowManager_->userWindow_;

			userWindow->StartGUIWindow();

			currentWindowManager_->Begin();

			if (AppStatistics::this_)
				AppStatistics::this_->Collect(true);

			currentWindowManager_->currentWindow_->BeginRenderUserWindow();
			userWindow->RenderGUIWindow();
			currentWindowManager_->currentWindow_->EndRenderUserWindow();

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

		void LayoutManager::Window(GLushort id, IMGUI::IWindow* obj, int percentageX, int percentageY, bool keepAspectRatio)
		{
			auto it = windowManagers_.find(obj);
			if(it == windowManagers_.end())
			{
				PNode rootNode(new Node);
				PWindowManager windowManager(new WindowManager(obj, id, rootNode, percentageX, percentageY, keepAspectRatio));
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
				auto itArea = windowManager->areas_.find(windowManager->id_);
				if (itArea != windowManager->areas_.end())
				{
					PLayoutArea area = itArea->second;
					if (area->IsInside(Vertex3(x, y, 0)))
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
					CHECK_ASSERT(focusedUserWindow_ != nullptr, __FILE__, __LINE__);
				}
			}

			
		}

		PLayoutArea LayoutManager::GetAreaForControl(GLushort id, LayoutType type, int percentageX, int percentageY, bool keepAspectRatio)
		{
			return currentWindowManager_->GetAreaForControl(id, type, percentageX, percentageY, keepAspectRatio);
		}

		void LayoutManager::BeginHorizontalArea(GLushort id, int percentageX, int percentageY, bool keepAspectRatio)
		{
			currentWindowManager_->BeginHorizontalArea(id, percentageX, percentageY, keepAspectRatio);
		}

		void LayoutManager::BeginVerticalArea(GLushort id, int percentageX, int percentageY, bool keepAspectRatio)
		{
			currentWindowManager_->BeginVerticalArea(id, percentageX, percentageY, keepAspectRatio);
		}

		float LayoutManager::EndArea(float scroll)
		{
			return currentWindowManager_->EndArea(scroll);
		}

		void LayoutManager::Spacer(GLushort id, int percentageX, int percentageY, bool keepAspectRatio)
		{
			currentWindowManager_->Spacer(id, percentageX, percentageY, keepAspectRatio);
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

		PNode LayoutManager::GetCurrentWindowNode() const
		{
			return currentWindowManager_->pRootNode_;
		}

		PWindowManager LayoutManager::GetCurrentWindowManager() const
		{
			return currentWindowManager_;
		}
	}
}