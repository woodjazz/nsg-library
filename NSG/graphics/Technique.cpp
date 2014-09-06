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
#include "pugixml.hpp"

namespace NSG
{
	Technique::Technique()
	{
	}

	Technique::~Technique()
	{

	}

	void Technique::Add(PPass pass)
	{
		CHECK_ASSERT(passes_.size() < Technique::MAX_PASSES, __FILE__, __LINE__);
		passes_.push_back(pass);
	}

	size_t Technique::GetNumPasses() const
	{
		return passes_.size();
	}

	bool Technique::Render()
	{
		bool drawn = false;
		auto it = passes_.begin();
		while(it != passes_.end())
			drawn |= (*it++)->Render();
		return drawn;
	}

	void Technique::Save(pugi::xml_node& node)
	{
		pugi::xml_node child = node.append_child("Technique");
		if(passes_.size())
		{
			pugi::xml_node childPasses = child.append_child("Passes");
			for(auto& obj: passes_)
				obj->Save(childPasses);
		}
	}

	void Technique::Load(const pugi::xml_node& node)
	{
		pugi::xml_node childPasses = node.child("Passes");
		if(childPasses)
		{
			pugi::xml_node childPass = childPasses.child("Pass");
			while(childPass)
			{
				PPass pass(new Pass);
				Add(pass);
				pass->Load(childPass);
				childPass = node.next_sibling("Pass");
			}
		}
	}
}