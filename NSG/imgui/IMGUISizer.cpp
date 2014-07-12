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
#include "IMGUISizer.h"
#include "IMGUISkin.h"
#include "IMGUILayoutManager.h"

namespace NSG
{
	namespace IMGUI
	{
		Sizer::Sizer(GLushort id, SizerType type, int percentageX, int percentageY, bool keepAspectRatio)
		: Object(id, LayoutType::CONTROL, false, percentageX, percentageY, keepAspectRatio),
		type_(type)
		{
			switch(type_)
			{
				case LEFT_TOP_SIZER:
					normalTechnique_ = skin_.sizerLeftTopTechnique_;
					break;

            	case TOP_SIZER:
            		normalTechnique_ = skin_.sizerTopTechnique_;
            		break;
            
            	case RIGHT_TOP_SIZER:
            		normalTechnique_ = skin_.sizerRightTopTechnique_;
            		break;

            	case LEFT_SIZER:
            		normalTechnique_ = skin_.sizerLeftTechnique_;
            		break;

            	case RIGHT_SIZER:
            		normalTechnique_ = skin_.sizerRightTechnique_;
            		break;

            	case LEFT_BOTTOM_SIZER:
            		normalTechnique_ = skin_.sizerLeftBottomTechnique_;
            		break;
            
            	case BOTTOM_SIZER:
            		normalTechnique_ = skin_.sizerBottomTechnique_;
            		break;

            	case RIGHT_BOTTOM_SIZER:
            		normalTechnique_ = skin_.sizerRightBottomTechnique_;
            		break;

            	default:
            		CHECK_ASSERT(false && "Unkonwn sizer type", __FILE__, __LINE__);
            		break;
			}

		}

		Sizer::~Sizer()
		{

		}

		void Sizer::Render()
		{
			Update();
		}

		void Sizer::UpdateControl()
		{
			
			if(layoutManager_.IsCurrentWindowActive() && mousedown_ && !lastTitleHit_)
			{
				if ((!lastSizerHit_ && node_->IsPointInsideBB(Vertex3(mouseDownX_, mouseDownY_, 0))) || lastSizerHit_ == id_)
		    	{
		    		lastSizerHit_ = id_;
		    		PNode windowNode = layoutManager_.GetCurrentWindowNode();
					Vertex3 globalScale0 = windowNode->GetGlobalScale();

					switch (type_)
					{
					case LEFT_TOP_SIZER:
					{
						Vertex3 scaleOffset = Vertex3(-mouseRelX_, mouseRelY_, 0);
						Vertex3 scale = windowNode->GetScale();
						windowNode->SetScale(scale + 0.5f * scaleOffset);
						Vertex3 globalPosition = windowNode->GetGlobalPosition();
						Vertex3 globalScale1 = windowNode->GetGlobalScale();
						Vertex3 offset = globalScale1 - globalScale0;
						windowNode->SetGlobalPosition(globalPosition + Vertex3(-offset.x, offset.y, 0));
						break;
					}

					case TOP_SIZER:
					{
						Vertex3 scaleOffset = Vertex3(0, mouseRelY_, 0);
						Vertex3 scale = windowNode->GetScale();
						windowNode->SetScale(scale + 0.5f * scaleOffset);
						Vertex3 globalPosition = windowNode->GetGlobalPosition();
						Vertex3 globalScale1 = windowNode->GetGlobalScale();
						Vertex3 offset = globalScale1 - globalScale0;
						windowNode->SetGlobalPosition(globalPosition + offset);
						break;
					}

					case RIGHT_TOP_SIZER:
					{
						Vertex3 scaleOffset = Vertex3(mouseRelX_, mouseRelY_, 0);
						Vertex3 scale = windowNode->GetScale();
						windowNode->SetScale(scale + 0.5f * scaleOffset);
						Vertex3 globalPosition = windowNode->GetGlobalPosition();
						Vertex3 globalScale1 = windowNode->GetGlobalScale();
						Vertex3 offset = globalScale1 - globalScale0;
						windowNode->SetGlobalPosition(globalPosition + offset);
						break;
					}

					case LEFT_SIZER:
					{
						Vertex3 scaleOffset = Vertex3(-mouseRelX_, 0, 0);
						Vertex3 scale = windowNode->GetScale();
						windowNode->SetScale(scale + 0.5f * scaleOffset);
						Vertex3 globalPosition = windowNode->GetGlobalPosition();
						Vertex3 globalScale1 = windowNode->GetGlobalScale();
						Vertex3 offset = globalScale1 - globalScale0;
						windowNode->SetGlobalPosition(globalPosition - offset);
						break;
					}

					case RIGHT_SIZER:
					{
						Vertex3 scaleOffset = Vertex3(mouseRelX_, 0, 0);
						Vertex3 scale = windowNode->GetScale();
						windowNode->SetScale(scale + 0.5f * scaleOffset);
						Vertex3 globalPosition = windowNode->GetGlobalPosition();
						Vertex3 globalScale1 = windowNode->GetGlobalScale();
						Vertex3 offset = globalScale1 - globalScale0;
						windowNode->SetGlobalPosition(globalPosition + offset);
						break;
					}

					case LEFT_BOTTOM_SIZER:
					{
						Vertex3 scaleOffset = Vertex3(-mouseRelX_, -mouseRelY_, 0);
						Vertex3 scale = windowNode->GetScale();
						windowNode->SetScale(scale + 0.5f * scaleOffset);
						Vertex3 globalPosition = windowNode->GetGlobalPosition();
						Vertex3 globalScale1 = windowNode->GetGlobalScale();
						Vertex3 offset = globalScale1 - globalScale0;
						windowNode->SetGlobalPosition(globalPosition - offset);
						break;
					}

					case BOTTOM_SIZER:
					{
						Vertex3 scaleOffset = Vertex3(0, -mouseRelY_, 0);
						Vertex3 scale = windowNode->GetScale();
						windowNode->SetScale(scale + 0.5f * scaleOffset);
						Vertex3 globalPosition = windowNode->GetGlobalPosition();
						Vertex3 globalScale1 = windowNode->GetGlobalScale();
						Vertex3 offset = globalScale1 - globalScale0;
						windowNode->SetGlobalPosition(globalPosition - offset);
						break;
					}

					case RIGHT_BOTTOM_SIZER:
					{
						Vertex3 scaleOffset = Vertex3(mouseRelX_, -mouseRelY_, 0);
						Vertex3 scale = windowNode->GetScale();
						windowNode->SetScale(scale + 0.5f * scaleOffset);
						Vertex3 globalPosition = windowNode->GetGlobalPosition();
						Vertex3 globalScale1 = windowNode->GetGlobalScale();
						Vertex3 offset = globalScale1 - globalScale0;
						windowNode->SetGlobalPosition(globalPosition + Vertex3(offset.x, -offset.y, 0));
						break;
					}

					default:
						CHECK_ASSERT(false && "Unkonwn sizer type", __FILE__, __LINE__);
						break;
					}

					


		    	}
		    }

		}

		PTechnique Sizer::GetNormalTechnique() const
		{
			return normalTechnique_;
		}
	}
}