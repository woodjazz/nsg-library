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
#include "Types.h"
#include "Allocators.h"
#include "GPUObject.h"
#include "VertexData.h"

namespace NSG
{
	class BufferManager
	{
	public:
		BufferManager();
		~BufferManager();

		void Invalidate();

		PVertexBuffer GetStaticVertexBuffer(GLsizeiptr bufferSize, GLsizeiptr bytesNeeded, const VertexsData& vertexes);
		PIndexBuffer GetStaticIndexBuffer(GLsizeiptr bufferSize, GLsizeiptr bytesNeeded, const Indexes& indexes);
		PVertexBuffer GetDynamicVertexBuffer(GLsizeiptr bufferSize, GLsizeiptr bytesNeeded, const VertexsData& vertexes);
		PIndexBuffer GetDynamicIndexBuffer(GLsizeiptr bufferSize, GLsizeiptr bytesNeeded, const Indexes& indexes);

	private:
		PVertexBuffer currentStaticVertexBuffer_;
		PIndexBuffer currentStaticIndexBuffer_;
		PVertexBuffer currentDynamicVertexBuffer_;
		PIndexBuffer currentDynamicIndexBuffer_;
	};
}