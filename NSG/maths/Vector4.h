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
    struct Vector3;
    struct Vector4
	{
		union { float x, r, s; };
		union { float y, g, t; };
		union { float z, b, p; };
		union { float w, a, q; };
        Vector4();
        Vector4(float a);
        Vector4(float a, float b, float c, float d);
        Vector4(const Vector4& v);
        Vector4(const Vector3& v, float d);
        const float& operator[](int i) const;
        float& operator[](int i);
        const Vector4& operator/=(float v);
        const Vector4& operator*=(float v);
		float Dot(const Vector4& v) const;
		Vector4 Floor() const;
		float Length() const;
		float Distance(const Vector4& v) const;
		Vector4 Fract() const;
        static const Vector4 Zero;
	};
	typedef Vector4 Vertex4;
	typedef Vector4 Rect;

	bool operator!=(const Vector4& v1, const Vector4& v2);
	bool operator==(const Vector4& v1, const Vector4& v2);
	Vector4 operator+(const Vector4& v1, const Vector4& v2);
    Vector4 operator-(const Vector4& v);
	Vector4 operator-(const Vector4& v1, const Vector4& v2);
	Vector4 operator+(const Vector4& v, float scalar);
	Vector4 operator-(const Vector4& v, float scalar);
	Vector4 operator*(const Vector4& v, float scalar);
    Vector4 operator*(const Vector4& v1, const Vector4& v2);
}
