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
#include "Keys.h"

namespace NSG
{
	namespace IMGUI
	{
		Object::Object(GLushort id, bool isReadOnly, LayoutType type, int percentageX, int percentageY)
		: id_(id),
		uistate_(*Context::this_->state_),
		area_(Context::this_->pLayoutManager_->GetAreaForControl(id, isReadOnly, type, percentageX, percentageY)),
        node_(area_->pNode_),
        areaSize_(Context::this_->pCamera_->GetModelViewProjection(node_.get()) * Vertex4(2,0,0,0))
		{
			CHECK_ASSERT(node_, __FILE__, __LINE__);
		}

		Object::~Object()
		{
#if 1
			size_t level = Context::this_->pLayoutManager_->GetNestingLevel();
			if(level > 0)
			{
	            if(currentTechnique_ && area_->type_ == LayoutType::Control)
	            {
				    size_t nPasses = currentTechnique_->GetNumPasses();
				    for(size_t i=0; i<nPasses; i++)
				    {
	            	    PMaterial material = currentTechnique_->GetPass(i)->GetMaterial();
	                    material->SetStencilOp(GL_DECR, GL_KEEP, GL_KEEP);
					    material->SetStencilFunc(GL_NEVER, 0, 0);
		            }
				    currentTechnique_->Set(node_);
				    currentTechnique_->Render();

				    for(size_t i=0; i<nPasses; i++)
				    {
	            	    PMaterial material = currentTechnique_->GetPass(i)->GetMaterial();
	                    material->SetStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		            }
	            }
	        }
#endif
            if(!area_->isReadOnly_)
            {
			    uistate_.lastwidget_ = id_;
            }
		}

		bool Object::IsStable() const
		{
			return Context::this_->pLayoutManager_->IsStable();
		}

		bool Object::Hit(float x, float y)
		{
			if(!Context::this_->pFrameColorSelection_)
				return false;

            if(currentTechnique_)
            {
            	currentTechnique_->Set(node_);
            	if(Context::this_->pFrameColorSelection_->Render(id_, x, y, currentTechnique_.get()))
            		return id_ == Context::this_->pFrameColorSelection_->GetSelected();
            }

            return false;
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

			size_t nPasses = currentTechnique_->GetNumPasses();
			for(size_t i=0; i<nPasses; i++)
			{
            	PMaterial material = currentTechnique_->GetPass(i)->GetMaterial();
            	size_t level = Context::this_->pLayoutManager_->GetNestingLevel();
                if(level > 0)
                {
                    material->SetStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
            	    material->SetStencilFunc(GL_EQUAL, level-1, ~GLuint(0));
                }
                else
                {
                    material->SetStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            	    material->SetStencilFunc(GL_NEVER, 0, 0);
                }


/*                if(area_->type_ == LayoutType::Control)
					material->SetStencilFunc(GL_LESS, 1, ~GLuint(0));
                else
	            	material->SetStencilFunc(GL_ALWAYS, 2, 0);*/
	        }
            
			currentTechnique_->Set(node_);
			currentTechnique_->Render();

            CHECK_GL_STATUS(__FILE__, __LINE__);
        }

		bool Object::HasFocus() const
		{
			return uistate_.kbditem_ == id_;
		}

		bool Object::IsActive() const
		{
			return uistate_.activeitem_ == id_;
		}

        void Object::FixCurrentTecnique()
        {
			if(uistate_.hotitem_ == id_)
			{
				if(uistate_.activeitem_ == id_ || uistate_.kbditem_ == id_) // Button is both 'hot' and 'active'
                    currentTechnique_ = GetActiveTechnique();
				else // Button is merely 'hot'
                    currentTechnique_ = GetHotTechnique();
			}
			else if(uistate_.activeitem_ == id_ || uistate_.kbditem_ == id_) // button is not hot, but it is active
				currentTechnique_ = GetActiveTechnique();
			else 
                currentTechnique_ = GetNormalTechnique();
        }

		bool Object::Update()
		{
			if(!IsStable())
				return false;

            if(!area_->isReadOnly_)			
			{
                FixCurrentTecnique();

				if (Hit(Context::this_->state_->mousex_, Context::this_->state_->mousey_)) // Check whether the button should be hot
				{
					uistate_.hotitem_ = id_;

					if (uistate_.activeitem_ == GLushort(-1) && uistate_.mousedown_)
					{
				  		uistate_.activeitem_ = id_;
				  		uistate_.kbditem_ = id_;
				  		uistate_.activeitem_needs_keyboard_ = NeedsKeyboard();
				  		OnActive();
				  	}
				}
				// If no widget has keyboard focus, take it
				if (uistate_.kbditem_ == 0)
				{
					uistate_.kbditem_ = id_;
					uistate_.activeitem_needs_keyboard_ = NeedsKeyboard();
					OnFocus();
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
                            uistate_.kbditem_ = uistate_.lastwidget_;
						}
                        else
                        {
						    // If tab is pressed, lose keyboard focus.
						    // Next widget will grab the focus.
						    uistate_.kbditem_ = 0;
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