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
#include "SceneNode.h"
#include "App.h"
#include "Check.h"
#include <vector>
#include <algorithm>

namespace NSG
{
	typedef std::vector<SceneNode*> SceneNodes;
	static SceneNodes s_SceneNodes;

	SceneNode::SceneNode() 
	: pApp_(App::GetPtrInstance()),
	pSelection_(pApp_->GetSelection())
	{
		s_SceneNodes.push_back(this);
	}
		
	SceneNode::~SceneNode()
	{
		auto it = std::find(s_SceneNodes.begin(), s_SceneNodes.end(), this);
		CHECK_ASSERT(it != s_SceneNodes.end(), __FILE__, __LINE__);
		s_SceneNodes.erase(it);
	}

	void SceneNode::SetMaterial(PGLES2Material pMaterial)
	{
		pMaterial_ = pMaterial;
	}

	void SceneNode::SetMesh(PGLES2Mesh pMesh)
	{
		pMesh_ = pMesh;
	}

	void SceneNode::SetBehavior(PBehavior pBehavior)
	{
		pBehavior_ = pBehavior;
		pBehavior_->SetSceneNode(this);
	}

	void SceneNode::Render(bool solid)
	{
		if(!pMaterial_ || !pMaterial_->IsReady()) 
			return;

        CHECK_ASSERT(glGetError() == GL_NO_ERROR, __FILE__, __LINE__);

		if(pSelection_->IsEnabled() && IsSelectionEnabled())
		{
			pSelection_->Render(this);
		}
		else
		{
			pMaterial_->Render(solid, this, pMesh_.get());
	    }
	}

	void SceneNode::RenderForSelect(GLuint position_loc)
	{
		pMesh_->Render(pMesh_->GetSolidDrawMode(), position_loc, -1, -1);
	}

	void SceneNode::Start()
	{
        for(auto &obj : s_SceneNodes)
        {
            if(obj->pBehavior_)
                obj->pBehavior_->Start();
        }
    }

	void SceneNode::Update()
	{
        for(auto &obj : s_SceneNodes)
        {
            if(obj->pBehavior_)
                obj->pBehavior_->Update();
        }
	}

	void SceneNode::LateUpdate()
	{
        for(auto &obj : s_SceneNodes)
        {
            if(obj->pBehavior_)
                obj->pBehavior_->LateUpdate();
        }
	}

	void SceneNode::Render()
	{
        for(auto &obj : s_SceneNodes)
        {
            if(obj->pBehavior_)
                obj->pBehavior_->Render();
        }
	}


}
