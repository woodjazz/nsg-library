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
#include "Quaternion.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "Util.h"
#include "Check.h"
#include "Maths.h"
#include <cmath>
namespace NSG
{
    const Quaternion Quaternion::Identity = Quaternion();
    Quaternion::Quaternion()
        : x(0), y(0), z(0), w(1)
    {
    }

    Quaternion::Quaternion(float d, float a, float b, float c)
        : x(a), y(b), z(c), w(d)
    {
    }

    Quaternion::Quaternion(const Vector3& eulerAngle)
    {
        Vector3 half = eulerAngle * 0.5f;
        Vector3 c(cosf(half.x), cosf(half.y), cosf(half.z));
        Vector3 s(sinf(half.x), sinf(half.y), sinf(half.z));
        w = c.x * c.y * c.z + s.x * s.y * s.z;
        x = s.x * c.y * c.z - c.x * s.y * s.z;
        y = c.x * s.y * c.z + s.x * c.y * s.z;
        z = c.x * c.y * s.z - s.x * s.y * c.z;
    }

	Quaternion::Quaternion(float angle, const Vector3& axis)
	{
		auto s = sinf(angle * 0.5f);
		w = cosf(angle * 0.5f);
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
	}


    Quaternion::Quaternion(const Quaternion& q)
        : x(q.x), y(q.y), z(q.z), w(q.w)
    {
    }

    Quaternion::Quaternion(const Matrix4& m)
    {
        *this = Matrix3(m).QuatCast();
    }

    Quaternion::Quaternion(const Matrix3& m)
    {
        *this = m.QuatCast();
    }

	Quaternion::Quaternion(const Vector3& direction, const Vector3& upDirection)
	{
		Vector3 forward = direction.Normalize();
		Vector3 v = (forward.Cross(upDirection)).Normalize();
		if (v.IsNaN())
		{
			auto up1 = upDirection;
			std::swap(up1.x, up1.y);
			v = forward.Cross(up1).Normalize();
			if (v.IsNaN())
			{
				up1 = upDirection;
				std::swap(up1.x, up1.z);
				v = forward.Cross(up1).Normalize();
				if (v.IsNaN())
				{
					up1 = upDirection;
					std::swap(up1.y, up1.z);
					v = forward.Cross(up1).Normalize();
					CHECK_ASSERT(!v.IsNaN());
				}
			}
		}
		Vector3 up = v.Cross(forward);
		Vector3 right = up.Cross(forward);

		*this = Quaternion(Matrix3(right, up, forward));
		CHECK_ASSERT(!IsNaN());
	}

    const Quaternion& Quaternion::operator/=(float s)
    {
        w /= s;
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    const Quaternion& Quaternion::operator*=(const Quaternion& q)
    {
        *this = *this * q;
        return *this;
    }

	float Quaternion::Dot(const Quaternion& q) const
	{
		Vector4 tmp(Vector4(x, y, z, w) * Vector4(q.x, q.y, q.z, q.w));
		return tmp.x + tmp.y + tmp.z + tmp.w;
	}

	Quaternion Quaternion::Inverse() const
	{
		Quaternion conjugate(w, -x, -y, -z);
		return conjugate / Dot(*this);
	}

	Quaternion Quaternion::Slerp(const Quaternion& b, float t) const
	{
		auto zb = b;
		auto cosTheta = Dot(b);
		if (cosTheta < 0)
		{
			// If cosTheta < 0, the interpolation will take the long way around the sphere.
			// To fix this, one quat must be negated.
			zb = -b;
			cosTheta = -cosTheta;
		}

		if (cosTheta > 1 - std::numeric_limits<float>::epsilon())
		{
			// Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
			return Quaternion(
				Mix(w, zb.w, t),
				Mix(x, zb.x, t),
				Mix(y, zb.y, t),
				Mix(z, zb.z, t));
		}
		else
		{
			// Essential Mathematics, page 467
			auto angle = acosf(cosTheta);
			auto q1 = *this * sinf((1.f - t) * angle);
			auto q2 = sinf(t * angle) * zb;
			auto  q = q1 + q2;
			return q / sinf(angle);
		}
	}

	float Quaternion::Roll() const
	{
		return std::atan2(2 * (x * y + w * z), w * w + x * x - y * y - z * z);
	}

	float Quaternion::Pitch() const
	{
		return std::atan2(2 * (y * z + w * x), w * w - x * x - y * y + z * z);
	}

	float Quaternion::Yaw() const
	{
		return asin(-2 * (x * z - w * y));
	}

	Vertex3 Quaternion::EulerAngles() const
	{
		return Vector3(Pitch(), Yaw(), Roll());
	}

	Quaternion Quaternion::Normalize() const
	{
		auto len = sqrt(Dot(*this));
		if (len <= 0)
			return Quaternion(1, 0, 0, 0);
		auto oneOverLen = 1 / len;
		return Quaternion(w * oneOverLen, x * oneOverLen, y * oneOverLen, z * oneOverLen);
	}

    bool operator!=(const Quaternion& q1, const Quaternion& q2)
    {
        return (q1.x != q2.x) || (q1.y != q2.y) || (q1.z != q2.z) || (q1.w != q2.w);
    }

    Vector3 operator*(const Quaternion& q,  const Vector3& v)
    {
        Vector3 quatVector(q.x, q.y, q.z);
        Vector3 uv(quatVector.Cross(v));
        Vector3 uuv(quatVector.Cross(uv));
        return v + ((uv * q.w) + uuv) * 2.f;
    }

    Quaternion operator*(const Quaternion& p, const Quaternion& q)
    {
        return Quaternion(p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z,
                          p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y,
                          p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z,
                          p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x);
    }

    Quaternion operator+(const Quaternion& p, const Quaternion& q)
    {
        return Quaternion(p.w + q.w, p.x + q.x, p.y + q.y, p.z + q.z);
    }

    Quaternion operator/(const Quaternion& q, float s)
    {
        return Quaternion(q.w / s, q.x / s, q.y / s, q.z / s);
    }

    Quaternion operator*(const Quaternion&  q, float s)
    {
        return Quaternion(q.w * s, q.x * s, q.y * s, q.z * s);
    }    

    Quaternion operator*(float s, const Quaternion& q)
    {
        return q * s;
    }    

    Quaternion operator-(const Quaternion& q)
    {
        return Quaternion(-q.w, -q.x, -q.y, -q.z);
    }

	bool Quaternion::IsNaN() const
	{
		return std::isnan(w) || std::isnan(x) || std::isnan(y) || std::isnan(z);
	}

	Quaternion Rotation(const Vector3& orig, const Vector3& dest)
	{
		auto cosTheta = orig.Dot(dest);
		Vector3 rotationAxis;

		if (cosTheta >= 1 - std::numeric_limits<float>::epsilon())
			return Quaternion();

		if (cosTheta < -1 + std::numeric_limits<float>::epsilon())
		{
			// special case when vectors in opposite directions :
			// there is no "ideal" rotation axis
			// So guess one; any will do as long as it's perpendicular to start
			// This implementation favors a rotation around the Up axis (Y),
			// since it's often what you want to do.
			rotationAxis = Vector3(0, 0, 1).Cross(orig);
			if (rotationAxis.Length2() < std::numeric_limits<float>::epsilon()) // bad luck, they were parallel, try again!
				rotationAxis = Vector3(1, 0, 0).Cross(orig);

			rotationAxis = rotationAxis.Normalize();
			return Quaternion(PI, rotationAxis);
		}

		// Implementation from Stan Melax's Game Programming Gems 1 article
		rotationAxis = orig.Cross(dest);

		auto s = sqrt((1 + cosTheta) * 2);
		auto invs = 1 / s;

		return Quaternion(
			s * 0.5f,
			rotationAxis.x * invs,
			rotationAxis.y * invs,
			rotationAxis.z * invs);
	}

}
