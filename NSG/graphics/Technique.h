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
#pragma once
#include "Types.h"

namespace pugi
{
	class xml_node;
}

namespace NSG
{
	class Technique
	{
	public:
		static const size_t MAX_PASSES = 10;
		Technique(Material* material);
		~Technique();
		void AddPass(PPass pass);
		size_t GetNumPasses() const;
		const PASSES& GetConstPasses() const { return passes_; }
		PPass GetPass(unsigned idx);
		void Save(pugi::xml_node& node);
		void Load(const pugi::xml_node& node);
		void Draw(Camera* camera);
		void Draw();
		void Draw(const Batch& batch);
		Material* GetMaterial() { return material_; }
		void CopyPasses(const PASSES& passes);
		bool IsTransparent() const;
		bool IsText() const;
		void Invalidate();
	private:
		Material* material_;
		PASSES passes_;
	};
}