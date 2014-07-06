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
#include "RoundedRectangleMesh.h"
#include "Types.h"
#include "Constants.h"
#include "Check.h"
#include "ResourceProcedural.h"
#include <algorithm>

namespace NSG
{
	RoundedRectangleMesh::RoundedRectangleMesh(float radius, float width, float height, int res, GLenum usage) 
	: Mesh(usage),
	radius_(radius),
	width_(width),
	height_(height),
	res_(res)
	{
		resource_ = PResource(new ResourceProcedural(this));
	}

	RoundedRectangleMesh::~RoundedRectangleMesh() 
	{
	}

	GLenum RoundedRectangleMesh::GetWireFrameDrawMode() const
	{
		return GL_LINE_LOOP;
	}

	GLenum RoundedRectangleMesh::GetSolidDrawMode() const
	{
		return GL_TRIANGLE_FAN;
	}

	void RoundedRectangleMesh::Build()
	{
		vertexsData_.clear();
		indexes_.clear();
		
        VertexsData& data = vertexsData_;

		float halfX = width_*0.5f;
		float halfY = height_*0.5f;	

		float angle = 0.0f;

		float radius1 = std::min(radius_, std::min(halfX, halfY));

		float totalSizeX = width_ + 2 * radius1;
		float totalSizeY = height_ + 2 * radius1;

		halfX -= (totalSizeX - width_)/2;
		halfY -= (totalSizeY - height_)/2;
		float coordXFactor= (2*halfX)/totalSizeX;
		float coordYFactor= (2*halfY)/totalSizeY;

		const float angleAdder = TWO_PI / (float)res_;

		for (int i = 0; i < res_; i++)
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
                vertexData.uv_.x = coordXFactor + 0.5f * (1 - coordXFactor) * vertexData.uv_.x;
            }
            else if(angle > PI/2)
            {
                vertexData.position_.x -= halfX;
                vertexData.uv_.x = -coordXFactor + 0.5f * (1 - coordXFactor) * vertexData.uv_.x;
            }
            
            if(angle >= 0 && angle < PI)
            {
				vertexData.position_.y += halfY;
                vertexData.uv_.y = coordYFactor + 0.5f * (1 - coordYFactor) * vertexData.uv_.y;
            }
            else if(angle >= PI)
            {
               vertexData.position_.y -= halfY;
               vertexData.uv_.y = -coordYFactor + 0.5f * (1 - coordYFactor) * vertexData.uv_.y;
            }

            vertexData.uv_.x = (vertexData.uv_.x + 1)/2.0f;
            vertexData.uv_.y = 1-(vertexData.uv_.y + 1)/2.0f;

			data.push_back(vertexData);

			angle += angleAdder;
		}
	}

	const char* RoundedRectangleMesh::GetName() const
	{
		return "RoundedRectangleMesh";
	}


}

