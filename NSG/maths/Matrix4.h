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
#include "Vector4.h"
namespace NSG
{
	struct Vector3;
	struct Quaternion;
    struct Matrix3;
    struct Matrix4
    {
        Vector4 value[4];
        Matrix4();
        Matrix4(float s);
        Matrix4(const Matrix4& m);
        Matrix4(const Matrix3& m);
        Matrix4
        (
            float x1, float y1, float z1, float w1,
            float x2, float y2, float z2, float w2,
            float x3, float y3, float z3, float w3,
            float x4, float y4, float z4, float w4
        );
        Matrix4(const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3);
		Matrix4(const Quaternion& q);
		Matrix4(const Vector3& position, const Quaternion& q, const Vector3& scale);
		Matrix4(const Vector3& position, const Quaternion& q);
		Matrix4(float left, float right, float bottom, float top, float zNear, float zFar);//ortho
		Matrix4(float fovy, float aspect, float zNear, float zFar);//perspective
        const Vector4& operator[](int i) const;
        Vector4& operator[](int i);
        const Matrix4& operator*=(float s);
        Vector3 Translation() const;
		Matrix4& Translate(const Vector3& v);
        Vector3 Scale() const;
        Matrix4& Scale(const Vector3& v);
        Matrix4 Inverse() const;
		Vector4 Row(int index) const;
		const Vector4& Column(int index) const;
		inline const float* GetPointer() const { return &(value[0].x); }
		Matrix4 GetSphericalBillboardMatrix() const;
		Matrix4 GetCylindricalBillboardMatrix() const;
		void Decompose(Vector3& position, Quaternion& q, Vector3& scale) const;
    };
    Matrix4 operator*(const Matrix4& m, float s);
    Vector4 operator*(const Matrix4& m, const Vector4& v);
    Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
}
