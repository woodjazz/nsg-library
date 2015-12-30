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
#pragma once
#include "Types.h"

namespace NSG
{
    class PlayerControl
    {
    public:
        PlayerControl();
        ~PlayerControl();
        void SetWindow(Window* window);
		SignalFloatFloat::PSignal SigMoved() { return signalMoved_; }
        SignalFloatFloat::PSignal SigLeftStickMoved() { return signalLeftStickMoved_; }
        SignalFloatFloat::PSignal SigRightStickMoved() { return signalRightStickMoved_; }
        SignalBool::PSignal SigButtonA() { return signalButtonA_; }
    private:
        void OnKey(int key, int action, int modifier);
        void OnMultiGesture(int timestamp, float x, float y, float dTheta, float dDist, int numFingers);
        void OnMousewheel(float x, float y);
        void OnMouseUp(int button, float x, float y);
        void OnMouseDown(int button, float x, float y);
        void OnMousemoved(float x, float y);
        void OnUpdate(float deltaTime);

        SignalFloatFloat::PSignal signalMoved_;
        SignalFloatFloat::PSignal signalLeftStickMoved_;
        SignalFloatFloat::PSignal signalRightStickMoved_;        
        SignalBool::PSignal signalButtonA_;
        SignalFloatFloat::PSlot slotMouseMoved_;
        SignalMouseButton::PSlot slotMouseDown_;
        SignalMouseButton::PSlot slotMouseUp_;
        SignalFloatFloat::PSlot slotMouseWheel_;
        SignalMultiGesture::PSlot slotMultiGesture_;
        SignalKey::PSlot slotKey_;
        SignalUpdate::PSlot slotUpdate_;
        SignalJoystickButton::PSlot slotJoystickDown_;
		SignalJoystickButton::PSlot slotJoystickUp_;
        SignalJoystickAxisMotion::PSlot slotJoystickAxisMotion_;
        SignalTouchFinger::PSlot slotTouchFinger_;
        Window* window_;
        SignalWindow::PSlot slotWindow_;
        float leftHorizontalAxis_;
        float leftVerticalAxis_;
        float rightHorizontalAxis_;
        float rightVerticalAxis_;
        bool left_;
        bool right_;
        bool forward_;
        bool backward_;
        typedef std::pair<bool, int64_t> FirgerId;
        FirgerId leftFingerId_;
        FirgerId rightFingerId_;
    };
}