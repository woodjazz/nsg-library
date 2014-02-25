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
#include "SphereMesh.h"
#include "Types.h"
#include "Log.h"

namespace NSG
{
	SphereMesh::SphereMesh(Color color, float radius, int slices, int stacks, PGLES2Program pProgram, PGLES2Texture pTexture, GLenum usage) 
	: Mesh(pProgram, pTexture, usage)
	{
		Mesh::Data& data = GetVertexData();

		for (int i=0; i<slices/2.0; i++) 
		{
			float r = 2*i*radius/(slices-1);
			
			for(int j=0; j<stacks; j++) 
			{
				float a = 2*glm::pi<float>() / stacks * j;
				float x = cos(a) * r;
				float y = sin(a) * r;
				float z = sqrt(std::max(0.0f, radius*radius - x*x - y*y));

				VertexData vertexData;
				vertexData.position_ = Vertex3(x,y,z);
				vertexData.normal_ = glm::normalize(vertexData.position_);
				vertexData.color_ = color;
				vertexData.uv_ = Vertex2((glm::atan(vertexData.normal_.y, vertexData.normal_.x) / glm::pi<float>() + 1.0) * 0.5, 
					(asin(vertexData.normal_.z) / glm::pi<float>() + 0.5));

				//TRACE_LOG("vertexData.position_=" << vertexData.position_.x << "," << vertexData.position_.y << "," << vertexData.position_.x << "\n");

				data.push_back(vertexData);
			}
		}

		// Mirror the other half with -z, but in the same stack order
		for (int i=(int)ceil(slices/2.0); i< slices; i++) 
		{
			for(int j=0; j<stacks; j++) 
			{
				int idx = (slices-1-i) * stacks + j;
				Vertex3 ov = data[idx].position_;

				VertexData vertexData;
				vertexData.position_ = Vertex3(ov.x, ov.y, -ov.z);
				vertexData.normal_ = glm::normalize(vertexData.position_);
				vertexData.color_ = color;
				vertexData.uv_ = Vertex2((glm::atan(vertexData.normal_.y, vertexData.normal_.x) / glm::pi<float>() + 1.0) * 0.5, 
					(asin(vertexData.normal_.z) / glm::pi<float>() + 0.5));

				data.push_back(vertexData);
			}
		}

		Indexes indexes;

		for (int i=0; i<slices-1; i++) 
		{
			for (int j=0; j<stacks; j++) 
			{
				indexes.push_back(   i  * stacks + j);
				indexes.push_back((i+1) * stacks + j);
				indexes.push_back(   i  * stacks + (j+1)%stacks);
				indexes.push_back(   i  * stacks + (j+1)%stacks);
				indexes.push_back((i+1) * stacks + j);
				indexes.push_back((i+1) * stacks + (j+1)%stacks);
			}
		}	

		SetIndices(indexes);

	    Redo();
	}

	SphereMesh::~SphereMesh() 
	{
	}

}