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
#include "Matrix4.h"
#include "Matrix3.h"
#include "Quaternion.h"
#include "Vector3.h"
#include "Util.h"
#include "Check.h"
#include <cmath>
namespace NSG
{
    Matrix4::Matrix4()
    {
        value[0] = Vector4(1, 0, 0, 0);
        value[1] = Vector4(0, 1, 0, 0);
        value[2] = Vector4(0, 0, 1, 0);
        value[3] = Vector4(0, 0, 0, 1);        
    }

    Matrix4::Matrix4(float s)
    {
        value[0] = Vector4(s, 0, 0, 0);
        value[1] = Vector4(0, s, 0, 0);
        value[2] = Vector4(0, 0, s, 0);
        value[3] = Vector4(0, 0, 0, s);
    }

    Matrix4::Matrix4(const Matrix4& m)
    {
        value[0] = m.value[0];
        value[1] = m.value[1];
        value[2] = m.value[2];
        value[3] = m.value[3];
    }

    Matrix4::Matrix4(const Matrix3& m)
    {
        value[0] = Vector4(m.value[0], 0);
        value[1] = Vector4(m.value[1], 0);
        value[2] = Vector4(m.value[2], 0);
        value[3] = Vector4(0, 0, 0, 1);
    }

    Matrix4::Matrix4
    (
        float x1, float y1, float z1, float w1,
        float x2, float y2, float z2, float w2,
        float x3, float y3, float z3, float w3,
        float x4, float y4, float z4, float w4
    )
    {
        value[0] = Vector4(x1, y1, z1, w1);
        value[1] = Vector4(x2, y2, z2, w2);
        value[2] = Vector4(x3, y3, z3, w3);
        value[3] = Vector4(x4, y4, z4, w4);
    }


    Matrix4::Matrix4(const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3)
    {
        value[0] = v0;
        value[1] = v1;
        value[2] = v2;
        value[3] = v3;
    }

	Matrix4::Matrix4(const Quaternion& q)
	{
		*this = Matrix4(Matrix3(q));
	}

	Matrix4::Matrix4(const Vector3& position, const Quaternion& q, const Vector3& scale)
	{
		*this = Matrix4().Translate(position) * Matrix4(q) * Matrix4().Scale(scale);
	}

	Matrix4::Matrix4(const Vector3& position, const Quaternion& q)
	{
		*this = Matrix4().Translate(position) * Matrix4(q);
	}

	Matrix4::Matrix4(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		Matrix4 m(1);
		m[0][0] = 2 / (right - left);
		m[1][1] = 2 / (top - bottom);
		m[2][2] = -2 / (zFar - zNear);
		m[3][0] = -(right + left) / (right - left);
		m[3][1] = -(top + bottom) / (top - bottom);
		m[3][2] = -(zFar + zNear) / (zFar - zNear);
		*this = m;
	}

	Matrix4::Matrix4(float fovy, float aspect, float zNear, float zFar)
	{
		CHECK_ASSERT(fabs(aspect - std::numeric_limits<float>::epsilon()) > 0);
		auto tanHalfFovy = tan(fovy / 2.f);
		Matrix4 m(0);
		m[0][0] = 1 / (aspect * tanHalfFovy);
		m[1][1] = 1 / (tanHalfFovy);
		m[2][2] = -(zFar + zNear) / (zFar - zNear);
		m[2][3] = -1;
		m[3][2] = -(2 * zFar * zNear) / (zFar - zNear);
		*this = m;
	}

    const Vector4& Matrix4::operator[](int i) const
    {
        CHECK_ASSERT(i >= 0 && i < 4);
        return value[i];
    }

    Vector4& Matrix4::operator[](int i)
    {
        CHECK_ASSERT(i >= 0 && i < 4);
        return value[i];
    }

    const Matrix4& Matrix4::operator*=(float s)
    {
        value[0] *= s;
        value[1] *= s;
        value[2] *= s;
        value[3] *= s;
        return *this;
    }

    /// Return the translation part.
    Vector3 Matrix4::Translation() const
    {
        return value[3];
    }

	Matrix4& Matrix4::Translate(const Vector3& v)
	{
		value[3] = value[0] * v[0] + value[1] * v[1] + value[2] * v[2] + value[3];
		return *this;
	}
	
    /// Return the scaling part.
    Vector3 Matrix4::Scale() const
    {
        return Vector3(
            sqrtf(value[0][0] * value[0][0] + value[0][1] * value[0][1] + value[0][2] * value[0][2]),
            sqrtf(value[1][0] * value[1][0] + value[1][1] * value[1][1] + value[1][2] * value[1][2]),
            sqrtf(value[2][0] * value[2][0] + value[2][1] * value[2][1] + value[2][2] * value[2][2])
        );
    }

    Matrix4& Matrix4::Scale(const Vector3& v)
    {
        value[0] *= v[0];
        value[1] *= v[1];
        value[2] *= v[2];
        return *this;
    }

	Vector4 Matrix4::Row(int index) const
	{
		return Vector4(value[0][index], value[1][index], value[2][index], value[3][index]);
	}

	const Vector4& Matrix4::Column(int index) const
	{
		return value[index];
	}

	Matrix4 Matrix4::GetSphericalBillboardMatrix() const
	{
		Matrix4 m(*this);
		m[0] = Vector4(1.0, 0.0, 0.0, m[0][3]);
		m[1] = Vector4(0.0, 1.0, 0.0, m[1][3]);
		m[2] = Vector4(0.0, 0.0, 1.0, m[2][3]);
		return m;
	}

	Matrix4 Matrix4::GetCylindricalBillboardMatrix() const
	{
		Matrix4 m(*this);
		m[0] = Vector4(1.0, 0.0, 0.0, m[0][3]);
		m[2] = Vector4(0.0, 0.0, 1.0, m[2][3]);
		return m;
	}

	void Matrix4::Decompose(Vector3& position, Quaternion& q, Vector3& scale) const
	{
		const Matrix4& m = *this;
		Vertex3 scaling(value[0].Length(), value[1].Length(), value[2].Length());
		Matrix3 tmp1(Matrix4().Scale(Vector3(1 / scaling.x, 1 / scaling.y, 1 / scaling.z)) * m);
		q = tmp1.QuatCast();
		position = Vertex3(value[3]);
		Matrix3 tmp2(tmp1.Inverse() * Matrix3(m));
		scale = Vertex3(tmp2[0].x, tmp2[1].y, tmp2[2].z);
		if (scale.IsZeroLength())
			scale = Vector3(1);// prevent zero scale
	}

    Matrix4 operator*(const Matrix4& m, float s)
    {
        return Matrix4(
            m[0] * s,
            m[1] * s,
            m[2] * s,
            m[3] * s);
    }

    Vector4 operator*(const Matrix4& m, const Vector4& v)
    {
        auto& mov0(v[0]);
        auto& mov1(v[1]);
        auto mul0 = m[0] * mov0;
        auto mul1 = m[1] * mov1;
        auto add0 = mul0 + mul1;
        auto& mov2(v[2]);
        auto& mov3(v[3]);
        auto mul2 = m[2] * mov2;
        auto mul3 = m[3] * mov3;
        auto add1 = mul2 + mul3;
        auto add2 = add0 + add1;
        return add2;        
    }

    Matrix4 operator*(const Matrix4& m1, const Matrix4& m2)
    {
        const Vector4& srcA0 = m1.value[0];
        const Vector4& srcA1 = m1.value[1];
        const Vector4& srcA2 = m1.value[2];
        const Vector4& srcA3 = m1.value[3];

        const Vector4& srcB0 = m2.value[0];
        const Vector4& srcB1 = m2.value[1];
        const Vector4& srcB2 = m2.value[2];
        const Vector4& srcB3 = m2.value[3];

        Matrix4 result;
        result[0] = srcA0 * srcB0[0] + srcA1 * srcB0[1] + srcA2 * srcB0[2] + srcA3 * srcB0[3];
        result[1] = srcA0 * srcB1[0] + srcA1 * srcB1[1] + srcA2 * srcB1[2] + srcA3 * srcB1[3];
        result[2] = srcA0 * srcB2[0] + srcA1 * srcB2[1] + srcA2 * srcB2[2] + srcA3 * srcB2[3];
        result[3] = srcA0 * srcB3[0] + srcA1 * srcB3[1] + srcA2 * srcB3[2] + srcA3 * srcB3[3];
        return result;
    }

    Matrix4 Matrix4::Inverse() const
    {
        auto Coef00 = value[2][2] * value[3][3] - value[3][2] * value[2][3];
        auto Coef02 = value[1][2] * value[3][3] - value[3][2] * value[1][3];
        auto Coef03 = value[1][2] * value[2][3] - value[2][2] * value[1][3];

        auto Coef04 = value[2][1] * value[3][3] - value[3][1] * value[2][3];
        auto Coef06 = value[1][1] * value[3][3] - value[3][1] * value[1][3];
        auto Coef07 = value[1][1] * value[2][3] - value[2][1] * value[1][3];

        auto Coef08 = value[2][1] * value[3][2] - value[3][1] * value[2][2];
        auto Coef10 = value[1][1] * value[3][2] - value[3][1] * value[1][2];
        auto Coef11 = value[1][1] * value[2][2] - value[2][1] * value[1][2];

        auto Coef12 = value[2][0] * value[3][3] - value[3][0] * value[2][3];
        auto Coef14 = value[1][0] * value[3][3] - value[3][0] * value[1][3];
        auto Coef15 = value[1][0] * value[2][3] - value[2][0] * value[1][3];

        auto Coef16 = value[2][0] * value[3][2] - value[3][0] * value[2][2];
        auto Coef18 = value[1][0] * value[3][2] - value[3][0] * value[1][2];
        auto Coef19 = value[1][0] * value[2][2] - value[2][0] * value[1][2];

        auto Coef20 = value[2][0] * value[3][1] - value[3][0] * value[2][1];
        auto Coef22 = value[1][0] * value[3][1] - value[3][0] * value[1][1];
        auto Coef23 = value[1][0] * value[2][1] - value[2][0] * value[1][1];

        Vector4 Fac0(Coef00, Coef00, Coef02, Coef03);
        Vector4 Fac1(Coef04, Coef04, Coef06, Coef07);
        Vector4 Fac2(Coef08, Coef08, Coef10, Coef11);
        Vector4 Fac3(Coef12, Coef12, Coef14, Coef15);
        Vector4 Fac4(Coef16, Coef16, Coef18, Coef19);
        Vector4 Fac5(Coef20, Coef20, Coef22, Coef23);

        Vector4 Vec0(value[1][0], value[0][0], value[0][0], value[0][0]);
        Vector4 Vec1(value[1][1], value[0][1], value[0][1], value[0][1]);
        Vector4 Vec2(value[1][2], value[0][2], value[0][2], value[0][2]);
        Vector4 Vec3(value[1][3], value[0][3], value[0][3], value[0][3]);

        Vector4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
        Vector4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
        Vector4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
        Vector4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

        Vector4 SignA(+1, -1, +1, -1);
        Vector4 SignB(-1, +1, -1, +1);
        Matrix4 Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

        Vector4 Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

        Vector4 Dot0(value[0] * Row0);
        auto Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

        auto OneOverDeterminant = 1.f / Dot1;

        return Inverse * OneOverDeterminant;
    }

}
