/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
    : material_(material),
    lightingMode_(LightingMode::VERTEXCOLOR)
    {
        GetPass(0); // force default pass creation
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
			AddPass(std::make_shared<Pass>());

        return passes_.at(idx); 
    }

    size_t Technique::GetNumPasses() const
    {
        return passes_.size();
    }

    void Technique::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Technique");
        child.append_attribute("lightingMode").set_value((int)lightingMode_);
        if (passes_.size())
        {
            pugi::xml_node childPasses = child.append_child("Passes");
            for (auto& obj : passes_)
                obj->Save(childPasses);
        }
    }

    void Technique::Load(const pugi::xml_node& node)
    {
        SetLightingMode((LightingMode)node.attribute("lightingMode").as_int());
		passes_.clear();
        pugi::xml_node childPasses = node.child("Passes");
        if (childPasses)
        {
            pugi::xml_node childPass = childPasses.child("Pass");
            while (childPass)
            {
                auto pass = std::make_shared<Pass>();
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
			AddPass(pass->Clone());
    }

    void Technique::FillShaderDefines(std::string& defines, const Light* light)
    {
        switch(lightingMode_)
        {
            case LightingMode::VERTEXCOLOR:
				defines += "#define VERTEXCOLOR\n";
                break;
            case LightingMode::UNLIT:
				defines += "#define UNLIT\n";
                break;
            case LightingMode::PERVERTEX:
            {
                if(light)
				    defines += "#define PER_VERTEX_LIGHTING\n";
                else
                    defines += "#define UNLIT\n";
                break;
            }
            case LightingMode::PERPIXEL:
            {
                if(light)
				    defines += "#define PER_PIXEL_LIGHTING\n";
                else
                    defines += "#define UNLIT\n";
                break;
            }
            default:
                CHECK_ASSERT(!"Unkwown lighting mode!!!", __FILE__, __LINE__);
                break;
        }
    }

}