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
#include "Behavior.h"
#include "GLES2Material.h"

namespace NSG
{
	SceneNode::SceneNode() 
	{
	}
		
	SceneNode::~SceneNode()
	{
	}

	void SceneNode::SetMaterial(PGLES2Material pMaterial)
	{
		pMaterial_ = pMaterial;
	}

    void SceneNode::SetMaterial(GLES2Material* pMaterial)
    {
		struct D 
		{ 
		    void operator()(GLES2Material* p) const 
		    {
		        //delete p; //do not delete
		    }
		};    	

		PGLES2Material pObj(pMaterial, D());
		SetMaterial(pObj);
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
        CHECK_GL_STATUS(__FILE__, __LINE__);

		if(pMesh_ && pMesh_->IsReady() && pMaterial_ && pMaterial_->IsReady()) 
        {
            pMaterial_->Render(solid, this, pMesh_.get());
        }

        auto it = children_.begin();

        while(it != children_.end())
        {
            SceneNode* p = static_cast<SceneNode*>(*it);
            CHECK_ASSERT(p && "Cannot cast to SceneNode", __FILE__, __LINE__);
            p->Render(solid);
            ++it;
        }

	}

	void SceneNode::Render2Select()
	{
        if(pMesh_)
        {
		    Context::this_->pFrameColorSelection_->Render(this);
        }

        auto it = children_.begin();

        while(it != children_.end())
        {
            SceneNode* p = static_cast<SceneNode*>(*it);
            CHECK_ASSERT(p && "Cannot cast to SceneNode", __FILE__, __LINE__);
            p->Render2Select();
            ++it;
        }
	}
}
