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
#include "App.h"

namespace NSG
{
    namespace IMGUI
    {
        Sizer::Sizer(SizerType type, float percentageX, float percentageY, Style& style)
            : Object(LayoutType::CONTROL, percentageX, percentageY, style),
              type_(type)
        {
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

            if (layoutManager_.IsCurrentWindowActive() && mousedown_)
            {
                if ((!lastHit_ && node_->IsPointInsideBB(Vertex3(mouseDownX_, mouseDownY_, 0))) || lastHit_ == id_)
                {
                    lastHit_ = id_;
                    PNode windowNode = layoutManager_.GetCurrentWindowNode();
                    Vertex3 globalScale0 = windowNode->GetGlobalScale();
                    Vertex3 scale0 = windowNode->GetScale();
                    Vertex3 globalPosition0 = windowNode->GetGlobalPosition();

                    switch (type_)
                    {
                    case SizerType::LEFT_TOP_SIZER:
                    {
                        Vertex3 scaleOffset = Vertex3(-mouseRelX_, mouseRelY_, 0);
                        Vertex3 scale = windowNode->GetScale();
                        windowNode->SetScale(scale + 0.5f * scaleOffset);
                        Vertex3 globalScale1 = windowNode->GetGlobalScale();
                        Vertex3 offset = globalScale1 - globalScale0;
                        windowNode->SetGlobalPosition(globalPosition0 + Vertex3(-offset.x, offset.y, 0));
                        break;
                    }

                    case SizerType::TOP_SIZER:
                    {
                        Vertex3 scaleOffset = Vertex3(0, mouseRelY_, 0);
                        Vertex3 scale = windowNode->GetScale();
                        windowNode->SetScale(scale + 0.5f * scaleOffset);
                        Vertex3 globalScale1 = windowNode->GetGlobalScale();
                        Vertex3 offset = globalScale1 - globalScale0;
                        windowNode->SetGlobalPosition(globalPosition0 + offset);
                        break;
                    }

                    case SizerType::RIGHT_TOP_SIZER:
                    {
                        Vertex3 scaleOffset = Vertex3(mouseRelX_, mouseRelY_, 0);
                        Vertex3 scale = windowNode->GetScale();
                        windowNode->SetScale(scale + 0.5f * scaleOffset);
                        Vertex3 globalScale1 = windowNode->GetGlobalScale();
                        Vertex3 offset = globalScale1 - globalScale0;
                        windowNode->SetGlobalPosition(globalPosition0 + offset);
                        break;
                    }

                    case SizerType::LEFT_SIZER:
                    {
                        Vertex3 scaleOffset = Vertex3(-mouseRelX_, 0, 0);
                        Vertex3 scale = windowNode->GetScale();
                        windowNode->SetScale(scale + 0.5f * scaleOffset);
                        Vertex3 globalScale1 = windowNode->GetGlobalScale();
                        Vertex3 offset = globalScale1 - globalScale0;
                        windowNode->SetGlobalPosition(globalPosition0 - offset);
                        break;
                    }

                    case SizerType::RIGHT_SIZER:
                    {
                        Vertex3 scaleOffset = Vertex3(mouseRelX_, 0, 0);
                        Vertex3 scale = windowNode->GetScale();
                        windowNode->SetScale(scale + 0.5f * scaleOffset);
                        Vertex3 globalScale1 = windowNode->GetGlobalScale();
                        Vertex3 offset = globalScale1 - globalScale0;
                        windowNode->SetGlobalPosition(globalPosition0 + offset);
                        break;
                    }

                    case SizerType::LEFT_BOTTOM_SIZER:
                    {
                        Vertex3 scaleOffset = Vertex3(-mouseRelX_, -mouseRelY_, 0);
                        Vertex3 scale = windowNode->GetScale();
                        windowNode->SetScale(scale + 0.5f * scaleOffset);
                        Vertex3 globalScale1 = windowNode->GetGlobalScale();
                        Vertex3 offset = globalScale1 - globalScale0;
                        windowNode->SetGlobalPosition(globalPosition0 - offset);
                        break;
                    }

                    case SizerType::BOTTOM_SIZER:
                    {
                        Vertex3 scaleOffset = Vertex3(0, -mouseRelY_, 0);
                        Vertex3 scale = windowNode->GetScale();
                        windowNode->SetScale(scale + 0.5f * scaleOffset);
                        Vertex3 globalScale1 = windowNode->GetGlobalScale();
                        Vertex3 offset = globalScale1 - globalScale0;
                        windowNode->SetGlobalPosition(globalPosition0 - offset);
                        break;
                    }

                    case SizerType::RIGHT_BOTTOM_SIZER:
                    {
                        Vertex3 scaleOffset = Vertex3(mouseRelX_, -mouseRelY_, 0);
                        Vertex3 scale = windowNode->GetScale();
                        windowNode->SetScale(scale + 0.5f * scaleOffset);
                        Vertex3 globalScale1 = windowNode->GetGlobalScale();
                        Vertex3 offset = globalScale1 - globalScale0;
                        windowNode->SetGlobalPosition(globalPosition0 + Vertex3(offset.x, -offset.y, 0));
                        break;
                    }

                    default:
                        CHECK_ASSERT(false && "Unkonwn sizer type", __FILE__, __LINE__);
                        break;
                    }

                    {
                        // Check limits
                        Vertex3 globalScale = windowNode->GetGlobalScale();
                        float pixelsX = globalScale.x * App::this_->GetViewSize().first;
                        float pixelsY = globalScale.y * App::this_->GetViewSize().second;
                        if (pixelsX < 40 || pixelsY < 40)
                        {
                            windowNode->SetGlobalPosition(globalPosition0);
                            windowNode->SetScale(scale0);
                        }
                    }
                }
            }

        }
    }
}