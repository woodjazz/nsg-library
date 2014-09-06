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
	Scene scene;

	PSphereMesh sphereMesh(new SphereMesh);
	PBoxMesh boxMesh(new BoxMesh);

	PSceneNode node1s = scene.CreateSceneNode("node 1");
	node1s->Set(sphereMesh);
	node1s->SetPosition(Vertex3(0,0,1));

	PSceneNode node1b = scene.CreateSceneNode("node 2");
	node1b->Set(boxMesh);
	node1b->SetPosition(Vertex3(0,0,1));

	PCamera camera = scene.CreateCamera("camera");

	std::vector<const SceneNode*> visibles;
	scene.GetVisibleNodes(camera.get(), visibles);
    CHECK_ASSERT(visibles.size() == 0, __FILE__, __LINE__);

    node1s->SetPosition(Vertex3(0,0,-10));

	scene.GetVisibleNodes(camera.get(), visibles);
    CHECK_ASSERT(visibles.size() == 1, __FILE__, __LINE__);

    node1b->SetPosition(Vertex3(0,0,-100));
	scene.GetVisibleNodes(camera.get(), visibles);
    CHECK_ASSERT(visibles.size() == 2, __FILE__, __LINE__);

	node1s->SetPosition(Vertex3(0, 0, 1));
	scene.GetVisibleNodes(camera.get(), visibles);
	CHECK_ASSERT(visibles.size() == 1, __FILE__, __LINE__);

	node1s->SetPosition(Vertex3(0, 0, 0.8f));
	scene.GetVisibleNodes(camera.get(), visibles);
	CHECK_ASSERT(visibles.size() == 2, __FILE__, __LINE__);

	camera->SetLookAt(Vector3(0, 0, 1));
	scene.GetVisibleNodes(camera.get(), visibles);
	CHECK_ASSERT(visibles.size() == 1, __FILE__, __LINE__);
	
	node1s->SetPosition(Vertex3(0, 0, -0.8f));
	scene.GetVisibleNodes(camera.get(), visibles);
	CHECK_ASSERT(visibles.size() == 1, __FILE__, __LINE__);

	camera->SetPosition(Vertex3(0, 0, 1));

	scene.GetVisibleNodes(camera.get(), visibles);
	CHECK_ASSERT(visibles.size() == 0, __FILE__, __LINE__);

}


void OctreeTest()
{
	Test01();
}