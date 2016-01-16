/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "ShadowMapDebug.h"
#include "Light.h"
#include "ShadowCamera.h"
#include "RenderingContext.h"
#include "Keys.h"
#include "Window.h"
#include "Renderer.h"
#include "DebugRenderer.h"
#include "Check.h"

namespace NSG
{
    ShadowMapDebug::ShadowMapDebug(PLight light)
        : light_(light),
          window_(nullptr),
          debugRendererEnabled_(false)
    {
        CHECK_ASSERT(light_);

        auto graphics = RenderingContext::GetPtr();
        if (graphics)
            SetWindow(graphics->GetWindow());

        slotWindow_ = RenderingContext::SigWindow()->Connect([this](Window * window)
        {
            if (!window_)
                SetWindow(window);
        });

        slotDebugRenderer_ = Renderer::GetPtr()->SigDebugRenderer()->Connect([&](DebugRenderer * renderer)
        {
            if (debugRendererEnabled_)
            {
                auto splits = light_->GetShadowSplits();
                light_->GetShadowCamera(0)->Debug(renderer, Color(COLOR_RED, 1.0));
                if (splits > 1)
                {
					light_->GetShadowCamera(1)->Debug(renderer, Color(COLOR_GREEN, 1.0));
                    if (splits > 2)
                    {
						light_->GetShadowCamera(2)->Debug(renderer, Color(COLOR_BLUE, 1.0));
                        if (splits > 3)
                        {
							light_->GetShadowCamera(3)->Debug(renderer, Color(COLOR_YELLOW, 1.0));
                        }
                    }
                }
            }
        });
    }

    ShadowMapDebug::~ShadowMapDebug()
    {

    }

    void ShadowMapDebug::SetWindow(Window* window)
    {
        if (window_ != window)
        {
            window_ = window;

            if (window)
            {
                slotKey_ = window->SigKey()->Connect([&](int key, int action, int modifier)
                {
                    OnKey(key, action, modifier);
                });
            }
            else
            {
                slotKey_ = nullptr;
            }
        }
    }

    void ShadowMapDebug::OnKey(int key, int action, int modifier)
    {
        if(modifier)
            return;
        
        if (NSG_KEY_0 == key && action)
            window_->ShowMap(nullptr);
        else if (NSG_KEY_1 == key && action)
        {
            window_->ShowMap(nullptr);
			window_->ShowMap(light_->GetShadowMap(0));
        }
        else if (NSG_KEY_2 == key && action)
        {
            window_->ShowMap(nullptr);
			window_->ShowMap(light_->GetShadowMap(1));
        }
        else if (NSG_KEY_3 == key && action)
        {
            window_->ShowMap(nullptr);
			window_->ShowMap(light_->GetShadowMap(2));
        }
        else if (NSG_KEY_4 == key && action)
        {
            window_->ShowMap(nullptr);
			window_->ShowMap(light_->GetShadowMap(3));
        }
		else if (NSG_KEY_M == key && action)
			debugRendererEnabled_ = !debugRendererEnabled_;
    }
}
