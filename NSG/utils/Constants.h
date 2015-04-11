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
#pragma once
#include "Types.h"

namespace NSG
{
    static const float PI = glm::pi<float>();
    static const float TWO_PI = 2 * PI;

    static const IndexType MAX_INDEX_VALUE = ~IndexType(0);

    static const Matrix4 IDENTITY_MATRIX(1.0f);

    static const Vector3 WORLD_X_COORD(1, 0, 0);
    static const Vector3 WORLD_Y_COORD(0, 1, 0);
    static const Vector3 WORLD_Z_COORD(0, 0, 1);

    static const Vector3 VECTOR3_RIGHT(1, 0, 0);
    static const Vector3 VECTOR3_UP(0, 1, 0);
    static const Vector3 VECTOR3_FORWARD(0, 0, -1);

    static const Vector3 VECTOR3_ZERO(0);
    static const Vector3 VECTOR3_ONE(1);
    static const Quaternion QUATERNION_IDENTITY;

    static const Color COLOR_BLACK(0, 0, 0, 1);
    static const Color COLOR_RED(1, 0, 0, 1);

    static const size_t MAX_NODES_IN_BATCH = 1000;
    static const float PRECISION = 0.01f;

	static const size_t MAX_BONES_PER_VERTEX = 4;
}