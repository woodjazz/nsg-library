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
#include "GLES2VertexBuffer.h"
#include "GLES2IndexBuffer.h"
#include "Types.h"
#include "Node.h"

namespace NSG
{
	struct VertexData
	{
		Vertex3 position_;
		Vertex3 normal_;
		Vertex2 uv_;
		Color color_;

		VertexData();
	};

	typedef GLushort IndexType;
	typedef std::vector<IndexType> Indexes;

	class GLES2Mesh
	{
	public:
		GLES2Mesh(GLenum usage);
		~GLES2Mesh();
		void Render(GLenum mode, GLuint position_loc, GLuint texcoord_loc, GLuint normal_loc, GLuint color_loc);
		void Redo();
		virtual GLenum GetWireFrameDrawMode() const = 0;
		virtual GLenum GetSolidDrawMode() const = 0;
	protected:
		typedef std::vector<VertexData> Data;
		Data vertexsData_;
		Indexes indexes_;
    private:
		PGLES2IndexBuffer pIBuffer_;
		PGLES2VertexBuffer pVBuffer_;
		GLenum usage_;
	};
}
