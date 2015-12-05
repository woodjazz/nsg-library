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
#include "IcoSphereMesh.h"
#include "Types.h"
#include "Constants.h"
#include "Check.h"
#include "Log.h"

namespace NSG
{
    IcoSphereMesh::IcoSphereMesh(const std::string& name)
        : ProceduralMesh(name)
    {
        Set();
        SetSerializable(false);
    }

    IcoSphereMesh::~IcoSphereMesh()
    {
    }

    void IcoSphereMesh::Set(float radius, int iterations)
    {
        if (radius_ != radius || iterations_ != iterations)
        {
            radius_ = radius;
            iterations_ = iterations;
            Invalidate();
        }
    }

    GLenum IcoSphereMesh::GetWireFrameDrawMode() const
    {
        return GL_LINE_STRIP;
    }

    GLenum IcoSphereMesh::GetSolidDrawMode() const
    {
        return GL_TRIANGLES;
    }

    size_t IcoSphereMesh::GetNumberOfTriangles() const
    {
        return indexes_.size() / 3;
    }

    void IcoSphereMesh::AllocateResources()
    {
		// based on code by Michael Broutin for the ogre-procedural library
		// http://code.google.com/p/ogre-procedural/source/browse/library/src/ProceduralIcoSphereGenerator.cpp

        vertexsData_.clear();
        indexes_.clear();
        indexesWireframe_.clear();

        VertexsData& data = vertexsData_;

        const float INVNORM = 1.f / sqrt(PHI * PHI + 1.f);

        //Generate icosahedron
        VertexData vertexData;
        vertexData.normal_ = vertexData.position_ = INVNORM * Vector3(-1, PHI, 0);
		data.push_back(vertexData);
        vertexData.normal_ = vertexData.position_ = INVNORM * Vector3(1, PHI, 0);
		data.push_back(vertexData);
        vertexData.normal_ = vertexData.position_ = INVNORM * Vector3(0, 1, -PHI);
		data.push_back(vertexData);
        vertexData.normal_ = vertexData.position_ = INVNORM * Vector3(0, 1, PHI);
		data.push_back(vertexData);
        vertexData.normal_ = vertexData.position_ = INVNORM * Vector3(-PHI, 0, -1);
		data.push_back(vertexData);
        vertexData.normal_ = vertexData.position_ = INVNORM * Vector3(-PHI, 0, 1);
		data.push_back(vertexData);
        vertexData.normal_ = vertexData.position_ = INVNORM * Vector3( PHI, 0, -1);
		data.push_back(vertexData);
        vertexData.normal_ = vertexData.position_ = INVNORM * Vector3( PHI, 0, 1);
		data.push_back(vertexData);
        vertexData.normal_ = vertexData.position_ = INVNORM * Vector3(0, -1, -PHI);
		data.push_back(vertexData);
        vertexData.normal_ = vertexData.position_ = INVNORM * Vector3(0, -1, PHI);
		data.push_back(vertexData);
        vertexData.normal_ = vertexData.position_ = INVNORM * Vector3(-1, -PHI, 0);
		data.push_back(vertexData);
        vertexData.normal_ = vertexData.position_ = INVNORM * Vector3( 1, -PHI, 0);
		data.push_back(vertexData);

        indexes_ = Indexes
        {
            0, 1, 2,
            0, 3, 1,
            0, 4, 5,
            1, 7, 6,
            1, 6, 2,
            1, 3, 7,
            0, 2, 4,
            0, 5, 3,
            2, 6, 8,
            2, 8, 4,
            3, 5, 9,
            3, 9, 7,
            11, 6, 7,
            10, 5, 4,
            10, 4, 8,
            10, 9, 5,
            11, 8, 6,
            11, 7, 9,
            10, 8, 11,
            10, 11, 9
        };

        auto& faces = indexes_;
        auto size = faces.size();

        // Tessellate
        for (auto iteration = 0; iteration < iterations_; iteration++)
        {
            size *= 4;
            Indexes newFaces;
            for (auto i = 0; i < size / 12; i++)
            {
                auto i1 = faces[i * 3];
                auto i2 = faces[i * 3 + 1];
                auto i3 = faces[i * 3 + 2];
				CHECK_ASSERT(std::numeric_limits<IndexType>::max() > data.size());
                auto i12 = (IndexType)data.size();
                auto i23 = i12 + 1;
                auto i13 = i12 + 2;
                auto v1 = data[i1].position_;
                auto v2 = data[i2].position_;
                auto v3 = data[i3].position_;
				//make 1 vertice at the center of each edge and project it onto the sphere
				VertexData vertexData;
				vertexData.normal_ = vertexData.position_ = Normalize(v1 + v2);
				data.push_back(vertexData);
				vertexData.normal_ = vertexData.position_ = Normalize(v2 + v3);
				data.push_back(vertexData);
				vertexData.normal_ = vertexData.position_ = Normalize(v1 + v3);
				data.push_back(vertexData);
                //now recreate indices
                newFaces.push_back(i1);
                newFaces.push_back(i12);
                newFaces.push_back(i13);
                newFaces.push_back(i2);
                newFaces.push_back(i23);
                newFaces.push_back(i12);
                newFaces.push_back(i3);
                newFaces.push_back(i13);
                newFaces.push_back(i23);
                newFaces.push_back(i12);
                newFaces.push_back(i23);
                newFaces.push_back(i13);
            }
            faces.swap(newFaces);
        }

        // Step 3 : generate texcoords
        for (auto i = 0; i < data.size(); i++)
        {
            const auto& vec = data[i].position_;
            auto r0 = sqrtf(vec.x * vec.x + vec.z * vec.z);
            auto alpha = atan2f(vec.z, vec.x);
            auto u = alpha / TWO_PI + .5f;
            auto v = atan2f(vec.y, r0) / PI + .5f;
            // reverse the u coord, so the default is texture mapped left to
            // right on the outside of a sphere //
			data[i].uv_[0] = Vector2(1 - u, v);
        }

        // Step 4 : fix texcoords
        // find vertices to split
        std::vector<IndexType> indexToSplit;
        for (auto i = 0; i < faces.size() / 3; i++)
        {
            auto& t0 = data[faces[i * 3 + 0]].uv_[0];
			auto& t1 = data[faces[i * 3 + 1]].uv_[0];
			auto& t2 = data[faces[i * 3 + 2]].uv_[0];

            if (abs(t2.x - t0.x) > 0.5f)
            {
                if (t0.x < 0.5f)
                    indexToSplit.push_back(faces[i * 3]);
                else
                    indexToSplit.push_back(faces[i * 3 + 2]);
            }
            if (abs(t1.x - t0.x) > 0.5f)
            {
                if (t0.x < 0.5f)
                    indexToSplit.push_back(faces[i * 3]);
                else
                    indexToSplit.push_back(faces[i * 3 + 1]);
            }
            if (abs(t2.x - t1.x) > 0.5f)
            {
                if (t1.x < 0.5f)
                    indexToSplit.push_back(faces[i * 3 + 1]);
                else
                    indexToSplit.push_back(faces[i * 3 + 2]);
            }
        }

        //split vertices
        for (auto i = 0; i < indexToSplit.size(); i++)
        {
            auto index = indexToSplit[i];
            //duplicate vertex
			VertexData vertexData;
			vertexData.position_ = data[index].position_;
			vertexData.uv_[0] = data[index].uv_[0] + Vector2(1, 0);
            data.push_back(vertexData);
			CHECK_ASSERT(std::numeric_limits<IndexType>::max() > data.size());
			IndexType newIndex = (IndexType)data.size() - 1;
            //reassign indices
            for (auto j = 0; j < faces.size(); j++)
            {
                if (faces[j] == index)
                {
                    auto index1 = faces[(j + 1) % 3 + (j / 3) * 3];
                    auto index2 = faces[(j + 2) % 3 + (j / 3) * 3];
                    if ((data[index1].uv_[0].x > 0.5f) || (data[index2].uv_[0].x > 0.5f))
                        faces[j] = newIndex;
                }
            }
        }

        // flip triangle winding order
        for (auto i = 0; i < faces.size(); i += 3)
            std::swap(faces[i + 1], faces[i + 2]);

        for (auto i = 0; i < data.size(); i++ )
            data[i].position_ *= radius_;

        CHECK_ASSERT(std::numeric_limits<IndexType>::max() > data.size());

		indexesWireframe_ = indexes_;

		Mesh::AllocateResources();
    }
}