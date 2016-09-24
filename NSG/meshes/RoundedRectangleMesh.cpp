/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include <algorithm>

namespace NSG
{
    RoundedRectangleMesh::RoundedRectangleMesh(const std::string& name)
        : ProceduralMesh(name)
    {
        Set();
        SetSerializable(false);
    }

    RoundedRectangleMesh::~RoundedRectangleMesh()
    {
    }

    void RoundedRectangleMesh::Set(float radius, float width, float height, int res)
    {
        if (radius_ != radius || width_ != width || height_ != height || res_ != res)
        {
            radius_ = radius;
            width_ = width;
            height_ = height;
            res_ = res;
            Invalidate();
        }
    }

    GLenum RoundedRectangleMesh::GetWireFrameDrawMode() const
    {
        return GL_LINE_LOOP;
    }

    GLenum RoundedRectangleMesh::GetSolidDrawMode() const
    {
        return GL_TRIANGLE_FAN;
    }

    size_t RoundedRectangleMesh::GetNumberOfTriangles() const
    {
        return vertexsData_.size() - 2;
    }

    void RoundedRectangleMesh::AllocateResources()
    {
        vertexsData_.clear();
        indexes_.clear();

        VertexsData& data = vertexsData_;

        float halfX = width_ * 0.5f;
        float halfY = height_ * 0.5f;

        float angle = 0.0f;

        float radius1 = std::min(radius_, std::min(halfX, halfY));

        float totalSizeX = width_ + 2 * radius1;
        float totalSizeY = height_ + 2 * radius1;

        halfX -= (totalSizeX - width_) / 2;
        halfY -= (totalSizeY - height_) / 2;
        float coordXFactor = (2 * halfX) / totalSizeX;
        float coordYFactor = (2 * halfY) / totalSizeY;

        const float angleAdder = TWO_PI / (float)res_;

        for (int i = 0; i < res_; i++)
        {
            VertexData vertexData;
            vertexData.normal_ = Vertex3(0, 0, 1); // always facing forward
            vertexData.position_.x = cos(angle);
            vertexData.position_.y = sin(angle);
            vertexData.position_.z = 0;
			vertexData.uv_[0] = Vertex2(vertexData.position_.x, vertexData.position_.y);

            vertexData.position_ = vertexData.position_ * radius1;

            if (angle < PI / 2 || angle > 3 * PI / 2)
            {
                vertexData.position_.x += halfX;
				vertexData.uv_[0].x = coordXFactor + 0.5f * (1 - coordXFactor) * vertexData.uv_[0].x;
            }
            else if (angle > PI / 2)
            {
                vertexData.position_.x -= halfX;
				vertexData.uv_[0].x = -coordXFactor + 0.5f * (1 - coordXFactor) * vertexData.uv_[0].x;
            }

            if (angle >= 0 && angle < PI)
            {
                vertexData.position_.y += halfY;
				vertexData.uv_[0].y = coordYFactor + 0.5f * (1 - coordYFactor) * vertexData.uv_[0].y;
            }
            else if (angle >= PI)
            {
                vertexData.position_.y -= halfY;
				vertexData.uv_[0].y = -coordYFactor + 0.5f * (1 - coordYFactor) * vertexData.uv_[0].y;
            }

			vertexData.uv_[0].x = (vertexData.uv_[0].x + 1) / 2.0f;
			vertexData.uv_[0].y = 1 - (vertexData.uv_[0].y + 1) / 2.0f;

            data.push_back(vertexData);

            angle += angleAdder;
        }

        Mesh::AllocateResources();
    }
}


