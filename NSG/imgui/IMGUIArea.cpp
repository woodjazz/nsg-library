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
#include "IMGUILayoutManager.h"
#include "FrameColorSelection.h"
#include "TextMesh.h"
#include "SceneNode.h"
#include "Technique.h"
#include "Graphics.h"
#include "Pass.h"
#include "Material.h"
#include "Keys.h"

namespace NSG
{
	namespace IMGUI
	{
		Area::Area(GLushort id, LayoutType type, int percentageX, int percentageY)
		: Object(id, true, type, percentageX, percentageY)
		{
            CHECK_ASSERT(type == LayoutType::Horizontal || type == LayoutType::Vertical, __FILE__, __LINE__);
		}

		Area::~Area()
		{
		}

		void Area::operator()()
		{
			Update();
		}

		void Area::UpdateControl()
		{
			float scrollx = 0;//area_->percentageX_/100.0f;
			float scrolly = 0;//area_->percentageY_/100.0f;

            auto it = area_->children_.begin();
            while(it != area_->children_.end())
            {
                PLayoutArea child = *(it++);

                scrollx += child->percentageX_;
                scrolly += child->percentageY_;
            }

            scrollx /= 100.0f;
            scrolly /= 100.0f;

            MouseRelPosition relPos = uistate_.GetMouseRelPosition();

			if((scrollx > 1 || scrolly > 1) &&  Hit(Context::this_->state_->mouseDownX_, Context::this_->state_->mouseDownY_))
			{
                if(uistate_.lastScrollHit_ <= id_)
                    uistate_.lastScrollHit_ = id_;
                else
                    return;
                
                //Context::this_->pSkin_->areaTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0.5f,0.5f,0.5f,1));

				MouseRelPosition relPos = uistate_.GetMouseRelPosition();

				Vertex3 position = area_->childrenRoot_->GetPosition();
                Vertex3 scale = area_->pNode_->GetScale();

				//TRACE_LOG("scale=" << scale << "scrolly=" << scrolly << " position=" <<position);

				if(scrollx >= 1)
				{
					float x = position.x;
					x += relPos.x_;
					if(x < 0 && -x <= (scrollx-1)*2)
						position.x = x;
				}

				if(scrolly >= 1)
				{
					float y = position.y;
					y += relPos.y_;
					if(y > 0 && y <= (scrolly-1)*2)
						position.y = y;
				}

				area_->childrenRoot_->SetPosition(position);
			}
            else
            {
                if(uistate_.lastScrollHit_ == id_)
                    uistate_.lastScrollHit_ = -1;

                //Context::this_->pSkin_->areaTechnique_->GetPass(0)->GetMaterial()->SetColor(Color(0.5f,0.5f,0.5f,0));
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