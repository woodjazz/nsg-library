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
#include "RectangleMesh.h"
#include "Types.h"
#include "Constants.h"
#include "Check.h"
#include "ResourceProcedural.h"
namespace NSG
{
	RectangleMesh::RectangleMesh(float width, float height)
	: width_(width),
	height_(height)
	{
		resource_ = PResource(new ResourceProcedural(this));
		SetSerializable(false);
	}

	RectangleMesh::~RectangleMesh() 
	{
	}

	GLenum RectangleMesh::GetWireFrameDrawMode() const
	{
		return GL_LINE_LOOP;
	}

	GLenum RectangleMesh::GetSolidDrawMode() const
	{
		return GL_TRIANGLE_FAN;
	}

    size_t RectangleMesh::GetNumberOfTriangles() const
    {
        return vertexsData_.size() - 2;
    }

	void RectangleMesh::Build()
	{
		vertexsData_.clear();
		indexes_.clear();
		
		VertexsData& data = vertexsData_;

		float halfX = width_ * 0.5f;
		float halfY = height_ * 0.5f;

		VertexData vertexData;
		vertexData.normal_ = Vertex3(0, 0, 1); // always facing forward
		
		vertexData.position_ = Vertex3(-halfX, -halfY, 0);
		vertexData.uv0_ = Vertex2(0, 1);
		data.push_back(vertexData);

		vertexData.position_ = Vertex3(halfX, -halfY, 0);
		vertexData.uv0_ = Vertex2(1, 1);
		data.push_back(vertexData);

		vertexData.position_ = Vertex3(halfX, halfY, 0);
		vertexData.uv0_ = Vertex2(1, 0);
		data.push_back(vertexData);

		vertexData.position_ = Vertex3(-halfX, halfY, 0);
		vertexData.uv0_ = Vertex2(0, 0);
		data.push_back(vertexData);		
	}

	const char* RectangleMesh::GetName() const
	{
		return "RectangleMesh";
	}


}

