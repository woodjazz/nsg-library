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
#include "SceneNode.h"
#include "assimp/IOSystem.hpp"
#include <memory>

struct aiScene;
struct aiNode;
namespace NSG
{
	class Model : public Assimp::IOSystem
	{
	public:
		Model();
		~Model();
		void Load(const char* filename);

        bool Exists(const char* filename) const;
		char getOsSeparator() const;
	    Assimp::IOStream* Open(const char* filename, const char* mode = "rb");
		void Close(Assimp::IOStream* pFile);

	private:
		void RecursiveLoad(const aiScene *sc, const aiNode* nd);
		PSceneNode pRoot_;
	};

	typedef std::shared_ptr<Model> PModel;
}