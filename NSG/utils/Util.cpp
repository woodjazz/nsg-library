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

#include "Util.h"
#include "Check.h"
#include "Plane.h"
#include "Constants.h"
#include "Path.h"
#include "StringConverter.h"
#include "pugixml.hpp"
#include "lz4.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <thread>
#ifndef IS_TARGET_WINDOWS
#include <unistd.h>
#include <cerrno>
#else
#include <windows.h>
#define snprintf _snprintf
#endif
#if EMSCRIPTEN
#include "emscripten.h"
#endif

namespace NSG
{
	btTransform ToTransform(const Vector3& pos, const Quaternion& rot)
	{
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
		trans.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
		return trans;
	}

	btVector3 ToBtVector3(const Vector3& obj)
	{
		return btVector3(obj.x, obj.y, obj.z);
	}

	Vector3 ToVector3(const btVector3& obj)
	{
		return Vector3(obj.x(), obj.y(), obj.z());
	}

	btQuaternion ToBtQuaternion(const Quaternion& q)
	{
		return btQuaternion(q.x, q.y, q.z, q.w);
	}

	bool IsNaN(const Quaternion& q)
	{
		return std::isnan(q.w) || std::isnan(q.x) || std::isnan(q.y) || std::isnan(q.z);
	}

	bool IsNaN(const Vector3& v)
	{
		return std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z);
	}

	Quaternion QuaternionFromLookRotation(const Vector3& direction, const Vector3& upDirection)
	{
		Vector3 forward = Normalize(direction);
		Vector3 v = Normalize(Cross(forward, upDirection));
		if (IsNaN(v))
		{
			auto up1 = upDirection;
			std::swap(up1.x, up1.y);
			v = Normalize(Cross(forward, up1));
			if (IsNaN(v))
			{
				up1 = upDirection;
				std::swap(up1.x, up1.z);
				v = Normalize(Cross(forward, up1));
				if (IsNaN(v))
				{
					up1 = upDirection;
					std::swap(up1.y, up1.z);
					v = Normalize(Cross(forward, up1));
					CHECK_ASSERT(!IsNaN(v));
				}
			}
		}
		Vector3 up = Cross(v, forward);
		Vector3 right = Cross(up, forward);

		Quaternion ret(Matrix3(right, up, forward));
		CHECK_ASSERT(!IsNaN(ret));
		return ret;
	}

	Quaternion ToQuaternion(const btQuaternion& q)
	{
		return Quaternion(q.w(), q.x(), q.y(), q.z());
	}

	/// Return the translation part.
	Vector3 Translation(const Matrix4& m)
	{
		return Vector3(m[3]);
	}

	/// Return the scaling part.
	Vector3 Scale(const Matrix4& m)
	{
		return Vector3(
			sqrtf(m[0][0] * m[0][0] + m[0][1] * m[0][1] + m[0][2] * m[0][2]),
			sqrtf(m[1][0] * m[1][0] + m[1][1] * m[1][1] + m[1][2] * m[1][2]),
			sqrtf(m[2][0] * m[2][0] + m[2][1] * m[2][1] + m[2][2] * m[2][2])
		);
	}

	Matrix4 Scale(const Matrix4& m, const Vector3& v)
	{
		Matrix4 result(0);
		result[0] = m[0] * v[0];
		result[1] = m[1] * v[1];
		result[2] = m[2] * v[2];
		result[3] = m[3];
		return result;
	}

	Quaternion QuatCast(const Matrix3& m)
	{
		auto fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
		auto fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
		auto fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
		auto fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

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
			result.x = (m[1][2] - m[2][1]) * mult;
			result.y = (m[2][0] - m[0][2]) * mult;
			result.z = (m[0][1] - m[1][0]) * mult;
			break;
		case 1:
			result.w = (m[1][2] - m[2][1]) * mult;
			result.x = biggestVal;
			result.y = (m[0][1] + m[1][0]) * mult;
			result.z = (m[2][0] + m[0][2]) * mult;
			break;
		case 2:
			result.w = (m[2][0] - m[0][2]) * mult;
			result.x = (m[0][1] + m[1][0]) * mult;
			result.y = biggestVal;
			result.z = (m[1][2] + m[2][1]) * mult;
			break;
		case 3:
			result.w = (m[0][1] - m[1][0]) * mult;
			result.x = (m[2][0] + m[0][2]) * mult;
			result.y = (m[1][2] + m[2][1]) * mult;
			result.z = biggestVal;
			break;
		default:
			CHECK_ASSERT(false);
			break;
		}
		return result;
	}

	void DecomposeMatrix(const Matrix4& m, Vertex3& position, Quaternion& q, Vertex3& scale)
	{
		Vertex3 scaling(Length(m[0]), Length(m[1]), Length(m[2]));
		Matrix3 tmp1(Scale(Matrix4(1.0f), Vertex3(1) / scaling) * m);
		q = QuatCast(tmp1);
		position = Vertex3(m[3]);
		Matrix3 tmp2(Inverse(tmp1) * Matrix3(m));
		scale = Vertex3(tmp2[0].x, tmp2[1].y, tmp2[2].z);
		if (IsZeroLength(scale))
			scale = Vector3(1);// prevent zero scale
	}

	Matrix4 Translate(const Matrix4& m, const Vector3& v)
	{
		Matrix4 result(m);
		result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
		return result;
	}

	Matrix3 Mat3Cast(const Quaternion& q)
	{
		Matrix3 result(1);
		auto qxx(q.x * q.x);
		auto qyy(q.y * q.y);
		auto qzz(q.z * q.z);
		auto qxz(q.x * q.z);
		auto qxy(q.x * q.y);
		auto qyz(q.y * q.z);
		auto qwx(q.w * q.x);
		auto qwy(q.w * q.y);
		auto qwz(q.w * q.z);

		result[0][0] = 1 - 2 * (qyy + qzz);
		result[0][1] = 2 * (qxy + qwz);
		result[0][2] = 2 * (qxz - qwy);

		result[1][0] = 2 * (qxy - qwz);
		result[1][1] = 1 - 2 * (qxx + qzz);
		result[1][2] = 2 * (qyz + qwx);

		result[2][0] = 2 * (qxz + qwy);
		result[2][1] = 2 * (qyz - qwx);
		result[2][2] = 1 - 2 * (qxx + qyy);
		return result;
	}

	Matrix4 Mat4Cast(const Quaternion& q)
	{
		return Matrix4(Mat3Cast(q));
	}

	Matrix4 ComposeMatrix(const Vertex3& position, const Quaternion& q, const Vertex3& scale)
	{
		return Translate(Matrix4(), position) * Mat4Cast(q) * Scale(Matrix4(1.f), scale);
	}

	Matrix4 ComposeMatrix(const Vertex3& position, const Quaternion& q)
	{
		return Translate(Matrix4(), position) * Mat4Cast(q);
	}

	Matrix4 ComposeMatrix(const Quaternion& q)
	{
		return Mat4Cast(q);
	}

	std::string GetUniqueName(const std::string& name)
	{
		static int counter = 0;
		const int MaxBuffer = 100;
		char buffer[MaxBuffer];
		if (name == "")
			snprintf(buffer, MaxBuffer, "Gen%d", counter++);
		else
			snprintf(buffer, MaxBuffer, "%s%d", name.c_str(), counter++);
		return buffer;
	}

	void GetPowerOfTwoValues(int& width, int& height)
	{
		int new_width = 1;
		int new_height = 1;

		while (new_width < width)
			new_width *= 2;

		while (new_height < height)
			new_height *= 2;

		width = new_width;
		height = new_height;
	}

	bool IsPowerOfTwo(int value)
	{
		return (!(value & (value - 1)) && value);
	}

	unsigned NextPowerOfTwo(unsigned value)
	{
		unsigned ret = 1;
		while (ret < value && ret < 0x80000000)
			ret <<= 1;
		return ret;
	}

	bool IsZeroLength(const Vector3& obj)
	{
		auto length = Length(obj);
		return length <= EPSILON;
	}

	GLushort Transform(GLubyte selected[4])
	{
		GLushort b3 = (GLushort)selected[3] / 0x10;
		GLushort b2 = (GLushort)selected[2] / 0x10;
		GLushort b1 = (GLushort)selected[1] / 0x10;
		GLushort b0 = (GLushort)selected[0] / 0x10;
		GLushort index = b3 << 12 | b2 << 8 | b1 << 4 | b0;
		return index;
	}

	Color Transform(GLushort id)
	{
		Color color;
		color[0] = (id & 0x000F) / 15.0f;
		color[1] = ((id & 0x00F0) >> 4) / 15.0f;
		color[2] = ((id & 0x0F00) >> 8) / 15.0f;
		color[3] = ((id & 0xF000) >> 12) / 15.0f;
		return color;
	}

	static size_t HeaderSize = 128;
	std::string CompressBuffer(const std::string& buf)
	{
		std::string buffer = ToString(buf.size()) + " ";
		buffer.resize(HeaderSize);
		buffer += buf;
		std::string compressBuffer;
		CHECK_ASSERT(buffer.size() < std::numeric_limits<int>::max());
		compressBuffer.resize(LZ4_compressBound((int)buffer.size()));
		auto bufferSize = LZ4_compress(buffer.c_str(), &compressBuffer[0], (int)buffer.size());
		CHECK_ASSERT(bufferSize >= 0);
		compressBuffer.resize(bufferSize);
		return compressBuffer;
	}


	std::string DecompressBuffer(const std::string& buffer)
	{
		std::string smallBuffer;
		smallBuffer.resize(HeaderSize);
		CHECK_ASSERT(buffer.size() < std::numeric_limits<int>::max());
		CHECK_ASSERT(smallBuffer.size() < std::numeric_limits<int>::max());
		LZ4_decompress_safe_partial(&buffer[0], &smallBuffer[0], (int)buffer.size(), (int)smallBuffer.size(), (int)smallBuffer.size());
		std::string::size_type bytes = ToInt(smallBuffer);
		bytes += smallBuffer.size();
		std::string outputBuffer;
		outputBuffer.resize(bytes);
		CHECK_ASSERT(bytes < std::numeric_limits<int>::max());
		int totalBytes = LZ4_decompress_safe(&buffer[0], &outputBuffer[0], (int)buffer.size(), (int)bytes);
		CHECK_ASSERT(totalBytes == bytes);
		outputBuffer.erase(outputBuffer.begin(), outputBuffer.begin() + smallBuffer.size());
		return outputBuffer;
	}

	bool IsScaleUniform(const Vector3& scale)
	{
		return Abs(scale.x - scale.y) < PRECISION && Abs(scale.x - scale.z) < PRECISION;
	}

	Vector3 Reflect(const Vector3& I, const Vector3& N)
	{
		return I - N * Dot(N, I) * Vector3(2);
	}

	Vector3 GetSlidingVector(const Vector3& dir2Target, const Vector3& hitNormal)
	{
		Vector3 reflection = Reflect(dir2Target, hitNormal);
		Vector3 parallelComponent = Dot(reflection, hitNormal) * hitNormal;
		Vector3 sliding = reflection - parallelComponent;
		return sliding;
	}

	float Dot(const Vector4& a, const Vector4& b)
	{
		Vector4 tmp(a * b);
		return tmp.x + tmp.y + tmp.z + tmp.w;
	}

	float Dot(const Vector3& a, const Vector3& b)
	{
		Vector3 tmp(a * b);
		return tmp.x + tmp.y + tmp.z;
	}

	float Dot(const Quaternion& a, const Quaternion& b)
	{
		Vector4 tmp(Vector4(a.x, a.y, a.z, a.w) * Vector4(b.x, b.y, b.z, b.w));
		return tmp.x + tmp.y + tmp.z + tmp.w;
	}

	Vector3 Cross(const Vector3& a, const Vector3& b)
	{
		return Vector3(
			a.y * b.z - b.y * a.z,
			a.z * b.x - b.z * a.x,
			a.x * b.y - b.x * a.y);
	}

	float Min(float a, float b)
	{
		return std::min(a, b);
	}

	Matrix3 Transpose(const Matrix3& a)
	{
		Matrix3 result;
		result[0][0] = a[0][0];
		result[0][1] = a[1][0];
		result[0][2] = a[2][0];
		result[1][0] = a[0][1];
		result[1][1] = a[1][1];
		result[1][2] = a[2][1];
		result[2][0] = a[0][2];
		result[2][1] = a[1][2];
		result[2][2] = a[2][2];
		return result;
	}

	Matrix3 Inverse(const Matrix3& m)
	{
		auto OneOverDeterminant = 1.f / (
			+m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
			- m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
			+ m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

		Matrix3 Inverse;
		Inverse[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]) * OneOverDeterminant;
		Inverse[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * OneOverDeterminant;
		Inverse[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]) * OneOverDeterminant;
		Inverse[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * OneOverDeterminant;
		Inverse[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]) * OneOverDeterminant;
		Inverse[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * OneOverDeterminant;
		Inverse[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]) * OneOverDeterminant;
		Inverse[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * OneOverDeterminant;
		Inverse[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * OneOverDeterminant;
		return Inverse;

	}

	Matrix4 Inverse(const Matrix4& m)
	{
		auto Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		auto Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		auto Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

		auto Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		auto Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
		auto Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

		auto Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		auto Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
		auto Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

		auto Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		auto Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
		auto Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

		auto Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		auto Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
		auto Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

		auto Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		auto Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		auto Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

		Vector4 Fac0(Coef00, Coef00, Coef02, Coef03);
		Vector4 Fac1(Coef04, Coef04, Coef06, Coef07);
		Vector4 Fac2(Coef08, Coef08, Coef10, Coef11);
		Vector4 Fac3(Coef12, Coef12, Coef14, Coef15);
		Vector4 Fac4(Coef16, Coef16, Coef18, Coef19);
		Vector4 Fac5(Coef20, Coef20, Coef22, Coef23);

		Vector4 Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
		Vector4 Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
		Vector4 Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
		Vector4 Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

		Vector4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
		Vector4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
		Vector4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
		Vector4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

		Vector4 SignA(+1, -1, +1, -1);
		Vector4 SignB(-1, +1, -1, +1);
		Matrix4 Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

		Vector4 Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

		Vector4 Dot0(m[0] * Row0);
		auto Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

		auto OneOverDeterminant = 1.f / Dot1;

		return Inverse * OneOverDeterminant;
	}

	Matrix4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		Matrix4 m(1);
		m[0][0] = 2 / (right - left);
		m[1][1] = 2 / (top - bottom);
		m[2][2] = -2 / (zFar - zNear);
		m[3][0] = -(right + left) / (right - left);
		m[3][1] = -(top + bottom) / (top - bottom);
		m[3][2] = -(zFar + zNear) / (zFar - zNear);
		return m;
	}

	Matrix4 Perspective(float fovy, float aspect, float zNear, float zFar)
	{
		CHECK_ASSERT(fabs(aspect - std::numeric_limits<float>::epsilon()) > 0);
		auto tanHalfFovy = tan(fovy / 2.f);
		Matrix4 m(0);
		m[0][0] = 1 / (aspect * tanHalfFovy);
		m[1][1] = 1 / (tanHalfFovy);
		m[2][2] = -(zFar + zNear) / (zFar - zNear);
		m[2][3] = -1;
		m[3][2] = -(2 * zFar * zNear) / (zFar - zNear);
		return m;
	}

	Quaternion Inverse(const Quaternion& q)
	{
		Quaternion conjugate(q.w, -q.x, -q.y, -q.z);
		return conjugate / Dot(q, q);
	}

	Quaternion AngleAxis(float angle, const Vector3& axis)
	{
		Quaternion q;
		auto s = sinf(angle * 0.5f);
		q.w = cosf(angle * 0.5f);
		q.x = axis.x * s;
		q.y = axis.y * s;
		q.z = axis.z * s;
		return q;
	}

	Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
	{
		return a * (1.0f - t) + b * t;
	}

	float Lerp(float a, float b, float t)
	{
		return a * (1.0f - t) + b * t;
	}

	float Mix(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t)
	{
		auto z = b;
		auto cosTheta = Dot(a, b);
		if (cosTheta < 0)
		{
			// If cosTheta < 0, the interpolation will take the long way around the sphere.
			// To fix this, one quat must be negated.
			z = -b;
			cosTheta = -cosTheta;
		}

		if (cosTheta > 1 - std::numeric_limits<float>::epsilon())
		{
			// Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
			return Quaternion(
				Mix(a.w, z.w, t),
				Mix(a.x, z.x, t),
				Mix(a.y, z.y, t),
				Mix(a.z, z.z, t));
		}
		else
		{
			// Essential Mathematics, page 467
			auto angle = acosf(cosTheta);
			return (sinf((1.f - t) * angle) * a + sinf(t * angle) * z) / sinf(angle);
		}
	}

	float Clamp(float value, float minVal, float maxVal)
	{
		return std::min(std::max(value, minVal), maxVal);
	}

	int Clamp(int value, int minVal, int maxVal)
	{
		return std::min(std::max(value, minVal), maxVal);
	}

	float Length(const Vector3& value)
	{
		return sqrt(Dot(value, value));
	}

	float Length(const Vector4& value)
	{
		return sqrt(Dot(value, value));
	}

	float Length2(const Vector3& value)
	{
		return Dot(value, value);
	}

	float Radians(float degrees)
	{
		return degrees * DEG2RAD;
	}

	float Degrees(float radians)
	{
		return radians * RAD2DEG;
	}

	Vector3 Radians(const Vector3& degrees)
	{
		return degrees * DEG2RAD;
	}

	Vector3 Degrees(const Vector3& radians)
	{
		return radians * RAD2DEG;
	}

	float Angle(const Vector3& a, const Vector3& b)
	{
		return acos(Clamp(Dot(a, b), -1.f, 1.f));
	}

	Vector3 Normalize(const Vector3& value)
	{
		return value * 1.f / Length(value);
	}

	Quaternion Normalize(const Quaternion& q)
	{
		auto len = sqrt(Dot(q, q));
		if (len <= 0)
			return Quaternion(1, 0, 0, 0);
		auto oneOverLen = 1 / len;
		return Quaternion(q.w * oneOverLen, q.x * oneOverLen, q.y * oneOverLen, q.z * oneOverLen);
	}

	float Distance(float a, float b)
	{
		return std::abs(b - a);
	}

	float Distance(const Vector4& a, const Vector4& b)
	{
		return Length(b - a);
	}

	float Distance(const Vector3& a, const Vector3& b)
	{
		return Length(b - a);
	}

	float Distance2(const Vector3& a, const Vector3& b)
	{
		return Dot(a, b);
	}

	float Abs(float value)
	{
		return std::abs(value);
	}

	float Cos(float value)
	{
		return std::cos(value);
	}

	float Floor(float value)
	{
		return std::floor(value);
	}

	Vector3 Floor(Vector3 value)
	{
		return Vector3(Floor(value.x), Floor(value.y), Floor(value.z));
	}

	Vector4 Floor(Vector4 value)
	{
		return Vector4(Floor(value.x), Floor(value.y), Floor(value.z), Floor(value.w));
	}


	Vector4 Row(const Matrix4& mat, int index)
	{
		return Vector4(mat[0][index], mat[1][index], mat[2][index], mat[3][index]);
	}

	Vector3 Row(const Matrix3 mat, int index)
	{
		return Vector3(mat[0][index], mat[1][index], mat[2][index]);
	}

	Vector4 Column(const Matrix4& mat, int index)
	{
		return mat[index];
	}

	Vector3 Column(const Matrix3& mat, int index)
	{
		return mat[index];
	}

	float Pow2(float x)
	{
		return x * x;
	}

	float Pow3(float x)
	{
		return x * x * x;
	}

	Vector3 CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float s)
	{
		auto s1 = s;
		auto s2 = Pow2(s);
		auto s3 = Pow3(s);
		auto f1 = -s3 + 2 * s2 - s;
		auto f2 = 3 * s3 - 5 * s2 + 2;
		auto f3 = -3 * s3 + 4 * s2 + s;
		auto f4 = s3 - s2;
		return (f1 * v1 + f2 * v2 + f3 * v3 + f4 * v4) / 2.f;
	}

	Vector4 Fract(const Vector4& value)
	{
		return value - Floor(value);
	}

	float Roll(const Quaternion& q)
	{
		return std::atan2(2 * (q.x * q.y + q.w * q.z), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z);
	}

	float Pitch(const Quaternion& q)
	{
		return std::atan2(2 * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
	}

	float Yaw(const Quaternion& q)
	{
		return asin(-2 * (q.x * q.z - q.w * q.y));
	}

	Vertex3 EulerAngles(const Quaternion& q)
	{
		return Vector3(Pitch(q), Yaw(q), Roll(q));
	}

	Matrix4 GetSphericalBillboardMatrix(Matrix4 m)
	{
		m[0] = Vector4(1.0, 0.0, 0.0, m[0][3]);
		m[1] = Vector4(0.0, 1.0, 0.0, m[1][3]);
		m[2] = Vector4(0.0, 0.0, 1.0, m[2][3]);
		return m;
	}

	Matrix4 GetCylindricalBillboardMatrix(Matrix4 m)
	{
		m[0] = Vector4(1.0, 0.0, 0.0, m[0][3]);
		m[2] = Vector4(0.0, 0.0, 1.0, m[2][3]);
		return m;
	}

#if 0
	void SleepMs(unsigned milliseconds)
	{
#if EMSCRIPTEN
		emscripten_sleep(milliseconds);
#else
		std::this_thread::sleep_for(Milliseconds(milliseconds));
#endif
	}
#endif

	Quaternion Rotation(const Vector3& orig, const Vector3& dest)
	{
		auto cosTheta = Dot(orig, dest);
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
			rotationAxis = Cross(Vector3(0, 0, 1), orig);
			if (Length2(rotationAxis) < std::numeric_limits<float>::epsilon()) // bad luck, they were parallel, try again!
				rotationAxis = Cross(Vector3(1, 0, 0), orig);

			rotationAxis = Normalize(rotationAxis);
			return AngleAxis(PI, rotationAxis);
		}

		// Implementation from Stan Melax's Game Programming Gems 1 article
		rotationAxis = Cross(orig, dest);

		auto s = sqrt((1 + cosTheta) * 2);
		auto invs = 1 / s;

		return Quaternion(
			s * 0.5f,
			rotationAxis.x * invs,
			rotationAxis.y * invs,
			rotationAxis.z * invs);
	}
}
