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
#include "Keys.h"
#include "Node.h"
#include <algorithm>
#include <limits>

namespace NSG
{
    namespace IMGUI
    {
        LayoutArea::LayoutArea(IdType id, LayoutArea* parent, PNode pNode, LayoutType type, float percentageX, float percentageY)
            : id_(id),
              percentageX_(percentageX),
              percentageY_(percentageY),
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
              scrollFactorAreaY_(0)
        {
            controlNodes_.node0_ = PNode(new Node);
            controlNodes_.node1_ = PNode(new Node);
            controlNodes_.node2_ = PNode(new Node);
            controlNodes_.node3_ = PNode(new Node);
            

			if (percentageX < 0)
				percentageX = 100;
			if (percentageY < 0)
				percentageY = 100;

            float scaleX = percentageX / 100.0f;
            float scaleY = percentageY / 100.0f;
            pNode->SetScale(Vertex3(scaleX, scaleY, 1));
            childrenRoot_->SetParent(pNode_);
        }

        void LayoutArea::CalculateScrollAreaFactor()
        {
            scrollFactorAreaX_ = 0;
            scrollFactorAreaY_ = 0;

            if (type_ == LayoutType::VERTICAL || type_ == LayoutType::WINDOW)
            {
                auto it = children_.begin();
                while (it != children_.end())
                {
                    PLayoutArea child = *(it++);
                    scrollFactorAreaX_ = std::max<float>(scrollFactorAreaX_, child->percentageX_);
                    scrollFactorAreaY_ += child->percentageY_;
                }
            }
            else
            {
                auto it = children_.begin();
                while (it != children_.end())
                {
                    PLayoutArea child = *(it++);
                    scrollFactorAreaX_ += child->percentageX_;
                    scrollFactorAreaY_ = std::max<float>(scrollFactorAreaY_, child->percentageY_);
                }
            }


            scrollFactorAreaX_ /= 100.0f;
            scrollFactorAreaY_ /= 100.0f;

            isXScrollable_ = scrollFactorAreaX_ > 1 + std::numeric_limits<float>::epsilon();
            isYScrollable_ = scrollFactorAreaY_ > 1 + std::numeric_limits<float>::epsilon();

            isScrollable_ = isXScrollable_ || isYScrollable_;
        }

        bool LayoutArea::IsVisible() const
        {
            if (!parent_)
            {
                static BoundingBox boxParent(Vertex3(-1, -1, 0), Vertex3(1, 1, 0));
                BoundingBox box(*pNode_.get());
                return boxParent.IsInside(box) != Intersection::OUTSIDE;
            }
            else
            {
                BoundingBox box(*pNode_.get());
                return BoundingBox(*parent_->pNode_.get()).IsInside(box) != Intersection::OUTSIDE && parent_->IsVisible();
            }
        }

        bool LayoutArea::IsInside(const Vertex3& point) const
        {
            return pNode_->IsPointInsideBB(point);
        }

        bool LayoutArea::HasSizeChanged(float percentageX, float percentageY) const
        {
			//if (type_ != LayoutType::LINE)
			{
				if (percentageX != percentageX_)
					return true;
				else if (percentageY != percentageY_)
					return true;
			}

            return false;
        }


        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        WindowManager::WindowManager(IWindow* userWindow, IdType id, PNode pRootNode, float percentageX, float percentageY, PWindowStyle style)
            : uistate_(*Context::this_->state_),
              id_(id),
              lastId_(static_cast<IdType>(IdsTypes::IMGUI_FIRST_VALID_ID)),
              layoutChanged_(false),
              retrievedAreas_(0),
              isStable_(false),
              pRootNode_(pRootNode),
              visible_(false),
              percentageX_(percentageX),
              percentageY_(percentageY),
              userWindow_(userWindow),
              hotitem_(-1),
              activeitem_(-1),
              kbditem_(-1),
              lastwidget_(0),
              currentWindow_(nullptr),
              created_(true),
              style_(style)
        {
        }

        size_t WindowManager::GetNestingLevel()
        {
            return nestedAreas_.size() + 1;
        }

        PLayoutArea WindowManager::InsertNewArea(IdType id, LayoutType type, float percentageX, float percentageY)
        {
            PLayoutArea pArea;

            if (!nestedAreas_.empty())
            {
                PLayoutArea parent = nestedAreas_.back()->GetArea();
                CHECK_ASSERT(parent->type_ != LayoutType::CONTROL, __FILE__, __LINE__);
                PNode pNode(new Node());
                pNode->SetParent(parent->childrenRoot_);
                pArea = PLayoutArea(new LayoutArea(id, parent.get(), pNode, type, percentageX, percentageY));
                parent->children_.push_back(pArea);
            }
            else
            {
                pArea = PLayoutArea(new LayoutArea(id, nullptr, pRootNode_, type, percentageX, percentageY));
            }

            auto it = areas_.insert(WindowManager::AREAS::value_type({ id, type }, pArea));
            CHECK_ASSERT(it.second, __FILE__, __LINE__);

            layoutChanged_ = true;

            return pArea;
        }

        void WindowManager::Begin()
        {
            hotitem_ = 0;
            lastId_ = static_cast<IdType>(IdsTypes::IMGUI_FIRST_VALID_ID);
            Graphics::this_->ClearStencilBuffer();
            BeginWindow();
        }

        void WindowManager::End()
        {
            EndWindow();

            if (retrievedAreas_ != areas_.size())
            {
                lastId_ = static_cast<IdType>(IdsTypes::IMGUI_FIRST_VALID_ID);
                nestedAreas_.clear();
                areas_.clear();
                isStable_ = false;
            }
            else if (layoutChanged_)
            {
                layoutChanged_ = false;
                isStable_ = false;
            }
            else
            {
                isStable_ = true;
            }

            retrievedAreas_ = 0;

            if (!uistate_.mousedown_)
            {
                activeitem_ = IdType(-1);
            }

            if (!IsStable())
                kbditem_ = 0; //layout changed
            else if (uistate_.keyentered_ == NSG_KEY_TAB && !uistate_.keymod_) //clear focus
                kbditem_ = 0;

        }

        PLayoutArea WindowManager::GetArea(IdType id, LayoutType type) const
        {
            PLayoutArea pArea;

            auto it = areas_.find(AreaKey { id, type });

            if (it != areas_.end())
                pArea = it->second;

            return pArea;
        }

        PLayoutArea WindowManager::GetAreaForControl(IdType id, LayoutType type, float percentageX, float percentageY)
        {
            PLayoutArea pArea = GetArea(id, type);

            if (!pArea)
            {
                CHECK_ASSERT(nestedAreas_.size() || type == LayoutType::WINDOW || type == LayoutType::HORIZONTAL || type == LayoutType::VERTICAL, __FILE__, __LINE__);
                pArea = InsertNewArea(id, type, percentageX, percentageY);
                if (pArea->parent_)
                    RecalculateLayout(pArea->parent_);
            }
			else if (pArea->HasSizeChanged(percentageX, percentageY))
            {
                pArea->percentageX_ = percentageX;
                pArea->percentageY_ = percentageY;
                RecalculateLayout(pArea->parent_);
            }

            CHECK_ASSERT(pArea, __FILE__, __LINE__);

            ++retrievedAreas_;

            Context::this_->pCurrentNode_ = pArea->pNode_;

            return pArea;
        }

        void WindowManager::BeginHorizontalArea(float percentageX, float percentageY, AreaStyle& style)
        {
            PArea obj = std::make_shared<Area>(LayoutType::HORIZONTAL, percentageX, percentageY, style);
            nestedAreas_.push_back(obj);
            obj->Render();

        }

        void WindowManager::BeginWindow()
        {
            CHECK_ASSERT(currentWindow_ == nullptr, __FILE__, __LINE__);
            currentWindow_ = new IMGUI::Window(userWindow_, percentageX_, percentageY_, *style_);
            CHECK_ASSERT(currentWindow_->GetArea()->type_ == LayoutType::WINDOW, __FILE__, __LINE__);
            nestedAreas_.push_back(PArea(currentWindow_));
            currentWindow_->Render();
        }

        void WindowManager::EndWindow()
        {
            nestedAreas_.pop_back();
            currentWindow_ = nullptr;
        }

        void WindowManager::BeginVerticalArea(float percentageX, float percentageY, AreaStyle& style)
        {
            PArea obj = std::make_shared<Area>(LayoutType::VERTICAL, percentageX, percentageY, style);
            nestedAreas_.push_back(obj);
            obj->Render();
        }

        float WindowManager::EndArea(float scroll)
        {
            PArea obj = nestedAreas_.back();

            if (scroll >= 0 && scroll <= 1)
            {
                obj->SetScroll(scroll);
            }

            scroll = obj->GetScroll();

            nestedAreas_.pop_back();

            return scroll;
        }

        void WindowManager::Spacer(float percentageX, float percentageY)
        {
            GetAreaForControl(GetValidId(), LayoutType::SPACER, percentageX, percentageY);
        }

        bool WindowManager::IsReady() const
        {
            return IsStable() && visible_;
        }

        bool WindowManager::IsStable() const
        {
            return isStable_;
        }

        void WindowManager::RecalculateLayout(LayoutArea* area)
        {
            if (!area->children_.empty())
            {
                auto it = area->children_.begin();
                float yPosition = 1; //y position to the top of current area
                float xPosition = -1; //x position to the left of current area

                while (it != area->children_.end())
                {
                    PLayoutArea pArea = *(it++);
                    float scaleY(1);
                    float scaleX(1);
                    Vertex3 position(0);

                    scaleY = pArea->percentageY_ / 100.0f;
                    scaleX = pArea->percentageX_ / 100.0f;

                    float stepY = scaleY;
                    float stepX = scaleX;

                    if (area->type_ == LayoutType::VERTICAL || area->type_ == LayoutType::WINDOW)
                    {
                        position.x = xPosition + stepX;
                        position.y = yPosition - stepY;
                        yPosition -= 2 * stepY;
                    }
                    else
                    {
                        CHECK_ASSERT(area->type_ == LayoutType::HORIZONTAL, __FILE__, __LINE__);
                        position.x = xPosition + stepX;
                        position.y = yPosition - stepY;
                        xPosition += 2 * stepX;
                    }

                    pArea->pNode_->SetPosition(position);
                    pArea->pNode_->SetScale(Vertex3(scaleX, scaleY, 1));
                }
            }

            area->CalculateScrollAreaFactor();
        }

        IdType WindowManager::GetValidId()
        {
            return lastId_++;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        LayoutManager::LayoutManager(PNode pRootNode)
            : focusedUserWindow_(App::this_),
              focusHasChanged_(false)
        {
            Window(App::this_, 100, 100, Context::this_->pSkin_->mainWindowStyle_); // Create main window
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

        void LayoutManager::MarkAllWindowsAsNonCreated()
        {
            for (const auto& kvp : windowManagers_)
                if (kvp.first != App::this_)
                    kvp.second->created_ = false;
        }

        void LayoutManager::RemoveAllNonCreatedWindows()
        {
            for (const auto& kvp : windowManagers_)
            {
                if (!kvp.second->created_)
                {
                    if (focusedUserWindow_ == kvp.first)
                        focusedUserWindow_ = App::this_;

                    windowsSequence_.erase(std::find(windowsSequence_.begin(), windowsSequence_.end(), kvp.first));
                    windowManagers_.erase(kvp.first);
                    RemoveAllNonCreatedWindows();
                    break;
                }
            }
        }


        void LayoutManager::Render()
        {
            MarkAllWindowsAsNonCreated(); // windows have to be removed if the user never calls them (IMGUIWindow)

            focusHasChanged_ = false;
            auto it = windowsSequence_.begin();
            while (it != windowsSequence_.end())
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

            RemoveAllNonCreatedWindows(); // windows have to be removed if the user never calls them (IMGUIWindow)

            if (AppStatistics::this_)
                AppStatistics::this_->Collect(true);
        }

        void LayoutManager::Window(IMGUI::IWindow* obj, float percentageX, float percentageY, PWindowStyle style)
        {
            auto it = windowManagers_.find(obj);
            if (it == windowManagers_.end())
            {
                PNode rootNode(new Node);

                IdType id = static_cast<IdType>(IdsTypes::IMGUI_MAIN_WINDOW_ID);

                if (!windowsSequence_.empty())
                    id = currentWindowManager_->GetValidId();

                PWindowManager windowManager(new WindowManager(obj, id, rootNode, percentageX, percentageY, style));
                auto result = windowManagers_.insert(WindowManagers::value_type(obj, windowManager));
                CHECK_ASSERT(result.second == true, __FILE__, __LINE__);
                windowsSequence_.push_back(obj);
            }
            else
            {
                it->second->created_ = true;
                it->second->style_ = style;
            }
        }

        void LayoutManager::SetWindowFocus(float x, float y)
        {
            auto it = windowsSequence_.rbegin();
            while (it != windowsSequence_.rend())
            {
                IWindow* userWindow = *(it++);
                PWindowManager windowManager = windowManagers_.find(userWindow)->second;
                auto itArea = windowManager->areas_.find({windowManager->id_, LayoutType::WINDOW});
                if (itArea != windowManager->areas_.end())
                {
                    PLayoutArea area = itArea->second;
                    if (area->IsInside(Vertex3(x, y, 0)))
                    {
                        focusedUserWindow_ = userWindow;
                        focusHasChanged_ = true;

                        if (userWindow != App::this_)   // check it is not the main window
                        {
                            windowsSequence_.erase(std::find(windowsSequence_.begin(), windowsSequence_.end(), userWindow));
                            windowsSequence_.push_back(userWindow); // move window to the end of the list (will be the last one rendered)
                        }

                        break;
                    }
                    CHECK_ASSERT(focusedUserWindow_ != nullptr, __FILE__, __LINE__);
                }
            }
        }

        PLayoutArea LayoutManager::GetAreaForControl(IdType id, LayoutType type, float percentageX, float percentageY)
        {
            return currentWindowManager_->GetAreaForControl(id, type, percentageX, percentageY);
        }

        void LayoutManager::BeginHorizontalArea(float percentageX, float percentageY, AreaStyle& style)
        {
            currentWindowManager_->BeginHorizontalArea(percentageX, percentageY, style);
        }

        void LayoutManager::BeginVerticalArea(float percentageX, float percentageY, AreaStyle& style)
        {
            currentWindowManager_->BeginVerticalArea(percentageX, percentageY, style);
        }

        float LayoutManager::EndArea(float scroll)
        {
            return currentWindowManager_->EndArea(scroll);
        }

        void LayoutManager::Spacer(float percentageX, float percentageY)
        {
            currentWindowManager_->Spacer(percentageX, percentageY);
        }

        size_t LayoutManager::GetNestingLevel()
        {
            return currentWindowManager_->GetNestingLevel();
        }

        bool LayoutManager::IsReady() const
        {
            return currentWindowManager_ && currentWindowManager_->IsReady();
        }

        IdType LayoutManager::GetValidId()
        {
            if (currentWindowManager_)
                return currentWindowManager_->GetValidId();

            CHECK_ASSERT(false, __FILE__, __LINE__);

            return 0;
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