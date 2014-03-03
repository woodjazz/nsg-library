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
#include "GLES2Material.h"
#include "GLES2VertexBuffer.h"
#include "GLES2IndexBuffer.h"
#include "Types.h"
#include "GLES2Camera.h"
#include "Node.h"

namespace NSG
{
	struct VertexData
	{
		Vertex3 position_;
		Vertex3 normal_;
		Vertex2 uv_;

		VertexData();
		VertexData(Vertex3 position, Vertex2 uv);
	};

	typedef GLushort IndexType;
	typedef std::vector<IndexType> Indexes;

	class GLES2Mesh
	{
	public:
		GLES2Mesh(PGLES2Material pMaterial, GLenum usage);
		~GLES2Mesh();
		void Render(PNode pNode);
		void Render(Node* pNode);
		void RenderForSelect(PNode pNode, GLuint position_loc, GLuint mvp_loc);
		void AddVertexData(const VertexData& data);
		void SetIndices(const Indexes& indexes);
		void Redo();
		typedef std::vector<VertexData> Data;
		Data& GetVertexData() { return vertexsData_; }
		enum Mode {POINTS, LINES, LINE_STRIP, TRIANGLES};
		void SetMode(Mode mode);
	private:
		PGLES2Material pMaterial_;
		PGLES2IndexBuffer pIBuffer_;
		PGLES2VertexBuffer pVBuffer_;
		GLuint texcoord_loc_;
		GLuint position_loc_;
		GLuint normal_loc_;
		GLuint color_loc_;
		Data vertexsData_;
		Indexes indexes_;
		GLenum usage_;
		GLenum mode_;
        bool loaded_;
	};

	typedef std::shared_ptr<GLES2Mesh> PGLES2Mesh;
}
