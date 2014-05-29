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
#include "IMGUI.h"
#include "IMGUIButton.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUILayoutManager.h"
#include "IMGUIState.h"
#include "IMGUIContext.h"

using namespace NSG;

namespace NSG 
{
	namespace IMGUI
	{
		void Begin()
		{
			Context::this_->Begin();
		}

		void End()
		{
			Context::this_->End();
		}

		bool IsStable()
		{
			return Context::this_->IsStable();
		}

		void InternalBeginHorizontal(GLushort id)
		{
			id = Context::this_->GetValidId(id);

			Context::this_->pLayoutManager_->BeginHorizontal(id);
		}

		void InternalEndHorizontal()
		{
			Context::this_->pLayoutManager_->EndHorizontal();
		}

		void InternalBeginVertical(GLushort id)
		{
			id = Context::this_->GetValidId(id);

			Context::this_->pLayoutManager_->BeginVertical(id);
		}

		void InternalEndVertical()
		{
			Context::this_->pLayoutManager_->EndVertical();
		}

		void InternalSpacer(GLushort id, int percentage)
		{
			id = Context::this_->GetValidId(id);

			Context::this_->pLayoutManager_->Spacer(id, percentage);
		}

		bool InternalButton(GLushort id, const std::string& text)
		{
			id = Context::this_->GetValidId(id);

			Button obj(id, text);
			
			return obj();
		}		

		void InternalLabel(GLushort id, const std::string& text)
		{
			id = Context::this_->GetValidId(id);

			Label obj(id, text);

			return obj();
		}

		std::string InternalTextField(GLushort id, const std::string& text, std::regex* pRegex)
		{	
			id = Context::this_->GetValidId(id);

			Text obj(id, text, pRegex);

			return obj();
		}		

        void OnMouseMove(float x, float y)
        {
            Context::this_->state_->OnMouseMove(x, y);
        }

        void OnMouseDown(float x, float y)
        {
            Context::this_->state_->OnMouseDown(x, y);
        }

        void OnMouseUp()
        {
            Context::this_->state_->OnMouseUp();
        }

        void OnKey(int key, int action, int modifier)
        {
        	Context::this_->state_->OnKey(key, action, modifier);
        }

        void OnChar(unsigned int character)
        {
        	Context::this_->state_->OnChar(character);
        }

        void DoTick()
        {
        	Context::this_->state_->DoTick();
        }
	}
}

