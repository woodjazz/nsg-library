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

		void LayoutArea::Set(const Vertex3& position, const Vertex3& scale)
		{
			pNode_->EnableUpdate(false);
			pNode_->SetPosition(position);
			pNode_->EnableUpdate(true);
			pNode_->SetScale(scale);
		}

		bool LayoutArea::IsInside(const Vertex3& point) const
		{
			BoundingBox box(*pNode_.get());
			return box.IsInside(point) != OUTSIDE;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		LayoutManager::WindowManager::WindowManager(PNode pRootNode, int percentageX, int percentageY)
		: layoutChanged_(false),
        visibleAreas_(0),
        newControlAdded_(false),
        isStable_(false),
        pRootNode_(pRootNode),
        visible_(true),
        percentageX_(percentageX),
        percentageY_(percentageY)
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
            	pCurrentArea->pNode_->EnableUpdate(false);
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
			Context::this_->pFrameColorSelection_->ClearDepthStencil();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			ClearStencilBuffer();
			//BeginWindow(IMGUI_FIRST_VERTICAL_AREA_ID, showTitle, percentageX_, percentageY_);
			BeginWindow(Context::this_->GetValidId(IMGUICOUNTER), showTitle, showBorder, percentageX_, percentageY_);
			
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
				CHECK_ASSERT(nestedAreas_.size() || type != LayoutType::Control, __FILE__, __LINE__);
				pArea = InsertNewArea(id, isReadOnly, type, percentageX, percentageY);
				newControlAdded_ = true;
			}

			CHECK_ASSERT(pArea, __FILE__, __LINE__);

            ++visibleAreas_;

            Context::this_->pCurrentNode_ = pArea->pNode_;
            
			return pArea;
		}

		void LayoutManager::WindowManager::BeginHorizontalArea(GLushort id, int percentageX, int percentageY)
		{
			PArea obj(new Area(id, LayoutType::Horizontal, percentageX, percentageY));
			(*obj)();			
			nestedAreas_.push_back(obj);
		}

		void LayoutManager::WindowManager::BeginWindow(GLushort id, bool showTitle, bool showBorder, int percentageX, int percentageY)
		{
            IMGUI::Window* window = new IMGUI::Window(id, showTitle, showBorder, percentageX, percentageY);
            (*window)();			
			nestedAreas_.push_back(PArea(window));
		}

		void LayoutManager::WindowManager::EndWindow()
		{
			nestedAreas_.pop_back();
		}

		void LayoutManager::WindowManager::BeginVerticalArea(GLushort id, int percentageX, int percentageY)
		{
			PArea obj(new Area(id, LayoutType::Vertical, percentageX, percentageY));
			(*obj)();			
			nestedAreas_.push_back(obj);
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
			GetAreaForControl(id, true, LayoutType::Control, percentageX, percentageY);
		}

		bool LayoutManager::WindowManager::IsStable() const 
		{
			return !newControlAdded_ && isStable_;
		}

		void LayoutManager::WindowManager::RecalculateLayout(PLayoutArea pCurrentArea)
		{
            if(pCurrentArea->children_.empty())
                return;

			auto it = pCurrentArea->children_.begin();
            float yPosition = pCurrentArea->controlArea_->GetTopPosition(); //y position to the top of current area
            float xPosition = pCurrentArea->controlArea_->GetLeftPosition(); //x position to the left of current area
            //float yTotalAreaPercentage = 50*(yPosition+1);
            //CHECK_ASSERT(yTotalAreaPercentage > 0, __FILE__, __LINE__);
			while(it != pCurrentArea->children_.end())
			{
				PLayoutArea pArea = *(it++);
				float scaleY(1);
				float scaleX(1);
				Vertex3 position(0);

                scaleY = pArea->percentageY_ / 100.0f;//yTotalAreaPercentage;
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

	            pArea->Set(position, Vertex3(scaleX, scaleY, 1));
			}
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		LayoutManager::LayoutManager(PNode pRootNode) 
		: mainWindowManager_(new WindowManager(pRootNode, 100, 100))
		{
		}

		void LayoutManager::Render()
		{
            currentWindowManager_ = mainWindowManager_;

            if(AppStatistics::this_)
			    AppStatistics::this_->Collect(false);

			currentWindowManager_->Begin(false, false);

            if(AppStatistics::this_)
			    AppStatistics::this_->Collect(true);

			App::this_->RenderGUIFrame();

            if(AppStatistics::this_)
			    AppStatistics::this_->Collect(false);

			currentWindowManager_->End();

			auto it = windowManagers_.begin();
			while(it != windowManagers_.end())
			{
				IMGUI::IWindow* window = it->first;
				currentWindowManager_ = it->second;

				if(currentWindowManager_->visible_)
				{
                    window->StartWindow();

					currentWindowManager_->Begin(true, true);
                    
                    if(AppStatistics::this_)
					    AppStatistics::this_->Collect(true);

					window->RenderWindow();
					
                    if(AppStatistics::this_)
                        AppStatistics::this_->Collect(false);

					currentWindowManager_->End();

                    window->EndWindow();

					currentWindowManager_->visible_ = !currentWindowManager_->IsStable();
				}

				++it;
			}

            if(AppStatistics::this_)
			    AppStatistics::this_->Collect(true);
		}

		void LayoutManager::Window(IMGUI::IWindow* obj, int percentageX, int percentageY)
		{
			auto it = windowManagers_.find(obj);
			if(it == windowManagers_.end())
			{
				PNode rootNode(new Node);
                PWindowManager windowManager(new WindowManager(rootNode, percentageX, percentageY));
				auto result = windowManagers_.insert(WindowManagers::value_type(obj, windowManager));
				CHECK_ASSERT(result.second == true, __FILE__, __LINE__);
			}
			else
			{
				it->second->visible_ = true;
			}
		}

		void LayoutManager::Invalidate()
		{
			auto it = windowManagers_.begin();
			while(it != windowManagers_.end())
			{
				IMGUI::IWindow* window = it->first;
				PWindowManager windowManager = it->second;
                auto itAreas = windowManager->areas_.begin();
                while(itAreas != windowManager->areas_.end())
                {
					PLayoutArea area = itAreas->second;
					windowManager->layoutChanged_ = true;
					++itAreas;
                }
				++it;
			}
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

		bool LayoutManager::IsStable() const 
		{
			return currentWindowManager_->IsStable();
		}

	}
}