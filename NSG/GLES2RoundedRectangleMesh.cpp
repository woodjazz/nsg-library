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
#include "GLES2RoundedRectangleMesh.h"
#include "Types.h"
#include "Constants.h"

namespace NSG
{
	GLES2RoundedRectangleMesh::GLES2RoundedRectangleMesh(float radius, float width, float height, int res, PGLES2Material pMaterial, GLenum usage) 
	: GLES2Mesh(pMaterial, usage),
	filled_(true)
	{
		GLES2Mesh::Data& data = GetVertexData();

		float halfX = width*0.5f;
		float halfY = height*0.5f;	

		SetFilled(filled_);
		SetSelectMode(S_TRIANGLE_FAN);

		float angle = 0.0f;

		float radius1 = std::min(radius, std::min(halfX, halfY));

		float totalSizeX = width + 2 * radius1;
		float totalSizeY = height + 2 * radius1;

		float xOffset = (totalSizeX - width)/2;
		float yOffset = (totalSizeY - height)/2;

		const float radiusRatio = 0.4f;
		const float sideRation = 1-radiusRatio;
		float radiusReduction = radiusRatio * std::min(xOffset, yOffset);
		float xReduction = sideRation * xOffset;
		float yReduction = sideRation * yOffset;

		halfX -= xReduction;
		halfY -= yReduction;

		radius1 -= radiusReduction;

		const float angleAdder = TWO_PI / (float)res;

		for (int i = 0; i < res; i++)
		{
			VertexData vertexData;
			vertexData.normal_ = Vertex3(0,0,1); // always facing forward
			vertexData.position_.x = cos(angle);
			vertexData.position_.y = sin(angle);
			vertexData.position_.z = 0;
			vertexData.uv_ = Vertex2(vertexData.position_.x, vertexData.position_.y);

            vertexData.position_ *= radius1;
			
            if(angle < PI/2 || angle > 3*PI/2)
            {
			    vertexData.position_.x += halfX;
            }
            else if(angle > PI/2)
            {
                vertexData.position_.x -= halfX;
            }
            
            if(angle >= 0 && angle < PI)
            {
			   vertexData.position_.y += halfY;
            }
            else if(angle >= PI)
            {
               vertexData.position_.y -= halfY;
            }
            
			data.push_back(vertexData);

			angle += angleAdder;
		}

		Redo();
	}

	GLES2RoundedRectangleMesh::~GLES2RoundedRectangleMesh() 
	{
	}

	void GLES2RoundedRectangleMesh::SetFilled(bool enable) 
	{ 
		filled_ = enable;

		if(filled_)
			SetMode(TRIANGLE_FAN);
		else
			SetMode(LINE_LOOP);
	}
}


