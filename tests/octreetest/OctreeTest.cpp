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
    PScene scene(App::this_->GetOrCreateScene("scene1"));
	App::this_->SetCurrentScene(scene);

    PSphereMesh sphereMesh(App::this_->CreateSphereMesh());
    PBoxMesh boxMesh(App::this_->CreateBoxMesh());

	PSceneNode node1s = scene->GetOrCreateChild<SceneNode>("node 1");
    node1s->Set(sphereMesh);
    node1s->SetPosition(Vertex3(0, 0, 1));

	PSceneNode node1b = scene->GetOrCreateChild<SceneNode>("node 2");
    node1b->Set(boxMesh);
    node1b->SetPosition(Vertex3(0, 0, 1));

	PCamera camera = scene->GetOrCreateChild<Camera>("camera");

    std::vector<const SceneNode*> visibles;
    scene->GetVisibleNodes(camera.get(), visibles);
    CHECK_ASSERT(visibles.size() == 0, __FILE__, __LINE__);

    node1s->SetPosition(Vertex3(0, 0, -10));

    scene->GetVisibleNodes(camera.get(), visibles);
    CHECK_ASSERT(visibles.size() == 1, __FILE__, __LINE__);

    node1b->SetPosition(Vertex3(0, 0, -100));
    scene->GetVisibleNodes(camera.get(), visibles);
    CHECK_ASSERT(visibles.size() == 2, __FILE__, __LINE__);

    node1s->SetPosition(Vertex3(0, 0, 1));
    scene->GetVisibleNodes(camera.get(), visibles);
    CHECK_ASSERT(visibles.size() == 1, __FILE__, __LINE__);

    node1s->SetPosition(Vertex3(0, 0, 0.8f));
    scene->GetVisibleNodes(camera.get(), visibles);
    CHECK_ASSERT(visibles.size() == 2, __FILE__, __LINE__);

	camera->SetGlobalLookAt(Vector3(0, 0, 1));
    scene->GetVisibleNodes(camera.get(), visibles);
    CHECK_ASSERT(visibles.size() == 1, __FILE__, __LINE__);

    node1s->SetPosition(Vertex3(0, 0, -0.8f));
    scene->GetVisibleNodes(camera.get(), visibles);
    CHECK_ASSERT(visibles.size() == 1, __FILE__, __LINE__);

    camera->SetPosition(Vertex3(0, 0, 1));

    scene->GetVisibleNodes(camera.get(), visibles);
    CHECK_ASSERT(visibles.size() == 0, __FILE__, __LINE__);

}

static void Test02()
{
	PScene scene(App::this_->GetOrCreateScene("scene2"));
	App::this_->SetCurrentScene(scene);

    PSphereMesh sphereMesh(App::this_->CreateSphereMesh());
    PBoxMesh boxMesh(App::this_->CreateBoxMesh());

	PSceneNode node1s = scene->GetOrCreateChild<SceneNode>("node 1");
    node1s->Set(sphereMesh);
    node1s->SetPosition(Vertex3(100, 0, -100));

	PSceneNode node1b = scene->GetOrCreateChild<SceneNode>("node 2");
    node1b->Set(boxMesh);
    node1b->SetPosition(Vertex3(-100, 0, -100));

    std::vector<const SceneNode*> nodes;
    Vertex3 origin(0);
    Vector3 direction1s(node1s->GetGlobalPosition() - origin);
    Ray ray(origin, direction1s);
    CHECK_ASSERT(scene->GetFastRayNodesIntersection(ray, nodes), __FILE__, __LINE__);
    CHECK_ASSERT(nodes.size() == 1, __FILE__, __LINE__);
    CHECK_ASSERT(nodes[0] == node1s.get(), __FILE__, __LINE__);

    {
        Ray ray(origin, direction1s, 140);
        CHECK_ASSERT(!scene->GetFastRayNodesIntersection(ray, nodes), __FILE__, __LINE__);
    }

    {
        Ray ray(origin, direction1s, 141);
        CHECK_ASSERT(scene->GetFastRayNodesIntersection(ray, nodes), __FILE__, __LINE__);
        CHECK_ASSERT(nodes.size() == 1, __FILE__, __LINE__);
        CHECK_ASSERT(nodes[0] == node1s.get(), __FILE__, __LINE__);
    }


    ray = Ray(origin, -direction1s);
    CHECK_ASSERT(!scene->GetFastRayNodesIntersection(ray, nodes), __FILE__, __LINE__);

    Vector3 direction1b(node1b->GetGlobalPosition() - origin);
    ray = Ray(origin, direction1b);
    CHECK_ASSERT(scene->GetFastRayNodesIntersection(ray, nodes), __FILE__, __LINE__);
    CHECK_ASSERT(nodes.size() == 1, __FILE__, __LINE__);
    CHECK_ASSERT(nodes[0] == node1b.get(), __FILE__, __LINE__);

    ray = Ray(origin, -direction1b);
    CHECK_ASSERT(!scene->GetFastRayNodesIntersection(ray, nodes), __FILE__, __LINE__);
}

static void Test03()
{
	PScene scene(App::this_->GetOrCreateScene("scene3"));
	App::this_->SetCurrentScene(scene);

    {
        PSphereMesh sphereMesh(App::this_->CreateSphereMesh());
		PSceneNode node1s = scene->GetOrCreateChild<SceneNode>("node 1");
        node1s->Set(sphereMesh);

        Vertex3 origin(0, 0, 100);
        Vector3 direction1s(node1s->GetGlobalPosition() - origin);
        Ray ray(origin, direction1s);
        std::vector<RayNodeResult> result;
        CHECK_ASSERT(scene->GetPreciseRayNodesIntersection(ray, result), __FILE__, __LINE__);
        CHECK_ASSERT(result.size() == 1, __FILE__, __LINE__);
        CHECK_ASSERT(result[0].node_ == node1s.get(), __FILE__, __LINE__);
        CHECK_ASSERT(glm::abs(result[0].distance_ - 99) < 0.1f, __FILE__, __LINE__);

        direction1s = node1s->GetGlobalPosition() - Vector3(0.45f, 0, 0) - origin;
        ray = Ray(origin, direction1s);

        CHECK_ASSERT(scene->GetPreciseRayNodesIntersection(ray, result), __FILE__, __LINE__);
        CHECK_ASSERT(result.size() == 1, __FILE__, __LINE__);
        CHECK_ASSERT(result[0].node_ == node1s.get(), __FILE__, __LINE__);
        CHECK_ASSERT(result[0].distance_ > 99, __FILE__, __LINE__);
    }
}

static void Test04()
{
	PScene scene(App::this_->GetOrCreateScene("scene4"));
	App::this_->SetCurrentScene(scene);

    {
        const float RADIUS = 0.5f;
        PSphereMesh sphereMesh(App::this_->CreateSphereMesh(RADIUS, 64));
        std::vector<PSceneNode> nodes;
        for (int i = 0; i < 100; i++)
        {
            std::stringstream ss;
            ss << i;
			PSceneNode node = scene->GetOrCreateChild<SceneNode>(ss.str());
            nodes.push_back(node);
            node->SetPosition(Vertex3(0, 0, -i));
            node->Set(sphereMesh);
        }

        Vertex3 origin(0, 0, 100);
        Vector3 direction1s(Vector3(0, 0, -1));
        Ray ray(origin, direction1s);
        RayNodeResult closest;
        CHECK_ASSERT(scene->GetClosestRayNodeIntersection(ray, closest), __FILE__, __LINE__);
        CHECK_ASSERT(closest.node_->GetName() == "0", __FILE__, __LINE__);
        CHECK_ASSERT(glm::abs(closest.distance_ - (99 + RADIUS)) < 0.01f, __FILE__, __LINE__);

        ray = Ray(Vertex3(0, 0, -101), -direction1s);
        CHECK_ASSERT(scene->GetClosestRayNodeIntersection(ray, closest), __FILE__, __LINE__);
        CHECK_ASSERT(closest.node_->GetName() == "99", __FILE__, __LINE__);
        CHECK_ASSERT(glm::abs(closest.distance_ - (1 + RADIUS)) < 0.01f, __FILE__, __LINE__);

        origin = Vertex3(5000, 0, 100);
        direction1s = Vertex3(0, 0, -50) - origin;
        ray = Ray(origin, direction1s);
        CHECK_ASSERT(scene->GetClosestRayNodeIntersection(ray, closest), __FILE__, __LINE__);
        CHECK_ASSERT(closest.node_->GetName() == "50", __FILE__, __LINE__);
        float d = glm::length(direction1s) - RADIUS;
        CHECK_ASSERT(glm::abs(closest.distance_ - d) < 0.01f, __FILE__, __LINE__);

        origin = Vertex3(5000, 0, -50);
        direction1s = Vertex3(0, 0, -50) - origin;
        ray = Ray(origin, direction1s);
        CHECK_ASSERT(scene->GetClosestRayNodeIntersection(ray, closest), __FILE__, __LINE__);
        CHECK_ASSERT(closest.node_->GetName() == "50", __FILE__, __LINE__);
        d = glm::length(direction1s) - RADIUS;
        CHECK_ASSERT(glm::abs(closest.distance_ - d) < 0.01f, __FILE__, __LINE__);

        origin = Vertex3(5000, 0, -50);
        direction1s = Vertex3(0, 0, -50.49f) - origin;
        ray = Ray(origin, direction1s);
        CHECK_ASSERT(scene->GetClosestRayNodeIntersection(ray, closest), __FILE__, __LINE__);
        CHECK_ASSERT(closest.node_->GetName() == "50", __FILE__, __LINE__);
        d = glm::length(direction1s) - 0.01f;
        CHECK_ASSERT(glm::abs(closest.distance_ - d) < 0.1f, __FILE__, __LINE__);

        origin = Vertex3(5000, 0, -50);
        direction1s = Vertex3(0, 0, -50.51f) - origin;
        ray = Ray(origin, direction1s);
        CHECK_ASSERT(scene->GetClosestRayNodeIntersection(ray, closest), __FILE__, __LINE__);
        CHECK_ASSERT(closest.node_->GetName() == "51", __FILE__, __LINE__);
        d = glm::length(direction1s) - 0.01f;
        CHECK_ASSERT(glm::abs(closest.distance_ - d) < 0.1f, __FILE__, __LINE__);

        origin = Vertex3(5000, 0, -50);
        direction1s = Vertex3(0, 0, -49.51f) - origin;
        ray = Ray(origin, direction1s);
        CHECK_ASSERT(scene->GetClosestRayNodeIntersection(ray, closest), __FILE__, __LINE__);
        CHECK_ASSERT(closest.node_->GetName() == "50", __FILE__, __LINE__);
        d = glm::length(direction1s) - 0.01f;
        CHECK_ASSERT(glm::abs(closest.distance_ - d) < 0.1f, __FILE__, __LINE__);
    }
}

static void Test05()
{
	PScene scene(App::this_->GetOrCreateScene("scene5"));
	App::this_->SetCurrentScene(scene);

	{
		const float RADIUS = 0.5f;
		PSphereMesh sphereMesh(App::this_->CreateSphereMesh(RADIUS, 64));
		PSceneNode node = scene->GetOrCreateChild<SceneNode>("0");
		node->SetPosition(Vertex3(0, 0, 0));
		const float SCALE = 0.1f;
		node->SetScale(Vertex3(SCALE));
		node->Set(sphereMesh);

		Vertex3 origin(0, 0, 1);
		Vector3 direction1s(Vector3(0, 0, -1));
		Ray ray(origin, direction1s);
		RayNodeResult closest;
		CHECK_ASSERT(scene->GetClosestRayNodeIntersection(ray, closest), __FILE__, __LINE__);
		CHECK_ASSERT(closest.node_->GetName() == "0", __FILE__, __LINE__);
		CHECK_ASSERT(glm::abs(closest.distance_ - (1 - RADIUS * SCALE)) < 0.01f, __FILE__, __LINE__);
	}
}

void OctreeTest()
{
    Test01();
    Test02();
    Test03();
    Test04();
	Test05();
}