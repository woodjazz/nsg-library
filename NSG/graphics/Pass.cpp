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
#include "Pass.h"
#include "GLES2Material.h"
#include "GLES2Mesh.h"

namespace NSG
{
	Pass::Pass()
	: drawMode_(SOLID),
	node_(nullptr)
	{

	}

	Pass::~Pass()
	{

	}

	void Pass::SetNode(Node* node)
	{
		node_ = node;
	}

	void Pass::Set(PGLES2Material material)
	{
		material_ = material;
	}

    void Pass::Set(GLES2Material* pMaterial)
    {
		struct D 
		{ 
		    void operator()(GLES2Material* p) const 
		    {
		        //delete p; //do not delete
		    }
		};    	

		PGLES2Material pObj(pMaterial, D());
		Set(pObj);
    }

	void Pass::Add(PGLES2Mesh mesh)
	{
		meshes_.push_back(mesh);
	}

	PGLES2Mesh Pass::GetMesh(int idx) const 
	{ 
		return meshes_.at(idx); 
	}

	void Pass::Render()
	{
		if(material_)
		{
			material_->Render(drawMode_ == SOLID, node_, meshes_);
		}
	}
}