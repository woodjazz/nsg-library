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
#include "IMGUIContext.h"
#include "IMGUISkin.h"
#include "Camera.h"
#include "Node.h"
#include "Keys.h"
#include "Keyboard.h"
#include "Material.h"
#include "IMGUILayoutManager.h"
#include "IMGUITextManager.h"
#include "FrameColorSelection.h"
#include "IMGUIState.h"

namespace NSG
{
	namespace IMGUI
	{
		Context::Context()
        : state_(new State),
        pSkin_(new Skin),
		pCamera_(new Camera),
		pCurrentNode_(new Node),
        pRootNode_(new Node),
        pLayoutManager_(new LayoutManager(pRootNode_, pCurrentNode_)),
    	pTextManager_(new TextManager),
    	pFrameColorSelection_(new FrameColorSelection(false)),
    	lastId_(0)
    	{
			pCamera_->EnableOrtho();
            pCamera_->SetFarClip(1000000);
            pCamera_->SetNearClip(-1000000);
		}

		Context::~Context()
		{
		}

		bool Context::IsStable() const
		{
			return pLayoutManager_->IsStable();
		}

		void Context::Begin()
		{
			lastId_ = 0;

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearStencil(0);
			glClear(GL_STENCIL_BUFFER_BIT);

			pCamera_->Activate();
			state_->hotitem_ = 0;

			pCurrentNode_ = Context::this_->pRootNode_;
			pLayoutManager_->Begin();
			pLayoutManager_->BeginVertical(0);
		}

		void Context::End()
		{
			pLayoutManager_->EndVertical();
			pLayoutManager_->End();
			
			if(!state_->activeitem_needs_keyboard_)
			{
				if(Keyboard::this_->Disable())
                {
					pCamera_->SetPosition(Vertex3(0,0,0));
				}
			}

			if(!state_->mousedown_)
			{
				state_->activeitem_ = GLushort(-1);
			}
			else
			{
				if(state_->activeitem_ == GLushort(-1))
				{
			  		state_->activeitem_needs_keyboard_ = false;
			  	}
			}

			// If no widget grabbed tab, clear focus
			if (!IsStable())
				state_->kbditem_ = 0;
            else if(state_->keyentered_ == NSG_KEY_TAB && !state_->keymod_)
                state_->kbditem_ = 0;
	
			// Clear the entered key
			state_->keyentered_ = 0;	
			state_->character_ = 0;	
		}

		PTextMesh Context::GetCurrentTextMesh(GLushort item)
		{
			return pTextManager_->GetTextMesh(item, pSkin_->fontFile_, pSkin_->fontSize_);
		}

		GLushort Context::GetValidId(GLushort id)
		{
			CHECK_ASSERT(id > 0, __FILE__, __LINE__);

			while(lastId_ >= id)
				++id;

			lastId_ = id;

			return id;
		}
	}
	
}