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
#include "CameraControl.h"
#include "Camera.h"
#include "Ray.h"
#include "PointOnSphere.h"
#include "Keys.h"

namespace NSG
{
    void CameraControl::Start()
    {
        lastX_ = lastY_ = 0;
        camera_ = dynamic_cast<Camera*>(sceneNode_);
        CHECK_ASSERT(camera_, __FILE__, __LINE__);
        pointOnSphere_ = PPointOnSphere(new PointOnSphere(Vertex3(0), sceneNode_->GetGlobalPosition()));
        updateOrientation_ = false;
        rightButtonDown_ = false;
        altKeyDown_ = false;
        SetSphereCenter(true);
    }

    void CameraControl::Update()
    {
        if (updateOrientation_)
        {
            Quaternion targetOrientation = sceneNode_->GetLookAtOrientation(pointOnSphere_->GetCenter(), pointOnSphere_->GetUp());
            Quaternion currentOrientation = sceneNode_->GetOrientation();
            float dot = glm::dot(currentOrientation, targetOrientation);
            bool close = 1 - dot * dot < PRECISION;
            if (!close)
            {
                //animate
                float factor = 3 * app_.GetDeltaTime();
                sceneNode_->SetOrientation(glm::slerp(currentOrientation, targetOrientation, factor));
            }
            else
            {
                sceneNode_->SetOrientation(targetOrientation);
                updateOrientation_ = false;
            }

        }
    }

    void CameraControl::OnMouseDown(int button, float x, float y)
    {
        lastX_ = x;
        lastY_ = y;

        if (button == NSG_BUTTON_RIGHT && !rightButtonDown_)
        {
            rightButtonDown_ = true;
        }
    }

    void CameraControl::OnMouseUp(int button, float x, float y)
    {
        if (button == NSG_BUTTON_RIGHT)
            rightButtonDown_ = false;
    }

    void CameraControl::Move(float x, float y)
    {
        float relX = (x - lastX_);
        float relY = (y - lastY_);
        if (altKeyDown_)
        {
            pointOnSphere_->IncAngles(PI * relX, PI * relY);
            sceneNode_->SetGlobalPosition(pointOnSphere_->GetPoint());
            sceneNode_->SetLookAt(pointOnSphere_->GetCenter(), pointOnSphere_->GetUp());
        }
        else
        {
            float deltaTime = App::this_->GetDeltaTime();
            Vertex3 position = sceneNode_->GetGlobalPosition();
            Quaternion q = sceneNode_->GetOrientation();
            q = q * glm::angleAxis(deltaTime * relX * 100, Vertex3(0, 1, 0));
            q = q * glm::angleAxis(deltaTime * relY * 100, Vertex3(1, 0, 0));
            sceneNode_->SetOrientation(q);
        }
    }


    void CameraControl::OnMouseMove(float x, float y)
    {
        if (rightButtonDown_ && !updateOrientation_)
        {
            Move(x, y);
        }

        lastX_ = x;
        lastY_ = y;
    }

    void CameraControl::SetPosition(const Vertex3& position)
    {
        Vertex3 oldPos = sceneNode_->GetGlobalPosition();
        sceneNode_->SetGlobalPosition(position);
        BoundingBox bb = scene_.GetWorldBoundingBoxBut(camera_);
        if (camera_->GetFrustum()->IsInside(bb) == Intersection::OUTSIDE)
            sceneNode_->SetGlobalPosition(oldPos);
    }

    void CameraControl::OnMouseWheel(float x, float y)
    {
        Vertex3 lookAtPoint = pointOnSphere_->GetCenter();
        Vertex3 position = sceneNode_->GetGlobalPosition();
        Vector3 looAtDir = sceneNode_->GetLookAtDirection();
        float distance = glm::distance(lookAtPoint, position);
        if (distance < 1)
            distance = 1;
        float factor = y > 0 ? 2.0f : -2.0f;
        distance /= factor;
        position += looAtDir * distance;
        SetPosition(position);
    }

    void CameraControl::OnMultiGesture(int timestamp, float x, float y, float dTheta, float dDist, int numFingers)
    {
        //TRACE_LOG("x=" << x << " y=" << y << " dTheta=" << dTheta << " dDist=" << dDist << " num=" << numFingers);
        if (numFingers == 2)
        {
            float factor = dDist * timestamp / 10.0f;
            Vertex3 position = sceneNode_->GetGlobalPosition();
            Vector3 looAtDir = sceneNode_->GetLookAtDirection();
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

    void CameraControl::SetSphereCenter(bool centerObj)
    {
        Vertex3 newCenter;
        Ray ray = camera_->GetScreenRay(lastX_, lastY_);
        RayNodeResult closest;
        if (scene_.GetClosestRayNodeIntersection(ray, closest))
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
            if (!scene_.GetVisibleBoundingBox(camera_, bb))
                bb = scene_.GetWorldBoundingBoxBut(camera_);
            newCenter = bb.Center();
        }

        if (pointOnSphere_->SetCenter(newCenter))
            updateOrientation_ = true;
    }

    void CameraControl::AutoZoom()
    {
        BoundingBox bb = scene_.GetWorldBoundingBoxBut(camera_);
        Vertex3 center = bb.Center();
        Vertex3 position = camera_->GetGlobalPosition();
        Vector3 lookAtDir(WORLD_Z_COORD);
        if (glm::distance(position, center) > PRECISION)
            lookAtDir = glm::normalize(position - center);
        float distance = std::max(std::max(bb.Size().x, bb.Size().y), bb.Size().z);
        if (distance < camera_->GetZNear())
            distance = 1 + camera_->GetZNear();

        position = center - lookAtDir * distance;

        if (pointOnSphere_->SetCenterAndPoint(center, position))
            camera_->SetGlobalPosition(position);
    }

    void CameraControl::OnKey(int key, int action, int modifier)
    {
        float deltaTime = App::this_->GetDeltaTime();

        switch (key)
        {
            case NSG_KEY_W:
                {
                    break;
                }

            case NSG_KEY_S:
                {
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

            case NSG_KEY_LALT:
                {
                    altKeyDown_ = action ? true : false;
                    break;
                }

        }
    }

}
