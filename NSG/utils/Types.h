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

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/epsilon.hpp"
#include "glm/gtx/spline.hpp"
#include "glm/gtx/io.hpp"
#include "SharedPointers.h"
#include "GLES2Includes.h"

#define STRINGIFY(S) #S

namespace NSG
{
    typedef glm::highp_ivec4 Recti;
	typedef glm::vec4 Vertex4;
	typedef glm::vec3 Vertex3;
	typedef glm::vec2 Vertex2;
	typedef glm::quat Quaternion;
    typedef glm::mat4 Matrix4;
    typedef glm::mat3 Matrix3;
    typedef glm::vec4 Color;

    typedef enum {NONE, ALPHA} BLEND_MODE ;

    typedef enum {LEFT_ALIGNMENT, CENTER_ALIGNMENT, RIGHT_ALIGNMENT} HorizontalAlignment;
    typedef enum {TOP_ALIGNMENT, MIDDLE_ALIGNMENT, BOTTOM_ALIGNMENT} VerticalAlignment;

    typedef std::pair<PNode, PMesh> MeshNode;

}