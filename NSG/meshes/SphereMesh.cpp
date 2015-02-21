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
#include "Constants.h"
#include "Check.h"
#include "Log.h"

namespace NSG
{
    SphereMesh::SphereMesh(const std::string& name)
        : Mesh(name)
    {
        Set();
        SetSerializable(false);
    }

    SphereMesh::~SphereMesh()
    {
    }

    void SphereMesh::Set(float radius, int res)
    {
        if (radius_ != radius || res_ != res)
        {
            radius_ = radius;
            res_ = res;
            Invalidate();
        }
    }

    GLenum SphereMesh::GetWireFrameDrawMode() const
    {
        return GL_LINES;
    }

    GLenum SphereMesh::GetSolidDrawMode() const
    {
        return GL_TRIANGLES;
    }

    size_t SphereMesh::GetNumberOfTriangles() const
    {
        return vertexsData_.size() / 3;
    }

    bool SphereMesh::IsValid()
    {
        return true;
    }

    void SphereMesh::AllocateResources()
    {
        vertexsData_.clear();
        indexes_.clear();
		indexesWireframe_.clear();

        VertexsData& data = vertexsData_;

        int doubleRes = res_ * 2;
        float polarInc = PI / (res_); // ringAngle
        float azimInc = TWO_PI / (doubleRes); // segAngle

        Vertex3 vert;
        Vertex2 tcoord;

        for (float i = 0; i < res_ + 1; i++)
        {
            float tr = sin( PI - i * polarInc );
            float ny = cos( PI - i * polarInc );

            tcoord.y = i / res_;

            for (float j = 0; j <= doubleRes; j++)
            {

                float nx = tr * sin(j * azimInc);
                float nz = tr * cos(j * azimInc);

                tcoord.x = j / (doubleRes);

                VertexData vertexData;
                vertexData.normal_ = Vertex3(nx, ny, nz);
                vertexData.position_ = vertexData.normal_ * radius_;
				vertexData.uv_[0] = tcoord;

                data.push_back(vertexData);
            }
        }

        int nr = doubleRes + 1;

        int index1, index2, index3;

        // Triangles
        // Front Face CCW
        for (int iy = 0; iy < res_; iy++)
        {
            for (int ix = 0; ix < doubleRes; ix++)
            {
                // first tri
                if (iy > 0)
                {
                    index1 = (iy + 0) * (nr) + (ix + 0);
                    index2 = (iy + 0) * (nr) + (ix + 1);
                    index3 = (iy + 1) * (nr) + (ix + 0);

                    indexes_.push_back(index1);
                    indexes_.push_back(index2);
                    indexes_.push_back(index3);

					indexesWireframe_.push_back(index1);
					indexesWireframe_.push_back(index2);
					indexesWireframe_.push_back(index1);
					indexesWireframe_.push_back(index3);
                }

                if (iy < res_ - 1 )
                {
                    // second tri
                    index1 = (iy + 0) * (nr) + (ix + 1);
                    index2 = (iy + 1) * (nr) + (ix + 1);
                    index3 = (iy + 1) * (nr) + (ix + 0);

                    indexes_.push_back(index1);
                    indexes_.push_back(index2);
                    indexes_.push_back(index3);

					/*
					indexesWireframe_.push_back(index3);
					indexesWireframe_.push_back(index2);
					indexesWireframe_.push_back(index2);
					indexesWireframe_.push_back(index1);
					*/
                }
            }
        }

        Mesh::AllocateResources();
    }
}