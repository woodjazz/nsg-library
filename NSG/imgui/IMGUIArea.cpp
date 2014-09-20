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
#include "IMGUIArea.h"
#include "IMGUIContext.h"
#include "IMGUIState.h"
#include "IMGUISkin.h"
#include "IMGUIStyle.h"
#include "IMGUI.h"
#include "IMGUILayoutManager.h"
#include "FrameColorSelection.h"
#include "TextMesh.h"
#include "Node.h"
#include "Technique.h"
#include "Graphics.h"
#include "Pass.h"
#include "Material.h"
#include "Keys.h"
#include "Node.h"
#include "App.h"
#include <algorithm>

namespace NSG
{
    namespace IMGUI
    {
        static const float SLIDER_WIDTH = 15; //pixels

        Area::Area(LayoutType type, float percentageX, float percentageY, AreaStyle& style)
            : Object(type, percentageX, percentageY, style),
              areaStyle_(style)
        {
            CHECK_ASSERT(type == LayoutType::WINDOW || type == LayoutType::HORIZONTAL || type == LayoutType::VERTICAL, __FILE__, __LINE__);

            maxPosX_ = 2 * (area_->scrollFactorAreaX_ - 1);
            maxPosY_ = 2 * (area_->scrollFactorAreaY_ - 1);
        }

        Area::~Area()
        {
            if (IsReady() && layoutManager_.IsCurrentWindowActive())
                UpdateScrolling();
        }

        void Area::SetScroll(float scroll)
        {
            CHECK_ASSERT(scroll >= 0 && scroll <= 1, __FILE__, __LINE__);

            if (area_->type_ == LayoutType::HORIZONTAL)
            {
                if (maxPosX_ > 0)
                {
                    Vertex3 position = area_->childrenRoot_->GetPosition();
                    position.x = scroll * maxPosX_;
                    area_->childrenRoot_->SetPosition(position);
                }
            }
            else
            {
                if (maxPosY_ > 0)
                {
                    Vertex3 position = area_->childrenRoot_->GetPosition();
                    position.y = scroll * maxPosY_;
                    area_->childrenRoot_->SetPosition(position);
                }
            }
        }

        float Area::GetScroll() const
        {
            float scroll = 0;

            if (area_->type_ == LayoutType::HORIZONTAL)
            {
                if (maxPosX_ > 0)
                {
                    Vertex3 position = area_->childrenRoot_->GetPosition();
                    scroll = position.x / maxPosX_;
                }
            }
            else
            {
                if (maxPosY_ > 0)
                {
                    Vertex3 position = area_->childrenRoot_->GetPosition();
                    scroll = position.y / maxPosY_;
                }
            }

            return scroll;
        }


        void Area::Render()
        {
            Update();
        }

        void Area::RenderSlider()
        {
            Graphics::this_->Set(Context::this_->controlMesh_.get());
            Graphics::this_->Set(areaStyle_.scrollMaterial_.get());
            PPass pass = areaStyle_.scrollPass_;
            pass->SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            pass->SetStencilFunc(GL_ALWAYS, 0, 0);
            pass->Render();
            Graphics::this_->Set(Context::this_->controlMesh_.get());
        }

        bool Area::HandleVerticalSlider(float maxPosY, float& yPosition)
        {
            // Draw slider
            Vertex3 areaGlobalScale = area_->pNode_->GetGlobalScale();

			Node& node(*area_->controlNodes_.node0_);
            node.SetParent(area_->pNode_);
            node.SetInheritScale(false);

            float yScale = 0.5f / area_->scrollFactorAreaY_;
            std::pair<int, int> viewSize = App::this_->GetViewSize();
            float xScale = SLIDER_WIDTH / (float)viewSize.first;

            Vertex3 globalScale(xScale, areaGlobalScale.y * yScale, 1);
            node.SetScale(globalScale);

            float offset = yPosition / maxPosY; //0..1
            float blind_area = (area_->isXScrollable_ && areaStyle_.showHScroll_) ? 2 * SLIDER_WIDTH / (float)viewSize.second : 0;
            const float TOP_POS = 1;
            float ypos = TOP_POS - yScale - 2 * (1 - yScale - blind_area) * offset;

            node.SetPosition(Vertex3(0, ypos, 0));

            Vertex3 globalPosition = node.GetGlobalPosition();
            globalPosition.x += areaGlobalScale.x - globalScale.x;
            node.SetGlobalPosition(globalPosition);

            Graphics::this_->SetNode(&node);

            RenderSlider();

            if (mousedown_)
            {
                Vertex3 position = node.GetPosition();
                Vertex3 scale = node.GetScale();
                position.y = 0;
                scale.y = 1;
                // In order to hit all the slider area: reset position and scale
                node.SetPosition(position);
                node.SetScale(scale);

                if ((!lastHit_ && node.IsPointInsideBB(Vertex3(mouseDownX_, mouseDownY_, 0))) || lastHit_ == static_cast<IdType>(IdsTypes::IMGUI_VERTICAL_SLIDER_ID))
                {
                    lastHit_ = static_cast<IdType>(IdsTypes::IMGUI_VERTICAL_SLIDER_ID);

                    Vertex3 globalPos = area_->pNode_->GetGlobalPosition();
                    Vertex3 globalScale = area_->pNode_->GetGlobalScale();
                    float yTop = globalPos.y + globalScale.y;
                    float yBottom = globalPos.y - globalScale.y;

                    float y1 = mousey_;
                    if (y1 <= yTop && y1 >= yBottom)
                    {
                        float a2 = 1 / (yBottom - yTop);
                        float y2 = a2 * y1 - a2 * yTop;
                        CHECK_ASSERT(y2 >= 0 && y2 <= 1, __FILE__, __LINE__);
                        yPosition = y2 * maxPosY;
                    }
                    return true;
                }
            }

            return false;
        }

        bool Area::HandleHorizontalSlider(float maxPosX, float& xPosition)
        {
            Vertex3 areaGlobalScale = area_->pNode_->GetGlobalScale();
            // Draw slider
            Node& node(*area_->controlNodes_.node0_);
            node.SetParent(area_->pNode_);
            node.SetInheritScale(false);

            std::pair<int, int> viewSize = App::this_->GetViewSize();
            float yScale = SLIDER_WIDTH / (float)viewSize.second;
            float xScale = 0.5f / area_->scrollFactorAreaX_;

            Vertex3 globalScale(areaGlobalScale.x * xScale, yScale, 1);
            node.SetScale(globalScale);

            float offset = -xPosition / maxPosX; //0..1
            float blind_area = (area_->isYScrollable_ && areaStyle_.showVScroll_) ? 2 * SLIDER_WIDTH / (float)viewSize.first : 0;
            const float LEFT_POS = -1;
            float xpos = LEFT_POS + xScale + 2 * (1 - xScale - blind_area) * offset;

            node.SetPosition(Vertex3(xpos, 0, 0));

            Vertex3 globalPosition = node.GetGlobalPosition();
            globalPosition.y -= areaGlobalScale.y - globalScale.y;
            node.SetGlobalPosition(globalPosition);

            Graphics::this_->SetNode(&node);

            RenderSlider();

            if (mousedown_)
            {
                Vertex3 position = node.GetPosition();
                Vertex3 scale = node.GetScale();
                position.x = 0;
                scale.x = 1;
                // In order to hit all the slider area: reset position and scale
                node.SetPosition(position);
                node.SetScale(scale);

                if ((!lastHit_ && node.IsPointInsideBB(Vertex3(mouseDownX_, mouseDownY_, 0))) || lastHit_ == static_cast<IdType>(IdsTypes::IMGUI_HORIZONTAL_SLIDER_ID))
                {
                    lastHit_ = static_cast<IdType>(IdsTypes::IMGUI_HORIZONTAL_SLIDER_ID);

                    Vertex3 globalPos = area_->pNode_->GetGlobalPosition();
                    Vertex3 globalScale = area_->pNode_->GetGlobalScale();
                    float xLeft = globalPos.x - globalScale.x;
                    float xRight = globalPos.x + globalScale.x;

                    float x1 = mousex_;
                    if (x1 >= xLeft && x1 <= xRight)
                    {
                        float a2 = 1 / (xRight - xLeft);
                        float x2 = a2 * x1 - a2 * xLeft;
                        CHECK_ASSERT(x2 >= 0 && x2 <= 1, __FILE__, __LINE__);
                        xPosition = -x2 * maxPosX;
                    }
                    return true;

                }
            }

            return false;
        }

        void Area::UpdateControl()
        {
            if (layoutManager_.IsCurrentWindowActive() && mousedown_)
            {
                if (area_->isScrollable_ && IsMouseButtonPressedInArea())
                    activeScrollArea_ = id_;
            }
        }

        void Area::UpdateScrolling()
        {
            if (activeScrollArea_ == id_ && (!lastHit_ || lastHit_ == static_cast<IdType>(IdsTypes::IMGUI_VERTICAL_SLIDER_ID) || lastHit_ == static_cast<IdType>(IdsTypes::IMGUI_HORIZONTAL_SLIDER_ID)))
            {
                Vertex3 position = area_->childrenRoot_->GetPosition();

                bool userMovedSlider = false;

                float maxPosX = 2 * (area_->scrollFactorAreaX_ - 1);
                float maxPosY = 2 * (area_->scrollFactorAreaY_ - 1);

                if (area_->isXScrollable_ && areaStyle_.showHScroll_)
                    userMovedSlider = HandleHorizontalSlider(maxPosX, position.x);

                if (area_->isYScrollable_ && areaStyle_.showVScroll_)
                    userMovedSlider = HandleVerticalSlider(maxPosY, position.y) || userMovedSlider;

                bool scrolled = false;
                if (!userMovedSlider)
                {
                    if (area_->isXScrollable_)
                    {
                        if (mouseRelX_ != 0)
                        {
                            float x = position.x;

                            float step = mouseRelX_ / area_->scrollFactorAreaX_;

                            x += step;

                            position.x = std::min<float>(0, std::max<float>(x, -maxPosX));

                            scrolled = true;
                        }
                    }

                    if (area_->isYScrollable_)
                    {
                        if (mouseRelY_ != 0)
                        {
                            float y = position.y;

                            float step = mouseRelY_ * area_->scrollFactorAreaY_;

                            y += step;

                            position.y = std::max<float>(0, std::min<float>(y, maxPosY));

                            scrolled = true;
                        }
                    }
                }
                else
                {
                    scrolled = true;
                }

                if (scrolled)
                    area_->childrenRoot_->SetPosition(position);
            }
        }
    }
}