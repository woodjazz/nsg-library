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

static void Test01()
{
	SphereMesh sphere(1, 100);

	{
		
		Node node;
		Camera camera;
		camera.SetFov(45);
		camera.SetNearClip(0.1f);
		camera.SetFarClip(10);
		PFrustum frustum = camera.GetFrustum();
		CHECK_ASSERT(frustum->IsVisible(node, sphere), __FILE__, __LINE__);
		node.SetPosition(Vertex3(20));
		CHECK_ASSERT(!frustum->IsVisible(node, sphere), __FILE__, __LINE__);
	}

	{
		Node node;
		node.SetPosition(Vertex3(20));
		node.SetScale(Vector3(100));

		Camera camera;
		camera.SetFov(45);
		camera.SetNearClip(0.1f);
		camera.SetFarClip(10);
		PFrustum frustum = camera.GetFrustum();
		CHECK_ASSERT(frustum->IsVisible(node, sphere), __FILE__, __LINE__);
		node.SetPosition(Vertex3(200));
		CHECK_ASSERT(!frustum->IsVisible(node, sphere), __FILE__, __LINE__);
	}

	{
		Node node;
		node.SetPosition(Vertex3(0, 0, 40));

		Camera camera;
		camera.SetFov(170);
		camera.SetNearClip(0.1f);
		camera.SetFarClip(1000);
		CHECK_ASSERT(camera.IsVisible(node, sphere), __FILE__, __LINE__);
		camera.SetNearClip(41.1f);
		CHECK_ASSERT(!camera.IsVisible(node, sphere), __FILE__, __LINE__);
	}

	{
		Node node;
		node.SetPosition(Vertex3(2, 0, 40));

		Camera camera;
		camera.SetFov(170);
		camera.SetNearClip(0.1f);
		camera.SetFarClip(1000);
		CHECK_ASSERT(camera.IsVisible(node, sphere), __FILE__, __LINE__);
		camera.SetFov(1);
		CHECK_ASSERT(!camera.IsVisible(node, sphere), __FILE__, __LINE__);
		node.SetPosition(Vertex3(0, 0, 40));
		CHECK_ASSERT(camera.IsVisible(node, sphere), __FILE__, __LINE__);
		camera.SetLookAt(Vertex3(-1, 0, 0));
		CHECK_ASSERT(!camera.IsVisible(node, sphere), __FILE__, __LINE__);
		camera.SetLookAt(Vertex3(0, 0, 1));
		CHECK_ASSERT(!camera.IsVisible(node, sphere), __FILE__, __LINE__);
		camera.SetPosition(Vertex3(0, 0, 45));
		CHECK_ASSERT(camera.IsVisible(node, sphere), __FILE__, __LINE__);
		camera.SetFarClip(3.9f);
		CHECK_ASSERT(!camera.IsVisible(node, sphere), __FILE__, __LINE__);
		camera.SetFarClip(4);
		CHECK_ASSERT(camera.IsVisible(node, sphere), __FILE__, __LINE__);
        node.SetScale(Vector3(0.9f));
        CHECK_ASSERT(!camera.IsVisible(node, sphere), __FILE__, __LINE__);
	}
}

static void Test02()
{
	{
		BoxMesh box(2, 4, 2, 2, 2, 2);
		Node node;

		Camera camera;
		camera.SetFov(179);
		camera.SetNearClip(0.1f);
		camera.SetFarClip(10);
		camera.SetPosition(Vertex3(1, 0, 0));
		CHECK_ASSERT(camera.IsVisible(node, box), __FILE__, __LINE__);
		camera.SetPosition(Vertex3(2, 0, 0));
		CHECK_ASSERT(!camera.IsVisible(node, box), __FILE__, __LINE__);
		node.SetOrientation(glm::angleAxis(glm::pi<float>() / 2, Vertex3(0, 0, 1)));
		CHECK_ASSERT(camera.IsVisible(node, box), __FILE__, __LINE__);
		node.SetPosition(Vertex3(1, 0, -2));
		CHECK_ASSERT(!camera.IsVisible(node, box), __FILE__, __LINE__);
	}
}

void CameraTest()
{
	Test01();
	Test02();
}