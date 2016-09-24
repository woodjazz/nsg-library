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

namespace NSG
{
    struct Vector4;
    struct Vector3
    {
        union { float x, r, s; };
        union { float y, g, t; };
        union { float z, b, p; };

        Vector3();
        Vector3(float a);
        Vector3(float a, float b, float c);
        Vector3(const Vector3& v);
        Vector3(const Vector4& v);
        const float& operator[](int i) const;
        float& operator[](int i);
        const Vector3& operator/=(float v);
        const Vector3& operator*=(float v);
		bool IsNaN() const;
		bool IsUniform() const;
		float Dot(const Vector3& v) const;
		Vector3 Cross(const Vector3& v) const;
		Vector3 Reflect(const Vector3& N) const;
		Vector3 GetSlidingVector(const Vector3& hitNormal) const;
		Vector3 Lerp(const Vector3& v, float t) const;
		Vector3 Floor() const;
		float Length() const;
		float Length2() const;
		bool IsZeroLength() const;
		Vector3 Radians() const;
		Vector3 Degrees() const;
		float Angle(const Vector3& v) const;
		Vector3 Normalize() const;
		float Distance(const Vector3& v) const;
		float Distance2(const Vector3& v) const;
		static Vector3 CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float s);
    };
    typedef Vector3 Vertex3;
    typedef Vector3 ColorRGB;

    bool operator!=(const Vector3& v1, const Vector3& v2);
    bool operator==(const Vector3& v1, const Vector3& v2);
    Vector3 operator+(const Vector3& v1, const Vector3& v2);
    Vector3 operator-(const Vector3& v);
    Vector3 operator-(const Vector3& v1, const Vector3& v2);
    Vector3 operator+(const Vector3& v, float scalar);
    Vector3 operator-(const Vector3& v, float scalar);
    Vector3 operator*(const Vector3& v, float scalar);
    Vector3 operator*(float scalar, const Vector3& v);
    Vector3 operator*(const Vector3& v1, const Vector3& v2);
    Vector3 operator/(const Vector3& v, float scalar);
	float Distance(const Vector3& a, const Vector3& b);
}
