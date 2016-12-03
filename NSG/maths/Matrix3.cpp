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
#include "Matrix3.h"
#include "Matrix4.h"
#include "Util.h"
#include "Check.h"
#include <cmath>
namespace NSG
{
    Matrix3::Matrix3()
    {
        value[0] = Vector3(1, 0, 0);
        value[1] = Vector3(0, 1, 0);
        value[2] = Vector3(0, 0, 1);
    }

    Matrix3::Matrix3(float s)
    {
        value[0] = Vector3(s, 0, 0);
        value[1] = Vector3(0, s, 0);
        value[2] = Vector3(0, 0, s);
    }

    Matrix3::Matrix3(const Matrix3& m)
    {
        value[0] = m.value[0];
        value[1] = m.value[1];
        value[2] = m.value[2];
    }

    Matrix3::Matrix3(const Matrix4& m)
    {
        value[0] = m.value[0];
        value[1] = m.value[1];
        value[2] = m.value[2];
    }

    Matrix3::Matrix3(const Vector3& v0, const Vector3& v1, const Vector3& v2)
    {
        value[0] = v0;
        value[1] = v1;
        value[2] = v2;
    }

    Matrix3::Matrix3
    (
        float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3
    )
    {
        value[0] = Vector3(x1, y1, z1);
        value[1] = Vector3(x2, y2, z2);
        value[2] = Vector3(x3, y3, z3);
    }

    Matrix3::Matrix3(const Quaternion& q)
    {
		auto qxx(q.x * q.x);
		auto qyy(q.y * q.y);
		auto qzz(q.z * q.z);
		auto qxz(q.x * q.z);
		auto qxy(q.x * q.y);
		auto qyz(q.y * q.z);
		auto qwx(q.w * q.x);
		auto qwy(q.w * q.y);
		auto qwz(q.w * q.z);

		value[0][0] = 1 - 2 * (qyy + qzz);
		value[0][1] = 2 * (qxy + qwz);
		value[0][2] = 2 * (qxz - qwy);

		value[1][0] = 2 * (qxy - qwz);
		value[1][1] = 1 - 2 * (qxx + qzz);
		value[1][2] = 2 * (qyz + qwx);

		value[2][0] = 2 * (qxz + qwy);
		value[2][1] = 2 * (qyz - qwx);
		value[2][2] = 1 - 2 * (qxx + qyy);
    }

    const Vector3& Matrix3::operator[](int i) const
    {
        CHECK_ASSERT(i >= 0 && i < 3);
        return value[i];
    }

    Vector3& Matrix3::operator[](int i)
    {
        CHECK_ASSERT(i >= 0 && i < 3);
        return value[i];
    }

    Matrix3 operator*(const Matrix3& m1, const Matrix3& m2)
    {
        auto& srcA00 = m1[0][0];
        auto& srcA01 = m1[0][1];
        auto& srcA02 = m1[0][2];
        auto& srcA10 = m1[1][0];
        auto& srcA11 = m1[1][1];
        auto& srcA12 = m1[1][2];
        auto& srcA20 = m1[2][0];
        auto& srcA21 = m1[2][1];
        auto& srcA22 = m1[2][2];

        auto& srcB00 = m2[0][0];
        auto& srcB01 = m2[0][1];
        auto& srcB02 = m2[0][2];
        auto& srcB10 = m2[1][0];
        auto& srcB11 = m2[1][1];
        auto& srcB12 = m2[1][2];
        auto& srcB20 = m2[2][0];
        auto& srcB21 = m2[2][1];
        auto& srcB22 = m2[2][2];

        Matrix3 result;
        result[0][0] = srcA00 * srcB00 + srcA10 * srcB01 + srcA20 * srcB02;
        result[0][1] = srcA01 * srcB00 + srcA11 * srcB01 + srcA21 * srcB02;
        result[0][2] = srcA02 * srcB00 + srcA12 * srcB01 + srcA22 * srcB02;
        result[1][0] = srcA00 * srcB10 + srcA10 * srcB11 + srcA20 * srcB12;
        result[1][1] = srcA01 * srcB10 + srcA11 * srcB11 + srcA21 * srcB12;
        result[1][2] = srcA02 * srcB10 + srcA12 * srcB11 + srcA22 * srcB12;
        result[2][0] = srcA00 * srcB20 + srcA10 * srcB21 + srcA20 * srcB22;
        result[2][1] = srcA01 * srcB20 + srcA11 * srcB21 + srcA21 * srcB22;
        result[2][2] = srcA02 * srcB20 + srcA12 * srcB21 + srcA22 * srcB22;
        return result;
    }

	Quaternion Matrix3::QuatCast() const
	{
		auto fourXSquaredMinus1 = value[0][0] - value[1][1] - value[2][2];
		auto fourYSquaredMinus1 = value[1][1] - value[0][0] - value[2][2];
		auto fourZSquaredMinus1 = value[2][2] - value[0][0] - value[1][1];
		auto fourWSquaredMinus1 = value[0][0] + value[1][1] + value[2][2];

		int biggestIndex = 0;
		auto fourBiggestSquaredMinus1 = fourWSquaredMinus1;
		if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourXSquaredMinus1;
			biggestIndex = 1;
		}
		if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourYSquaredMinus1;
			biggestIndex = 2;
		}
		if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourZSquaredMinus1;
			biggestIndex = 3;
		}

		auto biggestVal = sqrt(fourBiggestSquaredMinus1 + 1) * 0.5f;
		auto mult = 0.25f / biggestVal;

		Quaternion result;
		switch (biggestIndex)
		{
		case 0:
			result.w = biggestVal;
			result.x = (value[1][2] - value[2][1]) * mult;
			result.y = (value[2][0] - value[0][2]) * mult;
			result.z = (value[0][1] - value[1][0]) * mult;
			break;
		case 1:
			result.w = (value[1][2] - value[2][1]) * mult;
			result.x = biggestVal;
			result.y = (value[0][1] + value[1][0]) * mult;
			result.z = (value[2][0] + value[0][2]) * mult;
			break;
		case 2:
			result.w = (value[2][0] - value[0][2]) * mult;
			result.x = (value[0][1] + value[1][0]) * mult;
			result.y = biggestVal;
			result.z = (value[1][2] + value[2][1]) * mult;
			break;
		case 3:
			result.w = (value[0][1] - value[1][0]) * mult;
			result.x = (value[2][0] + value[0][2]) * mult;
			result.y = (value[1][2] + value[2][1]) * mult;
			result.z = biggestVal;
			break;
		default:
			CHECK_ASSERT(false);
			break;
		}
		return result;
	}

	Matrix3 Matrix3::Transpose() const
	{
		Matrix3 m;
		m[0][0] = value[0][0];
		m[0][1] = value[1][0];
		m[0][2] = value[2][0];
		m[1][0] = value[0][1];
		m[1][1] = value[1][1];
		m[1][2] = value[2][1];
		m[2][0] = value[0][2];
		m[2][1] = value[1][2];
		m[2][2] = value[2][2];
        return m;
	}

	Matrix3 Matrix3::Inverse() const
	{
		auto OneOverDeterminant = 1.f / (
			+value[0][0] * (value[1][1] * value[2][2] - value[2][1] * value[1][2])
			- value[1][0] * (value[0][1] * value[2][2] - value[2][1] * value[0][2])
			+ value[2][0] * (value[0][1] * value[1][2] - value[1][1] * value[0][2]));

		Matrix4 inverse;
		inverse[0][0] = +(value[1][1] * value[2][2] - value[2][1] * value[1][2]) * OneOverDeterminant;
		inverse[1][0] = -(value[1][0] * value[2][2] - value[2][0] * value[1][2]) * OneOverDeterminant;
		inverse[2][0] = +(value[1][0] * value[2][1] - value[2][0] * value[1][1]) * OneOverDeterminant;
		inverse[0][1] = -(value[0][1] * value[2][2] - value[2][1] * value[0][2]) * OneOverDeterminant;
		inverse[1][1] = +(value[0][0] * value[2][2] - value[2][0] * value[0][2]) * OneOverDeterminant;
		inverse[2][1] = -(value[0][0] * value[2][1] - value[2][0] * value[0][1]) * OneOverDeterminant;
		inverse[0][2] = +(value[0][1] * value[1][2] - value[1][1] * value[0][2]) * OneOverDeterminant;
		inverse[1][2] = -(value[0][0] * value[1][2] - value[1][0] * value[0][2]) * OneOverDeterminant;
		inverse[2][2] = +(value[0][0] * value[1][1] - value[1][0] * value[0][1]) * OneOverDeterminant;
		return inverse;
	}

	Vector3 Matrix3::Row(int index) const
	{
		return Vector3(value[0][index], value[1][index], value[2][index]);
	}

	const Vector3& Matrix3::Column(int index) const
	{
		return value[index];
	}
}
