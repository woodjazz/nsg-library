/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
    static const float PI = 3.1415926535897932384626433832795028f;
    static const float TWO_PI = 2 * PI;
    static const float PI90 = PI / 2.f;
    static const float PI45 = PI90 / 2.f;
    static const float PI5 = PI45 / 9.f;
    static const float PI10 = PI5 * 2.f;
    static const float PI15 = PI10 + PI5;
    static const float PI20 = PI10 * 2.f;
    static const float PI30 = PI10 * 3.f;
    static const float EPSILON = 0.000001f;
    static const IndexType MAX_INDEX_VALUE = ~IndexType(0);

    static const Matrix4 IDENTITY_MATRIX(1.0f);

    static const Vector3 WORLD_X_COORD(1, 0, 0);
    static const Vector3 WORLD_Y_COORD(0, 1, 0);
    static const Vector3 WORLD_Z_COORD(0, 0, 1);

    static const Vector3 VECTOR3_RIGHT(1, 0, 0);
    static const Vector3 VECTOR3_UP(0, 1, 0);
    static const Vector3 VECTOR3_FORWARD(0, 0, 1);
    static const Vector3 VECTOR3_LOOKAT_DIRECTION(0, 0, -1);

	static const Vector4 VECTOR4_ZERO(0);
    static const Vector3 VECTOR3_ZERO(0);
    static const Vector3 VECTOR3_ONE(1);
    static const Quaternion QUATERNION_IDENTITY;

    static const ColorRGB COLOR_WHITE(1, 1, 1);
    static const ColorRGB COLOR_BLACK(0, 0, 0);
    static const ColorRGB COLOR_RED(1, 0, 0);
    static const ColorRGB COLOR_GREEN(0, 1, 0);
    static const ColorRGB COLOR_BLUE(0, 0, 1);
    static const ColorRGB COLOR_YELLOW(1, 1, 0);
	static const ColorRGB COLOR_DODGER_BLUE(30/255.f, 144/255.f, 1);
	static const ColorRGB COLOR_DARK_ORANGE(1.f, 140 / 255.f, 0);
	static const ColorRGB COLOR_PEACH_PUFF(1.f, 218 / 255.f, 185 / 255.f);
	static const ColorRGB COLOR_SKY_BLUE(135 / 255.f, 206 / 255.f, 235 / 255.f);

    static const size_t MAX_NODES_IN_BATCH = 10000;
    static const float PRECISION = 0.01f;

    static const size_t MAX_BONES_PER_VERTEX = 4;
    static const int MAX_UVS = 2;

    static const float CAMERA_MIN_NEARCLIP = 0.01f;
    static const float CAMERA_MAX_FOV_DEGREES = 160.0f;
	static const float CAMERA_MAX_FOV_RADIANS = CAMERA_MAX_FOV_DEGREES * TWO_PI / 360.f;

	static const int MAX_SPLITS = 4; // shadow splits

    static const float MAX_WORLD_SIZE = 5000;

	static const size_t MAX_BONES0 = 64;
	static const size_t MAX_BONES1 = 48;
	static const size_t MAX_BONES2 = 32;
	static const size_t MAX_BONES = MAX_BONES0;

    static const float PHI = (1.f + sqrt(5.f)) * 0.5f; //golden ratio

}