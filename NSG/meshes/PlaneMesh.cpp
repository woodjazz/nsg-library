/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "PlaneMesh.h"
#include "Types.h"
#include "Check.h"

namespace NSG
{
    PlaneMesh::PlaneMesh(const std::string& name)
        : ProceduralMesh(name)
    {
        Set();
        SetSerializable(false);
    }

    PlaneMesh::~PlaneMesh()
    {
    }

    void PlaneMesh::Set(float width, float height, int columns, int rows)
    {
        if (width_ != width || height_ != height || columns_ != columns || rows_ != rows)
        {
            width_ = width;
            height_ = height;
            columns_ = columns;
            rows_ = rows;
            Invalidate();
        }
    }

    GLenum PlaneMesh::GetWireFrameDrawMode() const
    {
		return GL_LINES;
    }

    GLenum PlaneMesh::GetSolidDrawMode() const
    {
        return GL_TRIANGLES;
    }

    size_t PlaneMesh::GetNumberOfTriangles() const
    {
        return indexes_.size() / 3;
    }

    void PlaneMesh::AllocateResources()
    {
        vertexsData_.clear();
        indexes_.clear();
		indexesWireframe_.clear();

        VertexsData& data = vertexsData_;

        Vertex3 vert;
        Vertex3 normal(0, 0, 1); // always facing forward
        Vertex2 texcoord;

        // the origin of the plane is the center
        float halfW = width_ / 2.f;
        float halfH = height_ / 2.f;
        // add the vertexes
        for (int iy = 0; iy < rows_; iy++)
        {
            for (int ix = 0; ix < columns_; ix++)
            {
                // normalized tex coords //
                texcoord.x = ((float)ix / ((float)columns_ - 1.f));
                texcoord.y = ((float)iy / ((float)rows_ - 1.f));

                vert.x = texcoord.x * width_ - halfW;
                vert.y = texcoord.y * height_ - halfH;

                VertexData vertexData;
                vertexData.normal_ = normal;
                vertexData.position_ = vert;
				vertexData.uv_[0] = texcoord;

                data.push_back(vertexData);
            }
        }

        // Triangles
        // Front Face CCW
        for (int y = 0; y < rows_ - 1; y++)
        {
            for (int x = 0; x < columns_ - 1; x++)
            {
                IndexType i0 = y * columns_ + x;
                IndexType i1 = y * columns_ + x + 1;
                IndexType i2 = (y + 1) * columns_ + x;
                IndexType i3 = (y + 1) * columns_ + x + 1;

                // first triangle
                indexes_.push_back(i0);
                indexes_.push_back(i1);
                indexes_.push_back(i2);

                // second triangle
                indexes_.push_back(i1);
                indexes_.push_back(i3);
                indexes_.push_back(i2);

				indexesWireframe_.push_back(i0);
				indexesWireframe_.push_back(i1);
				indexesWireframe_.push_back(i1);
				indexesWireframe_.push_back(i3);
				indexesWireframe_.push_back(i3);
				indexesWireframe_.push_back(i2);
				indexesWireframe_.push_back(i2);
				indexesWireframe_.push_back(i0);

            }
        }

        Mesh::AllocateResources();
    }
}
