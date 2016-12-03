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
#include "Vector3.h"
namespace NSG
{
    struct Matrix4;
    struct Matrix3;
    struct Quaternion
	{
		float x, y, z, w;
        Quaternion();
        Quaternion(float w, float x, float y, float z);
        Quaternion(const Vector3& eulerAngle);
		Quaternion(float angle, const Vector3& axis);
        Quaternion(const Matrix4& m);
        Quaternion(const Matrix3& m);
        Quaternion(const Quaternion& q);
		Quaternion(const Vector3& direction, const Vector3& upDirection);
        const Quaternion& operator/=(float s);
        const Quaternion& operator*=(const Quaternion& q);
		bool IsNaN() const;
		float Dot(const Quaternion& q) const;
		Quaternion Inverse() const;
		Quaternion Slerp(const Quaternion& b, float t) const;
		float Roll() const;
		float Pitch() const;
		float Yaw() const;
		Vertex3 EulerAngles() const;
		Quaternion Normalize() const;
        static const Quaternion Identity;
	};
	bool operator!=(const Quaternion& q1, const Quaternion& q2);
	Vector3 operator*(const Quaternion& q,	const Vector3& v);
	Quaternion operator*(const Quaternion& p, const Quaternion& q);
    Quaternion operator+(const Quaternion& p, const Quaternion& q);
    Quaternion operator/(const Quaternion& q, float s);
    Quaternion operator*(const Quaternion&  q, float s);
    Quaternion operator*(float s, const Quaternion& q);
    Quaternion operator-(const Quaternion& q);
	Quaternion Rotation(const Vector3& orig, const Vector3& dest);
}
