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
		void BeginHorizontal_(GLushort id, int percentageX, int percentageY)
		{
			id = Context::this_->GetValidId(id);

			Context::this_->pLayoutManager_->BeginHorizontal(id, percentageX, percentageY);
		}

		void EndHorizontal_()
		{
			Context::this_->pLayoutManager_->EndHorizontal();
		}

		void BeginVertical_(GLushort id, int percentageX, int percentageY)
		{
			id = Context::this_->GetValidId(id);

			Context::this_->pLayoutManager_->BeginVertical(id, percentageX, percentageY);
		}

		void EndVertical_()
		{
			Context::this_->pLayoutManager_->EndVertical();
		}

		void Spacer_(GLushort id, int percentageX, int percentageY)
		{
			id = Context::this_->GetValidId(id);

			Context::this_->pLayoutManager_->Spacer(id, percentageX, percentageY);
		}

		bool Button_(GLushort id, const std::string& text, int percentageX, int percentageY)
		{
			id = Context::this_->GetValidId(id);

			Button obj(id, text, percentageX, percentageY);
			
			return obj();
		}		

		void Label_(GLushort id, const std::string& text, int percentageX, int percentageY)
		{
			id = Context::this_->GetValidId(id);

			Label obj(id, text, percentageX, percentageY);

			return obj();
		}

		std::string TextField_(GLushort id, const std::string& text, std::regex* pRegex, int percentageX, int percentageY)
		{	
			id = Context::this_->GetValidId(id);

			Text obj(id, text, pRegex, percentageX, percentageY);

			return obj();
		}	

		PSkin& Skin_()
		{
			return Context::this_->pSkin_;
		}

		PNode& Node_()
		{
			return Context::this_->pCurrentNode_;	
		}

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

