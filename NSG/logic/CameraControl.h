/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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
#include "Quaternion.h"
#include "SharedPointers.h"
#include "Types.h"
#include "Vector3.h"
#include "Vector4.h"

namespace NSG {
class CameraControl {
public:
    CameraControl(PCamera camera, PScene scene);
    CameraControl(PCamera camera);
    ~CameraControl();
    void Track(PNode node);
    void SetWindow(PWindow window);
    void AutoZoom();
    void OnUpdate(float deltaTime);
    void OnKey(int key, int action, int modifier);
    void Enable(bool enable);
    PSceneNode SelectObject(float x, float y);
    void SetViewRect(const Vector4& viewRect);
    void SetSelectionContext(RendererContext context) {
        selectionContext_ = context;
    }

private:
    bool TransformCoords(float& x, float& y);
    void OnMultiGesture(int timestamp, float x, float y, float dTheta,
                        float dDist, int numFingers);
    void OnMousewheel(float x, float y);
    void OnMouseUp(int button, float x, float y);
    void OnMouseDown(int button, float x, float y);
    void OnMouseMoved(float x, float y);
    void SetPosition(const Vertex3& position);
    void RayCastNewCenter(bool centerObj);
    void Move(float x, float y);
    float lastX_;
    float lastY_;
    bool leftButtonDown_;
    bool altKeyDown_;
    bool shiftKeyDown_;
    PPointOnSphere pointOnSphere_;
    PCamera camera_;
    PScene scene_;
    bool updateOrientation_;
    PWeakWindow window_;
    Vector3 originalPosition_;
    Quaternion originalOrientation_;
    PNode trackNode_;
    SignalWindow::PSlot slotWindow_;
    SignalFloatFloat::PSlot slotMouseMoved_;
    SignalMouseButton::PSlot slotMouseDown_;
    SignalMouseButton::PSlot slotMouseUp_;
    SignalFloatFloat::PSlot slotMouseWheel_;
    SignalMultiGesture::PSlot slotMultiGesture_;
    SignalKey::PSlot slotKey_;
    SignalUpdate::PSlot slotUpdate_;
    bool enableDebugPhysics_;
    bool enableColorSplits_;
    bool enabled_;
    Vector4 viewRect_;
    bool selection_;
    RendererContext selectionContext_;
};
}
