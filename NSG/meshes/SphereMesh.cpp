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
#include "ResourceProcedural.h"

namespace NSG
{
    SphereMesh::SphereMesh(float radius, int res)
        : radius_(radius),
          res_(res)
    {
        resource_ = PResource(new ResourceProcedural(this));
    }

    SphereMesh::~SphereMesh()
    {
    }

    GLenum SphereMesh::GetWireFrameDrawMode() const
    {
        return GL_LINE_LOOP;
    }

    GLenum SphereMesh::GetSolidDrawMode() const
    {
        return GL_TRIANGLES;
    }

    void SphereMesh::Build()
    {
        vertexsData_.clear();
        indexes_.clear();

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
                vertexData.uv_ = tcoord;

                data.push_back(vertexData);
            }
        }

        int nr = doubleRes + 1;

        int index1, index2, index3;

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
                    indexes_.push_back(index3);
                    indexes_.push_back(index2);
                }

                if (iy < res_ - 1 )
                {
                    // second tri
                    index1 = (iy + 0) * (nr) + (ix + 1);
                    index2 = (iy + 1) * (nr) + (ix + 1);
                    index3 = (iy + 1) * (nr) + (ix + 0);

                    indexes_.push_back(index1);
                    indexes_.push_back(index3);
                    indexes_.push_back(index2);
                }
            }
        }
    }

    const char* SphereMesh::GetName() const
    {
        return "SphereMesh";
    }


}