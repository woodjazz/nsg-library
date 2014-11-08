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
#include "EllipseMesh.h"
#include "Types.h"
#include "Constants.h"
#include "Check.h"
#include "Resource.h"
#include "ResourceProcedural.h"

namespace NSG
{
    EllipseMesh::EllipseMesh(float width, float height, int res)
        : Mesh("EllipseMesh"),
          width_(width),
          height_(height),
          res_(res)
    {
        resource_ = PResource(new ResourceProcedural(this));
        SetSerializable(false);
    }

    EllipseMesh::~EllipseMesh()
    {
    }

    GLenum EllipseMesh::GetWireFrameDrawMode() const
    {
        return GL_LINE_LOOP;
    }

    GLenum EllipseMesh::GetSolidDrawMode() const
    {
        return GL_TRIANGLE_FAN;
    }

    size_t EllipseMesh::GetNumberOfTriangles() const
    {
        return vertexsData_.size() - 2;
    }

    void EllipseMesh::Build()
    {
        vertexsData_.clear();
        indexes_.clear();

        VertexsData& data = vertexsData_;

        float halfX = width_ * 0.5f;
        float halfY = height_ * 0.5f;

        float angle = 0.0f;

        const float angleAdder = TWO_PI / (float)res_;

        for (int i = 0; i < res_; i++)
        {
            VertexData vertexData;
            vertexData.normal_ = Vertex3(0, 0, 1); // always facing forward
            vertexData.position_.x = cos(angle);
            vertexData.position_.y = sin(angle);
            vertexData.position_.z = 0;
            vertexData.uv0_ = Vertex2(vertexData.position_.x, vertexData.position_.y);
            vertexData.uv0_.x = (vertexData.uv0_.x + 1) / 2.0f;
            vertexData.uv0_.y = 1 - (vertexData.uv0_.y + 1) / 2.0f;

            vertexData.position_.x *= halfX;
            vertexData.position_.y *= halfY;

            data.push_back(vertexData);

            angle += angleAdder;
        }

    }

    const char* EllipseMesh::GetName() const
    {
        return "EllipseMesh";
    }

}

