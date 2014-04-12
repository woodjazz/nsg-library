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

		pMaterial_->Render(solid, this, pMesh_.get());
	}

	void SceneNode::Render2Select()
	{
		pSelection_->Render(this);
	}

	void SceneNode::RenderForSelect(GLuint position_loc)
	{
		pMesh_->Render(pMesh_->GetSolidDrawMode(), position_loc, -1, -1);
	}

	void SceneNode::StartAll()
	{
        for(auto &obj : s_SceneNodes)
        {
            if(obj->pBehavior_)
                obj->pBehavior_->Start();
        }
    }

	void SceneNode::UpdateAll()
	{
        for(auto &obj : s_SceneNodes)
        {
            if(obj->pBehavior_)
                obj->pBehavior_->Update();
        }
	}

	void SceneNode::LateUpdateAll()
	{
        for(auto &obj : s_SceneNodes)
        {
            if(obj->pBehavior_)
                obj->pBehavior_->LateUpdate();
        }
	}

	void SceneNode::RenderAll()
	{
        for(auto &obj : s_SceneNodes)
        {
            if(obj->pBehavior_)
                obj->pBehavior_->Render();
        }
	}

	void SceneNode::Render2SelectAll()
	{
        for(auto &obj : s_SceneNodes)
        {
            if(obj->pBehavior_)
                obj->pBehavior_->Render2Select();
        }
	}

    void SceneNode::OnMouseMoveAll(float x, float y)
    {
        for(auto &obj : s_SceneNodes)
        {
            if(obj->pBehavior_)
                obj->pBehavior_->OnMouseMove(x, y);
        }
    }

    void SceneNode::OnMouseDownAll(float x, float y)
    {
        for(auto &obj : s_SceneNodes)
        {
            if(obj->pBehavior_)
                obj->pBehavior_->OnMouseDown(x, y);
        }
    }

    void SceneNode::OnMouseUpAll()
    {
        for(auto &obj : s_SceneNodes)
        {
            if(obj->pBehavior_)
                obj->pBehavior_->OnMouseUp();
        }
    }

    void SceneNode::OnKeyAll(int key, int action, int modifier)
    {
        for(auto &obj : s_SceneNodes)
        {
            if(obj->pBehavior_)
                obj->pBehavior_->OnKey(key, action, modifier);
        }
    }

    void SceneNode::OnCharAll(unsigned int character)
    {
        for(auto &obj : s_SceneNodes)
        {
            if(obj->pBehavior_)
                obj->pBehavior_->OnChar(character);
        }
    }
}
