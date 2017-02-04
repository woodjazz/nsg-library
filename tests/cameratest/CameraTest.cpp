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
#include "NSG.h"
using namespace NSG;

static const Vector3 WORLD_X_COORD(1, 0, 0);
static const Vector3 WORLD_Y_COORD(0, 1, 0);
static const Vector3 WORLD_Z_COORD(0, 0, 1);

static void FrustumTest()
{
    PScene scene = std::make_shared<Scene>("scene000");

    {
        Vertex3 p0(0, 0, -1);
        Vertex3 p1(0, 1, -1);
        Vertex3 p2(1, 0, -1);
        Vector3 v1 = p1 - p0;
        Vector3 v2 = p2 - p0;
        Vector3 normal = v1.Cross(v2).Normalize();
        float d = -normal.Dot(p0);
        CHECK_CONDITION(normal == Vector3(0, 0, -1));
        CHECK_CONDITION(d == -1);
        CHECK_CONDITION(normal.Dot(p0) + d == 0);
        CHECK_CONDITION(normal.Dot(p1) + d == 0);
        CHECK_CONDITION(normal.Dot(p2) + d == 0);
    }

    PCamera camera = scene->GetOrCreateChild<Camera>("camera");
    camera->SetGlobalLookAtPosition(Vector3(0, 0, -1));

    {
        PFrustum frustum = camera->GetFrustum();
        const Plane& nearPlane = frustum->GetPlane(FrustumPlane::PLANE_NEAR);
        const Plane& leftPlane = frustum->GetPlane(FrustumPlane::PLANE_LEFT);
        const Plane& rightPlane = frustum->GetPlane(FrustumPlane::PLANE_RIGHT);
        const Plane& upPlane = frustum->GetPlane(FrustumPlane::PLANE_UP);
        const Plane& downPlane = frustum->GetPlane(FrustumPlane::PLANE_DOWN);
        const Plane& farPlane = frustum->GetPlane(FrustumPlane::PLANE_FAR);

		CHECK_CONDITION(Vector3(nearPlane.GetNormalD()).Dot(-WORLD_Z_COORD) == 1);
		CHECK_CONDITION(Vector3(leftPlane.GetNormalD()).Dot(WORLD_X_COORD) > 0);
		CHECK_CONDITION(Vector3(rightPlane.GetNormalD()).Dot(-WORLD_X_COORD) > 0);
		CHECK_CONDITION(Vector3(upPlane.GetNormalD()).Dot(-WORLD_Y_COORD) > 0);
		CHECK_CONDITION(Vector3(downPlane.GetNormalD()).Dot(WORLD_Y_COORD) > 0);
		CHECK_CONDITION(Vector3(farPlane.GetNormalD()).Dot(WORLD_Z_COORD) == 1);

        {
            Vector4 nearPlanePoint(camera->GetGlobalPosition() + Vector3(0, 0, -camera->GetZNear()), 1);
			float dot = nearPlane.GetNormalD().Dot(nearPlanePoint);
            CHECK_CONDITION(Distance(dot, 0.0f) < 0.02f);
        }

        {
            Vector4 farPlanePoint(camera->GetGlobalPosition() + Vector3(0, 0, -camera->GetZFar()), 1);
			float dot = farPlane.GetNormalD().Dot(farPlanePoint);
            CHECK_CONDITION(Distance(dot, 0.0f) < 0.02f);
        }

    }

    camera->SetPosition(Vertex3(0, 0, 100));
    camera->SetGlobalLookAtPosition(Vector3(0, 0, -1));

    {
        PFrustum frustum = camera->GetFrustum();
        const Plane& nearPlane = frustum->GetPlane(FrustumPlane::PLANE_NEAR);
        const Plane& leftPlane = frustum->GetPlane(FrustumPlane::PLANE_LEFT);
        const Plane& rightPlane = frustum->GetPlane(FrustumPlane::PLANE_RIGHT);
        const Plane& upPlane = frustum->GetPlane(FrustumPlane::PLANE_UP);
        const Plane& downPlane = frustum->GetPlane(FrustumPlane::PLANE_DOWN);
        const Plane& farPlane = frustum->GetPlane(FrustumPlane::PLANE_FAR);

		CHECK_CONDITION(Vector3(nearPlane.GetNormalD()).Dot(-WORLD_Z_COORD) == 1);
		CHECK_CONDITION(Vector3(leftPlane.GetNormalD()).Dot(WORLD_X_COORD) > 0);
		CHECK_CONDITION(Vector3(rightPlane.GetNormalD()).Dot(-WORLD_X_COORD) > 0);
		CHECK_CONDITION(Vector3(upPlane.GetNormalD()).Dot(-WORLD_Y_COORD) > 0);
		CHECK_CONDITION(Vector3(downPlane.GetNormalD()).Dot(WORLD_Y_COORD) > 0);
		CHECK_CONDITION(Vector3(farPlane.GetNormalD()).Dot(WORLD_Z_COORD) == 1);

        {
            Vector4 nearPlanePoint(camera->GetGlobalPosition() + Vector3(0, 0, -camera->GetZNear()), 1);
			float dot = nearPlane.GetNormalD().Dot(nearPlanePoint);
            CHECK_CONDITION(Distance(dot, 0.0f) < 0.02f);
        }

        {
            Vector4 farPlanePoint(camera->GetGlobalPosition() + Vector3(0, 0, -camera->GetZFar()), 1);
			float dot = farPlane.GetNormalD().Dot(farPlanePoint);
            CHECK_CONDITION(Distance(dot, 0.0f) < 0.02f);
        }


    }

    camera->SetPosition(Vertex3(10, 0, 1));
	auto lookAt = Vector3(10, 0, -1);
	camera->SetGlobalLookAtPosition(lookAt);
	auto retrievedLookAt = camera->GetLookAtDirection();
	CHECK_CONDITION(Distance(retrievedLookAt, Vector3(0, 0, -1)) < 0.001f);

    {
        PFrustum frustum = camera->GetFrustum();
        const Plane& nearPlane = frustum->GetPlane(FrustumPlane::PLANE_NEAR);
        const Plane& leftPlane = frustum->GetPlane(FrustumPlane::PLANE_LEFT);
        const Plane& rightPlane = frustum->GetPlane(FrustumPlane::PLANE_RIGHT);
        const Plane& upPlane = frustum->GetPlane(FrustumPlane::PLANE_UP);
        const Plane& downPlane = frustum->GetPlane(FrustumPlane::PLANE_DOWN);
        const Plane& farPlane = frustum->GetPlane(FrustumPlane::PLANE_FAR);

		CHECK_CONDITION(Vector3(nearPlane.GetNormalD()).Dot(-WORLD_Z_COORD) == 1);
		CHECK_CONDITION(Vector3(leftPlane.GetNormalD()).Dot(WORLD_X_COORD) > 0);
		CHECK_CONDITION(Vector3(rightPlane.GetNormalD()).Dot(-WORLD_X_COORD) > 0);
		CHECK_CONDITION(Vector3(upPlane.GetNormalD()).Dot(-WORLD_Y_COORD) > 0);
		CHECK_CONDITION(Vector3(downPlane.GetNormalD()).Dot(WORLD_Y_COORD) > 0);
		CHECK_CONDITION(Vector3(farPlane.GetNormalD()).Dot(WORLD_Z_COORD) == 1);

        {
            Vector4 nearPlanePoint(camera->GetGlobalPosition() + Vector3(0, 0, -camera->GetZNear()), 1);
			float dot = nearPlane.GetNormalD().Dot(nearPlanePoint);
            CHECK_CONDITION(Distance(dot, 0.0f) < 0.02f);
        }

        {
            Vector4 farPlanePoint(camera->GetGlobalPosition() + Vector3(0, 0, -camera->GetZFar()), 1);
			float dot = farPlane.GetNormalD().Dot(farPlanePoint);
            CHECK_CONDITION(Distance(dot, 0.0f) < 0.02f);
        }

    }


    camera->SetPosition(Vertex3(0, 0, -1));
    camera->SetGlobalLookAtPosition(Vector3(0, 0, 1));

    {
        PFrustum frustum = camera->GetFrustum();
        const Plane& nearPlane = frustum->GetPlane(FrustumPlane::PLANE_NEAR);
        const Plane& leftPlane = frustum->GetPlane(FrustumPlane::PLANE_LEFT);
        const Plane& rightPlane = frustum->GetPlane(FrustumPlane::PLANE_RIGHT);
        const Plane& upPlane = frustum->GetPlane(FrustumPlane::PLANE_UP);
        const Plane& downPlane = frustum->GetPlane(FrustumPlane::PLANE_DOWN);
        const Plane& farPlane = frustum->GetPlane(FrustumPlane::PLANE_FAR);

		CHECK_CONDITION(Vector3(nearPlane.GetNormalD()).Dot(WORLD_Z_COORD) == 1);
		CHECK_CONDITION(Vector3(leftPlane.GetNormalD()).Dot(-WORLD_X_COORD) > 0);
		CHECK_CONDITION(Vector3(rightPlane.GetNormalD()).Dot(WORLD_X_COORD) > 0);
		CHECK_CONDITION(Vector3(upPlane.GetNormalD()).Dot(-WORLD_Y_COORD) > 0);
		CHECK_CONDITION(Vector3(downPlane.GetNormalD()).Dot(WORLD_Y_COORD) > 0);
		CHECK_CONDITION(Vector3(farPlane.GetNormalD()).Dot(-WORLD_Z_COORD) == 1);

        {
            Vector4 nearPlanePoint(camera->GetGlobalPosition() + Vector3(0, 0, +camera->GetZNear()), 1);
			float dot = nearPlane.GetNormalD().Dot(nearPlanePoint);
            CHECK_CONDITION(Distance(dot, 0.0f) < 0.02f);
        }

        {
            Vector4 farPlanePoint(camera->GetGlobalPosition() + Vector3(0, 0, +camera->GetZFar()), 1);
			float dot = farPlane.GetNormalD().Dot(farPlanePoint);
            CHECK_CONDITION(Distance(dot, 0.0f) < 0.02f);
        }

    }
}

static void Test01()
{
    PScene scene = std::make_shared<Scene>("scene000");

    PSphereMesh sphere(Mesh::Create<SphereMesh>());
    sphere->Set(1, 100);
    {

        Node node;
        PCamera camera = scene->GetOrCreateChild<Camera>("camera");
        camera->SetFOVDegrees(45);
        camera->SetNearClip(0.1f);
        camera->SetFarClip(10);
        PFrustum frustum = camera->GetFrustum();
        CHECK_CONDITION(frustum->IsVisible(node, *sphere));
        node.SetPosition(Vertex3(20));
        CHECK_CONDITION(!frustum->IsVisible(node, *sphere));
    }

    {
        Node node;
        node.SetPosition(Vertex3(20));
        node.SetScale(Vector3(100));

        PCamera camera = scene->GetOrCreateChild<Camera>("camera");
		camera->SetFOVDegrees(45);
        camera->SetNearClip(0.1f);
        camera->SetFarClip(10);
        PFrustum frustum = camera->GetFrustum();
        CHECK_CONDITION(frustum->IsVisible(node, *sphere));
        node.SetPosition(Vertex3(200));
        CHECK_CONDITION(!frustum->IsVisible(node, *sphere));
    }

    {
        Node node;
        PCamera camera = scene->GetOrCreateChild<Camera>("camera");
		camera->SetFOVDegrees(45);
        camera->SetNearClip(0.1f);
        camera->SetFarClip(250);
        camera->SetPosition(Vertex3(0, 0, 10));
        camera->SetGlobalLookAtPosition(Vertex3(0));
        PFrustum frustum = camera->GetFrustum();
        CHECK_CONDITION(frustum->IsVisible(node, *sphere));
        camera->SetPosition(Vertex3(0, 0, -1.1f));
        camera->SetGlobalLookAtPosition(Vertex3(0, 0, -2));
        frustum = camera->GetFrustum();
        CHECK_CONDITION(!frustum->IsVisible(node, *sphere));
    }


    {
        Node node;
        node.SetPosition(Vertex3(0, 0, -40));

        PCamera camera = scene->GetOrCreateChild<Camera>("camera");
		camera->SetFOVDegrees(100);
        camera->SetNearClip(0.1f);
        camera->SetFarClip(1000);
        CHECK_CONDITION(camera->IsVisible(node, *sphere));
        camera->SetNearClip(41.1f);
        CHECK_CONDITION(!camera->IsVisible(node, *sphere));
    }

    {
        Node node;
        node.SetPosition(Vertex3(1.1f, 0, -1.1f));

        PCamera camera = scene->GetOrCreateChild<Camera>("camera");
		camera->SetFOVDegrees(170);
        camera->SetNearClip(0.1f);
        camera->SetFarClip(1000);
        CHECK_CONDITION(camera->IsVisible(node, *sphere));
		camera->SetFOVDegrees(0.0001f);
        CHECK_CONDITION(!camera->IsVisible(node, *sphere));
        node.SetPosition(Vertex3(0, 0, -40));
        CHECK_CONDITION(camera->IsVisible(node, *sphere));
        camera->SetGlobalLookAtPosition(Vertex3(-1, 0, 0));
        CHECK_CONDITION(!camera->IsVisible(node, *sphere));
        camera->SetGlobalLookAtPosition(Vertex3(0, 0, 1));
        CHECK_CONDITION(!camera->IsVisible(node, *sphere));
        camera->SetPosition(Vertex3(0, 0, -4));
        node.SetPosition(Vertex3(0, 0, -1));
        CHECK_CONDITION(camera->IsVisible(node, *sphere));
        camera->SetFarClip(1.9f);
        CHECK_CONDITION(!camera->IsVisible(node, *sphere));
        camera->SetFarClip(4);
        CHECK_CONDITION(camera->IsVisible(node, *sphere));
        node.SetPosition(Vertex3(-1, 0, -1));
        node.SetScale(Vector3(0.1f));
        CHECK_CONDITION(!camera->IsVisible(node, *sphere));
    }
}

static void Test02()
{
    {
        PBoxMesh box(Mesh::Create<BoxMesh>());
        box->Set(2, 4, 2);
        Node node;
        PScene scene = std::make_shared<Scene>("scene000");

        PCamera camera = scene->GetOrCreateChild<Camera>(GetUniqueName("camera"));
		camera->SetFOVDegrees(179);
        camera->SetNearClip(0.1f);
        camera->SetFarClip(10);
        camera->SetPosition(Vertex3(1, 0, 0));
        CHECK_CONDITION(camera->IsVisible(node, *box));
        camera->SetPosition(Vertex3(0, 0, -1.1f));
        CHECK_CONDITION(!camera->IsVisible(node, *box));
        node.SetOrientation(Quaternion(PI / 2, Vertex3(1, 0, 0)));
        CHECK_CONDITION(camera->IsVisible(node, *box));
        node.SetPosition(Vertex3(1, 0, 2));
        CHECK_CONDITION(!camera->IsVisible(node, *box));
    }
}

static void Test03()
{
    {
        PScene scene = std::make_shared<Scene>("scene000");

        PCamera camera = scene->GetOrCreateChild<Camera>(GetUniqueName("camera"));
        camera->EnableOrtho();
        camera->SetAspectRatio(1);

        {
            Vertex3 p0(0, 0, -1);
            Vertex3 worldPoint = camera->ScreenToWorld(p0);
            Vertex3 screenPoint = camera->WorldToScreen(worldPoint);
            CHECK_CONDITION(Distance(screenPoint, p0) < PRECISION);
        }



        {
            Vertex3 p0(-1, 0, -10);
            Vertex3 screenPoint = camera->WorldToScreen(p0);
            CHECK_CONDITION(Abs(-1 - screenPoint.x) < EPSILON);
            Vertex3 worldPos = camera->ScreenToWorld(screenPoint);
            CHECK_CONDITION(Distance(worldPos, p0) < PRECISION);
        }

        {
            Vertex3 p0(-1, 1, -10);
            Vertex3 screenPoint = camera->WorldToScreen(p0);
            CHECK_CONDITION(Abs(-1 - screenPoint.x) < EPSILON);
            CHECK_CONDITION(Abs(1 - screenPoint.y) < EPSILON);
            Vertex3 worldPos = camera->ScreenToWorld(screenPoint);
            CHECK_CONDITION(Distance(worldPos, p0) < PRECISION);
        }

        camera->DisableOrtho();

        {
            Vertex3 p0(0, 0, -1);
            Vertex3 worldPoint = camera->ScreenToWorld(p0);
            Vertex3 screenPoint = camera->WorldToScreen(worldPoint);
            CHECK_CONDITION(Distance(screenPoint, p0) < PRECISION);
        }

        {
            Vertex3 p0(-1, 0, -110.3f);
            Vertex3 screenPoint = camera->WorldToScreen(p0);
            CHECK_CONDITION(screenPoint.x < 0 && screenPoint.x > -0.5f);
            Vertex3 worldPos = camera->ScreenToWorld(screenPoint);
            CHECK_CONDITION(Distance(worldPos, p0) < 0.05f);
        }

        {
            Vertex3 p0(-1, 1, -10);
            Vertex3 screenPoint = camera->WorldToScreen(p0);
            CHECK_CONDITION(screenPoint.x < 0 && screenPoint.x > -0.5f);
            //CHECK_CONDITION(screenPoint.y > 0 && screenPoint.y < 0.5f);
            Vertex3 worldPos = camera->ScreenToWorld(screenPoint);
            CHECK_CONDITION(Distance(worldPos, p0) < 0.1f);
        }


        {
            Vertex3 p0(1, 0, -10);
            Vertex3 screenPoint = camera->WorldToScreen(p0);
            CHECK_CONDITION(screenPoint.x > 0 && screenPoint.x < 0.5f);
            Vertex3 worldPos = camera->ScreenToWorld(screenPoint);
            CHECK_CONDITION(Distance(worldPos, p0) < 0.05f);
        }

        {
            Vertex3 p0(1, 0, 10); // behind the screen
            Vertex3 screenPoint = camera->WorldToScreen(p0);
            //CHECK_CONDITION(screenPoint.x > 0 && screenPoint.x < 0.5f);
            Vertex3 worldPos = camera->ScreenToWorld(screenPoint);
            CHECK_CONDITION(Distance(worldPos, p0) < 0.1f);
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
        CHECK_CONDITION(Distance(ray.GetDirection(), Vector3::LookAt) < EPSILON);
        CHECK_CONDITION(Distance(ray.GetOrigin(), position) < 0.5f);
    }

    {
        Ray ray = camera->GetScreenRay(-0.9f, 0);
        CHECK_CONDITION(ray.GetOrigin().x < 0);
        CHECK_CONDITION(ray.GetDirection().x < 0);
        CHECK_CONDITION(ray.GetDirection().y == 0);
        CHECK_CONDITION(ray.GetDirection().z < 0);
    }

    {
        Ray ray = camera->GetScreenRay(1, 0);
        CHECK_CONDITION(ray.GetOrigin().x > 0);
        CHECK_CONDITION(ray.GetDirection().x > 0);
        CHECK_CONDITION(ray.GetDirection().y == 0);
        CHECK_CONDITION(ray.GetDirection().z < 0);
    }

    {
        Ray ray = camera->GetScreenRay(0, 1);
        CHECK_CONDITION(ray.GetOrigin().x == 0);
        CHECK_CONDITION(ray.GetOrigin().y > 0);
        CHECK_CONDITION(ray.GetDirection().x == 0);
        CHECK_CONDITION(ray.GetDirection().y > 0.3f);
        CHECK_CONDITION(ray.GetDirection().z < -0.9f);
    }

    {
        Ray ray = camera->GetScreenRay(0, -1);
        CHECK_CONDITION(ray.GetOrigin().y < 0);
        CHECK_CONDITION(ray.GetDirection().x == 0);
        CHECK_CONDITION(ray.GetDirection().y < 0.3f);
        CHECK_CONDITION(ray.GetDirection().z < -0.9f);
    }

    {
        camera->SetGlobalLookAtPosition(position + Vector3(-1, 0, 0));
        Ray ray = camera->GetScreenRay(0, 0);
        CHECK_CONDITION(Distance(ray.GetOrigin(), position) < 1);
        CHECK_CONDITION(Distance(ray.GetDirection(), -Vector3::Right) < 0.05f);
    }
    #endif
}

static void Test05()
{
    {
        PBoxMesh box(Mesh::Create<BoxMesh>());
        box->Set(1, 1, 1);
        CHECK_CONDITION(box->IsReady());
        PScene scene = std::make_shared<Scene>("scene000");

        PCamera camera = scene->GetOrCreateChild<Camera>(GetUniqueName("camera"));
        camera->SetNearClip(0.1f);
        camera->SetFarClip(250);
        camera->SetPosition(Vertex3(0, 0, -4));
        camera->SetGlobalLookAtPosition(-Vector3::LookAt);
        BoundingBox bb = box->GetBB();
        CHECK_CONDITION(camera->GetFrustum()->IsInside(bb) == Intersection::INSIDE);
        camera->SetPosition(Vertex3(0, 0, -250));
        CHECK_CONDITION(camera->GetFrustum()->IsInside(bb) == Intersection::INTERSECTS);
        camera->SetPosition(Vertex3(0, 0, -400));
        CHECK_CONDITION(camera->GetFrustum()->IsInside(bb) == Intersection::OUTSIDE);
    }
}

static void Test06()
{

    PScene scene = std::make_shared<Scene>();
	PCamera camera = scene->CreateChild<Camera>();
	camera->EnableOrtho();
    camera->SetNearClip(0);
    camera->SetFarClip(5);
	camera->SetAspectRatio(1);
	camera->SetOrthoScale(10);
	camera->SetPosition(Vertex3(0, 0, 1));
	//camera->SetGlobalLookAtPosition(VECTOR3_LOOKAT_DIRECTION);

	{
		auto vertices = camera->GetFrustum()->GetVertices();
		auto vertex0 = vertices[0];
		CHECK_CONDITION(Distance(vertex0, Vector3(5, 5, 1)) < 0.05f);
		auto vertex1 = vertices[1];
		CHECK_CONDITION(Distance(vertex1, Vector3(5, -5, 1)) < 0.05f);
		auto vertex2 = vertices[2];
		CHECK_CONDITION(Distance(vertex2, Vector3(-5, -5, 1)) < 0.05f);
		auto vertex3 = vertices[3];
		CHECK_CONDITION(Distance(vertex3, Vector3(-5, 5, 1)) < 0.05f);
		auto vertex4 = vertices[4];
		CHECK_CONDITION(Distance(vertex4, Vector3(5, 5, -4)) < 0.05f);
		auto vertex5 = vertices[5];
		CHECK_CONDITION(Distance(vertex5, Vector3(5, -5, -4)) < 0.05f);
		auto vertex6 = vertices[6];
		CHECK_CONDITION(Distance(vertex6, Vector3(-5, -5, -4)) < 0.05f);
		auto vertex7 = vertices[7];
		CHECK_CONDITION(Distance(vertex7, Vector3(-5, 5, -4)) < 0.05f);
	}

	{
		BoundingBox bb(*camera->GetFrustum());
		Vector3 min(-5, -5, -4);
		Vector3 max(5, 5, 1);
		CHECK_CONDITION(Distance(bb.min_, min) < 0.05f);
		CHECK_CONDITION(Distance(bb.max_, max) < 0.05f);
	}

	camera->DisableOrtho();
	camera->SetNearClip(1);
	camera->SetFarClip(6);

	{
		BoundingBox bb(*camera->GetFrustum());
		float x = 2.485f;
		float y = 2.485f;
		float z = camera->GetZFar() - camera->GetZNear();

		Vector3 min(-x, -y, -z);
		Vector3 max(x, y, 0);
		CHECK_CONDITION(Distance(bb.min_, min) < 0.05f);
		CHECK_CONDITION(Distance(bb.max_, max) < 0.05f);
	}

	/*
	Vector4 corner0(Vector3(-1), 1);
	Vector4 corner1(Vector3(1), 1);

	auto c0 = camera->GetViewProjectionInverseMatrix() * corner0;
	auto c1 = camera->GetViewProjectionInverseMatrix() * corner1;

	c0.x /= c0.w;
	c0.y /= c0.w;
	c0.z /= c0.w;

	c1.x /= c1.w;
	c1.y /= c1.w;
	c1.z /= c1.w;

    BoundingBox viewBox1(Vector3(-1), Vector3(1));
    viewBox1.Transform(camera->GetViewProjectionInverseMatrix());

    CHECK_CONDITION(Distance(viewBox0.min_, viewBox1.min_) < 0.05f);
    CHECK_CONDITION(Distance(viewBox0.max_, viewBox1.max_) < 0.05f);
	*/

}

static void Test07()
{
    PScene scene = std::make_shared<Scene>();
	PCamera camera = scene->CreateChild<Camera>();
	camera->EnableOrtho();
	camera->SetNearClip(0);
	camera->SetFarClip(5);
	camera->SetAspectRatio(1);
	camera->SetOrthoScale(10);
	camera->SetPosition(Vertex3(0, 0, 1));

	PBoxMesh box(Mesh::Create<BoxMesh>());
	auto node = scene->CreateChild<SceneNode>();
	node->SetMesh(box);

	CHECK_CONDITION(camera->IsVisible(*node));
	node->SetPosition(Vector3(0, 0, 1.95f));
	CHECK_CONDITION(camera->IsVisible(*node));

	node->SetPosition(Vector3(0, 0, 2));
	CHECK_CONDITION(!camera->IsVisible(*node));
	camera->SetNearClip(-1);
	CHECK_CONDITION(camera->IsVisible(*node));
	camera->SetNearClip(0);
	camera->SetPosition(Vertex3(0, 0, 1.1f));
	CHECK_CONDITION(camera->IsVisible(*node));
}

void CameraTest()
{
    auto window = Window::Create("window", 0, 0, 1, 1, (int)WindowFlag::HIDDEN);
    FrustumTest();
    Test01();
    Test02();
    Test03();
    Test04();
    Test05();
    Test06();
	Test07();
}
