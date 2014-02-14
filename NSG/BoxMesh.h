/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 N�stor Silveira Gorski

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
#include <vector>
#include <memory>
#include "NSG/GLES2Program.h"
#include "NSG/GLES2VertexBuffer.h"
#include "NSG/GLES2IndexBuffer.h"
#include "NSG/GLES2Texture.h"
#include "NSG/Types.h"
#include "Mesh.h"

namespace NSG
{
	class BoxMesh : public Mesh
	{
	public:
		BoxMesh(PGLES2Program pProgram, PGLES2Texture pTexture);
		~BoxMesh();
	};

	typedef std::shared_ptr<BoxMesh> PBoxMesh;
}
