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
#pragma once
#include "SharedPointers.h"
#include "Node.h"


namespace NSG
{
	class App;
	class SceneNode : public Node
	{
	public:
		SceneNode();
		~SceneNode();
		void Set(PTechnique technique);
		void Set(Technique* technique);
		PTechnique GetTechnique() const { return technique_; }
		void SetBehavior(PBehavior pBehavior);
		PBehavior GetBehavior() const { return pBehavior_; }
		virtual void Render();
		//virtual void Render2Select();
	private:
		PTechnique technique_;
		PBehavior pBehavior_;
	};
}