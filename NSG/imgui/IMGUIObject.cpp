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
#include "IMGUIObject.h"
#include "IMGUIContext.h"
#include "IMGUILayoutManager.h"
#include "IMGUIState.h"
#include "IMGUISkin.h"
#include "IMGUITextManager.h"
#include "Technique.h"
#include "Pass.h"
#include "FrameColorSelection.h"
#include "Material.h"
#include "Check.h"
#include "Camera.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Keys.h"
#include "App.h"

namespace NSG
{
	namespace IMGUI
	{
		static const Vertex3 AREA_SCREEN_SIZE(2,2,1); //default area size in screen coordinates

		Object::Object(GLushort id, LayoutType type, bool isWindow, int percentageX, int percentageY, bool keepAspectRatio)
		: id_(id),
		uistate_(*Context::this_->state_),
		skin_(*Context::this_->pSkin_),
		layoutManager_(*Context::this_->pLayoutManager_),
		currentWindowManager_(layoutManager_.GetCurrentWindowManager()),
		area_(layoutManager_.GetAreaForControl(id, type, percentageX, percentageY, keepAspectRatio)),
		lastTitleHit_(uistate_.lastTitleHit_),
		lastSizerHit_(uistate_.lastSizerHit_),
        node_(area_->pNode_),
        areaSize_(Context::this_->pCamera_->GetModelViewProjection(node_.get()) * Vertex4(AREA_SCREEN_SIZE, 0)),
		mouseDownX_(uistate_.mouseDownX_),
		mouseDownY_(uistate_.mouseDownY_),
		mousex_(uistate_.mousex_),
		mousey_(uistate_.mousey_),
		mousedown_(uistate_.mousedown_),
		mouseup_(uistate_.mouseup_),
		mouseRelX_(uistate_.mouseRelX_),
		mouseRelY_(uistate_.mouseRelY_),
		activeScrollArea_(uistate_.activeScrollArea_),
		level_(layoutManager_.GetNestingLevel()),
		type_(type),
		isWindow_(isWindow),
		drawn_(false),
		hotitem_(currentWindowManager_->hotitem_),
		activeitem_(currentWindowManager_->activeitem_),
		kbditem_(currentWindowManager_->kbditem_),
		lastwidget_(currentWindowManager_->lastwidget_),
		activeitem_needs_keyboard_(uistate_.activeitem_needs_keyboard_),
		viewSize_(App::this_->GetViewSize()),
		keepAspectRatio_(keepAspectRatio)
		{
			CHECK_ASSERT(node_, __FILE__, __LINE__);
		}

		Object::~Object()
		{
			if (drawn_ && !isWindow_)
			{
				CHECK_GL_STATUS(__FILE__, __LINE__);

				skin_.stencilTechnique_->GetPass(0)->SetStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
				skin_.stencilTechnique_->GetPass(0)->SetStencilFunc(GL_EQUAL, level_, ~GLuint(0));
				skin_.stencilTechnique_->Set(node_);
				skin_.stencilTechnique_->Render();

				CHECK_GL_STATUS(__FILE__, __LINE__);
			}
		}

		bool Object::IsReady() const
		{
			return Context::this_->pLayoutManager_->IsReady();
		}

		PTechnique Object::GetActiveTechnique() const
		{
			return Context::this_->pSkin_->activeTechnique_;
		}

		PTechnique Object::GetHotTechnique() const
		{
			return Context::this_->pSkin_->hotTechnique_;
		}

		PTechnique Object::GetNormalTechnique() const
		{
			return Context::this_->pSkin_->normalTechnique_;
		}

        void Object::Draw()
        {
            CHECK_GL_STATUS(__FILE__, __LINE__);

            // always level_ > 0 (stencil referenced value cannot be negative)
            // stencil's reference value is clamped to the range 0 2n-1 (so negative values become 0)
            // see material->SetStencilFunc(GL_EQUAL, level_-1, ~GLuint(0));
			CHECK_ASSERT(level_ > 0, __FILE__, __LINE__);

			skin_.stencilTechnique_->GetPass(0)->SetStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			skin_.stencilTechnique_->GetPass(0)->SetStencilFunc(GL_EQUAL, level_-1, ~GLuint(0));
			skin_.stencilTechnique_->Set(node_);
			skin_.stencilTechnique_->Render();

			size_t nPasses = currentTechnique_->GetNumPasses();
			for(size_t i=0; i<nPasses; i++)
			{
            	PPass pass = currentTechnique_->GetPass(i);
        	    pass->SetStencilFunc(GL_EQUAL, level_, ~GLuint(0));
	        }
            
			currentTechnique_->Render();

            CHECK_GL_STATUS(__FILE__, __LINE__);

            drawn_ = true;
        }

		bool Object::HasFocus() const
		{
			return kbditem_ == id_;
		}

		void Object::OnFocus(bool needsKeyboard)
		{
		}	

		void Object::OnActive()
		{
		}	

		void Object::OnHot()
		{
		}	

		bool Object::IsActive() const
		{
			return activeitem_ == id_;
		}

		bool Object::IsHot() const
		{
			return hotitem_ == id_;
		}

        void Object::FixCurrentTecnique()
        {
			if(activeitem_ == id_)// button is not hot, but it is active
				currentTechnique_ = GetActiveTechnique();
			else if(hotitem_ == id_)
				currentTechnique_ = GetHotTechnique();
			else 
                currentTechnique_ = GetNormalTechnique();

            currentTechnique_->Set(node_);
        }

        bool Object::IsMouseInArea() const
        {
        	return area_->IsInside(Vertex3(mousex_, mousey_, 0));
        }

        bool Object::IsMouseButtonPressedInArea() const
        {
        	return area_->IsInside(Vertex3(mouseDownX_, mouseDownY_, 0));
        }

		void Object::DisableKeyboard()
		{
			if(Keyboard::this_->Disable())
			{
				Context::this_->pCamera_->SetPosition(Vertex3(0,0,0));
			}
		}

		bool Object::Update()
		{
			if(!IsReady() || !area_->IsVisible())
				return false;

			if (layoutManager_.IsCurrentWindowActive())
			{
				if(!lastSizerHit_ && !lastTitleHit_ && IsMouseInArea()) // Check whether the button should be hot
				{
					OnHot();

					if (IsHot())
					{
						if (mousedown_ && activeitem_ == GLushort(-1) && IsMouseButtonPressedInArea())
						{
					  		OnActive();
					  	}
						else if (mouseup_ && IsMouseButtonPressedInArea())
						{
							OnFocus(true);
						}
					}
				}
				// If no widget has keyboard focus, take it
				if (kbditem_ == 0)
				{
					OnFocus(false);
				}

				// If we have keyboard focus, we'll need to process the keys
				if (HasFocus())
				{
					switch (uistate_.keyentered_)
					{
					case NSG_KEY_TAB:
						// If shift was also pressed, we want to move focus
						// to the previous widget instead.
						if (uistate_.keymod_ & NSG_KEY_MOD_SHIFT)
						{
                            kbditem_ = lastwidget_;
						}
                        else
                        {
						    // If tab is pressed, lose keyboard focus.
						    // Next widget will grab the focus.
						    kbditem_ = 0;
                        }
						// Also clear the key so that next widget
						// won't process it
						uistate_.keyentered_ = 0;
						break;

					case NSG_KEY_BACKSPACE:
						OnKey(NSG_KEY_BACKSPACE);
						break;   

	                case NSG_KEY_DELETE:
	                	OnKey(NSG_KEY_DELETE);
	                    break;

	                case NSG_KEY_RIGHT:
	                	OnKey(NSG_KEY_RIGHT);
	                    break;

	                case NSG_KEY_LEFT:
	                	OnKey(NSG_KEY_LEFT);
	                    break;

	                case NSG_KEY_HOME:
	                	OnKey(NSG_KEY_HOME);
	                    break;

	                case NSG_KEY_END:
						OnKey(NSG_KEY_END);
	                    break;

	                case NSG_KEY_ENTER:
	                	OnKey(NSG_KEY_ENTER);
						break;
					}

		            if (uistate_.character_ >= 32 && uistate_.character_ < 256)
		           		OnChar(uistate_.character_);
				}
			}

			FixCurrentTecnique();

            Draw();

            UpdateControl();

            return true;
		}
	}
}