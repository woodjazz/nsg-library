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
#include "NSG.h"
using namespace NSG;

static void FrustumTest()
{
	PScene scene = std::make_shared<Scene>("scene000");

	{
		Vertex3 p0(0, 0, -1);
		Vertex3 p1(0, 1, -1);
		Vertex3 p2(1, 0, -1);
		Vector3 v1 = p1 - p0;
		Vector3 v2 = p2 - p0;
		Vector3 normal = glm::normalize(glm::cross(v1, v2));
		float d = -glm::dot(normal, p0);
		CHECK_ASSERT(normal == Vector3(0, 0, -1), __FILE__, __LINE__);
		CHECK_ASSERT(d == -1, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(normal, p0) + d == 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(normal, p1) + d == 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(normal, p2) + d == 0, __FILE__, __LINE__);
	}

	PCamera camera = scene->GetOrCreateChild<Camera>("camera");

	camera->SetGlobalLookAt(Vector3(0, 0, -1));

	{
		PFrustum frustum = camera->GetFrustum();
		const Plane& nearPlane = frustum->GetPlane(FrustumPlane::PLANE_NEAR);
		const Plane& leftPlane = frustum->GetPlane(FrustumPlane::PLANE_LEFT);
		const Plane& rightPlane = frustum->GetPlane(FrustumPlane::PLANE_RIGHT);
		const Plane& upPlane = frustum->GetPlane(FrustumPlane::PLANE_UP);
		const Plane& downPlane = frustum->GetPlane(FrustumPlane::PLANE_DOWN);
		const Plane& farPlane = frustum->GetPlane(FrustumPlane::PLANE_FAR);

		CHECK_ASSERT(glm::dot(Vector3(nearPlane.normald_), -WORLD_Z_COORD) == 1, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(leftPlane.normald_), WORLD_X_COORD) > 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(rightPlane.normald_), -WORLD_X_COORD) > 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(upPlane.normald_), -WORLD_Y_COORD) > 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(downPlane.normald_), WORLD_Y_COORD) > 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(farPlane.normald_), WORLD_Z_COORD) == 1, __FILE__, __LINE__);

		{
			Vector4 nearPlanePoint(camera->GetGlobalPosition() + Vector3(0, 0, -camera->GetZNear()), 1);
			float dot = glm::dot(nearPlane.normald_, nearPlanePoint);
			CHECK_ASSERT(glm::distance(dot, 0.0f) < 0.02f, __FILE__, __LINE__);
		}

		{
			Vector4 farPlanePoint(camera->GetGlobalPosition() + Vector3(0, 0, -camera->GetZFar()), 1);
			float dot = glm::dot(farPlane.normald_, farPlanePoint);
			CHECK_ASSERT(glm::distance(dot, 0.0f) < 0.02f, __FILE__, __LINE__);
		}

	}

	camera->SetPosition(Vertex3(0, 0, 100));
	camera->SetGlobalLookAt(Vector3(0, 0, -1));

	{
		PFrustum frustum = camera->GetFrustum();
		const Plane& nearPlane = frustum->GetPlane(FrustumPlane::PLANE_NEAR);
		const Plane& leftPlane = frustum->GetPlane(FrustumPlane::PLANE_LEFT);
		const Plane& rightPlane = frustum->GetPlane(FrustumPlane::PLANE_RIGHT);
		const Plane& upPlane = frustum->GetPlane(FrustumPlane::PLANE_UP);
		const Plane& downPlane = frustum->GetPlane(FrustumPlane::PLANE_DOWN);
		const Plane& farPlane = frustum->GetPlane(FrustumPlane::PLANE_FAR);

		CHECK_ASSERT(glm::dot(Vector3(nearPlane.normald_), -WORLD_Z_COORD) == 1, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(leftPlane.normald_), WORLD_X_COORD) > 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(rightPlane.normald_), -WORLD_X_COORD) > 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(upPlane.normald_), -WORLD_Y_COORD) > 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(downPlane.normald_), WORLD_Y_COORD) > 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(farPlane.normald_), WORLD_Z_COORD) == 1, __FILE__, __LINE__);

		{
			Vector4 nearPlanePoint(camera->GetGlobalPosition() + Vector3(0, 0, -camera->GetZNear()), 1);
			float dot = glm::dot(nearPlane.normald_, nearPlanePoint);
			CHECK_ASSERT(glm::distance(dot, 0.0f) < 0.02f, __FILE__, __LINE__);
		}

		{
			Vector4 farPlanePoint(camera->GetGlobalPosition() + Vector3(0, 0, -camera->GetZFar()), 1);
			float dot = glm::dot(farPlane.normald_, farPlanePoint);
			CHECK_ASSERT(glm::distance(dot, 0.0f) < 0.02f, __FILE__, __LINE__);
		}


	}

	camera->SetPosition(Vertex3(10, 0, 1));
	camera->SetGlobalLookAt(Vector3(10, 0, -1));

	{
		PFrustum frustum = camera->GetFrustum();
		const Plane& nearPlane = frustum->GetPlane(FrustumPlane::PLANE_NEAR);
		const Plane& leftPlane = frustum->GetPlane(FrustumPlane::PLANE_LEFT);
		const Plane& rightPlane = frustum->GetPlane(FrustumPlane::PLANE_RIGHT);
		const Plane& upPlane = frustum->GetPlane(FrustumPlane::PLANE_UP);
		const Plane& downPlane = frustum->GetPlane(FrustumPlane::PLANE_DOWN);
		const Plane& farPlane = frustum->GetPlane(FrustumPlane::PLANE_FAR);

		CHECK_ASSERT(glm::dot(Vector3(nearPlane.normald_), -WORLD_Z_COORD) == 1, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(leftPlane.normald_), WORLD_X_COORD) > 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(rightPlane.normald_), -WORLD_X_COORD) > 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(upPlane.normald_), -WORLD_Y_COORD) > 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(downPlane.normald_), WORLD_Y_COORD) > 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(farPlane.normald_), WORLD_Z_COORD) == 1, __FILE__, __LINE__);

		{
			Vector4 nearPlanePoint(camera->GetGlobalPosition() + Vector3(0, 0, -camera->GetZNear()), 1);
			float dot = glm::dot(nearPlane.normald_, nearPlanePoint);
			CHECK_ASSERT(glm::distance(dot, 0.0f) < 0.02f, __FILE__, __LINE__);
		}

		{
			Vector4 farPlanePoint(camera->GetGlobalPosition() + Vector3(0, 0, -camera->GetZFar()), 1);
			float dot = glm::dot(farPlane.normald_, farPlanePoint);
			CHECK_ASSERT(glm::distance(dot, 0.0f) < 0.02f, __FILE__, __LINE__);
		}

	}


	camera->SetPosition(Vertex3(0, 0, -1));
	camera->SetGlobalLookAt(Vector3(0, 0, 1));

	{
		PFrustum frustum = camera->GetFrustum();
		const Plane& nearPlane = frustum->GetPlane(FrustumPlane::PLANE_NEAR);
		const Plane& leftPlane = frustum->GetPlane(FrustumPlane::PLANE_LEFT);
		const Plane& rightPlane = frustum->GetPlane(FrustumPlane::PLANE_RIGHT);
		const Plane& upPlane = frustum->GetPlane(FrustumPlane::PLANE_UP);
		const Plane& downPlane = frustum->GetPlane(FrustumPlane::PLANE_DOWN);
		const Plane& farPlane = frustum->GetPlane(FrustumPlane::PLANE_FAR);

		CHECK_ASSERT(glm::dot(Vector3(nearPlane.normald_), WORLD_Z_COORD) == 1, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(leftPlane.normald_), -WORLD_X_COORD) > 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(rightPlane.normald_), WORLD_X_COORD) > 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(upPlane.normald_), -WORLD_Y_COORD) > 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(downPlane.normald_), WORLD_Y_COORD) > 0, __FILE__, __LINE__);
		CHECK_ASSERT(glm::dot(Vector3(farPlane.normald_), -WORLD_Z_COORD) == 1, __FILE__, __LINE__);

		{
			Vector4 nearPlanePoint(camera->GetGlobalPosition() + Vector3(0, 0, +camera->GetZNear()), 1);
			float dot = glm::dot(nearPlane.normald_, nearPlanePoint);
			CHECK_ASSERT(glm::distance(dot, 0.0f) < 0.02f, __FILE__, __LINE__);
		}

		{
			Vector4 farPlanePoint(camera->GetGlobalPosition() + Vector3(0, 0, +camera->GetZFar()), 1);
			float dot = glm::dot(farPlane.normald_, farPlanePoint);
			CHECK_ASSERT(glm::distance(dot, 0.0f) < 0.02f, __FILE__, __LINE__);
		}

	}
}

static void Test01()
{
	PScene scene = std::make_shared<Scene>("scene000");

	PSphereMesh sphere(App::this_->CreateSphereMesh(1, 100));

    {

        Node node;
		PCamera camera = scene->GetOrCreateChild<Camera>("camera");
        camera->SetFOV(45);
        camera->SetNearClip(0.1f);
        camera->SetFarClip(10);
        PFrustum frustum = camera->GetFrustum();
        CHECK_ASSERT(frustum->IsVisible(node, *sphere), __FILE__, __LINE__);
        node.SetPosition(Vertex3(20));
        CHECK_ASSERT(!frustum->IsVisible(node, *sphere), __FILE__, __LINE__);
    }

    {
        Node node;
        node.SetPosition(Vertex3(20));
        node.SetScale(Vector3(100));

		PCamera camera = scene->GetOrCreateChild<Camera>("camera");
        camera->SetFOV(45);
        camera->SetNearClip(0.1f);
        camera->SetFarClip(10);
        PFrustum frustum = camera->GetFrustum();
        CHECK_ASSERT(frustum->IsVisible(node, *sphere), __FILE__, __LINE__);
        node.SetPosition(Vertex3(200));
        CHECK_ASSERT(!frustum->IsVisible(node, *sphere), __FILE__, __LINE__);
    }

    {
        Node node;
		PCamera camera = scene->GetOrCreateChild<Camera>("camera");
        camera->SetFOV(45);
        camera->SetNearClip(0.1f);
        camera->SetFarClip(250);
        camera->SetPosition(Vertex3(0, 0, 10));
		camera->SetGlobalLookAt(Vertex3(0));
        PFrustum frustum = camera->GetFrustum();
        CHECK_ASSERT(frustum->IsVisible(node, *sphere), __FILE__, __LINE__);
        camera->SetPosition(Vertex3(0, 0, -1.1f));
		camera->SetGlobalLookAt(Vertex3(0, 0, -2));
		frustum = camera->GetFrustum();
        CHECK_ASSERT(!frustum->IsVisible(node, *sphere), __FILE__, __LINE__);
    }


    {
        Node node;
        node.SetPosition(Vertex3(0, 0, -40));

		PCamera camera = scene->GetOrCreateChild<Camera>("camera");
        camera->SetFOV(100);
        camera->SetNearClip(0.1f);
        camera->SetFarClip(1000);
        CHECK_ASSERT(camera->IsVisible(node, *sphere), __FILE__, __LINE__);
        camera->SetNearClip(41.1f);
        CHECK_ASSERT(!camera->IsVisible(node, *sphere), __FILE__, __LINE__);
    }

    {
        Node node;
        node.SetPosition(Vertex3(1.1f, 0, -1.1f));

		PCamera camera = scene->GetOrCreateChild<Camera>("camera");
        camera->SetFOV(170);
        camera->SetNearClip(0.1f);
        camera->SetFarClip(1000);
        CHECK_ASSERT(camera->IsVisible(node, *sphere), __FILE__, __LINE__);
        camera->SetFOV(0.0001f);
        CHECK_ASSERT(!camera->IsVisible(node, *sphere), __FILE__, __LINE__);
        node.SetPosition(Vertex3(0, 0, -40));
        CHECK_ASSERT(camera->IsVisible(node, *sphere), __FILE__, __LINE__);
		camera->SetGlobalLookAt(Vertex3(-1, 0, 0));
        CHECK_ASSERT(!camera->IsVisible(node, *sphere), __FILE__, __LINE__);
		camera->SetGlobalLookAt(Vertex3(0, 0, 1));
        CHECK_ASSERT(!camera->IsVisible(node, *sphere), __FILE__, __LINE__);
        camera->SetPosition(Vertex3(0, 0, -4));
		node.SetPosition(Vertex3(0, 0, -1));
        CHECK_ASSERT(camera->IsVisible(node, *sphere), __FILE__, __LINE__);
        camera->SetFarClip(1.9f);
        CHECK_ASSERT(!camera->IsVisible(node, *sphere), __FILE__, __LINE__);
        camera->SetFarClip(4);
        CHECK_ASSERT(camera->IsVisible(node, *sphere), __FILE__, __LINE__);
		node.SetPosition(Vertex3(-1, 0, -1));
        node.SetScale(Vector3(0.1f));
        CHECK_ASSERT(!camera->IsVisible(node, *sphere), __FILE__, __LINE__);
    }
}

static void Test02()
{
    {
        PBoxMesh box(App::this_->CreateBoxMesh(2, 4, 2));
        Node node;
		PScene scene = std::make_shared<Scene>("scene000");

		PCamera camera = scene->GetOrCreateChild<Camera>(GetUniqueName("camera"));
        camera->SetFOV(179);
		camera->SetNearClip(0.1f);
		camera->SetFarClip(10);
		camera->SetPosition(Vertex3(1, 0, 0));
		CHECK_ASSERT(camera->IsVisible(node, *box), __FILE__, __LINE__);
		camera->SetPosition(Vertex3(0, 0, -1.1f));
		CHECK_ASSERT(!camera->IsVisible(node, *box), __FILE__, __LINE__);
        node.SetOrientation(glm::angleAxis(glm::pi<float>() / 2, Vertex3(1, 0, 0)));
		CHECK_ASSERT(camera->IsVisible(node, *box), __FILE__, __LINE__);
        node.SetPosition(Vertex3(1, 0, 2));
		CHECK_ASSERT(!camera->IsVisible(node, *box), __FILE__, __LINE__);
    }
}

static void Test03()
{
	{
		PScene scene = std::make_shared<Scene>("scene000");

		PCamera camera = scene->GetOrCreateChild<Camera>(GetUniqueName("camera"));
		camera->EnableOrtho();
		float cameraDepth = camera->GetZFar() - camera->GetZNear();

		{
			Vertex3 p0(0, 0, -10);
			Vertex3 screenPoint0 = camera->WorldToScreen(p0);
			//Vertex3 w0 = camera->ScreenToWorld(Vector3(screenPoint0));

			Vertex3 p1(0, 0, -100);
			Vertex3 screenPoint1 = camera->WorldToScreen(p1);

			Vertex3 p2(0, 0, -200);
			Vertex3 screenPoint2 = camera->WorldToScreen(p2);

			Vertex3 p3(0, 0, -400);
			Vertex3 screenPoint3 = camera->WorldToScreen(p3);


		}

		{
			Vertex3 p0(0, 0, -1);
			Vertex3 worldPoint = camera->ScreenToWorld(p0);
			Vertex3 screenPoint = camera->WorldToScreen(worldPoint);
			CHECK_ASSERT(glm::distance(screenPoint, p0) < PRECISION, __FILE__, __LINE__);
		}



		{
			Vertex3 p0(-1, 0, -10);
			Vertex3 screenPoint = camera->WorldToScreen(p0);
			CHECK_ASSERT(glm::abs(-1 - screenPoint.x) < glm::epsilon<float>(), __FILE__, __LINE__);
			Vertex3 worldPos = camera->ScreenToWorld(screenPoint);
			CHECK_ASSERT(glm::distance(worldPos, p0) < PRECISION, __FILE__, __LINE__);
		}

		{
			Vertex3 p0(-1, 1, -10);
			Vertex3 screenPoint = camera->WorldToScreen(p0);
			CHECK_ASSERT(glm::abs(-1 - screenPoint.x) < glm::epsilon<float>(), __FILE__, __LINE__);
			CHECK_ASSERT(glm::abs(1 - screenPoint.y) < glm::epsilon<float>(), __FILE__, __LINE__);
			Vertex3 worldPos = camera->ScreenToWorld(screenPoint);
			CHECK_ASSERT(glm::distance(worldPos, p0) < PRECISION, __FILE__, __LINE__);
		}

		camera->DisableOrtho();

		{
			Vertex3 p0(0, 0, -1);
			Vertex3 worldPoint = camera->ScreenToWorld(p0);
			Vertex3 screenPoint = camera->WorldToScreen(worldPoint);
			CHECK_ASSERT(glm::distance(screenPoint, p0) < PRECISION, __FILE__, __LINE__);
		}

		{
			Vertex3 p0(-1, 0, -110.3f);
			Vertex3 screenPoint = camera->WorldToScreen(p0);
			CHECK_ASSERT(screenPoint.x < 0 && screenPoint.x > -0.5f, __FILE__, __LINE__);
			Vertex3 worldPos = camera->ScreenToWorld(screenPoint);
			CHECK_ASSERT(glm::distance(worldPos, p0) < 0.05f, __FILE__, __LINE__);
		}

		{
			Vertex3 p0(-1, 1, -10);
			Vertex3 screenPoint = camera->WorldToScreen(p0);
			CHECK_ASSERT(screenPoint.x < 0 && screenPoint.x > -0.5f, __FILE__, __LINE__);
			//CHECK_ASSERT(screenPoint.y > 0 && screenPoint.y < 0.5f, __FILE__, __LINE__);
			Vertex3 worldPos = camera->ScreenToWorld(screenPoint);
			CHECK_ASSERT(glm::distance(worldPos, p0) < 0.1f, __FILE__, __LINE__);
		}


		{
			Vertex3 p0(1, 0, -10);
			Vertex3 screenPoint = camera->WorldToScreen(p0);
			CHECK_ASSERT(screenPoint.x > 0 && screenPoint.x < 0.5f, __FILE__, __LINE__);
			Vertex3 worldPos = camera->ScreenToWorld(screenPoint);
			CHECK_ASSERT(glm::distance(worldPos, p0) < 0.05f, __FILE__, __LINE__);
		}

		{
			Vertex3 p0(1, 0, 10); // behind the screen
			Vertex3 screenPoint = camera->WorldToScreen(p0);
			//CHECK_ASSERT(screenPoint.x > 0 && screenPoint.x < 0.5f, __FILE__, __LINE__);
			Vertex3 worldPos = camera->ScreenToWorld(screenPoint);
			CHECK_ASSERT(glm::distance(worldPos, p0) < 0.1f, __FILE__, __LINE__);
		}


	}
}

static void Test04()
{
#if 1
	PScene scene = std::make_shared<Scene>("scene000");

	PCamera camera = scene->GetOrCreateChild<Camera>(GetUniqueName("camera"));
	Vertex3 position(0, 0, 0);
	camera->SetPosition(position);

	{
		Ray ray = camera->GetScreenRay(0, 0);
		CHECK_ASSERT(glm::distance(ray.GetDirection(), VECTOR3_FORWARD) < glm::epsilon<float>(), __FILE__, __LINE__);
		CHECK_ASSERT(glm::distance(ray.GetOrigin(), position) < 0.5f, __FILE__, __LINE__);
	}

	{
		Ray ray = camera->GetScreenRay(-0.9f, 0);
		CHECK_ASSERT(glm::distance(ray.GetOrigin(), position) < 1, __FILE__, __LINE__);
		CHECK_ASSERT(ray.GetDirection().x < 0, __FILE__, __LINE__);
		CHECK_ASSERT(ray.GetDirection().y == 0, __FILE__, __LINE__);
		CHECK_ASSERT(ray.GetDirection().z < 0, __FILE__, __LINE__);
	}

	{
		Ray ray = camera->GetScreenRay(1, 0);
		CHECK_ASSERT(glm::distance(ray.GetOrigin(), position) < 1, __FILE__, __LINE__);
		CHECK_ASSERT(ray.GetDirection().x > 0, __FILE__, __LINE__);
		CHECK_ASSERT(ray.GetDirection().y == 0, __FILE__, __LINE__);
		CHECK_ASSERT(ray.GetDirection().z < 0, __FILE__, __LINE__);
	}

	{
		Ray ray = camera->GetScreenRay(0, 1);
		CHECK_ASSERT(glm::distance(ray.GetOrigin(), position) < 1, __FILE__, __LINE__);
		CHECK_ASSERT(ray.GetDirection().x == 0, __FILE__, __LINE__);
		CHECK_ASSERT(ray.GetDirection().y > 0.3f, __FILE__, __LINE__);
		CHECK_ASSERT(ray.GetDirection().z < -0.9f, __FILE__, __LINE__);
	}

	{
		Ray ray = camera->GetScreenRay(0, -1);
		CHECK_ASSERT(glm::distance(ray.GetOrigin(), position) < 1, __FILE__, __LINE__);
		CHECK_ASSERT(ray.GetDirection().x == 0, __FILE__, __LINE__);
		CHECK_ASSERT(ray.GetDirection().y < 0.3f, __FILE__, __LINE__);
		CHECK_ASSERT(ray.GetDirection().z < -0.9f, __FILE__, __LINE__);
	}

	{
		camera->SetGlobalLookAt(position + Vector3(-1, 0, 0));
		Ray ray = camera->GetScreenRay(0, 0);
		CHECK_ASSERT(glm::distance(ray.GetOrigin(), position) < 1, __FILE__, __LINE__);
		CHECK_ASSERT(glm::distance(ray.GetDirection(), -VECTOR3_RIGHT) < 0.05f, __FILE__, __LINE__);
	}
#endif
}

static void Test05()
{
	{
		PBoxMesh box(App::this_->CreateBoxMesh(1, 1, 1));
		CHECK_ASSERT(box->IsReady(), __FILE__, __LINE__);
		PScene scene = std::make_shared<Scene>("scene000");

		PCamera camera = scene->GetOrCreateChild<Camera>(GetUniqueName("camera"));
		camera->SetNearClip(0.1f);
		camera->SetFarClip(250);
		camera->SetPosition(Vertex3(0, 0, -4));
		camera->SetGlobalLookAt(-VECTOR3_FORWARD);
		BoundingBox bb = box->GetBB();
		CHECK_ASSERT(camera->GetFrustum()->IsInside(bb) == Intersection::INSIDE, __FILE__, __LINE__);
		camera->SetPosition(Vertex3(0, 0, -250));
		CHECK_ASSERT(camera->GetFrustum()->IsInside(bb) == Intersection::INTERSECTS, __FILE__, __LINE__);
		camera->SetPosition(Vertex3(0, 0, -400));
		CHECK_ASSERT(camera->GetFrustum()->IsInside(bb) == Intersection::OUTSIDE, __FILE__, __LINE__);
	}
}


void CameraTest()
{
	App app;
	auto window = app.GetOrCreateWindow("window", 0, 0, 1, 1);
	FrustumTest();
    Test01();
    Test02();
	Test03();
	Test04();
	Test05();
}