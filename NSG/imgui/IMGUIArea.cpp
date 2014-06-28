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
#include "IMGUI.h"
#include "IMGUILayoutManager.h"
#include "FrameColorSelection.h"
#include "TextMesh.h"
#include "SceneNode.h"
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

		Area::Area(GLushort id, LayoutType type, int percentageX, int percentageY)
		: Object(id, true, type, percentageX, percentageY),
		lastSliderHit_(Context::this_->state_->lastSliderHit_),
		sliderTechnique_(Context::this_->pSkin_->sliderTechnique_)
		{
			CHECK_ASSERT(type == LayoutType::Horizontal || type == LayoutType::Vertical, __FILE__, __LINE__);

			area_->controlArea_ = this;

			maxPosX_ = 2*(area_->scrollFactorAreaX_-1);
			maxPosY_ = 2*(area_->scrollFactorAreaY_-1);
		}

		Area::~Area()
		{
            if(activeWindow_ <= id_)
			    UpdateScrolling();
		}

		float Area::GetTopPosition() const
		{
			return 1; //y position to the top of current area
		}

		float Area::GetLeftPosition() const
		{
			return -1; //x position to the left of current area
		}


		void Area::SetScroll(float scroll) 
		{ 
			CHECK_ASSERT(scroll >=0 && scroll <= 1, __FILE__, __LINE__);

			if(area_->type_ == LayoutType::Horizontal)
			{
				if(maxPosX_ > 0)
				{
					Vertex3 position = area_->childrenRoot_->GetPosition();
					position.x = scroll*maxPosX_;
					area_->childrenRoot_->SetPosition(position);
				}
			}
			else
			{
				if(maxPosY_ > 0)
				{
					Vertex3 position = area_->childrenRoot_->GetPosition();
					position.y = scroll*maxPosY_;
					area_->childrenRoot_->SetPosition(position);
				}
			}
		}

		float Area::GetScroll() const 
		{ 
			float scroll = 0;

			if(area_->type_ == LayoutType::Horizontal)
			{
				if(maxPosX_ > 0)
				{
					Vertex3 position = area_->childrenRoot_->GetPosition();
					scroll = position.x/maxPosX_;
				}
			}
			else
			{
				if(maxPosY_ > 0)
				{
					Vertex3 position = area_->childrenRoot_->GetPosition();
					scroll = position.y/maxPosY_;
				}
			}

			return scroll;
		}


		void Area::operator()()
		{
			Update();
		}

		void Area::RenderSlider()
		{
			size_t nPasses = sliderTechnique_->GetNumPasses();
			for(size_t i=0; i<nPasses; i++)
			{
            	PPass pass = sliderTechnique_->GetPass(i);
                pass->SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        	    pass->SetStencilFunc(GL_ALWAYS, 0, 0);
	        }

		    sliderTechnique_->Render();
		}

		bool Area::HitArea(GLushort id, float screenX, float screenY)
		{
			if(area_->IsInside(Vertex3(screenX, screenY, 0)))
			{
				FixCurrentTecnique();

				if(currentTechnique_)
				{
					PPass pass = Context::this_->pFrameColorSelection_->GetPass();
			        pass->SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			        pass->SetStencilFunc(GL_ALWAYS, 0, 0);

				   	return Context::this_->pFrameColorSelection_->Hit(id, screenX, screenY, currentTechnique_.get());
				}
			}

			return false;
		}

		bool Area::HitSlider(GLushort id, float screenX, float screenY)
		{
			if(area_->IsInside(Vertex3(screenX, screenY, 0)))
			{
		        PPass pass = Context::this_->pFrameColorSelection_->GetPass();
		        pass->SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		        pass->SetStencilFunc(GL_ALWAYS, 0, 0);

			   	return Context::this_->pFrameColorSelection_->Hit(id, screenX, screenY, sliderTechnique_.get());
			}

			return false;
		}

		bool Area::HandleVerticalSlider(float maxPosY, float& yPosition)
		{
	        // Draw slider
		    Node node;
		    node.SetParent(area_->pNode_);

		    float yScale = 0.5f/area_->scrollFactorAreaY_;
            std::pair<int32_t, int32_t> viewSize = App::this_->GetViewSize();
            float xScale = SLIDER_WIDTH/(float)viewSize.first;
		    node.SetScale(Vertex3(xScale, yScale, 1));

	        float offset = yPosition/maxPosY; //0..1
            float blind_area = SLIDER_WIDTH/(float)viewSize.second;;
            float ypos = 1 - yScale - 2*(1 - yScale - blind_area)*offset;
            Vertex3 globalScale = node.GetGlobalScale();
            float xpos = 1 - globalScale.x;
	        node.SetPosition(Vertex3(xpos, ypos, 0));

		    sliderTechnique_->Set(&node);

		    RenderSlider();

		    if(mousedown_)
		    {
		    	Vertex3 position = node.GetPosition();
		    	Vertex3 scale = node.GetScale();
		    	position.y = 0;
		    	scale.y=1;
		    	// In order to hit all the slider area: reset position and scale 
		    	node.SetPosition(position);
		    	node.SetScale(scale);

		    	if(HitSlider(IMGUI_VERTICAL_SLIDER_ID, mouseDownX_, mouseDownY_) || lastSliderHit_ == IMGUI_VERTICAL_SLIDER_ID)
		    	{
		    		lastSliderHit_ = IMGUI_VERTICAL_SLIDER_ID;

	            	Vertex3 globalPos = area_->pNode_->GetGlobalPosition();
	                Vertex3 globalScale = area_->pNode_->GetGlobalScale();
	                float yTop = globalPos.y + globalScale.y;
	            	float yBottom = globalPos.y - globalScale.y;

	                float y1 = mousey_;
	                if(y1 <= yTop && y1 >= yBottom)
	                {
		                float a2 = 1/(yBottom-yTop);
		                float y2 = a2*y1-a2*yTop;
		                CHECK_ASSERT(y2 >=0 && y2 <= 1, __FILE__, __LINE__);
						yPosition = y2 * maxPosY;
		            }
                    return true;
		    	}
		    }

		    return false;
		}

		bool Area::HandleHorizontalSlider(float maxPosX, float& xPosition)
		{
	        // Draw slider
		    Node node;
		    node.SetParent(area_->pNode_);

            std::pair<int32_t, int32_t> viewSize = App::this_->GetViewSize();
            float yScale = SLIDER_WIDTH/(float)viewSize.second;
		    float xScale = 0.5f/area_->scrollFactorAreaX_;
		    node.SetScale(Vertex3(xScale, yScale, 1));

	        float offset = -xPosition/maxPosX; //0..1
            float blind_area = SLIDER_WIDTH/(float)viewSize.first;
            float xpos = -1 + xScale + 2*(1 - xScale - blind_area)*offset;
            Vertex3 globalScale = node.GetGlobalScale();
            float ypos = -1 + globalScale.y;
	        node.SetPosition(Vertex3(xpos, ypos, 0));

		    sliderTechnique_->Set(&node);

		    RenderSlider();

		    if(mousedown_)
		    {
		    	Vertex3 position = node.GetPosition();
		    	Vertex3 scale = node.GetScale();
		    	position.x = 0;
		    	scale.x=1;
		    	// In order to hit all the slider area: reset position and scale 
		    	node.SetPosition(position);
		    	node.SetScale(scale);

		    	if(HitSlider(IMGUI_HORIZONTAL_SLIDER_ID, mouseDownX_, mouseDownY_) || lastSliderHit_ == IMGUI_HORIZONTAL_SLIDER_ID)
		    	{
		    		lastSliderHit_ = IMGUI_HORIZONTAL_SLIDER_ID;

	                Vertex3 globalPos = area_->pNode_->GetGlobalPosition();
                    Vertex3 globalScale = area_->pNode_->GetGlobalScale();
	                float xLeft = globalPos.x - globalScale.x;
	                float xRight = globalPos.x + globalScale.x;

                    float x1 = mousex_;
                    if(x1 >= xLeft && x1 <= xRight)
                    {
	                    float a2 = 1/(xRight-xLeft);
	                    float x2 = a2*x1-a2*xLeft;
	                    CHECK_ASSERT(x2 >=0 && x2 <= 1, __FILE__, __LINE__);
					    xPosition = -x2 * maxPosX;
                    }
                    return true;
		    	}
		    }

		    return false;
		}

	    void Area::UpdateControl()
	    {
	    	if(mousedown_)
	    	{
	    		if(activeScrollArea_ < id_ && area_->isScrollable_ && HitArea(id_, mouseDownX_, mouseDownY_))
					activeScrollArea_ = id_;
            }
	    }

		void Area::UpdateScrolling()
		{
            if(activeScrollArea_ <= id_ && HitArea(id_, mouseDownX_, mouseDownY_))
			{
                MouseRelPosition relPos = uistate_.GetMouseRelPosition();

				Vertex3 position = area_->childrenRoot_->GetPosition();

				bool userMovedSlider = false;

                float maxPosX = 2*(area_->scrollFactorAreaX_-1);
                float maxPosY = 2*(area_->scrollFactorAreaY_-1);
                
				if(area_->scrollFactorAreaX_ > 1)
                    userMovedSlider = HandleHorizontalSlider(maxPosX, position.x);

                if(area_->scrollFactorAreaY_ > 1 && !userMovedSlider)
                    userMovedSlider = HandleVerticalSlider(maxPosY, position.y);

				if(!userMovedSlider)
				{
					if(area_->scrollFactorAreaX_ > 1)
					{
                        if(relPos.x_ != 0)
                        {
                    	    float x = position.x;

                            float step = relPos.x_ * maxPosX / std::floor(area_->scrollFactorAreaX_);

                            x += step;

                            position.x = std::min<float>(0, std::max<float>(x, -maxPosX));
                        }
					}

					if(area_->scrollFactorAreaY_ > 1)
					{
                        if(relPos.y_ != 0)
                        {
                    	    float y = position.y;

                            float step = relPos.y_ * maxPosY / std::floor(area_->scrollFactorAreaY_);

                            y += step;

                            position.y = std::max<float>(0, std::min<float>(y, maxPosY));
                        }
					}
				}

				area_->childrenRoot_->SetPosition(position);
			}
		}	

		PTechnique Area::GetActiveTechnique() const
		{
			return Context::this_->pSkin_->areaTechnique_;
		}

		PTechnique Area::GetHotTechnique() const
		{
			return Context::this_->pSkin_->areaTechnique_;
		}

		PTechnique Area::GetNormalTechnique() const
		{
			return Context::this_->pSkin_->areaTechnique_;
		}

	}
}