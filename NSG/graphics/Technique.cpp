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

    void Technique::Add(Pass* pass)
    {
		struct D 
		{ 
		    void operator()(Pass* p) const 
		    {
		        //delete p; //do not delete
		    }
		};    	

		PPass pObj(pass, D());
		Add(pObj);
    }

    void Technique::Set(PNode node)
    {
    	auto it = passes_.begin();
    	while(it != passes_.end())
    		(*it++)->SetAll(node);
    }

    void Technique::Set(Node* node)
    {
    	auto it = passes_.begin();
    	while(it != passes_.end())
    		(*it++)->SetAll(node);
    }

	size_t Technique::GetNumPasses() const
	{
		return passes_.size();
	}

	void Technique::Render()
	{
		auto it = passes_.begin();
		while(it != passes_.end())
			(*it++)->Render();
	}
}