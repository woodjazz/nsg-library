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
#include "GLES2BoxMesh.h"

namespace NSG
{
	GLES2BoxMesh::GLES2BoxMesh(float width, float height, float depth, int resX, int resY, int resZ, GLenum usage) 
	: GLES2Mesh(usage)
	{
		// halves //
		float halfW = width * .5f;
		float halfH = height * .5f;
		float halfD = depth * .5f;

		Vertex3 vert;
		Vertex2 texcoord;
		Vertex3 normal;
		int vertOffset = 0;

		GLES2Mesh::Data& data = GetVertexData();

		// TRIANGLES //

		// Front Face //
		normal = Vertex3(0, 0, 1);
		// add the vertexes //
		for(int iy = 0; iy < resY; iy++) 
		{
		    for(int ix = 0; ix < resX; ix++) 
		    {
		        
		        // normalized tex coords //
		        texcoord.x = ((float)ix/((float)resX-1.f));
		        texcoord.y = ((float)iy/((float)resY-1.f));
		        
		        vert.x = texcoord.x * width - halfW;
		        vert.y = texcoord.y * height - halfH;
		        vert.z = halfD;

				VertexData vertexData;
				vertexData.normal_ = normal;
				vertexData.position_ = vert;
				vertexData.uv_ = texcoord;

				data.push_back(vertexData);
		    }
		}

		Indexes indexes;

		for(int y = 0; y < resY-1; y++)
		{
		    for(int x = 0; x < resX-1; x++) 
		    {
		        // first triangle //
		        indexes.push_back((y)*resX + x + vertOffset);
		        indexes.push_back((y)*resX + x+1 + vertOffset);
		        indexes.push_back((y+1)*resX + x + vertOffset);
		        
		        // second triangle //
		        indexes.push_back((y)*resX + x+1 + vertOffset);
		        indexes.push_back((y+1)*resX + x+1 + vertOffset);
		        indexes.push_back((y+1)*resX + x + vertOffset);
		    }
		}

		vertOffset = data.size();


		// Right Side Face //
		normal = Vertex3(1, 0, 0);
		// add the vertexes //
		for(int iy = 0; iy < resY; iy++) 
		{
		    for(int ix = 0; ix < resZ; ix++) 
		    {
		        // normalized tex coords //
		        texcoord.x = ((float)ix/((float)resZ-1.f));
		        texcoord.y = ((float)iy/((float)resY-1.f));
		        
		        //vert.x = texcoord.x * width - halfW;
		        vert.x = halfW;
		        vert.y = texcoord.y * height - halfH;
		        vert.z = texcoord.x * -depth + halfD;
		        
				VertexData vertexData;
				vertexData.normal_ = normal;
				vertexData.position_ = vert;
				vertexData.uv_ = texcoord;

				data.push_back(vertexData);
		    }
		}

		for(int y = 0; y < resY-1; y++) 
		{
		    for(int x = 0; x < resZ-1; x++) 
		    {
		        // first triangle //
		        indexes.push_back((y)*resZ + x + vertOffset);
		        indexes.push_back((y)*resZ + x+1 + vertOffset);
		        indexes.push_back((y+1)*resZ + x + vertOffset);
		        
		        // second triangle //
		        indexes.push_back((y)*resZ + x+1 + vertOffset);
		        indexes.push_back((y+1)*resZ + x+1 + vertOffset);
		        indexes.push_back((y+1)*resZ + x + vertOffset);
		    }
		}

		vertOffset = data.size();

		// Left Side Face //
		normal = Vertex3(-1, 0, 0);
		// add the vertexes //
		for(int iy = 0; iy < resY; iy++) 
		{
		    for(int ix = 0; ix < resZ; ix++) 
		    {
		        
		        // normalized tex coords //
		        texcoord.x = ((float)ix/((float)resZ-1.f));
		        texcoord.y = ((float)iy/((float)resY-1.f));
		        
		        //vert.x = texcoord.x * width - halfW;
		        vert.x = -halfW;
		        vert.y = texcoord.y * height - halfH;
		        vert.z = texcoord.x * depth - halfD;
		        
				VertexData vertexData;
				vertexData.normal_ = normal;
				vertexData.position_ = vert;
				vertexData.uv_ = texcoord;

				data.push_back(vertexData);
		    }
		}

		for(int y = 0; y < resY-1; y++) 
		{
		    for(int x = 0; x < resZ-1; x++) 
		    {
		        // first triangle //
		        indexes.push_back((y)*resZ + x + vertOffset);
		        indexes.push_back((y)*resZ + x+1 + vertOffset);
		        indexes.push_back((y+1)*resZ + x + vertOffset);
		        
		        // second triangle //
		        indexes.push_back((y)*resZ + x+1 + vertOffset);
		        indexes.push_back((y+1)*resZ + x+1 + vertOffset);
		        indexes.push_back((y+1)*resZ + x + vertOffset);
		    }
		}

		vertOffset = data.size();


		// Back Face //
		normal = Vertex3(0, 0, -1);
		// add the vertexes //
		for(int iy = 0; iy < resY; iy++) 
		{
		    for(int ix = 0; ix < resX; ix++) 
		    {
		        
		        // normalized tex coords //
		        texcoord.x = ((float)ix/((float)resX-1.f));
		        texcoord.y = ((float)iy/((float)resY-1.f));
		        
		        vert.x = texcoord.x * -width + halfW;
		        vert.y = texcoord.y * height - halfH;
		        vert.z = -halfD;
		        
				VertexData vertexData;
				vertexData.normal_ = normal;
				vertexData.position_ = vert;
				vertexData.uv_ = texcoord;

				data.push_back(vertexData);
		    }
		}

		for(int y = 0; y < resY-1; y++) 
		{
		    for(int x = 0; x < resX-1; x++) 
		    {
		        // first triangle //
		        indexes.push_back((y)*resX + x + vertOffset);
		        indexes.push_back((y)*resX + x+1 + vertOffset);
		        indexes.push_back((y+1)*resX + x + vertOffset);
		        
		        // second triangle //
		        indexes.push_back((y)*resX + x+1 + vertOffset);
		        indexes.push_back((y+1)*resX + x+1 + vertOffset);
		        indexes.push_back((y+1)*resX + x + vertOffset);
		    }
		}

		vertOffset = data.size();

		// Top Face //
		normal = Vertex3(0, -1, 0);
		// add the vertexes //
		for(int iy = 0; iy < resZ; iy++) 
		{
		    for(int ix = 0; ix < resX; ix++) 
		    {
		        
		        // normalized tex coords //
		        texcoord.x = ((float)ix/((float)resX-1.f));
		        texcoord.y = ((float)iy/((float)resZ-1.f));
		        
		        vert.x = texcoord.x * width - halfW;
		        //vert.y = texcoord.y * height - halfH;
		        vert.y = -halfH;
		        vert.z = texcoord.y * depth - halfD;
		        
				VertexData vertexData;
				vertexData.normal_ = normal;
				vertexData.position_ = vert;
				vertexData.uv_ = texcoord;

				data.push_back(vertexData);
		    }
		}

		for(int y = 0; y < resZ-1; y++) 
		{
		    for(int x = 0; x < resX-1; x++) 
		    {
		        // first triangle //
		        indexes.push_back((y)*resX + x + vertOffset);
		        indexes.push_back((y)*resX + x+1 + vertOffset);
		        indexes.push_back((y+1)*resX + x + vertOffset);
		        
		        // second triangle //
		        indexes.push_back((y)*resX + x+1 + vertOffset);
		        indexes.push_back((y+1)*resX + x+1 + vertOffset);
		        indexes.push_back((y+1)*resX + x + vertOffset);
		    }
		}

		vertOffset = data.size();

		// Bottom Face //
		normal = Vertex3(0, 1, 0);
		// add the vertexes //
		for(int iy = 0; iy < resZ; iy++) 
		{
		    for(int ix = 0; ix < resX; ix++) 
		    {
		        // normalized tex coords //
		        texcoord.x = ((float)ix/((float)resX-1.f));
		        texcoord.y = ((float)iy/((float)resZ-1.f));
		        
		        vert.x = texcoord.x * width - halfW;
		        //vert.y = texcoord.y * height - halfH;
		        vert.y = halfH;
		        vert.z = texcoord.y * -depth + halfD;
		        
				VertexData vertexData;
				vertexData.normal_ = normal;
				vertexData.position_ = vert;
				vertexData.uv_ = texcoord;

				data.push_back(vertexData);
		    }
		}

		for(int y = 0; y < resZ-1; y++) 
		{
		    for(int x = 0; x < resX-1; x++) 
		    {
		        // first triangle //
		        indexes.push_back((y)*resX + x + vertOffset);
		        indexes.push_back((y)*resX + x+1 + vertOffset);
		        indexes.push_back((y+1)*resX + x + vertOffset);
		        
		        // second triangle //
		        indexes.push_back((y)*resX + x+1 + vertOffset);
		        indexes.push_back((y+1)*resX + x+1 + vertOffset);
		        indexes.push_back((y+1)*resX + x + vertOffset);
		    }
		}

	    SetIndices(indexes);
	    Redo();
	}

	GLES2BoxMesh::~GLES2BoxMesh() 
	{
	}
}
