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
#include "SphereMesh.h"
#include "Check.h"
#include "Log.h"
#include "Maths.h"
#include "Types.h"

namespace NSG {
SphereMesh::SphereMesh(const std::string& name)
    : ProceduralMesh(name), radius_(0), rings_(0) {
    Set();
    SetSerializable(false);
}

SphereMesh::~SphereMesh() {}

void SphereMesh::Set(float radius, int rings) {
    if (radius_ != radius || rings_ != rings) {
        radius_ = radius;
        rings_ = rings;
        Invalidate();
    }
}

GLenum SphereMesh::GetWireFrameDrawMode() const { return GL_LINE_STRIP; }

GLenum SphereMesh::GetSolidDrawMode() const { return GL_TRIANGLES; }

size_t SphereMesh::GetNumberOfTriangles() const { return indexes_.size() / 3; }

void SphereMesh::AllocateResources() {
    vertexsData_.clear();
    indexes_.clear();
    indexesWireframe_.clear();

    VertexsData& data = vertexsData_;

    int segments = rings_ * 2;
    float ringInc = PI / rings_;
    float segInc = TWO_PI / segments;

    Vertex2 tcoord;

    for (float ring = 0; ring < rings_ + 1; ring++) {
        auto alpha = PI - ring * ringInc;
        auto tr = sin(alpha);
        auto ny = cos(alpha);

        tcoord.y = ring / rings_;

        for (float seg = 0; seg <= segments; seg++) {
            auto beta = seg * segInc;
            auto nx = tr * sin(beta);
            auto nz = tr * cos(beta);

            tcoord.x = seg / segments;

            VertexData vertexData;
            vertexData.normal_ = Vertex3(nx, ny, nz);
            vertexData.position_ = vertexData.normal_ * radius_;
            vertexData.uv_[0] = tcoord;

            data.push_back(vertexData);
        }
    }

    int nr = segments + 1;

    int index0, index1, index2, index3;

    // Triangles
    // Front Face CCW
    for (int ring = 0; ring < rings_; ring++) {
        for (int segment = 0; segment < segments; segment++) {
            //	i2_____i3
            //	| \		|
            //	|	\	|
            //	|	  \	|
            //	i0_____i1

            index0 = nr * (ring + 0) + (segment + 0);
            index1 = nr * (ring + 0) + (segment + 1);
            index2 = nr * (ring + 1) + (segment + 0);
            index3 = nr * (ring + 1) + (segment + 1);

            indexesWireframe_.push_back(index2);
            indexesWireframe_.push_back(index0);
            indexesWireframe_.push_back(index1);

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

    Mesh::AllocateResources();
}
}
