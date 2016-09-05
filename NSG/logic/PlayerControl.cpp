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
#include "PlayerControl.h"
#include "RenderingContext.h"
#include "Keys.h"
#include "Window.h"
#include "Engine.h"
#include "Log.h"
#include "SharedFromPointer.h"

namespace NSG
{
    PlayerControl::PlayerControl()
        : signalMoved_(new SignalFloatFloat),
          signalLeftStickMoved_(new SignalFloatFloat),
          signalRightStickMoved_(new SignalFloatFloat),
          signalButtonA_(new SignalBool),
          leftHorizontalAxis_(0),
          leftVerticalAxis_(0),
          rightHorizontalAxis_(0),
          rightVerticalAxis_(0),
          left_(false),
          right_(false),
          forward_(false),
          backward_(false),
          leftFingerId_{false, 0},
          rightFingerId_{false, 0}
    {
        auto graphics = RenderingContext::GetPtr();
        if (graphics)
            SetWindow(graphics->GetWindow().lock());
        slotWindow_ = RenderingContext::SigWindow()->Connect([this](Window * window)
        {
            if (!window_.lock())
                SetWindow(SharedFromPointer(window));
        });

        slotUpdate_ = Engine::SigUpdate()->Connect([this](float deltaTime)
        {
            OnUpdate(deltaTime);
        });
    }

    PlayerControl::~PlayerControl()
    {
    }

    void PlayerControl::SetWindow(PWindow window)
    {
        if (window_.lock() != window)
        {
            window_ = window;

            if (window)
            {
                slotMouseMoved_ = window->SigFloatFloat()->Connect([this](float x, float y)
                {
                    OnMousemoved(x, y);
                });

                slotMouseDown_ = window->SigMouseDown()->Connect([this](int button, float x, float y)
                {
                    OnMouseDown(button, x, y);
                });

                slotMouseUp_ = window->SigMouseUp()->Connect([this](int button, float x, float y)
                {
                    OnMouseUp(button, x, y);
                });

                slotMouseWheel_ = window->SigMouseWheel()->Connect([this](float x, float y)
                {
                    OnMousewheel(x, y);
                });

                slotMultiGesture_ = window->SigMultiGesture()->Connect([this](int timestamp, float x, float y, float dTheta, float dDist, int numFingers)
                {
                    OnMultiGesture(timestamp, x, y, dTheta, dDist, numFingers);
                });

                slotKey_ = window->SigKey()->Connect([this](int key, int action, int modifier)
                {
                    OnKey(key, action, modifier);
                });

                #ifndef IS_TARGET_MOBILE

                slotJoystickDown_ = window->SigJoystickDown()->Connect([this](int joystickID, JoystickButton button)
                {
                    if (JoystickButton::BUTTON_A == button)
                        signalButtonA_->Run(true);

                });

                slotJoystickUp_ = window->SigJoystickUp()->Connect([this](int joystickID, JoystickButton button)
                {
                    if (JoystickButton::BUTTON_A == button)
                        signalButtonA_->Run(false);
                });

                slotJoystickAxisMotion_ = window->SigJoystickAxisMotion()->Connect([this](int joystickID, JoystickAxis axis, float position)
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
                            rightHorizontalAxis_ = position;
                            break;
                        case JoystickAxis::RIGHTY:
                            rightVerticalAxis_ = -position;
                            break;
                        case JoystickAxis::TRIGGERLEFT:
                            break;
                        case JoystickAxis::TRIGGERRIGHT:
                            break;
                        default:
                            break;
                    }
                });

                #endif

                slotTouchFinger_ = window->SigTouchFinger()->Connect([this](const TouchFingerEvent & event)
                {
                    //LOGW("x=%f y=%f touchId=%lld fingerId=%lld", event.x, event.y, event.touchId, event.fingerId);

                    if (event.type == TouchFingerEvent::Type::DOWN)
                    {
                        if (event.x < 0.5f)
                        {
                            leftHorizontalAxis_ = leftVerticalAxis_ = 0;
                            leftFingerId_ = FirgerId{true, event.fingerId};
                        }
                        else
                        {
                            rightHorizontalAxis_ = rightVerticalAxis_ = 0;
                            rightFingerId_ = FirgerId{true, event.fingerId};
                            signalButtonA_->Run(true);
                        }
                    }
                    else if (event.type == TouchFingerEvent::Type::MOTION)
                    {
                        if (leftFingerId_.first && leftFingerId_.second == event.fingerId)
                        {
                            leftHorizontalAxis_ += event.dx;
                            leftVerticalAxis_ -= event.dy;
                        }
                        else if (rightFingerId_.first && rightFingerId_.second == event.fingerId)
                        {
                            rightHorizontalAxis_ += event.dx;
                            rightVerticalAxis_ -= event.dy;
                        }
                    }
                    else // UP
                    {
                        if (leftFingerId_.first && leftFingerId_.second == event.fingerId)
                        {
                            leftHorizontalAxis_ = leftVerticalAxis_ = 0;
                            leftFingerId_ = FirgerId{false, 0};
                        }
                        else if (rightFingerId_.first && rightFingerId_.second == event.fingerId)
                        {
                            rightHorizontalAxis_ = rightVerticalAxis_ = 0;
                            rightFingerId_ = FirgerId{false, 0};
                            signalButtonA_->Run(false);
                        }
                    }
                });
            }
            else
            {
                signalMoved_ = nullptr;
                signalLeftStickMoved_ = nullptr;
                signalRightStickMoved_ = nullptr;                
                signalButtonA_ = nullptr;
                slotMouseMoved_ = nullptr;
                slotMouseDown_ = nullptr;
                slotMouseUp_ = nullptr;
                slotMouseWheel_ = nullptr;
                slotMultiGesture_ = nullptr;
                slotKey_ = nullptr;
                slotJoystickDown_ = nullptr;
                slotJoystickUp_ = nullptr;
                slotJoystickAxisMotion_ = nullptr;
                slotTouchFinger_ = nullptr;
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
            float x  = 0;
            float y = 0;
            if (forward_)
                y += 1;
            if (backward_)
                y -= 1;
            if (right_)
                x += 1;
            if (left_)
                x -= 1;
            signalMoved_->Run(x, y);
        }
        else
        {
            leftHorizontalAxis_ = Clamp(leftHorizontalAxis_, -1.f, 1.f);
            leftVerticalAxis_ = Clamp(leftVerticalAxis_, -1.f, 1.f);
            signalLeftStickMoved_->Run(leftHorizontalAxis_, leftVerticalAxis_);
            rightHorizontalAxis_ = Clamp(rightHorizontalAxis_, -1.f, 1.f);
            rightVerticalAxis_ = Clamp(rightVerticalAxis_, -1.f, 1.f);
            signalRightStickMoved_->Run(rightHorizontalAxis_, rightVerticalAxis_);
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

            case NSG_KEY_SPACE:
                {
                    signalButtonA_->Run(action ? true : false);
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

