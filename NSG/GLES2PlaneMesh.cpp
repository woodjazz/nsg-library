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
#include "GLES2PlaneMesh.h"
#include "Types.h"

namespace NSG
{
	GLES2PlaneMesh::GLES2PlaneMesh(float width, float height, int columns, int rows, GLenum usage) 
	: GLES2Mesh(usage)
	{
		GLES2Mesh::Data& data = vertexsData_;

		Vertex3 vert;
		Vertex3 normal(0, 0, 1); // always facing forward //
		Vertex2 texcoord;

		// the origin of the plane is the center //
		float halfW = width/2.f;
		float halfH = height/2.f;
		// add the vertexes //
		for(int iy = 0; iy < rows; iy++) 
		{
		    for(int ix = 0; ix < columns; ix++) 
		    {
		        // normalized tex coords //
		        texcoord.x = ((float)ix/((float)columns-1.f));
		        texcoord.y = ((float)iy/((float)rows-1.f));
		        
		        vert.x = texcoord.x * width - halfW;
		        vert.y = texcoord.y * height - halfH;

				VertexData vertexData;
				vertexData.normal_ = normal;
				vertexData.position_ = vert;
				vertexData.uv_ = texcoord;

				data.push_back(vertexData);
		    }
		}

	    // Triangles //
	    for(int y = 0; y < rows-1; y++) 
	    {
	        for(int x = 0; x < columns-1; x++) 
	        {
	            // first triangle //
	            indexes_.push_back((y)*columns + x);
	            indexes_.push_back((y)*columns + x+1);
	            indexes_.push_back((y+1)*columns + x);
	            
	            // second triangle //
	            indexes_.push_back((y)*columns + x+1);
	            indexes_.push_back((y+1)*columns + x+1);
	            indexes_.push_back((y+1)*columns + x);
	        }
	    }
	}

	GLES2PlaneMesh::~GLES2PlaneMesh() 
	{
	}

	GLenum GLES2PlaneMesh::GetWireFrameDrawMode() const
	{
		return GL_LINE_LOOP;
	}

	GLenum GLES2PlaneMesh::GetSolidDrawMode() const
	{
		return GL_TRIANGLES;
	}

}
