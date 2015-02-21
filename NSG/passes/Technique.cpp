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
#include "Technique.h"
#include "Pass.h"
#include "SceneNode.h"
#include "Check.h"
#include "Pass.h"
#include "Batch.h"
#include "Program.h"
#include "Graphics.h"
#include "Material.h"
#include "pugixml.hpp"

namespace NSG
{
    Technique::Technique(Material* material)
    : material_(material)
    {
    }

    Technique::~Technique()
    {
    }

    void Technique::AddPass(PPass pass)
    {
        CHECK_ASSERT(passes_.size() < Technique::MAX_PASSES, __FILE__, __LINE__);
        passes_.push_back(pass);
		if (material_)
			material_->Invalidate();
    }

    PPass Technique::GetPass(unsigned idx) 
    { 
        if(passes_.size() <= idx)
			AddPass(std::make_shared<Pass>(this));

        return passes_.at(idx); 
    }

    size_t Technique::GetNumPasses() const
    {
        return passes_.size();
    }

    void Technique::Draw(Camera* camera)
    {
        Graphics::this_->SetCamera(camera);
        Draw();
    }

    void Technique::Draw()
    {
        for(auto& pass: passes_)
            pass->Draw();
    }

	void Technique::Draw(const Batch& batch)
	{
		auto graphics = Graphics::this_;
		graphics->SetMesh(batch.GetMesh());
		if (batch.AllowInstancing())
		{
			graphics->SetNode(nullptr);
			graphics->DrawInstancedActiveMesh(GetPass(0).get(), batch);
		}
		else
		{
			auto& nodes = batch.GetNodes();
			for (auto& node : nodes)
			{
				SceneNode* sn = (SceneNode*)node;
				graphics->SetNode(sn);
				for (auto& pass : passes_)
					graphics->DrawActiveMesh(pass.get());
			}
		}
	}

    void Technique::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Technique");
        if (passes_.size())
        {
            pugi::xml_node childPasses = child.append_child("Passes");
            for (auto& obj : passes_)
                obj->Save(childPasses);
        }
    }

    void Technique::Load(const pugi::xml_node& node)
    {
		passes_.clear();
        pugi::xml_node childPasses = node.child("Passes");
        if (childPasses)
        {
            pugi::xml_node childPass = childPasses.child("Pass");
            while (childPass)
            {
                auto pass = std::make_shared<Pass>(this);
                AddPass(pass);
                pass->Load(childPass, GetMaterial());
                childPass = childPass.next_sibling("Pass");
            }
        }
    }

    void Technique::CopyPasses(const PASSES& passes)
    {
        passes_.clear();
        for (auto& pass : passes)
			AddPass(pass->Clone(material_));
    }

	bool Technique::IsTransparent() const
	{
		for (auto& pass : passes_)
			if (pass->IsTransparent())
				return true;
		return false;
	}

	bool Technique::IsText() const
	{
		for (auto& pass : passes_)
			if (pass->IsText())
				return true;
		return false;
	}

}