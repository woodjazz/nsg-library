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
#include "LinesMesh.h"
#include "Types.h"
#include "Constants.h"
#include "Check.h"

namespace NSG
{
    LinesMesh::LinesMesh(const std::string& name)
        : ProceduralMesh(name, true)
    {
        SetSerializable(false);
    }

    LinesMesh::~LinesMesh()
    {
    }

    void LinesMesh::Add(const Vector3& start, const Vector3& end, const Color& color)
    {
        Data data{ start, end, color };
        lines_.push_back(data);
        Invalidate();
    }

    void LinesMesh::Clear()
    {
		lines_.clear();
    }

    GLenum LinesMesh::GetWireFrameDrawMode() const
    {
        return GL_LINES;
    }

    GLenum LinesMesh::GetSolidDrawMode() const
    {
        return GL_LINES;
    }

    size_t LinesMesh::GetNumberOfTriangles() const
    {
        return 0;
    }

    void LinesMesh::AllocateResources()
    {
        vertexsData_.clear();
        indexes_.clear();
        indexesWireframe_.clear();

        IndexType idx = 0;
        VertexsData& data = vertexsData_;
        for (auto& line : lines_)
        {
            VertexData vertexData;
            vertexData.position_ = line.start;
            vertexData.color_ = line.color;
            data.push_back(vertexData);
            vertexData.position_ = line.end;
            data.push_back(vertexData);
        }

        Mesh::AllocateResources();
    }

}

