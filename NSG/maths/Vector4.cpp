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
#include "Vector4.h"
#include "Util.h"
#include "Check.h"
#include <cmath>
namespace NSG
{
    Vector4::Vector4()
        : x(0), y(0), z(0), w(0)
    {
    }

    Vector4::Vector4(float a)
        : x(a), y(a), z(a), w(a)
    {
    }

    Vector4::Vector4(float a, float b, float c, float d)
        : x(a), y(b), z(c), w(d)
    {
    }

    Vector4::Vector4(const Vector4& v)
        : x(v.x), y(v.y), z(v.z), w(v.w)
    {
    }

    Vector4::Vector4(const Vector3& v, float d)
        : x(v.x), y(v.y), z(v.z), w(d)
    {
    }

    const float& Vector4::operator[](int i) const
    {
        CHECK_ASSERT(i >= 0 && i < 4);
        return (&x)[i];
    }

    float& Vector4::operator[](int i)
    {
        CHECK_ASSERT(i >= 0 && i < 4);
        return (&x)[i];
    }

    const Vector4& Vector4::operator/=(float v)
    {
        x /= v;
        y /= v;
        z /= v;
        w /= v;
        return *this;
    }

    const Vector4& Vector4::operator*=(float v)
    {
        x *= v;
        y *= v;
        z *= v;
        w *= v;
        return *this;
    }

	float Vector4::Dot(const Vector4& v) const
	{
		Vector4 tmp(*this * v);
		return tmp.x + tmp.y + tmp.z + tmp.w;
	}

	Vector4 Vector4::Floor() const
	{
		return Vector4(std::floor(x), std::floor(y), std::floor(z), std::floor(w));
	}

	float Vector4::Length() const
	{
		return sqrt(Dot(*this));
	}

	float Vector4::Distance(const Vector4& v) const
	{
		return (v - *this).Length();
	}

	Vector4 Vector4::Fract() const
	{
		return *this - Floor();
	}

    bool operator!=(const Vector4& v1, const Vector4& v2)
    {
        return (v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z) || (v1.w != v2.w);
    }

    bool operator==(const Vector4& v1, const Vector4& v2)
    {
        return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z) && (v1.w == v2.w);
    }

    Vector4 operator+(const Vector4& v1, const Vector4& v2)
    {
        return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
    }

    Vector4 operator-(const Vector4& v)
    {
        return Vector4(-v.x, -v.y, -v.z, -v.w);   
    }

    Vector4 operator-(const Vector4& v1, const Vector4& v2)
    {
        return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
    }

    Vector4 operator+(const Vector4& v, float scalar)
    {
        return Vector4(v.x + scalar, v.y + scalar, v.z + scalar, v.w + scalar);
    }

    Vector4 operator-(const Vector4& v, float scalar)
    {
        return Vector4(v.x - scalar, v.y - scalar, v.z - scalar, v.w - scalar);
    }

    Vector4 operator*(const Vector4& v, float scalar)
    {
        return Vector4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
    }

    Vector4 operator*(const Vector4& v1, const Vector4& v2)
    {
        return Vector4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
    }


}
