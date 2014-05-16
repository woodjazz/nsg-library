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
#include "GLES2ModelMesh.h"
#include "Check.h"
#include "assimp/mesh.h"

namespace NSG
{
	GLES2ModelMesh::GLES2ModelMesh(GLenum usage, const aiMesh* mesh) 
	: GLES2Mesh(usage),
    face_mode_(-1)
	{
		VertexsData& data = vertexsData_;

		for(size_t v=0; v<mesh->mNumVertices; v++)
		{
			VertexData vertexData;

			vertexData.position_ = Vertex3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);

			if(mesh->HasNormals())
			{
				vertexData.normal_ = Vertex3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
			}

			if(mesh->HasTextureCoords(0))
			{
				vertexData.uv_ = Vertex2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
			}

            data.push_back(vertexData);
		}
		
		if(mesh->HasFaces())
		{
			for (size_t j=0; j<mesh->mNumFaces; ++j) 
			{
				const struct aiFace* face = &mesh->mFaces[j];

				GLenum face_mode;

				switch(face->mNumIndices) 
				{
					case 1: face_mode = GL_POINTS; break;
					case 2: face_mode = GL_LINES; break;
					case 3: face_mode = GL_TRIANGLES; break;
					default: face_mode = GL_TRIANGLE_FAN; break; //GL_POLYGON; break;
				}			

                if(face_mode_ == -1)
                {
                    face_mode_ = face_mode;
                }
                
				CHECK_ASSERT(face_mode_ == face_mode, __FILE__, __LINE__);

				for(size_t k=0; k<face->mNumIndices; k++) 
				{
					int index = face->mIndices[k];

					indexes_.push_back(index);
				}	
			}		
		}
	}

	GLES2ModelMesh::~GLES2ModelMesh()
	{

	}

	GLenum GLES2ModelMesh::GetWireFrameDrawMode() const
	{
		return face_mode_;
	}

	GLenum GLES2ModelMesh::GetSolidDrawMode() const
	{
		return face_mode_;
	}

}
