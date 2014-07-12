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
#include "Allocators.h"
#include "PODVector.h"
#include <chrono>

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

    typedef enum {BLEND_NONE, BLEND_ALPHA} BLEND_MODE ;

    typedef enum {LEFT_ALIGNMENT, CENTER_ALIGNMENT, RIGHT_ALIGNMENT} HorizontalAlignment;
    typedef enum {TOP_ALIGNMENT, MIDDLE_ALIGNMENT, BOTTOM_ALIGNMENT} VerticalAlignment;

    namespace IMGUI
    {
        typedef enum {
            VERTICAL, 
            HORIZONTAL, 
            CONTROL, 
            SPACER
        } LayoutType;

        typedef enum {
            IMGUI_UNKNOWN_ID, 
			IMGUI_MAIN_WINDOW_ID,
            IMGUI_WINDOW_AREA_ID0,
			IMGUI_WINDOW_AREA_ID1,
			IMGUI_WINDOW_AREA_ID2,
			IMGUI_WINDOW_AREA_ID3,
			IMGUI_WINDOW_AREA_ID4,
			IMGUI_WINDOW_AREA_ID5,
			IMGUI_WINDOW_AREA_ID6,
			IMGUI_WINDOW_AREA_ID7,
			IMGUI_WINDOW_AREA_ID8,
			IMGUI_WINDOW_AREA_ID9,
            IMGUI_WINDOW_AREA_IDA,
            IMGUI_WINDOW_AREA_IDB,
            IMGUI_WINDOW_AREA_IDC,
            IMGUI_WINDOW_AREA_IDD,
            IMGUI_VERTICAL_SLIDER_ID, 
            IMGUI_HORIZONTAL_SLIDER_ID, 
            IMGUI_FIRST_VALID_ID
        } IdsTypes;

        typedef enum {
            LEFT_TOP_SIZER,
            TOP_SIZER,
            RIGHT_TOP_SIZER,
            LEFT_SIZER,
            RIGHT_SIZER,
            LEFT_BOTTOM_SIZER,
            BOTTOM_SIZER,
            RIGHT_BOTTOM_SIZER
        } SizerType;
    }

    typedef std::pair<PNode, PMesh> MeshNode;

    typedef std::chrono::milliseconds Milliseconds;
    typedef std::chrono::seconds Seconds;
    typedef std::chrono::steady_clock Clock;
    typedef Clock::time_point TimePoint;

    typedef enum {OUTSIDE, INTERSECTS, INSIDE} Intersection;

    struct VertexData
    {
        Vertex3 position_;
        Vertex3 normal_;
        Vertex2 uv_;
        Color color_;

        VertexData();
    };

    typedef GLushort IndexType;

    typedef enum {
        ATT_POSITION, 
        ATT_NORMAL, 
        ATT_TEXTURECOORD, 
        ATT_COLOR
    } AttributesLoc;

	const IndexType MAX_INDEX_VALUE = ~IndexType(0);

	typedef std::vector<VertexData> VertexsData;
	typedef std::vector<IndexType> Indexes;

	static const Matrix4 IDENTITY_MATRIX(1.0f);
}