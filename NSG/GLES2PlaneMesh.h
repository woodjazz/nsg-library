/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 N�stor Silveira Gorski

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
#include <memory>
#include "GLES2Material.h"
#include "GLES2Mesh.h"

namespace NSG
{
	class GLES2PlaneMesh : public GLES2Mesh
	{
	public:
		GLES2PlaneMesh(float width, float height, int columns, int rows, GLenum usage);
		~GLES2PlaneMesh();
		GLenum GetWireFrameDrawMode() const;
		GLenum GetSolidDrawMode() const;
	};

	typedef std::shared_ptr<GLES2PlaneMesh> PGLES2PlaneMesh;
}
