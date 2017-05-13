/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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
#include "CylinderMesh.h"
#include "Check.h"
#include "Log.h"
#include "Maths.h"
#include "Types.h"
#include <cmath>

namespace NSG {
CylinderMesh::CylinderMesh(const std::string& name) : ProceduralMesh(name) {
    Set();
    SetSerializable(false);
}

CylinderMesh::~CylinderMesh() {}

void CylinderMesh::Set(float radius, float height, int rings, bool capped) {
    if (radius_ != radius || height_ != height || rings_ != rings ||
        capped_ != capped) {
        radius_ = radius;
        height_ = height;
        rings_ = rings;
        capped_ = capped;
        Invalidate();
    }
}

void CylinderMesh::SetOrientation(const Quaternion& q) {
    if (q_ != q) {
        q_ = q;
        Invalidate();
    }
}

GLenum CylinderMesh::GetWireFrameDrawMode() const { return GL_LINE_STRIP; }

GLenum CylinderMesh::GetSolidDrawMode() const { return GL_TRIANGLES; }

size_t CylinderMesh::GetNumberOfTriangles() const {
    return indexes_.size() / 3;
}

void CylinderMesh::AllocateResources() {
    vertexsData_.clear();
    indexes_.clear();
    indexesWireframe_.clear();

    VertexsData& data = vertexsData_;

    int segments = rings_ * 2;
    float segInc = TWO_PI / segments;

    if (capped_) {
        // insert bottom center point
        VertexData vertexData;
        vertexData.normal_ = q_ * Vector3::Up * (-1);
        vertexData.position_ = Vector3::Zero;
        vertexData.uv_[0] = Vertex2(0.5f, 0.5f);
        data.push_back(vertexData);

        for (int seg = 0; seg <= segments; seg++) {
            auto beta = seg * segInc;
            auto x = sin(beta);
            auto z = cos(beta);

            vertexData.position_ = q_ * Vertex3(radius_ * x, 0, radius_ * z);
            vertexData.uv_[0] = Vertex2(0.5f * (x + 1), 1 - 0.5f * (z + 1));
            data.push_back(vertexData);
        }
    }

    Vertex2 tcoord;
    for (float ring = 0; ring <= rings_; ring++) {
        auto y = ring / rings_;
        tcoord.y = y;

        for (float seg = 0; seg <= segments; seg++) {
            auto beta = seg * segInc;
            auto x = sin(beta);
            auto z = cos(beta);

            tcoord.x = seg / segments;

            VertexData vertexData;
            vertexData.normal_ = q_ * Vertex3(x, 0, z);
            vertexData.position_ =
                q_ * Vertex3(radius_ * x, y * height_, radius_ * z);
            vertexData.uv_[0] = tcoord;

            data.push_back(vertexData);
        }
    }

    int indexBase = 0;

    if (capped_) {
        // Vertices for top face
        // Insert top center point
        VertexData vertexData;
        vertexData.normal_ = q_ * Vector3::Up;
        vertexData.position_ = q_ * Vector3(0, height_, 0);
        vertexData.uv_[0] = Vertex2(0.5f, 0.5f);
        data.push_back(vertexData);

        for (float seg = 0; seg <= segments; seg++) {
            auto beta = seg * segInc;
            auto x = sin(beta);
            auto z = cos(beta);

            vertexData.position_ =
                q_ * Vertex3(radius_ * x, height_, radius_ * z);
            vertexData.uv_[0] = Vertex2(0.5f * (x + 1), 1 - 0.5f * (z + 1));
            data.push_back(vertexData);
        }

        // Indices for bottom face
        int seg = 0;
        for (; seg < segments; seg++) {
            indexesWireframe_.push_back(seg + 1);
            indexesWireframe_.push_back(seg);
            indexesWireframe_.push_back(0);
            indexes_.push_back(seg + 1);
            indexes_.push_back(seg);
            indexes_.push_back(0);
        }
        indexes_.push_back(1);
        indexes_.push_back(seg);
        indexes_.push_back(0);

        indexBase += 2 + segments;
    }

    int nr = segments + 1;
    int index0 = 0;
    int index1 = 0;
    int index2 = 0;
    int index3 = 0;

    // Triangles
    // Front Face CCW
    for (int ring = 0; ring < rings_; ring++) {
        for (int segment = 0; segment < segments; segment++) {
            //  i2_____i3
            //  | \     |
            //  |   \   |
            //  |     \ |
            //  i0_____i1

            index0 = indexBase + nr * (ring + 0) + (segment + 0);
            index1 = indexBase + nr * (ring + 0) + (segment + 1);
            index2 = indexBase + nr * (ring + 1) + (segment + 0);
            index3 = indexBase + nr * (ring + 1) + (segment + 1);

            indexesWireframe_.push_back(index2);
            indexesWireframe_.push_back(index0);
            indexesWireframe_.push_back(index1);
            if (!capped_ && ring == rings_ - 1) {
                // close last ring
                indexesWireframe_.push_back(index3);
                indexesWireframe_.push_back(index2);
            }

            // first triangle
            indexes_.push_back(index0);
            indexes_.push_back(index1);
            indexes_.push_back(index2);

            // second triangle
            indexes_.push_back(index1);
            indexes_.push_back(index3);
            indexes_.push_back(index2);
        }
    }

    if (capped_) {
        int index = index3 + 1;
        // Top face
        int seg = 0;
        for (; seg < segments; seg++) {
            indexesWireframe_.push_back(index);
            indexesWireframe_.push_back(index + seg);
            indexesWireframe_.push_back(index + seg + 1);
            indexes_.push_back(index);
            indexes_.push_back(index + seg);
            indexes_.push_back(index + seg + 1);
        }
        indexes_.push_back(index);
        indexes_.push_back(index + seg);
        indexes_.push_back(index + 1);
    }

    Mesh::AllocateResources();
}
}
