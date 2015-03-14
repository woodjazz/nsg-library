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
	auto scene = std::make_shared<Scene>("scene1");

    auto sphereMesh(App::this_->CreateSphereMesh());
    auto boxMesh(App::this_->CreateBoxMesh());

	auto node1s = scene->GetOrCreateChild<SceneNode>("node 1");
    node1s->SetMesh(sphereMesh);
    node1s->SetPosition(Vertex3(0, 0, 1));

	auto node1b = scene->GetOrCreateChild<SceneNode>("node 2");
    node1b->SetMesh(boxMesh);
    node1b->SetPosition(Vertex3(0, 0, 1));

	auto camera = scene->GetOrCreateChild<Camera>("camera");

    std::vector<SceneNode*> visibles;
    scene->GetVisibleNodes(camera.get(), visibles);
    CHECK_CONDITION(visibles.size() == 0, __FILE__, __LINE__);

    node1s->SetPosition(Vertex3(0, 0, -10));

    scene->GetVisibleNodes(camera.get(), visibles);
    CHECK_CONDITION(visibles.size() == 1, __FILE__, __LINE__);

    node1b->SetPosition(Vertex3(0, 0, -100));
    scene->GetVisibleNodes(camera.get(), visibles);
    CHECK_CONDITION(visibles.size() == 2, __FILE__, __LINE__);

    node1s->SetPosition(Vertex3(0, 0, 1));
    scene->GetVisibleNodes(camera.get(), visibles);
    CHECK_CONDITION(visibles.size() == 1, __FILE__, __LINE__);

    node1s->SetPosition(Vertex3(0, 0, 0.8f));
    scene->GetVisibleNodes(camera.get(), visibles);
    CHECK_CONDITION(visibles.size() == 2, __FILE__, __LINE__);

	camera->SetGlobalLookAt(Vector3(0, 0, 1));
    scene->GetVisibleNodes(camera.get(), visibles);
    CHECK_CONDITION(visibles.size() == 1, __FILE__, __LINE__);

    node1s->SetPosition(Vertex3(0, 0, -0.8f));
    scene->GetVisibleNodes(camera.get(), visibles);
    CHECK_CONDITION(visibles.size() == 1, __FILE__, __LINE__);

    camera->SetPosition(Vertex3(0, 0, 1));

    scene->GetVisibleNodes(camera.get(), visibles);
    CHECK_CONDITION(visibles.size() == 0, __FILE__, __LINE__);

}

static void Test02()
{
	auto scene = std::make_shared<Scene>("scene2");

    auto sphereMesh(App::this_->CreateSphereMesh());
    auto boxMesh(App::this_->CreateBoxMesh());

	auto node1s = scene->GetOrCreateChild<SceneNode>("node 1");
    node1s->SetMesh(sphereMesh);
    node1s->SetPosition(Vertex3(100, 0, -100));

	auto node1b = scene->GetOrCreateChild<SceneNode>("node 2");
	node1b->SetMesh(boxMesh);
    node1b->SetPosition(Vertex3(-100, 0, -100));

    std::vector<SceneNode*> nodes;
    Vertex3 origin(0);
    Vector3 direction1s(node1s->GetGlobalPosition() - origin);
    Ray ray(origin, direction1s);
	CHECK_CONDITION(scene->GetFastRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, nodes), __FILE__, __LINE__);
    CHECK_CONDITION(nodes.size() == 1, __FILE__, __LINE__);
    CHECK_CONDITION(nodes[0] == node1s.get(), __FILE__, __LINE__);

    {
        Ray ray(origin, direction1s, 140);
		CHECK_CONDITION(!scene->GetFastRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, nodes), __FILE__, __LINE__);
    }

    {
        Ray ray(origin, direction1s, 141);
		CHECK_CONDITION(scene->GetFastRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, nodes), __FILE__, __LINE__);
        CHECK_CONDITION(nodes.size() == 1, __FILE__, __LINE__);
        CHECK_CONDITION(nodes[0] == node1s.get(), __FILE__, __LINE__);
    }


    ray = Ray(origin, -direction1s);
	CHECK_CONDITION(!scene->GetFastRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, nodes), __FILE__, __LINE__);

    Vector3 direction1b(node1b->GetGlobalPosition() - origin);
    ray = Ray(origin, direction1b);
	CHECK_CONDITION(scene->GetFastRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, nodes), __FILE__, __LINE__);
    CHECK_CONDITION(nodes.size() == 1, __FILE__, __LINE__);
    CHECK_CONDITION(nodes[0] == node1b.get(), __FILE__, __LINE__);

    ray = Ray(origin, -direction1b);
	CHECK_CONDITION(!scene->GetFastRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, nodes), __FILE__, __LINE__);
}

static void Test03()
{
	auto scene = std::make_shared<Scene>("scene3");

    {
        auto sphereMesh(App::this_->CreateSphereMesh());
		auto node1s = scene->GetOrCreateChild<SceneNode>("node 1");
		node1s->SetMesh(sphereMesh);

        Vertex3 origin(0, 0, 100);
        Vector3 direction1s(node1s->GetGlobalPosition() - origin);
        Ray ray(origin, direction1s);
        std::vector<RayNodeResult> result;
		CHECK_CONDITION(scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
        CHECK_CONDITION(result.size() == 1, __FILE__, __LINE__);
        CHECK_CONDITION(result[0].node_ == node1s.get(), __FILE__, __LINE__);
        CHECK_CONDITION(glm::abs(result[0].distance_ - 99) < 0.1f, __FILE__, __LINE__);

        direction1s = node1s->GetGlobalPosition() - Vector3(0.45f, 0, 0) - origin;
        ray = Ray(origin, direction1s);

		CHECK_CONDITION(scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
        CHECK_CONDITION(result.size() == 1, __FILE__, __LINE__);
        CHECK_CONDITION(result[0].node_ == node1s.get(), __FILE__, __LINE__);
        CHECK_CONDITION(result[0].distance_ > 99, __FILE__, __LINE__);
    }
}

static void Test04()
{
	auto scene = std::make_shared<Scene>("scene4");

    {
        const float RADIUS = 0.5f;
        auto sphereMesh(App::this_->CreateSphereMesh(RADIUS, 64));
        std::vector<PSceneNode> nodes;
        for (int i = 0; i < 100; i++)
        {
            std::stringstream ss;
            ss << i;
			auto node = scene->GetOrCreateChild<SceneNode>(ss.str());
            nodes.push_back(node);
            node->SetPosition(Vertex3(0, 0, -i));
			node->SetMesh(sphereMesh);
        }

        Vertex3 origin(0, 0, 100);
        Vector3 direction1s(Vector3(0, 0, -1));
        Ray ray(origin, direction1s);
        RayNodeResult closest;
		CHECK_CONDITION(scene->GetClosestRayNodeIntersection(RenderLayer::DEFAULT_LAYER, ray, closest), __FILE__, __LINE__);
        CHECK_CONDITION(closest.node_->GetName() == "0", __FILE__, __LINE__);
        CHECK_CONDITION(glm::abs(closest.distance_ - (99 + RADIUS)) < 0.01f, __FILE__, __LINE__);

        ray = Ray(Vertex3(0, 0, -101), -direction1s);
		CHECK_CONDITION(scene->GetClosestRayNodeIntersection(RenderLayer::DEFAULT_LAYER, ray, closest), __FILE__, __LINE__);
        CHECK_CONDITION(closest.node_->GetName() == "99", __FILE__, __LINE__);
        CHECK_CONDITION(glm::abs(closest.distance_ - (1 + RADIUS)) < 0.01f, __FILE__, __LINE__);

        origin = Vertex3(5000, 0, 100);
        direction1s = Vertex3(0, 0, -50) - origin;
        ray = Ray(origin, direction1s);
		CHECK_CONDITION(scene->GetClosestRayNodeIntersection(RenderLayer::DEFAULT_LAYER, ray, closest), __FILE__, __LINE__);
        CHECK_CONDITION(closest.node_->GetName() == "50", __FILE__, __LINE__);
        float d = glm::length(direction1s) - RADIUS;
        CHECK_CONDITION(glm::abs(closest.distance_ - d) < 0.01f, __FILE__, __LINE__);

        origin = Vertex3(5000, 0, -50);
        direction1s = Vertex3(0, 0, -50) - origin;
        ray = Ray(origin, direction1s);
		CHECK_CONDITION(scene->GetClosestRayNodeIntersection(RenderLayer::DEFAULT_LAYER, ray, closest), __FILE__, __LINE__);
        CHECK_CONDITION(closest.node_->GetName() == "50", __FILE__, __LINE__);
        d = glm::length(direction1s) - RADIUS;
        CHECK_CONDITION(glm::abs(closest.distance_ - d) < 0.01f, __FILE__, __LINE__);

        origin = Vertex3(5000, 0, -50);
        direction1s = Vertex3(0, 0, -50.49f) - origin;
        ray = Ray(origin, direction1s);
		CHECK_CONDITION(scene->GetClosestRayNodeIntersection(RenderLayer::DEFAULT_LAYER, ray, closest), __FILE__, __LINE__);
        CHECK_CONDITION(closest.node_->GetName() == "50", __FILE__, __LINE__);
        d = glm::length(direction1s) - 0.01f;
        CHECK_CONDITION(glm::abs(closest.distance_ - d) < 0.1f, __FILE__, __LINE__);

        origin = Vertex3(5000, 0, -50);
        direction1s = Vertex3(0, 0, -50.51f) - origin;
        ray = Ray(origin, direction1s);
		CHECK_CONDITION(scene->GetClosestRayNodeIntersection(RenderLayer::DEFAULT_LAYER, ray, closest), __FILE__, __LINE__);
        CHECK_CONDITION(closest.node_->GetName() == "51", __FILE__, __LINE__);
        d = glm::length(direction1s) - 0.01f;
        CHECK_CONDITION(glm::abs(closest.distance_ - d) < 0.1f, __FILE__, __LINE__);

        origin = Vertex3(5000, 0, -50);
        direction1s = Vertex3(0, 0, -49.51f) - origin;
        ray = Ray(origin, direction1s);
		CHECK_CONDITION(scene->GetClosestRayNodeIntersection(RenderLayer::DEFAULT_LAYER, ray, closest), __FILE__, __LINE__);
        CHECK_CONDITION(closest.node_->GetName() == "50", __FILE__, __LINE__);
        d = glm::length(direction1s) - 0.01f;
        CHECK_CONDITION(glm::abs(closest.distance_ - d) < 0.1f, __FILE__, __LINE__);
    }
}

static void Test05()
{
	auto scene = std::make_shared<Scene>("scene5");

	{
		const float RADIUS = 0.5f;
		auto sphereMesh(App::this_->CreateSphereMesh(RADIUS, 64));
		auto node = scene->GetOrCreateChild<SceneNode>("0");
		node->SetPosition(Vertex3(0, 0, 0));
		const float SCALE = 0.1f;
		node->SetScale(Vertex3(SCALE));
		node->SetMesh(sphereMesh);

		Vertex3 origin(0, 0, 1);
		Vector3 direction1s(Vector3(0, 0, -1));
		Ray ray(origin, direction1s);
		RayNodeResult closest;
		CHECK_CONDITION(scene->GetClosestRayNodeIntersection(RenderLayer::DEFAULT_LAYER, ray, closest), __FILE__, __LINE__);
		CHECK_CONDITION(closest.node_->GetName() == "0", __FILE__, __LINE__);
		CHECK_CONDITION(glm::abs(closest.distance_ - (1 - RADIUS * SCALE)) < 0.01f, __FILE__, __LINE__);
	}
}

static void Test06()
{
    auto scene = std::make_shared<Scene>();
	auto quad = App::this_->CreateQuadMesh(0.5f);
	auto nodeLU = scene->CreateChild<SceneNode>();
	nodeLU->SetMesh(quad);
	nodeLU->SetPosition(Vector3(-0.5f, 0.5f, 0));
	auto nodeRU = scene->CreateChild<SceneNode>();
	nodeRU->SetMesh(quad);
	nodeRU->SetPosition(Vector3(0.5f, 0.5f, 0));
	auto nodeLB = scene->CreateChild<SceneNode>();
	nodeLB->SetMesh(quad);
	nodeLB->SetPosition(Vector3(-0.5f, -0.5f, 0));
	auto nodeRB = scene->CreateChild<SceneNode>();
	nodeRB->SetMesh(quad);
	nodeRB->SetPosition(Vector3(0.5f, -0.5f, 0));

	{
		Ray ray = Camera::GetRay(0, 0);
		std::vector<RayNodeResult> result;
		CHECK_CONDITION(!scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
	}

	{
		Ray ray = Camera::GetRay(-0.5f, 0.5f);
		std::vector<RayNodeResult> result;
		CHECK_CONDITION(scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
		CHECK_CONDITION(result.size() == 1, __FILE__, __LINE__);
		CHECK_CONDITION(result[0].node_ == nodeLU.get(), __FILE__, __LINE__);
	}

	{
		Ray ray = Camera::GetRay(0.5f, 0.5f);
		std::vector<RayNodeResult> result;
		CHECK_CONDITION(scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
		CHECK_CONDITION(result.size() == 1, __FILE__, __LINE__);
		CHECK_CONDITION(result[0].node_ == nodeRU.get(), __FILE__, __LINE__);
	}

	{
		Ray ray = Camera::GetRay(-0.5f, -0.5f);
		std::vector<RayNodeResult> result;
		CHECK_CONDITION(scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
		CHECK_CONDITION(result.size() == 1, __FILE__, __LINE__);
		CHECK_CONDITION(result[0].node_ == nodeLB.get(), __FILE__, __LINE__);
	}

	{
		Ray ray = Camera::GetRay(0.5f, -0.5f);
		std::vector<RayNodeResult> result;
		CHECK_CONDITION(scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
		CHECK_CONDITION(result.size() == 1, __FILE__, __LINE__);
		CHECK_CONDITION(result[0].node_ == nodeRB.get(), __FILE__, __LINE__);
	}

}

static void Test07()
{
    auto xml = App::this_->GetOrCreateResourceFile("data/AnonymousPro132.xml");
    auto atlas = std::make_shared<FontAtlas>(xml);
    
    auto textCenter = atlas->GetOrCreateMesh("A", CENTER_ALIGNMENT, MIDDLE_ALIGNMENT);
    auto textLeftTop = atlas->GetOrCreateMesh("B", LEFT_ALIGNMENT, TOP_ALIGNMENT);
    auto textRightTop = atlas->GetOrCreateMesh("C", RIGHT_ALIGNMENT, TOP_ALIGNMENT);
    auto textLeftBottom = atlas->GetOrCreateMesh("D", LEFT_ALIGNMENT, BOTTOM_ALIGNMENT);
    auto textRightBottom = atlas->GetOrCreateMesh("E", RIGHT_ALIGNMENT, BOTTOM_ALIGNMENT);
   
	auto scene = std::make_shared<Scene>("scene");

    auto nodeCenter = scene->GetOrCreateChild<SceneNode>("nodeCenter");
    auto nodeLeftTop = scene->GetOrCreateChild<SceneNode>("nodeLeftTop");
    auto nodeRightTop = scene->GetOrCreateChild<SceneNode>("nodeRightTop");
    auto nodeLeftBottom = scene->GetOrCreateChild<SceneNode>("nodeLeftBottom");
    auto nodeRightBottom = scene->GetOrCreateChild<SceneNode>("nodeRightBottom");
	
	nodeCenter->SetMesh(textCenter);
	nodeLeftTop->SetMesh(textLeftTop);
	nodeRightTop->SetMesh(textRightTop);
	nodeLeftBottom->SetMesh(textLeftBottom);
	nodeRightBottom->SetMesh(textRightBottom);
	
	auto material = App::this_->CreateMaterial();
	material->SetTextMap(atlas->GetTexture());

	nodeCenter->SetMaterial(material);
	nodeLeftTop->SetMaterial(material);
	nodeRightTop->SetMaterial(material);
	nodeLeftBottom->SetMaterial(material);
	nodeRightBottom->SetMaterial(material);

	atlas->SetViewSize(256, 256);

    {
        Ray ray = Camera::GetRay(0, 0.f);
        std::vector<RayNodeResult> result;
		CHECK_CONDITION(scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
        CHECK_CONDITION(result.size() == 1, __FILE__, __LINE__);
        CHECK_CONDITION(result[0].node_ == nodeCenter.get(), __FILE__, __LINE__);
    }

	{
		Ray ray = Camera::GetRay(-0.9f, 0.9f);
		std::vector<RayNodeResult> result;
		CHECK_CONDITION(scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
		CHECK_CONDITION(result.size() == 1, __FILE__, __LINE__);
		CHECK_CONDITION(result[0].node_ == nodeLeftTop.get(), __FILE__, __LINE__);
	}

	{
		Ray ray = Camera::GetRay(0.9f, 0.9f);
		std::vector<RayNodeResult> result;
		CHECK_CONDITION(scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
		CHECK_CONDITION(result.size() == 1, __FILE__, __LINE__);
		CHECK_CONDITION(result[0].node_ == nodeRightTop.get(), __FILE__, __LINE__);
	}

	{
		Ray ray = Camera::GetRay(-0.9f, -0.9f);
		std::vector<RayNodeResult> result;
		CHECK_CONDITION(scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
		CHECK_CONDITION(result.size() == 1, __FILE__, __LINE__);
		CHECK_CONDITION(result[0].node_ == nodeLeftBottom.get(), __FILE__, __LINE__);
	}

	{
		Ray ray = Camera::GetRay(0.9f, -0.9f);
		std::vector<RayNodeResult> result;
		CHECK_CONDITION(scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
		CHECK_CONDITION(result.size() == 1, __FILE__, __LINE__);
		CHECK_CONDITION(result[0].node_ == nodeRightBottom.get(), __FILE__, __LINE__);
	}

}

static void Test08()
{
	auto xml = App::this_->GetOrCreateResourceFile("data/AnonymousPro132.xml");
	auto atlas = std::make_shared<FontAtlas>(xml);

	auto textCenter = atlas->GetOrCreateMesh("A", CENTER_ALIGNMENT, MIDDLE_ALIGNMENT);
	auto textLeftTop = atlas->GetOrCreateMesh("B", LEFT_ALIGNMENT, TOP_ALIGNMENT);
	auto textRightTop = atlas->GetOrCreateMesh("C", RIGHT_ALIGNMENT, TOP_ALIGNMENT);
	auto textLeftBottom = atlas->GetOrCreateMesh("D", LEFT_ALIGNMENT, BOTTOM_ALIGNMENT);
	auto textRightBottom = atlas->GetOrCreateMesh("E", RIGHT_ALIGNMENT, BOTTOM_ALIGNMENT);

	auto scene = std::make_shared<Scene>("scene");
	auto camera = scene->CreateChild<Camera>();
	camera->EnableOrtho();

	auto nodeCenter = scene->GetOrCreateChild<SceneNode>("nodeCenter");
	auto nodeLeftTop = scene->GetOrCreateChild<SceneNode>("nodeLeftTop");
	auto nodeRightTop = scene->GetOrCreateChild<SceneNode>("nodeRightTop");
	auto nodeLeftBottom = scene->GetOrCreateChild<SceneNode>("nodeLeftBottom");
	auto nodeRightBottom = scene->GetOrCreateChild<SceneNode>("nodeRightBottom");

	nodeCenter->SetMesh(textCenter);
	nodeLeftTop->SetMesh(textLeftTop);
	nodeRightTop->SetMesh(textRightTop);
	nodeLeftBottom->SetMesh(textLeftBottom);
	nodeRightBottom->SetMesh(textRightBottom);

	auto material = App::this_->CreateMaterial();
	material->SetTextMap(atlas->GetTexture());

	nodeCenter->SetMaterial(material);
	nodeLeftTop->SetMaterial(material);
	nodeRightTop->SetMaterial(material);
	nodeLeftBottom->SetMaterial(material);
	nodeRightBottom->SetMaterial(material);

	atlas->SetViewSize(256, 256);
	camera->SetAspectRatio(256, 256);

	{
		Ray ray = Camera::GetRay(0, 0.f);
		std::vector<RayNodeResult> result;
		CHECK_CONDITION(scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
		CHECK_CONDITION(result.size() == 1, __FILE__, __LINE__);
		CHECK_CONDITION(result[0].node_ == nodeCenter.get(), __FILE__, __LINE__);
	}

	{
		Ray ray = Camera::GetRay(-0.9f, 0.9f);
		std::vector<RayNodeResult> result;
		CHECK_CONDITION(scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
		CHECK_CONDITION(result.size() == 1, __FILE__, __LINE__);
		CHECK_CONDITION(result[0].node_ == nodeLeftTop.get(), __FILE__, __LINE__);
	}

	{
		Ray ray = Camera::GetRay(0.9f, 0.9f);
		std::vector<RayNodeResult> result;
		CHECK_CONDITION(scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
		CHECK_CONDITION(result.size() == 1, __FILE__, __LINE__);
		CHECK_CONDITION(result[0].node_ == nodeRightTop.get(), __FILE__, __LINE__);
	}

	{
		Ray ray = Camera::GetRay(-0.9f, -0.9f);
		std::vector<RayNodeResult> result;
		CHECK_CONDITION(scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
		CHECK_CONDITION(result.size() == 1, __FILE__, __LINE__);
		CHECK_CONDITION(result[0].node_ == nodeLeftBottom.get(), __FILE__, __LINE__);
	}

	{
		Ray ray = Camera::GetRay(0.9f, -0.9f);
		std::vector<RayNodeResult> result;
		CHECK_CONDITION(scene->GetPreciseRayNodesIntersection(RenderLayer::DEFAULT_LAYER, ray, result), __FILE__, __LINE__);
		CHECK_CONDITION(result.size() == 1, __FILE__, __LINE__);
		CHECK_CONDITION(result[0].node_ == nodeRightBottom.get(), __FILE__, __LINE__);
	}

}

void OctreeTest()
{
	App app;
	auto window = app.GetOrCreateWindow("window", 0, 0, 1, 1);
    Test01();
    Test02();
    Test03();
    Test04();
	Test05();
	Test06();
    Test07();
	Test08();
}