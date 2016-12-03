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
#include "CameraControl.h"
#include "Camera.h"
#include "Scene.h"
#include "Frustum.h"
#include "RenderingContext.h"
#include "Ray.h"
#include "PointOnSphere.h"
#include "Keys.h"
#include "Window.h"
#include "Engine.h"
#include "Renderer.h"
#include "Check.h"
#include "Maths.h"
#include "SharedFromPointer.h"

namespace NSG
{
    CameraControl::CameraControl(PCamera camera, PScene scene)
        : camera_(camera),
          scene_(scene),
          originalPosition_(camera->GetGlobalPosition()),
          originalOrientation_(camera->GetGlobalOrientation()),
          enableDebugPhysics_(false),
          enableColorSplits_(false),
          enabled_(true),
		  selection_(false),
		  selectionContext_(RendererContext::DEFAULT)
    {
        CHECK_ASSERT(camera_);
        CHECK_ASSERT(scene_);
        lastX_ = lastY_ = 0;
        pointOnSphere_ = PPointOnSphere(new PointOnSphere(Vertex3(0), camera_->GetGlobalPosition()));
        updateOrientation_ = false;
        leftButtonDown_ = false;
        altKeyDown_ = false;
        shiftKeyDown_ = false;

        SetWindow(camera->GetWindow());

        slotWindow_ = camera->SigWindow()->Connect([this](PWindow window)
        {
            SetWindow(window);
        });

        slotUpdate_ = Engine::SigUpdate()->Connect([this](float deltaTime)
        {
            OnUpdate(deltaTime);
        });
    }

    CameraControl::CameraControl(PCamera camera)
        : CameraControl(camera, camera->GetScene())
    {

    }

    CameraControl::~CameraControl()
    {

    }

    bool CameraControl::TransformCoords(float& x, float& y)
    {
        if (viewRect_ != Vector4::Zero)
        {
			auto window = window_.lock().get();
			if (!window)
				return false;
            auto width = window->GetWidth();
            auto height = window->GetHeight();
			auto x1 = (x + 1) * 0.5f;
			auto y1 = (-y + 1) * 0.5f;
			auto vWidth = viewRect_[2] - viewRect_[0];
			auto vHeight = viewRect_[3] - viewRect_[1];

			x1 *= width;
			y1 *= height;

			if (Clamp(x1, viewRect_[0], viewRect_[2]) != x1)
				return false;

			if (Clamp(y1, viewRect_[1], viewRect_[3]) != y1)
				return false;

			x1 = (x1 - viewRect_[0]) / (vWidth);
			y1 = (y1 - viewRect_[1]) / (vHeight);

			x = -1 + 2 * x1;
			y = 1 - 2 * y1;
        }

		return true;
    }

    void CameraControl::SetWindow(PWindow window)
    {
        if (window_.lock() != window)
        {
            window_ = window;

            if (window)
            {
                slotMouseMoved_ = window->SigFloatFloat()->Connect([&](float x, float y)
                {
					if(TransformCoords(x, y))
						OnMouseMoved(x, y);
                });

                slotMouseDown_ = window->SigMouseDown()->Connect([&](int button, float x, float y)
                {
					if (TransformCoords(x, y))
						OnMouseDown(button, x, y);
                });

                slotMouseUp_ = window->SigMouseUp()->Connect([&](int button, float x, float y)
                {
					if (TransformCoords(x, y))
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

    void CameraControl::OnMouseMoved(float x, float y)
    {
        if (!enabled_)
            return;

        if (leftButtonDown_ && !updateOrientation_)
            Move(x, y);
        lastX_ = x;
        lastY_ = y;
    }

    void CameraControl::OnMouseDown(int button, float x, float y)
    {
        if (!enabled_)
            return;

        lastX_ = x;
        lastY_ = y;

        if (button == NSG_BUTTON_LEFT)
        {
            if (altKeyDown_)
                RayCastNewCenter(false);
            leftButtonDown_ = true;
        }
    }

    void CameraControl::OnMouseUp(int button, float x, float y)
    {
        if (!enabled_)
            return;

		if (button == NSG_BUTTON_LEFT)
            leftButtonDown_ = false;
    }

    void CameraControl::OnMousewheel(float x, float y)
    {
        if (!enabled_)
            return;

        Vertex3 lookAtPoint = pointOnSphere_->GetCenter();
        Vertex3 position = camera_->GetGlobalPosition();
        Vector3 looAtDir = camera_->GetLookAtDirection();
        float distance = lookAtPoint.Distance(position);
        if (distance < 1)
            distance = 1;
        float factor = y > 0 ? 2.0f : -2.0f;
        distance /= factor;
        position = position + looAtDir * distance;
        SetPosition(position);
    }

    void CameraControl::OnUpdate(float deltaTime)
    {
        if (!enabled_)
            return;

        if (trackNode_ && pointOnSphere_->SetCenter(trackNode_->GetGlobalPosition()))
            updateOrientation_ = true;

        if (updateOrientation_)
        {
            Quaternion targetOrientation = camera_->GetLookAtOrientation(pointOnSphere_->GetCenter(), pointOnSphere_->GetUp());
            Quaternion currentOrientation = camera_->GetOrientation();
            float dot = currentOrientation.Dot(targetOrientation);
            bool close = 1 - dot * dot < PRECISION;
            if (!close)
            {
                //animate
                float factor = 3 * deltaTime;
                camera_->SetOrientation(currentOrientation.Slerp(targetOrientation, factor));
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
        if (!enabled_)
            return;

        if (numFingers == 2)
        {
            float factor = dDist * timestamp / 10.0f;
            Vertex3 position = camera_->GetGlobalPosition();
            Vector3 looAtDir = camera_->GetLookAtDirection();
            position = position + looAtDir * factor;
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
        if (!enabled_)
            return;

        switch (key)
        {
            case NSG_KEY_G:
                if (action)
                {
                    enableColorSplits_ = !enableColorSplits_;
                    camera_->EnableColorSplits(enableColorSplits_);
                    break;
                }

            case NSG_KEY_1:
                if (shiftKeyDown_)
                    camera_->SetMaxShadowSplits(1);
                break;

            case NSG_KEY_2:
                if (shiftKeyDown_)
                    camera_->SetMaxShadowSplits(2);
                break;

            case NSG_KEY_3:
                if (shiftKeyDown_)
                    camera_->SetMaxShadowSplits(3);
                break;

            case NSG_KEY_4:
                if (shiftKeyDown_)
                    camera_->SetMaxShadowSplits(4);
                break;

            case NSG_KEY_Z:
                AutoZoom();
                break;

            case NSG_KEY_P:
                if (action)
                {
                    enableDebugPhysics_ = !enableDebugPhysics_;
                    Renderer::GetPtr()->EnableDebugPhysics(enableDebugPhysics_);
                    camera_->SetUniformsNeedUpdate();
                }
                break;

            case NSG_KEY_F:
                if (action)
                    RayCastNewCenter(false);
                break;


            case NSG_KEY_C:
                if (action)
                    RayCastNewCenter(true);
                break;

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
            default:
                break;
        }
    }

    void CameraControl::Move(float x, float y)
    {
        if (!enabled_)
            return;

        float relX = (x - lastX_);
        float relY = (y - lastY_);
        if (altKeyDown_ && shiftKeyDown_)
        {
            auto radius = pointOnSphere_->GetRadius();
            OnMousewheel(0, radius * relY);
        }
        else if (altKeyDown_ && selection_)
        {
            pointOnSphere_->IncAngles(PI * relX, PI * relY);
            camera_->SetGlobalPosition(pointOnSphere_->GetPoint());
            camera_->SetGlobalLookAtPosition(pointOnSphere_->GetCenter(), pointOnSphere_->GetUp());
        }
        else if (shiftKeyDown_)
        {
            auto pos = pointOnSphere_->GetPoint();
            auto rot = camera_->GetGlobalOrientation();
            auto radius = pointOnSphere_->GetRadius();
            auto offset = rot * (radius * Vector3(relX, relY, 0));
            pos = pos + offset;
            SetPosition(pos);
            pointOnSphere_->SetCenter(pointOnSphere_->GetCenter() + offset);
        }
    }

    void CameraControl::SetPosition(const Vertex3& position)
    {
        if (pointOnSphere_->SetPoint(position))
        {
            Vertex3 oldPos = camera_->GetGlobalPosition();
            camera_->SetGlobalPosition(position);
            BoundingBox bb = scene_->GetWorldBoundingBoxBut(camera_.get());
            if (camera_->GetFrustum()->IsInside(bb) == Intersection::OUTSIDE)
            {
                pointOnSphere_->SetPoint(oldPos);
                camera_->SetGlobalPosition(oldPos);
            }
        }
    }

    void CameraControl::Track(PNode node)
    {
        trackNode_ = node;
    }

    PSceneNode CameraControl::SelectObject(float x, float y)
    {
        auto oldContext = Renderer::GetPtr()->SetContextType(selectionContext_);
		TransformCoords(x, y);
        Ray ray = camera_->GetScreenRay(lastX_, lastY_);
        RayNodeResult closest;
		auto selection = scene_->GetClosestRayNodeIntersection(ray, closest);
        Renderer::GetPtr()->SetContextType(oldContext);
		if (selection)
		{
			selection_ = selection;
			return std::dynamic_pointer_cast<SceneNode>(SharedFromPointerNode(closest.node_));
		}
        return nullptr;
    }

    void CameraControl::RayCastNewCenter(bool centerObj)
    {
        auto oldContext = Renderer::GetPtr()->SetContextType(selectionContext_);
        Vertex3 newCenter;
        Ray ray = camera_->GetScreenRay(lastX_, lastY_);
        RayNodeResult closest;
		auto selection = scene_->GetClosestRayNodeIntersection(ray, closest);
        Renderer::GetPtr()->SetContextType(oldContext);
		if (selection)
        {
			selection_ = selection;
            LOGI("CamCenter in %s", closest.node_->GetName().c_str());
            if (centerObj)
            {
                newCenter = closest.node_->GetGlobalPosition();
            }
            else
            {
                newCenter = ray.GetPoint(closest.distance_);
            }

            if (pointOnSphere_->SetCenter(newCenter))
                updateOrientation_ = true;
        }
    }

    void CameraControl::AutoZoom()
    {
        BoundingBox bb = scene_->GetWorldBoundingBoxBut(camera_.get());
        Vertex3 center = bb.Center();
        float distance = std::max(std::max(bb.Size().x, bb.Size().y), bb.Size().z);
        if (distance < camera_->GetZNear())
            distance = 1 + camera_->GetZNear();
        Vertex3 position = camera_->GetGlobalPosition();
        if (bb.IsInside(position))
            position = center + Vector3::Forward * distance;
        else
        {
            auto lookAtDir = (position - center).Normalize();
            position = center + lookAtDir * distance;
        }

        if (pointOnSphere_->SetCenterAndPoint(center, position))
            camera_->SetGlobalPosition(position);
    }

    void CameraControl::Enable(bool enable)
    {
        enabled_ = enable;
    }

    void CameraControl::SetViewRect(const Vector4& viewRect)
    {
        viewRect_ = viewRect;
    }
}
