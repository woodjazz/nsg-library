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
#include "PlayerControl.h"
#include "Graphics.h"
#include "Keys.h"
#include "Window.h"
#include "Engine.h"

namespace NSG
{
    PlayerControl::PlayerControl()
        : signalMoved_(new SignalFloatFloat),
          window_(nullptr),
          engine_(nullptr),
          leftHorizontalAxis_(0),
          leftVerticalAxis_(0),
          left_(false),
          right_(false),
          forward_(false),
          backward_(false)
    {
        if (Graphics::GetPtr())
        {
            auto window = Graphics::GetPtr()->GetWindow();
            if (window)
                SetWindow(window);
        }

        slotWindowCreated_ = Window::SigReady()->Connect([this](Window * window)
        {
            if (!window_)
                SetWindow(window);
        });

        SetEngine(Engine::GetPtr().get());

        slotEngineCreated_ = Engine::SigReady()->Connect([this](Engine * engine)
        {
            if (!engine_)
                SetEngine(engine);
        });

    }

    PlayerControl::~PlayerControl()
    {

    }

    void PlayerControl::SetEngine(Engine* engine)
    {
        if (engine_ != engine)
        {
            engine_ = engine;

            if (engine)
            {
                slotUpdate_ = engine->SigUpdate()->Connect([&](float deltaTime)
                {
                    OnUpdate(deltaTime);
                });
            }
        }
        else
        {
            slotUpdate_ = nullptr;
        }
    }

    void PlayerControl::SetWindow(Window* window)
    {
        if (window_ != window)
        {
            window_ = window;

            if (window)
            {
                slotMouseMoved_ = window->SigFloatFloat()->Connect([&](float x, float y)
                {
                    OnMousemoved(x, y);
                });

                slotMouseDown_ = window->SigMouseDown()->Connect([&](int button, float x, float y)
                {
                    OnMouseDown(button, x, y);
                });

                slotMouseUp_ = window->SigMouseUp()->Connect([&](int button, float x, float y)
                {
                    OnMouseUp(button, x, y);
                });

                slotMouseWheel_ = window->SigMouseWheel()->Connect([&](float x, float y)
                {
                    OnMousewheel(x, y);
                });

                slotMultiGesture_ = window->SigMultiGesture()->Connect([&](int timestamp, float x, float y, float dTheta, float dDist, int numFingers)
                {
                    OnMultiGesture(timestamp, x, y, dTheta, dDist, numFingers);
                });

                slotKey_ = window->SigKey()->Connect([&](int key, int action, int modifier)
                {
                    OnKey(key, action, modifier);
                });

                slotJoystickDown_ = window->SigJoystickDown()->Connect([&](int joystickID, unsigned button)
                {

                });

                slotJoystickUp_ = window->SigJoystickUp()->Connect([&](int joystickID, unsigned button)
                {

                });

                slotJoystickAxisMotion_ = window->SigJoystickAxisMotion()->Connect([&](int joystickID, JoystickAxis axis, float position)
                {
                    switch (axis)
                    {
                        case JoystickAxis::LEFTX:
                            leftHorizontalAxis_ = position;
                            break;
                        case JoystickAxis::LEFTY:
                            leftVerticalAxis_ = -position;
                            break;
                        case JoystickAxis::RIGHTX:
                            break;
                        case JoystickAxis::RIGHTY:
                            break;
                        case JoystickAxis::TRIGGERLEFT:
                            break;
                        case JoystickAxis::TRIGGERRIGHT:
                            break;
                        default:
                            break;
                    }
                });
            }
            else
            {
                slotMouseMoved_ = nullptr;
                slotMouseDown_ = nullptr;
                slotMouseUp_ = nullptr;
                slotMouseWheel_ = nullptr;
                slotMultiGesture_ = nullptr;
                slotKey_ = nullptr;
                slotJoystickDown_ = nullptr;
                slotJoystickUp_ = nullptr;
                slotJoystickAxisMotion_ = nullptr;
            }
        }
    }

    void PlayerControl::OnMousemoved(float x, float y)
    {
    }

    void PlayerControl::OnUpdate(float deltaTime)
    {
        if (forward_ || backward_ || right_ || left_)
        {
            if (forward_)
                signalMoved_->Run(0, 1);
            if (backward_)
                signalMoved_->Run(0, -1);
            if (right_)
                signalMoved_->Run(1, 0);
            if (left_)
                signalMoved_->Run(-1, 0);
        }
        else
        {
            leftHorizontalAxis_ = glm::clamp(leftHorizontalAxis_, -1.f, 1.f);
            leftVerticalAxis_ = glm::clamp(leftVerticalAxis_, -1.f, 1.f);
            if (leftHorizontalAxis_ || leftVerticalAxis_)
                signalMoved_->Run(leftHorizontalAxis_, leftVerticalAxis_);
        }
    }


    void PlayerControl::OnMouseDown(int button, float x, float y)
    {
    }

    void PlayerControl::OnMouseUp(int button, float x, float y)
    {
    }

    void PlayerControl::OnMousewheel(float x, float y)
    {
    }

    void PlayerControl::OnMultiGesture(int timestamp, float x, float y, float dTheta, float dDist, int numFingers)
    {
    }

    void PlayerControl::OnKey(int key, int action, int modifier)
    {
        switch (key)
        {
            case NSG_KEY_UP:
            case NSG_KEY_W:
                {
                    forward_ = action ? true : false;
                    break;
                }

            case NSG_KEY_DOWN:
            case NSG_KEY_S:
                {
                    backward_ = action ? true : false;
                    break;
                }

            case NSG_KEY_LEFT:
            case NSG_KEY_A:
                {
                    left_ = action ? true : false;
                    break;
                }

            case NSG_KEY_RIGHT:
            case NSG_KEY_D:
                {
                    right_ = action ? true : false;
                    break;
                }

            case NSG_KEY_Q:
                {
                    break;
                }

            case NSG_KEY_E:
                {
                    break;
                }

            case NSG_KEY_F:
                {
                    break;
                }

            case NSG_KEY_C:
                {
                    break;
                }

            case NSG_KEY_LALT:
                {
                    break;
                }

            case NSG_KEY_LSHIFT:
                {
                    break;
                }
        }
    }
}
