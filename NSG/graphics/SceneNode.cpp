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
#include "FrameColorSelection.h"
#include "App.h"
#include "Check.h"
#include "Behavior.h"
#include "Technique.h"
#include "Graphics.h"
#include "Material.h"
#include "Mesh.h"
#include "Scene.h"

namespace NSG
{
    SceneNode::SceneNode()
        : octant_(nullptr),
          occludee_(true),
          worldBBNeedsUpdate_(true)
    {
    }

    SceneNode::~SceneNode()
    {
    }

    void SceneNode::Set(PMaterial material)
    {
        material_ = material;
    }

    void SceneNode::Set(Material* material)
    {
        struct D
        {
            void operator()(Material* p) const
            {
                //delete p; //do not delete
            }
        };

        PMaterial obj(material, D());
        Set(obj);
    }

    void SceneNode::Set(PMesh mesh)
    {
        if (mesh != mesh_)
        {
            mesh_ = mesh;
            worldBB_ = BoundingBox();
            OnDirty();
        }
    }

    void SceneNode::Set(Mesh* mesh)
    {
        struct D
        {
            void operator()(Mesh* p) const
            {
                //delete p; //do not delete
            }
        };

        PMesh obj(mesh, D());
        Set(obj);
    }

    void SceneNode::SetBehavior(PBehavior pBehavior)
    {
        pBehavior_ = pBehavior;
        pBehavior_->SetSceneNode(this);
    }

    void SceneNode::Render()
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        Graphics::this_->Set(material_.get());
        Graphics::this_->Set(mesh_.get());
        Graphics::this_->Set(this);

        material_->GetTechnique()->Render();

        auto it = children_.begin();
        while (it != children_.end())
        {
            SceneNode* p = dynamic_cast<SceneNode*>(*it);
            CHECK_ASSERT(p && "Cannot cast to SceneNode", __FILE__, __LINE__);
            p->Render();
            ++it;
        }
    }

    void SceneNode::OnDirty() const
    {
        worldBBNeedsUpdate_ = true;

        if (octant_ && Context::this_->scene_)
            Context::this_->scene_->Update(this);
    }

    const BoundingBox& SceneNode::GetWorldBoundingBox() const
    {
        if (worldBBNeedsUpdate_)
        {
            if (mesh_ && mesh_->IsReady())
            {
                worldBB_ = mesh_->GetBB();
                worldBB_.Transform(*this);
                worldBBNeedsUpdate_ = false;
            }
        }
        return worldBB_;
    }
}
