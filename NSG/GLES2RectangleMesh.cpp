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
#include "Check.h"

namespace NSG
{
	GLES2RectangleMesh::GLES2RectangleMesh(float width, float height, GLenum usage) 
	: GLES2Mesh(usage),
	width_(width),
	height_(height)
	{
	}

	GLES2RectangleMesh::~GLES2RectangleMesh() 
	{
	}

	GLenum GLES2RectangleMesh::GetWireFrameDrawMode() const
	{
		return GL_LINE_LOOP;
	}

	GLenum GLES2RectangleMesh::GetSolidDrawMode() const
	{
		return GL_TRIANGLE_FAN;
	}

	void GLES2RectangleMesh::Build()
	{
		vertexsData_.clear();
		indexes_.clear();
		
		VertexsData& data = vertexsData_;

		float halfX = width_ * 0.5f;
		float halfY = height_ * 0.5f;

		VertexData vertexData;
		vertexData.normal_ = Vertex3(0, 0, 1); // always facing forward
		
		vertexData.position_ = Vertex3(-halfX, -halfY, 0);
		vertexData.uv_ = Vertex2(0, 1);
		data.push_back(vertexData);

		vertexData.position_ = Vertex3(halfX, -halfY, 0);
		vertexData.uv_ = Vertex2(1, 1);
		data.push_back(vertexData);

		vertexData.position_ = Vertex3(halfX, halfY, 0);
		vertexData.uv_ = Vertex2(1, 0);
		data.push_back(vertexData);

		vertexData.position_ = Vertex3(-halfX, halfY, 0);
		vertexData.uv_ = Vertex2(0, 0);
		data.push_back(vertexData);		
	}

	const char* GLES2RectangleMesh::GetName() const
	{
		return "RectangleMesh";
	}


}

