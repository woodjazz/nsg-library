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
#include "IMGUIWindow.h"
#include "IMGUI.h"
#include "IMGUIContext.h"
#include "IMGUISkin.h"
#include "IMGUIState.h"
#include "IMGUILayoutManager.h"
#include "FrameColorSelection.h"
#include "Node.h"
#include "Technique.h"
#include "Pass.h"
#include "App.h"
#include "Material.h"

namespace NSG
{
	namespace IMGUI
	{
		static const float TITLE_HEIGHT = 25; //pixels 

		Window::Window(GLushort id, bool showTitle, bool showBorder, int percentageX, int percentageY)
		: Area(id, LayoutType::Vertical, percentageX, percentageY),
		titleTechnique_(Context::this_->pSkin_->titleTechnique_),
		borderTechnique_(Context::this_->pSkin_->borderTechnique_),
		showTitle_(showTitle),
		showBorder_(showBorder),
		lastTitleHit_(Context::this_->state_->lastTitleHit_),
        viewSize_(App::this_->GetViewSize())
		{

		}

	    Window::~Window()
	    {

	    }

		float Window::GetTopPosition() const
		{
			if(showTitle_)
			{
	            Vertex3 windowScale = area_->pNode_->GetScale();
	            float yScale = TITLE_HEIGHT/((float)viewSize_.second*windowScale.y);
				return 1-(2*yScale);
			}
			else
			{
				return 1;
			}
		}

		bool Window::HitTitle(GLushort id, float screenX, float screenY)
		{
			if(area_->IsInside(Vertex3(screenX, screenY, 0)))
			{
		        PMaterial material = Context::this_->pFrameColorSelection_->GetMaterial();
		        material->SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		        material->SetStencilFunc(GL_ALWAYS, 0, 0);

			   	return Context::this_->pFrameColorSelection_->Hit(id, screenX, screenY, titleTechnique_.get());
			}

			return false;
		}


	    void Window::UpdateControl()
	    {
	    	if(mousedown_)
	    	{
	    		if(activeWindow_ < id_ && HitArea(id_, mouseDownX_, mouseDownY_))
					activeWindow_ = id_;
            }
	    	else if(activeWindow_ == id_)
	    	{
	    		activeWindow_ = IMGUI_UNKNOWN_ID;
	    	}

		    if(showTitle_)
		    {
				Node node;
				node.SetParent(area_->pNode_);
		        Vertex3 windowScale = area_->pNode_->GetScale();
		        float yScale = TITLE_HEIGHT/((float)viewSize_.second * windowScale.y);
				node.SetScale(Vertex3(1, yScale, 1));

		        float ypos = 1 - yScale;

				node.SetPosition(Vertex3(0,ypos,0));
				titleTechnique_->Set(&node);
				RenderTitle();

		    	if(mousedown_ && HitTitle(IMGUI_TITLE_ID, mouseDownX_, mouseDownY_) || lastTitleHit_ == IMGUI_TITLE_ID)
		    	{
		    		lastTitleHit_ = IMGUI_TITLE_ID;
		    		Vertex3 position = area_->pNode_->GetPosition();
		    		position.x += mouseRelX_;
		    		position.y += mouseRelY_;
		    		area_->pNode_->SetPosition(position);
		    	}
			}

			if(showBorder_)
			{
				borderTechnique_->Set(area_->pNode_);
				RenderBorder();
			}
	    }

		void Window::RenderTitle()
		{
			size_t nPasses = titleTechnique_->GetNumPasses();
			for(size_t i=0; i<nPasses; i++)
			{
            	PMaterial material = titleTechnique_->GetPass(i)->GetMaterial();
                material->SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        	    material->SetStencilFunc(GL_ALWAYS, 0, 0);
	        }

		    titleTechnique_->Render();
		}

		void Window::RenderBorder()
		{
			size_t nPasses = borderTechnique_->GetNumPasses();
			for(size_t i=0; i<nPasses; i++)
			{
            	PMaterial material = borderTechnique_->GetPass(i)->GetMaterial();
                material->SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        	    material->SetStencilFunc(GL_ALWAYS, 0, 0);
	        }

		    borderTechnique_->Render();
		}

		void Window::operator()()
		{
			Update();
		}

		PTechnique Window::GetActiveTechnique() const
		{
			return Context::this_->pSkin_->windowTechnique_;
		}

		PTechnique Window::GetHotTechnique() const
		{
			return Context::this_->pSkin_->windowTechnique_;
		}

		PTechnique Window::GetNormalTechnique() const
		{
			return Context::this_->pSkin_->windowTechnique_;
		}

	}
}
