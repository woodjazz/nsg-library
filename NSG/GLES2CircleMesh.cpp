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
#include "GLES2CircleMesh.h"
#include "Types.h"
#include "Constants.h"

namespace NSG
{
	GLES2CircleMesh::GLES2CircleMesh(float radius, int res, GLenum usage) 
	: GLES2Mesh(usage),
	filled_(true)
	{
		GLES2Mesh::Data& data = GetVertexData();

		SetFilled(filled_);
		SetSelectMode(GL_TRIANGLE_FAN);

		float angle = 0.0f;

		const float angleAdder = TWO_PI / (float)res;

		for (int i = 0; i < res; i++)
		{
			VertexData vertexData;
			vertexData.normal_ = Vertex3(0,0,1); // always facing forward
			vertexData.position_.x = cos(angle);
			vertexData.position_.y = sin(angle);
			vertexData.position_.z = 0;
            vertexData.uv_ = Vertex2(vertexData.position_.x, vertexData.position_.y);

            vertexData.uv_.x = (vertexData.uv_.x + 1)/2.0f;
            vertexData.uv_.y = 1-(vertexData.uv_.y + 1)/2.0f;
			
			vertexData.position_ *= radius;

			data.push_back(vertexData);

			angle += angleAdder;
		}

		Redo();
	}

	GLES2CircleMesh::~GLES2CircleMesh() 
	{
	}

	void GLES2CircleMesh::SetFilled(bool enable) 
	{ 
		filled_ = enable;

		if(filled_)
			SetMode(GL_TRIANGLE_FAN);
		else
			SetMode(GL_LINE_LOOP);
	}
}

