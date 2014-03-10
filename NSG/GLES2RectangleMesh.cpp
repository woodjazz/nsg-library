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
#include "GLES2RectangleMesh.h"
#include "Types.h"
#include "Constants.h"

namespace NSG
{
	GLES2RectangleMesh::GLES2RectangleMesh(float width, float height, PGLES2Material pMaterial, GLenum usage) 
	: GLES2Mesh(pMaterial, usage),
	filled_(true)
	{
		float halfX = width * 0.5f;
		float halfY = height * 0.5f;
		GLES2Mesh::Data& data = GetVertexData();

		SetFilled(filled_);
		SetSelectMode(GL_TRIANGLE_FAN);

		VertexData vertexData;
		vertexData.normal_ = Vertex3(-halfX, -halfY, 1); // always facing forward
		
		vertexData.position_ = Vertex3(-halfX, -halfY, 0);
		vertexData.uv_ = Vertex2(vertexData.position_.x, vertexData.position_.y);
		data.push_back(vertexData);

		vertexData.position_ = Vertex3(halfX, -halfY, 0);
		vertexData.uv_ = Vertex2(vertexData.position_.x, vertexData.position_.y);
		data.push_back(vertexData);

		vertexData.position_ = Vertex3(halfX, halfY, 0);
		vertexData.uv_ = Vertex2(vertexData.position_.x, vertexData.position_.y);
		data.push_back(vertexData);

		vertexData.position_ = Vertex3(-halfX, halfY, 0);
		vertexData.uv_ = Vertex2(vertexData.position_.x, vertexData.position_.y);
		data.push_back(vertexData);		

		Redo();
	}

	GLES2RectangleMesh::~GLES2RectangleMesh() 
	{
	}

	void GLES2RectangleMesh::SetFilled(bool enable) 
	{ 
		filled_ = enable;

		if(filled_)
			SetMode(GL_TRIANGLE_FAN);
		else
			SetMode(GL_LINE_LOOP);
	}
}

