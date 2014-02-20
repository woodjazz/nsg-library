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
#pragma once
#include <vector>
#include <memory>
#include "NSG/GLES2Program.h"
#include "NSG/GLES2VertexBuffer.h"
#include "NSG/GLES2IndexBuffer.h"
#include "NSG/GLES2Texture.h"
#include "NSG/Types.h"
#include "Camera.h"
#include "Node.h"

namespace NSG
{
	struct VertexData
	{
		Vertex3 position_;
		Vertex3 color_;
		//Vertex3 normal_;
		Vertex2 uv_;

		VertexData(Vertex3 position, Vertex3 color, Vertex2 uv);
	};

	typedef GLubyte IndexType;
	typedef std::vector<IndexType> Indexes;

	class Mesh
	{
	public:
		Mesh(PGLES2Program pProgram, PGLES2Texture pTexture, GLenum usage);
		~Mesh();
		void Render(PNode pNode);
		void RenderForSelect(PNode pNode, GLushort id);
		void AddVertexData(const VertexData& data);
		void SetIndices(const Indexes& indexes);
		void Redo();
		typedef std::vector<VertexData> Data;
		Data& GetVertexData() { return vertexsData_; }

	private:
		void SetModelViewProjection(GLuint mvp_loc, PNode pNode);

		PGLES2Program pProgram_;
		PGLES2IndexBuffer pIBuffer_;
		PGLES2VertexBuffer pVBuffer_;
		PGLES2Texture pTexture_;

		GLuint texture_loc_;
		GLuint position_loc_;
		GLuint texcoord_loc_;
		GLuint color_loc_;
        GLuint mvp_loc_;

		PGLES2Program pSelectProgram_;
		GLuint select_mvp_loc_;
        GLuint select_position_loc_;
        GLuint select_color_loc_;

		Data vertexsData_;
		Indexes indexes_;

		GLenum usage_;
	};

	typedef std::shared_ptr<Mesh> PMesh;
}
