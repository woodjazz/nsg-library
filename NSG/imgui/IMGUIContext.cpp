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
#include "Graphics.h"
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
    	pFrameColorSelection_(new FrameColorSelection(false, false)),
    	lastId_(0)
    	{
			pCamera_->EnableOrtho();
            pCamera_->SetFarClip(1000000);
            pCamera_->SetNearClip(-1000000);
            pFrameColorSelection_->GetMaterial()->EnableStencilTest(true);
            pFrameColorSelection_->GetMaterial()->EnableDepthTest(false);
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
			state_->Begin();

			lastId_ = 0;

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			ClearStencilBuffer();

			pCamera_->Activate();

			pCurrentNode_ = Context::this_->pRootNode_;
			pLayoutManager_->Begin();
		}

		void Context::End()
		{
			pLayoutManager_->End();

			state_->End();
		}

		PTextMesh Context::GetCurrentTextMesh(GLushort item)
		{
			return pTextManager_->GetTextMesh(item, pSkin_->fontFile_, pSkin_->fontSize_);
		}

		GLushort Context::GetValidId(GLushort id)
		{
			CHECK_ASSERT(id > 0, __FILE__, __LINE__);

			if(lastId_ >= id)
				id = ++lastId_;
			else
				lastId_ = id;

			return id;
		}
	}
	
}