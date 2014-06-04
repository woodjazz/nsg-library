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

class MyBehavior : public Behavior
{
public:
	MyBehavior()
	{
	}
	~MyBehavior()
	{
	}

	void Start()
	{
		PBoxMesh pMesh(new BoxMesh(1,1,1, 2,2,2, GL_STATIC_DRAW));
        PTechnique technique(new Technique);
        pSceneNode_->Set(technique);
        PPass pass(new Pass);
    	pass->Add(pSceneNode_, pMesh);
	}

	void Update()
	{
	}

	void Render()
	{
		//pSceneNode_->Render(true);
	}
#if 0
	void Render2Select()
	{
		pSceneNode_->Render2Select();
	}
#endif	
};

struct Test : public App 
{
    PSceneNode sceneNode_;
    PCamera camera_;
    PFrameColorSelection colorSelection_;

	void Start()
	{
        colorSelection_ = PFrameColorSelection(new FrameColorSelection(false));
		sceneNode_ = PSceneNode(new SceneNode);
		sceneNode_->SetBehavior(PBehavior(new MyBehavior));	
		camera_ = PCamera(new Camera);
        camera_->EnableOrtho();
		camera_->Activate();
	}

	void Render()
	{
		sceneNode_->SetPosition(Vertex3(0, 0, 0));
        colorSelection_->Render(sceneNode_->GetId(), 0, 0, sceneNode_->GetTechnique().get());
        GLushort id = colorSelection_->GetSelected();
		CHECK_ASSERT(id == sceneNode_->GetId(), __FILE__, __LINE__);
		
		sceneNode_->SetPosition(Vertex3(-1, 0, 0));
        colorSelection_->Render(sceneNode_->GetId(), 0, 0, sceneNode_->GetTechnique().get());
        id = colorSelection_->GetSelected();
		CHECK_ASSERT(id == 0, __FILE__, __LINE__);

		sceneNode_->SetPosition(Vertex3(-1, 0, 0));
        colorSelection_->Render(sceneNode_->GetId(), 0, 0, sceneNode_->GetTechnique().get());
        id = colorSelection_->GetSelected();
		CHECK_ASSERT(id == sceneNode_->GetId(), __FILE__, __LINE__);

		sceneNode_->SetPosition(Vertex3(1, 1, 0));
        colorSelection_->Render(sceneNode_->GetId(), 0, 0, sceneNode_->GetTechnique().get());
        id = colorSelection_->GetSelected();
		CHECK_ASSERT(id == sceneNode_->GetId(), __FILE__, __LINE__);
	}

	bool ShallExit() const 
	{ 
		return true; 
	}
};


NSG_MAIN(Test);
