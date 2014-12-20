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
int NSG_MAIN(int argc, char* argv[])
{
	using namespace NSG;

	App app;
	auto window = app.GetOrCreateWindow("window", 0, 0, 10, 10);
	auto colorSelection = std::make_shared<FrameColorSelection>();

	auto scene = std::make_shared<Scene>("scene000");
	auto pMesh = app.CreateBoxMesh(1, 1, 1, 2, 2, 2);

	auto sceneNode = scene->GetOrCreateChild<SceneNode>("scene node");
	sceneNode->SetMesh(pMesh);

	auto slotRender = window->signalRender_->Connect([&]()
	{
		sceneNode->SetPosition(Vertex3(0, 0, 0));
        std::vector<SceneNode*> nodes;
        nodes.push_back(sceneNode.get());
        
		colorSelection->Render(sceneNode->GetId(), 0, 0, nodes);
        GLushort id = colorSelection->GetSelected();
		CHECK_ASSERT(id == sceneNode->GetId(), __FILE__, __LINE__);
		
		sceneNode->SetPosition(Vertex3(-1, 0, 0));
		colorSelection->Render(sceneNode->GetId(), 0, 0, nodes);
        id = colorSelection->GetSelected();
		CHECK_ASSERT(id != sceneNode->GetId(), __FILE__, __LINE__);

		sceneNode->SetPosition(Vertex3(-1, 0, 0));
		colorSelection->Render(sceneNode->GetId(), -0.7f, 0, nodes);
        id = colorSelection->GetSelected();
		CHECK_ASSERT(id == sceneNode->GetId(), __FILE__, __LINE__);

		sceneNode->SetPosition(Vertex3(1, 1, 0));
		colorSelection->Render(sceneNode->GetId(), -0.7f, 0, nodes);
        id = colorSelection->GetSelected();
		CHECK_ASSERT(id != sceneNode->GetId(), __FILE__, __LINE__);

		sceneNode->SetPosition(Vertex3(1, 1, 0));
		colorSelection->Render(sceneNode->GetId(), 0.75f, 0.75f, nodes);
		id = colorSelection->GetSelected();
		CHECK_ASSERT(id == sceneNode->GetId(), __FILE__, __LINE__);

		colorSelection = nullptr;
		window = nullptr;
	});

	return app.Run();
};

