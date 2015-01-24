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
#include "MeshConverter.h"
#include "ModelMesh.h"
#include "Check.h"
#include "App.h"
#include "Util.h"
#include "assimp/mesh.h"

namespace NSG
{
    MeshConverter::MeshConverter(const aiMesh* aiMesh)
        : face_mode_(-1),
          aiMesh_(aiMesh)
    {
    	std::string name = aiMesh->mName.C_Str();

		if (name.empty())
			name = GetUniqueName();

        VertexsData data;

        CHECK_ASSERT(data.empty(), __FILE__, __LINE__);

        for (size_t v = 0; v < aiMesh_->mNumVertices; v++)
        {
            VertexData vertexData;

            vertexData.position_ = Vertex3(aiMesh_->mVertices[v].x, aiMesh_->mVertices[v].y, aiMesh_->mVertices[v].z);

            if (aiMesh_->HasNormals())
            {
                vertexData.normal_ = Vertex3(aiMesh_->mNormals[v].x, aiMesh_->mNormals[v].y, aiMesh_->mNormals[v].z);
            }

            if (aiMesh_->HasTextureCoords(0))
            {
				vertexData.uv_[0] = Vertex2(aiMesh_->mTextureCoords[0][v].x, aiMesh_->mTextureCoords[0][v].y);
            }

            if (aiMesh_->HasTextureCoords(1))
            {
				vertexData.uv_[1] = Vertex2(aiMesh_->mTextureCoords[1][v].x, aiMesh_->mTextureCoords[1][v].y);
            }

            if (aiMesh_->HasVertexColors(0))
            {
                vertexData.color_ = Vertex4(aiMesh_->mColors[0][v].r, aiMesh_->mColors[0][v].g, aiMesh_->mColors[0][v].b, aiMesh_->mColors[0][v].a);
            }

			// do not read tangents since they are calculated
			//if (aiMesh_->HasTangentsAndBitangents())
			//	vertexData.tangent_ = Vertex3(aiMesh_->mTangents[v].x, aiMesh_->mTangents[v].y, aiMesh_->mTangents[v].z);

            data.push_back(vertexData);
        }

        Indexes indexes;

        if (aiMesh_->HasFaces())
        {
            for (size_t j = 0; j < aiMesh_->mNumFaces; ++j)
            {
                const struct aiFace* face = &aiMesh_->mFaces[j];

                GLenum face_mode;

                switch (face->mNumIndices)
                {
                    case 1: face_mode = GL_POINTS; break;
                    case 2: face_mode = GL_LINES; break;
                    case 3: face_mode = GL_TRIANGLES; break;
                    default: face_mode = GL_TRIANGLE_FAN; break; //GL_POLYGON; break;
                }

                if (face_mode_ == -1)
                {
                    face_mode_ = face_mode;
                }

                CHECK_ASSERT(face_mode_ == face_mode, __FILE__, __LINE__);

                for (size_t k = 0; k < face->mNumIndices; k++)
                {
                    int index = face->mIndices[k];

                    indexes.push_back(index);
                }

                CHECK_ASSERT(indexes.size() % 3 == 0, __FILE__, __LINE__);
            }
        }

		mesh_ = App::this_->GetOrCreateModelMesh(name);
		mesh_->SetData(data, indexes);
    }

    MeshConverter::~MeshConverter()
    {
    }
}

