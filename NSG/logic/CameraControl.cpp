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
#include "CameraControl.h"
#include "Camera.h"
#include "Scene.h"
#include "Frustum.h"
#include "Graphics.h"
#include "Ray.h"
#include "PointOnSphere.h"
#include "Keys.h"
#include "Window.h"
#include "Engine.h"

namespace NSG
{
    CameraControl::CameraControl(PCamera camera)
        : camera_(camera),
          window_(nullptr),
          engine_(nullptr),
          originalPosition_(camera->GetGlobalPosition()),
          originalOrientation_(camera->GetGlobalOrientation())
    {
        CHECK_ASSERT(camera_, __FILE__, __LINE__);
        lastX_ = lastY_ = 0;
        pointOnSphere_ = PPointOnSphere(new PointOnSphere(Vertex3(0), camera_->GetGlobalPosition()));
        updateOrientation_ = false;
        leftButtonDown_ = false;
        altKeyDown_ = false;
        shiftKeyDown_ = false;
        //SetSphereCenter(true);

        if (Graphics::this_)
        {
            auto window = Graphics::this_->GetWindow();
            if (window)
                SetWindow(window);
        }

        slotWindowCreated_ = Window::SigReady()->Connect([this](Window * window)
        {
            if (!window_)
                SetWindow(window);
        });

        SetEngine(Engine::GetPtr());

        slotEngineCreated_ = Engine::SigReady()->Connect([this](Engine * engine)
        {
            if (!engine_)
                SetEngine(engine);
        });
    }

    CameraControl::~CameraControl()
    {

    }

    void CameraControl::SetEngine(Engine* engine)
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


    void CameraControl::SetWindow(Window* window)
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
            }
            else
            {
                slotMouseMoved_ = nullptr;
                slotMouseDown_ = nullptr;
                slotMouseUp_ = nullptr;
                slotMouseWheel_ = nullptr;
                slotMultiGesture_ = nullptr;
                slotKey_ = nullptr;
            }
        }
    }

    void CameraControl::OnMousemoved(float x, float y)
    {
        if (leftButtonDown_ && !updateOrientation_)
            Move(x, y);
        lastX_ = x;
        lastY_ = y;
    }

    void CameraControl::OnMouseDown(int button, float x, float y)
    {
        lastX_ = x;
        lastY_ = y;

        if (button == NSG_BUTTON_LEFT)
            leftButtonDown_ = true;
    }

    void CameraControl::OnMouseUp(int button, float x, float y)
    {
        if (button == NSG_BUTTON_LEFT)
            leftButtonDown_ = false;
    }

    void CameraControl::OnMousewheel(float x, float y)
    {
        Vertex3 lookAtPoint = pointOnSphere_->GetCenter();
        Vertex3 position = camera_->GetGlobalPosition();
        Vector3 looAtDir = camera_->GetLookAtDirection();
        float distance = glm::distance(lookAtPoint, position);
        if (distance < 1)
            distance = 1;
        float factor = y > 0 ? 2.0f : -2.0f;
        distance /= factor;
        position += looAtDir * distance;
        SetPosition(position);
    }

    void CameraControl::OnUpdate(float deltaTime)
    {
        if (updateOrientation_)
        {
            Quaternion targetOrientation = camera_->GetLookAtOrientation(pointOnSphere_->GetCenter(), pointOnSphere_->GetUp());
            Quaternion currentOrientation = camera_->GetOrientation();
            float dot = glm::dot(currentOrientation, targetOrientation);
            bool close = 1 - dot * dot < PRECISION;
            if (!close)
            {
                //animate
                float factor = 3 * deltaTime;
                camera_->SetOrientation(glm::slerp(currentOrientation, targetOrientation, factor));
            }
            else
            {
                camera_->SetOrientation(targetOrientation);
                updateOrientation_ = false;
            }
        }
    }

    void CameraControl::OnMultiGesture(int timestamp, float x, float y, float dTheta, float dDist, int numFingers)
    {
        if (numFingers == 2)
        {
            float factor = dDist * timestamp / 10.0f;
            Vertex3 position = camera_->GetGlobalPosition();
            Vector3 looAtDir = camera_->GetLookAtDirection();
            position += looAtDir * factor;
            SetPosition(position);
        }
        #if 0
        else if (numFingers == 3)
        {
            Move(x, y);
            lastX_ = x;
            lastY_ = y;
        }
        #endif

    }

    void CameraControl::OnKey(int key, int action, int modifier)
    {
        switch (key)
        {
            case NSG_KEY_W:
                {
                    break;
                }

            case NSG_KEY_S:
                {
                    camera_->EnableColorSplits(action ? true : false);
                    break;
                }

            case NSG_KEY_1:
                {
                    if (shiftKeyDown_)
                        camera_->SetMaxShadowSplits(1);
                    break;
                }

            case NSG_KEY_2:
                {
                    if (shiftKeyDown_)
                        camera_->SetMaxShadowSplits(2);
                    break;
                }

            case NSG_KEY_3:
                {
                    if (shiftKeyDown_)
                        camera_->SetMaxShadowSplits(3);
                    break;
                }

            case NSG_KEY_4:
                {
                    if (shiftKeyDown_)
                        camera_->SetMaxShadowSplits(4);
                    break;
                }

            case NSG_KEY_A:
                {
                    AutoZoom();
                    break;
                }

            case NSG_KEY_D:
                {
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
                    if (action)
                        SetSphereCenter(false);
                    break;
                }

            case NSG_KEY_C:
                {
                    if (action)
                        SetSphereCenter(true);
                    break;
                }

            case NSG_KEY_R:
                {
                    camera_->SetGlobalPosition(originalPosition_);
                    camera_->SetGlobalOrientation(originalOrientation_);
                    lastX_ = lastY_ = 0;
                    pointOnSphere_ = PPointOnSphere(new PointOnSphere(Vertex3(0), camera_->GetGlobalPosition()));
                    updateOrientation_ = false;
                    break;
                }

            case NSG_KEY_LALT:
                {
                    altKeyDown_ = action ? true : false;
                    break;
                }

            case NSG_KEY_LSHIFT:
                {
                    shiftKeyDown_ = action ? true : false;
                    break;
                }

        }

    }

    void CameraControl::Move(float x, float y)
    {
        float relX = (x - lastX_);
        float relY = (y - lastY_);
        if (altKeyDown_)
        {
            pointOnSphere_->IncAngles(PI * relX, PI * relY);
            camera_->SetGlobalPosition(pointOnSphere_->GetPoint());
            camera_->SetGlobalLookAt(pointOnSphere_->GetCenter(), pointOnSphere_->GetUp());
        }
    }


    void CameraControl::SetPosition(const Vertex3& position)
    {
        if (pointOnSphere_->SetPoint(position))
        {
            Vertex3 oldPos = camera_->GetGlobalPosition();
            camera_->SetGlobalPosition(position);
            BoundingBox bb = camera_->GetScene()->GetWorldBoundingBoxBut(camera_.get());
            if (camera_->GetFrustum()->IsInside(bb) == Intersection::OUTSIDE)
            {
                pointOnSphere_->SetPoint(oldPos);
                camera_->SetGlobalPosition(oldPos);
            }
        }
    }

    void CameraControl::SetSphereCenter(bool centerObj)
    {
        Vertex3 newCenter;
        Ray ray = camera_->GetScreenRay(lastX_, lastY_);
        RayNodeResult closest;
        if (camera_->GetScene()->GetClosestRayNodeIntersection(ray, closest))
        {
            if (centerObj)
            {
                newCenter = closest.node_->GetGlobalPosition();
            }
            else
            {
                newCenter = ray.GetPoint(closest.distance_);
            }
        }
        else
        {
            BoundingBox bb;
            if (!camera_->GetScene()->GetVisibleBoundingBox(camera_.get(), bb))
                bb = camera_->GetScene()->GetWorldBoundingBoxBut(camera_.get());
            newCenter = ray.GetPoint(glm::distance(bb.Center(), camera_->GetGlobalPosition()));
            //newCenter = bb.Center();
        }

        if (pointOnSphere_->SetCenter(newCenter))
            updateOrientation_ = true;
    }

    void CameraControl::AutoZoom()
    {
        BoundingBox bb = camera_->GetScene()->GetWorldBoundingBoxBut(camera_.get());
        Vertex3 center = bb.Center();
        Vertex3 position = camera_->GetGlobalPosition();
        Vector3 lookAtDir(WORLD_Z_COORD);
        if (glm::distance(position, center) > PRECISION)
            lookAtDir = glm::normalize(position - center);
        float distance = std::max(std::max(bb.Size().x, bb.Size().y), bb.Size().z);
        if (distance < camera_->GetZNear())
            distance = 1 + camera_->GetZNear();

        position = center + lookAtDir * distance;

        if (pointOnSphere_->SetCenterAndPoint(center, position))
            camera_->SetGlobalPosition(position);
    }
}
