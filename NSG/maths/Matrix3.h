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
#pragma once
#include "Vector3.h"
namespace NSG {
struct Matrix4;
struct Quaternion;
struct Matrix3 {
    Vector3 value[4];
    Matrix3();
    Matrix3(float s);
    Matrix3(const Matrix3& m);
    Matrix3(const Matrix4& m);
    Matrix3(const Vector3& v0, const Vector3& v1, const Vector3& v2);
    Matrix3(float x1, float y1, float z1, float x2, float y2, float z2,
            float x3, float y3, float z3);
    Matrix3(const Quaternion& q);
    const Vector3& operator[](int i) const;
    Vector3& operator[](int i);
    Quaternion QuatCast() const;
    Matrix3 Transpose() const;
    Matrix3 Inverse() const;
    Vector3 Row(int index) const;
    const Vector3& Column(int index) const;
    inline const float* GetPointer() const { return &(value[0].x); }
};
Matrix3 operator*(const Matrix3& m1, const Matrix3& m2);
}
