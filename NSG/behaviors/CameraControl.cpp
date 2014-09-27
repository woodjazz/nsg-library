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
		rotateAround_ = false;

		Vertex3 center;
		FindLookAtPoint(true, center);
		sphere_ = PSphere(new Sphere(center, sceneNode_->GetGlobalPosition()));

		rightButtonDown_ = false;
	}

	void CameraControl::Update()
    {
    }

	void CameraControl::OnMouseDown(int button, float x, float y)
    {
		lastX_ = x;
		lastY_ = y;

		if (button == NSG_BUTTON_RIGHT)
			rightButtonDown_ = true;
    }

	void CameraControl::OnMouseUp(int button, float x, float y)
	{
		if (button == NSG_BUTTON_RIGHT)
			rightButtonDown_ = false;
	}

	void CameraControl::OnMouseMove(float x, float y)
	{
		if (rightButtonDown_)
		{
			float relX = (x - lastX_);
			float relY = (y - lastY_);
			if (rotateAround_)
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
		Quaternion q = sceneNode_->GetGlobalOrientation();
		position += q * VECTOR3_FORWARD * deltaTime * y * 100.0f;
		sceneNode_->SetGlobalPosition(position);
	}

	bool CameraControl::FindLookAtPoint(bool centerObj, Vertex3& point)
	{
		Camera* camera = Camera::GetActiveCamera();

		if (!camera)
			return false;

		std::vector<RayNodeResult> result;
		PRay ray = camera->GetScreenRay(lastX_, lastY_);
		RayNodeResult closest;
		if (scene_.GetClosestRayNodeIntersection(*ray, closest))
		{
			if (centerObj)
				point = closest.node_->GetGlobalPosition();
			else
				point = ray->GetPoint(closest.distance_);
		}
		else
		{
			std::vector<const SceneNode*> visibles;
			scene_.GetVisibleNodes(camera, visibles);
			BoundingBox box;
			for (auto& obj : visibles)
				box.Merge(obj->GetWorldBoundingBox());

			float distance = glm::distance(camera->GetGlobalPosition(), box.Center());
			point = camera->GetGlobalPosition() + camera->GetGlobalOrientation() * VECTOR3_FORWARD * distance;
		}

		return true;
	}

	void CameraControl::OnKey(int key, int action, int modifier)
	{
		float deltaTime = App::this_->GetDeltaTime();

		switch (key)
		{
		case NSG_KEY_W:
		{
			Vertex3 position = sceneNode_->GetGlobalPosition();
			Quaternion q = sceneNode_->GetGlobalOrientation();
			position += q * VECTOR3_FORWARD * deltaTime;
			sceneNode_->SetGlobalPosition(position);
			break;
		}

		case NSG_KEY_S:
		{
			Vertex3 position = sceneNode_->GetGlobalPosition();
			Quaternion q = sceneNode_->GetGlobalOrientation();
			position -= q * VECTOR3_FORWARD * deltaTime;
			sceneNode_->SetGlobalPosition(position);
			break;
		}

		case NSG_KEY_A:
		{
			Vertex3 position = sceneNode_->GetGlobalPosition();
			Quaternion q = sceneNode_->GetOrientation();
			q = q * glm::angleAxis(deltaTime * glm::radians(10.0f), Vertex3(0, 1, 0));
			sceneNode_->SetOrientation(q);
			break;
		}

		case NSG_KEY_D:
		{
			Vertex3 position = sceneNode_->GetGlobalPosition();
			Quaternion q = sceneNode_->GetOrientation();
			q = q * glm::angleAxis(deltaTime * glm::radians(-10.0f), Vertex3(0, 1, 0));
			sceneNode_->SetOrientation(q);
			break;
		}

		case NSG_KEY_Q:
		{
			Vertex3 position = sceneNode_->GetGlobalPosition();
			Quaternion q = sceneNode_->GetGlobalOrientation();
			position += q * VECTOR3_UP * deltaTime;
			sceneNode_->SetGlobalPosition(position);
			break;
		}

		case NSG_KEY_E:
		{
			Vertex3 position = sceneNode_->GetGlobalPosition();
			Quaternion q = sceneNode_->GetGlobalOrientation();
			position -= q * VECTOR3_UP * deltaTime;
			sceneNode_->SetGlobalPosition(position);
			break;
		}

		case NSG_KEY_F:
		{
			Vertex3 center;
			if(FindLookAtPoint(false, center))
				sphere_->SetCenter(center);
			break;
		}

		case NSG_KEY_C:
		{
			Vertex3 center;
			if (FindLookAtPoint(true, center))
				sphere_->SetCenter(center);
			break;
		}

		case NSG_KEY_LALT:
		{
			if (action && !rotateAround_)
			{
				rotateAround_ = true;
				sceneNode_->SetGlobalPosition(sphere_->GetPosition());
				sceneNode_->SetLookAt(sphere_->GetCenter(), sphere_->GetUp());
			}
			else if (!action)
			{
				rotateAround_ = false;
			}

			break;
		}

		}
	}

}
