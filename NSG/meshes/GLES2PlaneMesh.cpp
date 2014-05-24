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
#include "Check.h"
#include "ResourceProcedural.h"
namespace NSG
{
	GLES2PlaneMesh::GLES2PlaneMesh(float width, float height, int columns, int rows, GLenum usage) 
	: GLES2Mesh(usage),
	 width_(width),
	 height_(height),
	 columns_(columns),
	 rows_(rows)
	{
		resource_ = PResource(new ResourceProcedural(this));
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

	void GLES2PlaneMesh::Build()
	{
		vertexsData_.clear();
		indexes_.clear();
		
		VertexsData& data = vertexsData_;

		Vertex3 vert;
		Vertex3 normal(0, 0, 1); // always facing forward //
		Vertex2 texcoord;

		// the origin of the plane is the center //
		float halfW = width_/2.f;
		float halfH = height_/2.f;
		// add the vertexes //
		for(int iy = 0; iy < rows_; iy++) 
		{
		    for(int ix = 0; ix < columns_; ix++) 
		    {
		        // normalized tex coords //
		        texcoord.x = ((float)ix/((float)columns_-1.f));
		        texcoord.y = ((float)iy/((float)rows_-1.f));
		        
		        vert.x = texcoord.x * width_ - halfW;
		        vert.y = texcoord.y * height_ - halfH;

				VertexData vertexData;
				vertexData.normal_ = normal;
				vertexData.position_ = vert;
				vertexData.uv_ = texcoord;

				data.push_back(vertexData);
		    }
		}

	    // Triangles //
	    for(int y = 0; y < rows_-1; y++) 
	    {
	        for(int x = 0; x < columns_-1; x++) 
	        {
	            // first triangle //
	            indexes_.push_back((y)*columns_ + x);
	            indexes_.push_back((y)*columns_ + x+1);
	            indexes_.push_back((y+1)*columns_ + x);
	            
	            // second triangle //
	            indexes_.push_back((y)*columns_ + x+1);
	            indexes_.push_back((y+1)*columns_ + x+1);
	            indexes_.push_back((y+1)*columns_ + x);
	        }
	    }
	}

	const char* GLES2PlaneMesh::GetName() const
	{
		return "PlaneMesh";
	}


}
