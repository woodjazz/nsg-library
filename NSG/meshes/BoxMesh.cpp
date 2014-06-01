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
#include "BoxMesh.h"
#include "Check.h"
#include "ResourceProcedural.h"

namespace NSG
{
	BoxMesh::BoxMesh(float width, float height, float depth, int resX, int resY, int resZ, GLenum usage) 
	: Mesh(usage),
	width_(width),
	height_(height),
	depth_(depth),
	resX_(resX),
	resY_(resY),
	resZ_(resZ)
	{
		resource_ = PResource(new ResourceProcedural(this));
	}

	BoxMesh::~BoxMesh() 
	{
	}

	GLenum BoxMesh::GetWireFrameDrawMode() const
	{
		return GL_LINE_LOOP;
	}

	GLenum BoxMesh::GetSolidDrawMode() const
	{
		return GL_TRIANGLES;
	}

	void BoxMesh::Build()
	{
		// halves //
		float halfW = width_ * .5f;
		float halfH = height_ * .5f;
		float halfD = depth_ * .5f;

		Vertex3 vert;
		Vertex2 texcoord;
		Vertex3 normal;
		int vertOffset = 0;

		vertexsData_.clear();
		indexes_.clear();

		VertexsData& data = vertexsData_;

		// TRIANGLES //

		// Front Face //
		normal = Vertex3(0, 0, 1);
		// add the vertexes //
		for(int iy = 0; iy < resY_; iy++) 
		{
		    for(int ix = 0; ix < resX_; ix++) 
		    {
		        
		        // normalized tex coords //
		        texcoord.x = ((float)ix/((float)resX_-1.f));
		        texcoord.y = ((float)iy/((float)resY_-1.f));
		        
		        vert.x = texcoord.x * width_ - halfW;
		        vert.y = texcoord.y * height_ - halfH;
		        vert.z = halfD;

				VertexData vertexData;
				vertexData.normal_ = normal;
				vertexData.position_ = vert;
				vertexData.uv_ = texcoord;

				data.push_back(vertexData);
		    }
		}

		Indexes indexes;

		for(int y = 0; y < resY_-1; y++)
		{
		    for(int x = 0; x < resX_-1; x++) 
		    {
		        // first triangle //
		        indexes_.push_back((y)*resX_ + x + vertOffset);
		        indexes_.push_back((y)*resX_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resX_ + x + vertOffset);
		        
		        // second triangle //
		        indexes_.push_back((y)*resX_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resX_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resX_ + x + vertOffset);
		    }
		}

		vertOffset = data.size();


		// Right Side Face //
		normal = Vertex3(1, 0, 0);
		// add the vertexes //
		for(int iy = 0; iy < resY_; iy++) 
		{
		    for(int ix = 0; ix < resZ_; ix++) 
		    {
		        // normalized tex coords //
		        texcoord.x = ((float)ix/((float)resZ_-1.f));
		        texcoord.y = ((float)iy/((float)resY_-1.f));
		        
		        //vert.x = texcoord.x * width - halfW;
		        vert.x = halfW;
		        vert.y = texcoord.y * height_ - halfH;
		        vert.z = texcoord.x * -depth_ + halfD;
		        
				VertexData vertexData;
				vertexData.normal_ = normal;
				vertexData.position_ = vert;
				vertexData.uv_ = texcoord;

				data.push_back(vertexData);
		    }
		}

		for(int y = 0; y < resY_-1; y++) 
		{
		    for(int x = 0; x < resZ_-1; x++) 
		    {
		        // first triangle //
		        indexes_.push_back((y)*resZ_ + x + vertOffset);
		        indexes_.push_back((y)*resZ_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resZ_ + x + vertOffset);
		        
		        // second triangle //
		        indexes_.push_back((y)*resZ_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resZ_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resZ_ + x + vertOffset);
		    }
		}

		vertOffset = data.size();

		// Left Side Face //
		normal = Vertex3(-1, 0, 0);
		// add the vertexes //
		for(int iy = 0; iy < resY_; iy++) 
		{
		    for(int ix = 0; ix < resZ_; ix++) 
		    {
		        
		        // normalized tex coords //
		        texcoord.x = ((float)ix/((float)resZ_-1.f));
		        texcoord.y = ((float)iy/((float)resY_-1.f));
		        
		        //vert.x = texcoord.x * width - halfW;
		        vert.x = -halfW;
		        vert.y = texcoord.y * height_ - halfH;
		        vert.z = texcoord.x * depth_ - halfD;
		        
				VertexData vertexData;
				vertexData.normal_ = normal;
				vertexData.position_ = vert;
				vertexData.uv_ = texcoord;

				data.push_back(vertexData);
		    }
		}

		for(int y = 0; y < resY_-1; y++) 
		{
		    for(int x = 0; x < resZ_-1; x++) 
		    {
		        // first triangle //
		        indexes_.push_back((y)*resZ_ + x + vertOffset);
		        indexes_.push_back((y)*resZ_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resZ_ + x + vertOffset);
		        
		        // second triangle //
		        indexes_.push_back((y)*resZ_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resZ_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resZ_ + x + vertOffset);
		    }
		}

		vertOffset = data.size();


		// Back Face //
		normal = Vertex3(0, 0, -1);
		// add the vertexes //
		for(int iy = 0; iy < resY_; iy++) 
		{
		    for(int ix = 0; ix < resX_; ix++) 
		    {
		        
		        // normalized tex coords //
		        texcoord.x = ((float)ix/((float)resX_-1.f));
		        texcoord.y = ((float)iy/((float)resY_-1.f));
		        
		        vert.x = texcoord.x * -width_ + halfW;
		        vert.y = texcoord.y * height_ - halfH;
		        vert.z = -halfD;
		        
				VertexData vertexData;
				vertexData.normal_ = normal;
				vertexData.position_ = vert;
				vertexData.uv_ = texcoord;

				data.push_back(vertexData);
		    }
		}

		for(int y = 0; y < resY_-1; y++) 
		{
		    for(int x = 0; x < resX_-1; x++) 
		    {
		        // first triangle //
		        indexes_.push_back((y)*resX_ + x + vertOffset);
		        indexes_.push_back((y)*resX_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resX_ + x + vertOffset);
		        
		        // second triangle //
		        indexes_.push_back((y)*resX_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resX_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resX_ + x + vertOffset);
		    }
		}

		vertOffset = data.size();

		// Top Face //
		normal = Vertex3(0, -1, 0);
		// add the vertexes //
		for(int iy = 0; iy < resZ_; iy++) 
		{
		    for(int ix = 0; ix < resX_; ix++) 
		    {
		        
		        // normalized tex coords //
		        texcoord.x = ((float)ix/((float)resX_-1.f));
		        texcoord.y = ((float)iy/((float)resZ_-1.f));
		        
		        vert.x = texcoord.x * width_ - halfW;
		        //vert.y = texcoord.y * height - halfH;
		        vert.y = -halfH;
		        vert.z = texcoord.y * depth_ - halfD;
		        
				VertexData vertexData;
				vertexData.normal_ = normal;
				vertexData.position_ = vert;
				vertexData.uv_ = texcoord;

				data.push_back(vertexData);
		    }
		}

		for(int y = 0; y < resZ_-1; y++) 
		{
		    for(int x = 0; x < resX_-1; x++) 
		    {
		        // first triangle //
		        indexes_.push_back((y)*resX_ + x + vertOffset);
		        indexes_.push_back((y)*resX_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resX_ + x + vertOffset);
		        
		        // second triangle //
		        indexes_.push_back((y)*resX_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resX_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resX_ + x + vertOffset);
		    }
		}

		vertOffset = data.size();

		// Bottom Face //
		normal = Vertex3(0, 1, 0);
		// add the vertexes //
		for(int iy = 0; iy < resZ_; iy++) 
		{
		    for(int ix = 0; ix < resX_; ix++) 
		    {
		        // normalized tex coords //
		        texcoord.x = ((float)ix/((float)resX_-1.f));
		        texcoord.y = ((float)iy/((float)resZ_-1.f));
		        
		        vert.x = texcoord.x * width_ - halfW;
		        //vert.y = texcoord.y * height - halfH;
		        vert.y = halfH;
		        vert.z = texcoord.y * -depth_ + halfD;
		        
				VertexData vertexData;
				vertexData.normal_ = normal;
				vertexData.position_ = vert;
				vertexData.uv_ = texcoord;

				data.push_back(vertexData);
		    }
		}

		for(int y = 0; y < resZ_-1; y++) 
		{
		    for(int x = 0; x < resX_-1; x++) 
		    {
		        // first triangle //
		        indexes_.push_back((y)*resX_ + x + vertOffset);
		        indexes_.push_back((y)*resX_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resX_ + x + vertOffset);
		        
		        // second triangle //
		        indexes_.push_back((y)*resX_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resX_ + x+1 + vertOffset);
		        indexes_.push_back((y+1)*resX_ + x + vertOffset);
		    }
		}
	}

	const char* BoxMesh::GetName() const
	{
		return "BoxMesh";
	}
}
