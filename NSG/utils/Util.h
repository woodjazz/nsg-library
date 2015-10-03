/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2015 Néstor Silveira Gorski

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
#include "Types.h"
#include "Constants.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btQuaternion.h"
#include <string>

namespace NSG
{
	btTransform ToTransform(const Vector3& pos, const Quaternion& rot);
	btVector3 ToBtVector3(const Vector3& obj);
	Vector3 ToVector3(const btVector3& obj);
	btQuaternion ToBtQuaternion(const Quaternion& q);
	Quaternion ToQuaternion(const btQuaternion& q);
	inline bool Equals(float lhs, float rhs) { return lhs + EPSILON >= rhs && lhs - EPSILON <= rhs; }
	bool IsNaN(const Quaternion& q);
	bool IsNaN(const Vector3& v);
	Quaternion QuaternionFromLookRotation(const Vector3& direction, const Vector3& upDirection);
	Vector3 Translation(const Matrix4& m);
	Vector3 Scale(const Matrix4& m);
	void DecomposeMatrix(const Matrix4& m, Vertex3& position, Quaternion& q, Vertex3& scale);
	Matrix4 ComposeMatrix(const Vertex3& position, const Quaternion& q, const Vertex3& scale);
	Matrix4 ComposeMatrix(const Vertex3& position, const Quaternion& q);
	Matrix4 ComposeMatrix(const Quaternion& q);
	unsigned NextPowerOfTwo(unsigned value);
	std::string GetUniqueName(const std::string& name = "");
	void GetPowerOfTwoValues(int& width, int& height);
	bool IsPowerOfTwo(int value);
	bool IsZeroLength(const Vector3& obj);
	GLushort Transform(GLubyte selected[4]);
	Color Transform(GLushort id);
	std::string CompressBuffer(const std::string& buf);
	std::string DecompressBuffer(const std::string& buffer);
	bool IsScaleUniform(const Vector3& scale);
	Vector3 GetSlidingVector(const Vector3& dir2Target, const Vector3& hitNormal);
	float Clamp(float value, float minVal, float maxVal);
	int Clamp(int value, int minVal, int maxVal);
	float Length(const Vector3& value);
	float Length(const Vector4& value);
	float Length2(const Vector3& value);
	float Radians(float degrees);
	float Degrees(float radians);
	Vector3 Radians(const Vector3& degrees);
	Vector3 Degrees(const Vector3& radians);
	float Angle(const Vector3& a, const Vector3& b);
	Vector3 Normalize(const Vector3& value);
	Quaternion Normalize(const Quaternion& value);
	float Distance(float a, float b);
	float Distance(const Vector4& a, const Vector4& b);
    float Distance(const Vector3& a, const Vector3& b);
    float Distance2(const Vector3& a, const Vector3& b);
	float Dot(const Vector4& a, const Vector4& b);
	float Dot(const Vector3& a, const Vector3& b);
	float Dot(const Quaternion& a, const Quaternion& b);
    Vector3 Cross(const Vector3& a, const Vector3& b);
	float Min(float a, float b);
    Matrix3 Transpose(const Matrix3& a);
    Matrix3 Inverse(const Matrix3& a);
    Matrix4 Inverse(const Matrix4& a);
    Matrix4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar);
    Matrix4 Perspective(float fovyRadians, float aspectRatio, float zNear, float zFar);
    Matrix4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
    Quaternion Inverse(const Quaternion& a);
    Quaternion AngleAxis(float radians, const Vector3& axis);
	Vector3 Lerp(const Vector3& a, const Vector3& b, float t);
	float Lerp(float a, float b, float t);
	Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);
    float Abs(float value);
    float Cos(float value);
    float Floor(float value);
    Vector3 Floor(Vector3 value);
	inline const float* GetPointer(const Matrix4& mat) { return &(mat[0].x); }
	inline const float* GetPointer(const Matrix3& mat) { return &(mat[0].x); }
	Vector4 Row(const Matrix4& mat, int index);
	Vector3 Row(const Matrix3& mat, int index);
	Vector4 Column(const Matrix4& mat, int index);
	Vector3 Column(const Matrix3& mat, int index);
	Vector3 CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float s);
	Vector4 Fract(const Vector4& value);
	Vertex3 EulerAngles(const Quaternion& q);
	Matrix4 GetSphericalBillboardMatrix(Matrix4 m);
	Matrix4 GetCylindricalBillboardMatrix(Matrix4 m);
}
