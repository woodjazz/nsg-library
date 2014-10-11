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
#include "Sphere.h"
#include "Keys.h"

namespace NSG
{
    void CameraControl::Start()
    {
		lastX_ = lastY_ = 0;
        camera_ = dynamic_cast<Camera*>(sceneNode_);
        CHECK_ASSERT(camera_, __FILE__, __LINE__);
        sphere_ = PSphere(new Sphere(Vertex3(0), sceneNode_->GetGlobalPosition()));
        updateOrientation_ = false;
        rightButtonDown_ = false;
        altKeyDown_ = false;
        SetSphereCenter(true);
    }

    void CameraControl::Update()
    {
        if (updateOrientation_)
        {
            Quaternion targetOrientation = sceneNode_->GetLookAtOrientation(sphere_->GetCenter(), sphere_->GetUp());
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

    void CameraControl::OnMouseMove(float x, float y)
    {
        if (rightButtonDown_ && !updateOrientation_)
        {
            float relX = (x - lastX_);
            float relY = (y - lastY_);
            if (altKeyDown_)
            {
                sphere_->IncAngles(PI * relX, PI * relY);
                sceneNode_->SetGlobalPosition(sphere_->GetPosition());
                sceneNode_->SetLookAt(sphere_->GetCenter(), sphere_->GetUp());
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

        lastX_ = x;
        lastY_ = y;
    }

    void CameraControl::OnMouseWheel(float x, float y)
    {
        float deltaTime = App::this_->GetDeltaTime();
        Vertex3 position = sceneNode_->GetGlobalPosition();
        Vertex3 lookAtPoint = sphere_->GetCenter();
        float distance = glm::distance(lookAtPoint, position);
        distance /= 2 * y;
        Vector3 looAtDir = sceneNode_->GetLookAtDirection();
        position += looAtDir * distance;
        BoundingBox bb = scene_.GetWorldBoundingBoxBut(camera_);
        float dmin = glm::distance(bb.min_, position);
        float dmax = glm::distance(bb.max_, position);
        if (dmin < camera_->GetZFar() && dmax < camera_->GetZFar())
        {
            if (sphere_->SetPosition(position))
                sceneNode_->SetGlobalPosition(position);
        }
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
                TRACE_LOG("***1***");
                newCenter = closest.node_->GetGlobalPosition();
            }
            else
            {
                TRACE_LOG("***2***");
                newCenter = ray.GetPoint(closest.distance_);
            }
        }
        else
        {
            TRACE_LOG("***3***");
            BoundingBox bb;
            if (!scene_.GetVisibleBoundingBox(camera_, bb))
                bb = scene_.GetWorldBoundingBoxBut(camera_);
            newCenter = bb.Center();
        }

		//TRACE_LOG("1--->Up=" << sphere_->GetUp());
		//TRACE_LOG("1--->Theta=" << sphere_->GetTheta());
		//TRACE_LOG("1--->Phi=" << sphere_->GetPhi());
        if (sphere_->SetCenter(newCenter))
            updateOrientation_ = true;

		//TRACE_LOG("2--->Up=" << sphere_->GetUp());
		//TRACE_LOG("2--->Theta=" << sphere_->GetTheta());
		//TRACE_LOG("2--->Phi=" << sphere_->GetPhi());
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

		position = center + lookAtDir * distance;

		if(sphere_->SetCenterAndPosition(center, position))
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
