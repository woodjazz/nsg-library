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
#include "Vector3.h"
#include "Util.h"
#include "Check.h"
#include "Maths.h"
#include <cmath>
namespace NSG
{
    Vector3::Vector3()
        : x(0), y(0), z(0)
    {
    }

    Vector3::Vector3(float a)
        : x(a), y(a), z(a)
    {
    }

    Vector3::Vector3(float a, float b, float c)
        : x(a), y(b), z(c)
    {
    }

    Vector3::Vector3(const Vector3& v)
        : x(v.x), y(v.y), z(v.z)
    {
    }

    Vector3::Vector3(const Vector4& v)
        : x(v.x), y(v.y), z(v.z)
    {
    }

    const float& Vector3::operator[](int i) const
    {
        CHECK_ASSERT(i >= 0 && i < 3);
        return (&x)[i];
    }

    float& Vector3::operator[](int i)
    {
        CHECK_ASSERT(i >= 0 && i < 3);
        return (&x)[i];
    }

    const Vector3& Vector3::operator/=(float v)
    {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }

    const Vector3& Vector3::operator*=(float v)
    {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

	bool Vector3::IsNaN() const
	{
		return std::isnan(x) || std::isnan(y) || std::isnan(z);
	}

	bool Vector3::IsUniform() const
	{
		return Abs(x - y) < PRECISION && Abs(x - z) < PRECISION;
	}

	float Vector3::Dot(const Vector3& v) const
	{
		Vector3 tmp(*this * v);
		return tmp.x + tmp.y + tmp.z;
	}

	Vector3 Vector3::Cross(const Vector3& v) const
	{
		return Vector3(
			y * v.z - v.y * z,
			z * v.x - v.z * x,
			x * v.y - v.x * y);
	}

	Vector3 Vector3::Reflect(const Vector3& N) const
	{
		return *this - N * N.Dot(*this) * Vector3(2);
	}
	
	Vector3 Vector3::GetSlidingVector(const Vector3& hitNormal) const
	{
		Vector3 reflection = Reflect(hitNormal);
		Vector3 parallelComponent = reflection.Dot(hitNormal) * hitNormal;
		Vector3 sliding = reflection - parallelComponent;
		return sliding;
	}

	Vector3 Vector3::Lerp(const Vector3& v, float t) const
	{
		return *this * (1.0f - t) + v * t;
	}

	Vector3 Vector3::Floor() const
	{
		return Vector3(std::floor(x), std::floor(y), std::floor(z));
	}

	float Vector3::Length() const
	{
		return sqrt(Dot(*this));
	}
	
	float Vector3::Length2() const
	{
		return Dot(*this);
	}

	bool Vector3::IsZeroLength() const
	{
		return Length() <= EPSILON;
	}

	Vector3 Vector3::Radians() const
	{
		return *this * DEG2RAD;
	}

	Vector3 Vector3::Degrees() const
	{
		return *this * RAD2DEG;
	}

	float Vector3::Angle(const Vector3& v) const
	{
		return acos(Clamp(Dot(v), -1.f, 1.f));
	}

	Vector3 Vector3::Normalize() const
	{
		return *this * 1.f / Length();
	}

	float Vector3::Distance(const Vector3& v) const
	{
		return (v - *this).Length();
	}

	float Vector3::Distance2(const Vector3& v) const
	{
		return Dot(v);
	}

	Vector3 Vector3::CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float s)
	{
		auto s2 = Pow2(s);
		auto s3 = Pow3(s);
		auto f1 = -s3 + 2 * s2 - s;
		auto f2 = 3 * s3 - 5 * s2 + 2;
		auto f3 = -3 * s3 + 4 * s2 + s;
		auto f4 = s3 - s2;
		return (f1 * v1 + f2 * v2 + f3 * v3 + f4 * v4) / 2.f;
	}

    bool operator!=(const Vector3& v1, const Vector3& v2)
    {
        return (v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z);
    }

    bool operator==(const Vector3& v1, const Vector3& v2)
    {
        return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
    }

    Vector3 operator+(const Vector3& v1, const Vector3& v2)
    {
        return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);   
    }

    Vector3 operator-(const Vector3& v)
    {
        return Vector3(-v.x, -v.y, -v.z);   
    }

    Vector3 operator-(const Vector3& v1, const Vector3& v2)
    {
        return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);   
    }

    Vector3 operator+(const Vector3& v, float scalar)
    {
        return Vector3(v.x + scalar, v.y + scalar, v.z + scalar);
    }

    Vector3 operator-(const Vector3& v, float scalar)
    {
        return Vector3(v.x - scalar, v.y - scalar, v.z - scalar);
    }

    Vector3 operator*(const Vector3& v, float scalar)
    {
        return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
    }

    Vector3 operator*(float scalar, const Vector3& v)
    {
        return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
    }

    Vector3 operator*(const Vector3& v1, const Vector3& v2)
    {
        return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z); 
    }

    Vector3 operator/(const Vector3& v, float scalar)
    {
        return Vector3(v.x / scalar, v.y / scalar, v.z / scalar);
    }

	float Distance(const Vector3& a, const Vector3& b)
	{
		return a.Distance(b);
	}
}
