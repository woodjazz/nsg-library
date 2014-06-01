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
#include "Behavior.h"
#include "App.h"
#include "Check.h"
#include <vector>
#include <algorithm>

typedef std::vector<NSG::Behavior*> Collection;
static Collection collection;

namespace NSG 
{
	Behavior::Behavior()
	{
		collection.push_back(this);
	}

	Behavior::~Behavior()
	{
		auto it = std::find(collection.begin(), collection.end(), this);
		CHECK_ASSERT(it != collection.end(), __FILE__, __LINE__);
		collection.erase(it);
	}

	void Behavior::SetSceneNode(SceneNode* pSceneNode) 
	{
		pSceneNode_ = pSceneNode; 
	}

	void Behavior::StartAll()
	{
        for(auto &obj : collection)
        {
			obj->Start();
        }
    }

	void Behavior::UpdateAll()
	{
        for(auto &obj : collection)
        {
			obj->Update();
        }
	}

	void Behavior::RenderAll()
	{
        for(auto &obj : collection)
        {
			obj->Render();
        }
	}

    void Behavior::RenderAllBut(const Behavior* behavior)
    {
        for(auto &obj : collection)
        {
            if(obj != behavior)
                obj->Render();
        }
    }

	void Behavior::Render2SelectAll()
	{
        for(auto &obj : collection)
        {
			obj->Render2Select();
        }
	}

    void Behavior::OnMouseMoveAll(float x, float y)
    {
        for(auto &obj : collection)
        {
			obj->OnMouseMove(x, y);
        }
    }

    void Behavior::OnMouseDownAll(float x, float y)
    {
        for(auto &obj : collection)
        {
			obj->OnMouseDown(x, y);
        }
    }

    void Behavior::OnMouseUpAll()
    {
        for(auto &obj : collection)
        {
			obj->OnMouseUp();
        }
    }

    void Behavior::OnKeyAll(int key, int action, int modifier)
    {
        for(auto &obj : collection)
        {
			obj->OnKey(key, action, modifier);
        }
    }

    void Behavior::OnCharAll(unsigned int character)
    {
        for(auto &obj : collection)
        {
			obj->OnChar(character);
        }
    }
}