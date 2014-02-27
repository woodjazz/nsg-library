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
	SphereMesh::SphereMesh(Color color, float radius, int res, PGLES2Material pMaterial, GLenum usage) 
	: Mesh(pMaterial, usage)
	{
		Mesh::Data& data = GetVertexData();

		static const float PI = glm::pi<float>();
		static const float TWO_PI = 2*PI;

		float doubleRes = res*2.f;
		float polarInc = PI/(res); // ringAngle
		float azimInc = TWO_PI/(doubleRes); // segAngle 

		Vertex3 vert;
    	Vertex2 tcoord;
    
		for(float i = 0; i < res+1; i++) 
		{
		    float tr = sin( PI-i * polarInc );
		    float ny = cos( PI-i * polarInc );
		    
		    tcoord.y = i / res;
		    
		    for(float j = 0; j <= doubleRes; j++) {
		        
		        float nx = tr * sin(j * azimInc);
		        float nz = tr * cos(j * azimInc);
		        
		        tcoord.x = j / (doubleRes);

				VertexData vertexData;
				vertexData.normal_ = Vertex3(nx,ny,nz);
				vertexData.position_ = vertexData.normal_ * radius;
				vertexData.color_ = color;
				vertexData.uv_ = tcoord;
				
				data.push_back(vertexData);
		    }
		}
    
    	Indexes indexes;

	    int nr = doubleRes+1;
        
        int index1, index2, index3;
        
        for(float iy = 0; iy < res; iy++) 
        {
            for(float ix = 0; ix < doubleRes; ix++) 
            {
                // first tri //
                if(iy > 0) 
                {
                    index1 = (iy+0) * (nr) + (ix+0);
                    index2 = (iy+0) * (nr) + (ix+1);
                    index3 = (iy+1) * (nr) + (ix+0);
                    
                    indexes.push_back(index1);
                    indexes.push_back(index3);
                    indexes.push_back(index2);
                }
                
                if(iy < res-1 ) 
                {
                    // second tri //
                    index1 = (iy+0) * (nr) + (ix+1);
                    index2 = (iy+1) * (nr) + (ix+1);
                    index3 = (iy+1) * (nr) + (ix+0);
                    
                    indexes.push_back(index1);
                    indexes.push_back(index3);
                    indexes.push_back(index2);
                }
            }
        }

		SetIndices(indexes);

	    Redo();
	}

	SphereMesh::~SphereMesh() 
	{
	}

}