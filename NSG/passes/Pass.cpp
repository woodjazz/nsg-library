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
#include "Material.h"
#include "Mesh.h"

namespace NSG
{
	Pass::Pass()
	: drawMode_(SOLID)
	{

	}

	Pass::~Pass()
	{

	}

	void Pass::SetNode(int idx, PNode node)
	{
		meshNodes_.at(idx).first = node;
	}

	void Pass::Set(PMaterial material)
	{
		material_ = material;
	}

    void Pass::Set(Material* pMaterial)
    {
		struct D {void operator()(Material* p) const {}};    	
		PMaterial pObj(pMaterial, D());
		Set(pObj);
    }

	void Pass::Add(PNode node, PMesh mesh)
	{
		meshNodes_.push_back(MeshNode(node, mesh));
	}

	void Pass::Add(Node* node, PMesh mesh)
	{
		struct D {void operator()(Node* p) const {}};
		PNode obj(node, D());
		Add(obj, mesh);
	}

    void Pass::SetAll(PNode node)
    {
        auto it = meshNodes_.begin();
        while(it != meshNodes_.end())
            (it++)->first = node;
    }

    void Pass::SetAll(Node* node)
    {
    	struct D {void operator()(Node* p) const {}};
    	PNode obj(node, D());
        auto it = meshNodes_.begin();
        while(it != meshNodes_.end())
            (it++)->first = obj;
    }

	PMesh Pass::GetMesh(int idx) const 
	{ 
		return meshNodes_.at(idx).second; 
	}

	void Pass::Render()
	{
		if(material_)
			material_->Render(drawMode_ == SOLID, meshNodes_);
	}
}