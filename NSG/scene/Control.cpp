/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "Control.h"
#include "Material.h"
#include "Window.h"
#include "Scene.h"
#include "Graphics.h"
#include "Check.h"
#include "Technique.h"
#include "Pass.h"
#include "RectangleMesh.h"

namespace NSG
{
    Control::Control(const std::string& name)
        : SceneNode(name),
          pushed_(false),
          mouseEntered_(false),
          hAlign_(HorizontalAlignment::CENTER_ALIGNMENT),
          vAlign_(VerticalAlignment::MIDDLE_ALIGNMENT),
          leftMargin_(0),
          rightMargin_(0),
          topMargin_(0),
          bottomMargin_(0),
          window_(nullptr),
          signalPush_(new SignalButtonMouse),
          signalPop_(new SignalButtonMouse),
          signalClicked_(new SignalButtonMouse),
          signalMouseEnter_(new SignalEmpty),
          signalMouseLeave_(new SignalEmpty)
    {
        if (Graphics::this_)
        {
            auto window = Graphics::this_->GetWindow();
            if (window)
                SetWindow(window);
        }

        slotWindowCreated_ = Window::SignalReady()->Connect([this](Window * window)
        {
            if (!window_)
                SetWindow(window);
        });

    }

    Control::~Control()
    {
    }

    void Control::Align()
    {
        if (mesh_)
        {
            const BoundingBox& bb = GetWorldBoundingBox();
            Vector3 position = GetPosition();
            if (hAlign_ == CENTER_ALIGNMENT)
                position.x = 0;
            else if (hAlign_ == RIGHT_ALIGNMENT)
                position.x = 1 - rightMargin_ - bb.Size().x / 2.f;
            else
                position.x = -1 + leftMargin_ + bb.Size().x / 2.f;

            if (vAlign_ == MIDDLE_ALIGNMENT)
                position.y = 0;
            else if (vAlign_ == TOP_ALIGNMENT)
                position.y = 1 - topMargin_ - bb.Size().y / 2.f;
            else
                position.y = -1 + bottomMargin_ + bb.Size().y / 2.f;

            SetPosition(position);
        }
    }

    void Control::SetScreenAlignment(HorizontalAlignment hAlign, VerticalAlignment vAlign)
    {
        if (hAlign_ != hAlign || vAlign_ != vAlign)
        {
            hAlign_ = hAlign;
            vAlign_ = vAlign;
            Align();
        }
    }

    void Control::SetLeftScreenMargin(float margin)
    {
        if (leftMargin_ != margin)
        {
            leftMargin_ = margin;
            Align();
        }
    }

    void Control::SetRightScreenMargin(float margin)
    {
        if (rightMargin_ != margin)
        {
            rightMargin_ = margin;
            Align();
        }
    }

    void Control::SetTopScreenMargin(float margin)
    {
        if (topMargin_ != margin)
        {
            topMargin_ = margin;
            Align();
        }
    }

    void Control::SetBottomScreenMargin(float margin)
    {
        if (bottomMargin_ != margin)
        {
            bottomMargin_ = margin;
            Align();
        }
    }


    void Control::OnSceneSet()
    {
        auto scene = GetScene();
        slotNodeMouseDown_ = scene->SignalNodeMouseDown()->Connect([&](SceneNode * node, int button, float x, float y)
        {
            if (node == this)
            {
                mouseEntered_ = true;
                pushed_ = true;
                signalPush_->Run(button);
            }
        });
    }

    void Control::SetWindow(Window* window)
    {
        if (window_ != window)
        {
            if (window)
            {
                slotMouseMoved_ = window->SignalFloatFloat()->Connect([&](float x, float y)
                {
                    if (signalMouseEnter_->HasSlots() || signalMouseLeave_->HasSlots())
                    {
                        auto scene = GetScene();
                        SceneNode* node = scene->GetClosestNode(x, y);
                        if (node == this)
                        {
                            if (!mouseEntered_)
                            {
                                mouseEntered_ = true;
                                signalMouseEnter_->Run();
                            }
                        }
                        else
                        {
                            if (mouseEntered_)
                            {
                                mouseEntered_ = false;
                                signalMouseLeave_->Run();
                            }
                        }
                    }
                });

                slotMouseUp_ = window->SignalMouseUp()->Connect([&](int button, float x, float y)
                {
                    if (pushed_)
                    {
                        pushed_ = false;
                        signalPop_->Run(button);
                        if (signalClicked_->HasSlots())
                        {
                            auto scene = GetScene();
                            SceneNode* node = scene->GetClosestNode(x, y);
                            if (node == this)
                                signalClicked_->Run(button);
                        }
                    }
                });
            }
            else
            {
                slotMouseUp_ = nullptr;
            }
        }
    }
}