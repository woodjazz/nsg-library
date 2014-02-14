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
#include "BoxMesh.h"

namespace NSG
{
	BoxMesh::BoxMesh(PGLES2Program pProgram, PGLES2Texture pTexture) 
	: Mesh(pProgram, pTexture)
	{
		AddVertexData(VertexData(Vertex3(-1.0, -1.0, +1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(1.0, 0.0)));
	    AddVertexData(VertexData(Vertex3(+1.0, -1.0, +1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(0.0, 0.0)));
	    AddVertexData(VertexData(Vertex3(+1.0, +1.0, +1.0), Vertex3(0.5, 0.0, 0.0), Vertex2(0.0, 1.0)));
	    AddVertexData(VertexData(Vertex3(-1.0, +1.0, +1.0), Vertex3(0.5, 0.0, 0.0), Vertex2(1.0, 1.0)));

		AddVertexData(VertexData(Vertex3(+1.0, -1.0, -1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(1.0, 0.0)));
	    AddVertexData(VertexData(Vertex3(+1.0, +1.0, -1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(0.0, 0.0)));
	    AddVertexData(VertexData(Vertex3(+1.0, +1.0, +1.0), Vertex3(0.0, 0.5, 0.0), Vertex2(0.0, 1.0)));
	    AddVertexData(VertexData(Vertex3(+1.0, -1.0, +1.0), Vertex3(0.0, 0.5, 0.0), Vertex2(1.0, 1.0)));

		AddVertexData(VertexData(Vertex3(-1.0, +1.0, -1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(1.0, 0.0)));
	    AddVertexData(VertexData(Vertex3(-1.0, +1.0, +1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(0.0, 0.0)));
	    AddVertexData(VertexData(Vertex3(+1.0, +1.0, +1.0), Vertex3(0.0, 0.0, 0.5), Vertex2(0.0, 1.0)));
	    AddVertexData(VertexData(Vertex3(+1.0, +1.0, -1.0), Vertex3(0.0, 0.0, 0.5), Vertex2(1.0, 1.0)));

		AddVertexData(VertexData(Vertex3(+1.0, +1.0, -1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(1.0, 1.0)));
	    AddVertexData(VertexData(Vertex3(-1.0, +1.0, -1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(0.0, 1.0)));
	    AddVertexData(VertexData(Vertex3(-1.0, -1.0, -1.0), Vertex3(1.0, 0.0, 0.0), Vertex2(0.0, 0.0)));
	    AddVertexData(VertexData(Vertex3(+1.0, -1.0, -1.0), Vertex3(1.0, 0.0, 0.0), Vertex2(1.0, 0.0)));

		AddVertexData(VertexData(Vertex3(-1.0, +1.0, +1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(1.0, 1.0)));
	    AddVertexData(VertexData(Vertex3(-1.0, -1.0, +1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(0.0, 1.0)));
	    AddVertexData(VertexData(Vertex3(-1.0, -1.0, -1.0), Vertex3(0.0, 1.0, 0.0), Vertex2(0.0, 0.0)));
	    AddVertexData(VertexData(Vertex3(-1.0, +1.0, -1.0), Vertex3(0.0, 1.0, 0.0), Vertex2(1.0, 0.0)));

		AddVertexData(VertexData(Vertex3(+1.0, -1.0, +1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(1.0, 1.0)));
	    AddVertexData(VertexData(Vertex3(+1.0, -1.0, -1.0), Vertex3(0.0, 0.0, 0.0), Vertex2(0.0, 1.0)));
	    AddVertexData(VertexData(Vertex3(-1.0, -1.0, -1.0), Vertex3(0.0, 0.0, 1.0), Vertex2(0.0, 0.0)));
	    AddVertexData(VertexData(Vertex3(-1.0, -1.0, +1.0), Vertex3(0.0, 0.0, 1.0), Vertex2(1.0, 0.0)));


		const IndexType kCubeIndexes[36] = {
			2,  1,  0,  3,  2,  0,
			6,  5,  4,  7,  6,  4,
			10,  9,  8, 11, 10,  8,
			14, 13, 12, 15, 14, 12,
			18, 17, 16, 19, 18, 16,
			22, 21, 20, 23, 22, 20,
		};

	    Indexes indexes;
	    indexes.insert(indexes.end(), &kCubeIndexes[0], &kCubeIndexes[sizeof(kCubeIndexes)/sizeof(IndexType)]);
	    SetIndices(indexes);
	    Redo();
	}

	BoxMesh::~BoxMesh() 
	{
	}
}