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

struct Test : public App 
{
    PSceneNode sceneNode_;
    PCamera camera_;
    PFrameColorSelection colorSelection_;
	bool exit_;

	Test()
		:exit_(false)
	{

	}

	void Start() override
	{
        PBoxMesh pMesh(new BoxMesh(1,1,1, 2,2,2));
        
		sceneNode_ = PSceneNode(new SceneNode);
        sceneNode_->Set(pMesh);

        colorSelection_ = PFrameColorSelection(new FrameColorSelection(true, true));
        
		camera_ = PCamera(new Camera);
        camera_->EnableOrtho();
		camera_->Activate();
	}

	void RenderFrame() override
	{
		sceneNode_->SetPosition(Vertex3(0, 0, 0));
        std::vector<SceneNode*> nodes;
        nodes.push_back(sceneNode_.get());
        
		colorSelection_->Render(sceneNode_->GetId(), 0, 0, nodes);
        GLushort id = colorSelection_->GetSelected();
		CHECK_ASSERT(id == sceneNode_->GetId(), __FILE__, __LINE__);
		
		sceneNode_->SetPosition(Vertex3(-1, 0, 0));
		colorSelection_->Render(sceneNode_->GetId(), 0, 0, nodes);
        id = colorSelection_->GetSelected();
		CHECK_ASSERT(id != sceneNode_->GetId(), __FILE__, __LINE__);

		sceneNode_->SetPosition(Vertex3(-1, 0, 0));
		colorSelection_->Render(sceneNode_->GetId(), -0.7f, 0, nodes);
        id = colorSelection_->GetSelected();
		CHECK_ASSERT(id == sceneNode_->GetId(), __FILE__, __LINE__);

		sceneNode_->SetPosition(Vertex3(1, 1, 0));
		colorSelection_->Render(sceneNode_->GetId(), -0.7f, 0, nodes);
        id = colorSelection_->GetSelected();
		CHECK_ASSERT(id != sceneNode_->GetId(), __FILE__, __LINE__);

		sceneNode_->SetPosition(Vertex3(1, 1, 0));
		colorSelection_->Render(sceneNode_->GetId(), 0.75f, 0.75f, nodes);
		id = colorSelection_->GetSelected();
		CHECK_ASSERT(id == sceneNode_->GetId(), __FILE__, __LINE__);

		exit_ = true;
	}

	bool ShallExit() const override
	{ 
		return exit_;
	}
};


NSG_MAIN(Test);
