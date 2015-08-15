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
#include "ModelMesh.h"
#include "Types.h"
#include "Check.h"
#include "Graphics.h"
#include "pugixml.hpp"

namespace NSG
{
    ModelMesh::ModelMesh(const std::string& name)
        : Mesh(name),
          face_mode_(GL_TRIANGLES)
    {
    }

    ModelMesh::~ModelMesh()
    {
    }

    GLenum ModelMesh::GetWireFrameDrawMode() const
    {
        return GL_LINES;
    }

    GLenum ModelMesh::GetSolidDrawMode() const
    {
        return face_mode_;
    }

    size_t ModelMesh::GetNumberOfTriangles() const
    {
        if (face_mode_ == GL_TRIANGLES)
            return indexes_.size() / 3;
        else if (face_mode_ == GL_TRIANGLE_FAN)
            return vertexsData_.size() - 2;
        else
            return 0;
    }

	void ModelMesh::Load(const pugi::xml_node& node)
	{
		//SetFaceMode(node.attribute("wireFrameDrawMode").as_int()); // redundant
		auto drawModeAtt = node.attribute("solidDrawMode");
		if (drawModeAtt)
			SetFaceMode(node.attribute("solidDrawMode").as_int());
		Mesh::Load(node);
	}
}
