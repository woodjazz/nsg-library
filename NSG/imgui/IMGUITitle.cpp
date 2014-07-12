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
#include "IMGUITitle.h"
#include "IMGUIContext.h"
#include "IMGUISkin.h"
#include "TextMesh.h"
#include "IMGUILayoutManager.h"

namespace NSG
{
	namespace IMGUI
	{
		Title::Title(GLushort id, const std::string& text, int maxLength, int percentageX, int percentageY, bool keepAspectRatio)
			: Label(id, text, maxLength, percentageX, percentageY, keepAspectRatio)
		{
		}

		Title::~Title()
		{
		}

		PTechnique Title::GetNormalTechnique() const
		{
			return skin_.titleTechnique_;
		}

		void Title::Render()
		{
			Update();
		}		

	    void Title::UpdateControl()
	    {
	    	Label::UpdateControl();
	    	
			if(layoutManager_.IsCurrentWindowActive() && mousedown_ && !lastSizerHit_)
			{
		    	if((!lastTitleHit_ && node_->IsPointInsideBB(Vertex3(mouseDownX_, mouseDownY_, 0))) || lastTitleHit_ == id_)
		    	{
		    		lastTitleHit_ = id_;
		    		PNode windowNode = layoutManager_.GetCurrentWindowNode();
					Vertex3 position = windowNode->GetPosition();
		    		position.x += mouseRelX_;
		    		position.y += mouseRelY_;
					windowNode->SetPosition(position);
		    	}
		    }
	    	
	    }

	}
}