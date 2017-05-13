/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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
#include "UtilConverter.h"
#include "Matrix3.h"
#include "Plane.h"
#include "Util.h"
#include <cmath>

namespace BlenderConverter {
using namespace NSG;
Matrix4 ToMatrix(const float m[][4]) {
    return Matrix4(m[0][0], m[0][1], m[0][2], m[0][3], m[1][0], m[1][1],
                   m[1][2], m[1][3], m[2][0], m[2][1], m[2][2], m[2][3],
                   m[3][0], m[3][1], m[3][2], m[3][3]);
}

Matrix3 ToMatrix(const float m[][3]) {
    return Matrix3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2],
                   m[2][0], m[2][1], m[2][2]);
}

Matrix4 GeneratePointTransformMatrix(const Plane& plane,
                                     const Vector3& center) {
    Vector3 normal = plane.GetNormal();
    Vector3 sideA = Vector3::Right;
    if (std::abs(normal.Dot(sideA)) > 0.999f)
        sideA = Vector3::Up;
    Vector3 sideB(normal.Cross(sideA).Normalize());
    sideA = sideB.Cross(normal);
    Matrix3 rot(sideA, sideB, normal);
    Matrix4 result = Matrix4().Translate(center) * Matrix4(rot);
    return result.Inverse();
}
}
