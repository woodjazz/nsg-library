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
#include <string>
#include <algorithm>
#include "Types.h"
#include "Check.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btQuaternion.h"

namespace NSG
{
    btTransform ToTransform(const Vector3& pos, const Quaternion& rot);
    btVector3 ToBtVector3(const Vector3& obj);
    Vector3 ToVector3(const btVector3& obj);
    btQuaternion ToBtQuaternion(const Quaternion& q);
    Quaternion ToQuaternion(const btQuaternion& q);

	Vector3 Lerp(const Vector3& lhs, const Vector3& rhs, float t);
	void DecomposeMatrix(const Matrix4& m, Vertex3& position, Quaternion& q, Vertex3& scale);
	inline unsigned NextPowerOfTwo(unsigned value)
	{
	    unsigned ret = 1;
	    while (ret < value && ret < 0x80000000)
	        ret <<= 1;
	    return ret;
	}
	Vertex2 GetVertex2(const std::string& buffer);
	Vertex3 GetVertex3(const std::string& buffer);
	Vertex4 GetVertex4(const std::string& buffer);
	Quaternion GetQuaternion(const std::string& buffer);
	Matrix4 GetMatrix4(const std::string& buffer);

	std::string ToString(const Vertex2& obj);
	std::string ToString(const Vertex3& obj);
	std::string ToString(const Vertex4& obj);
	std::string ToString(const Quaternion& obj);
	std::string ToString(const Matrix4& m);
	std::string ToString(int obj);
	std::string ToString(size_t obj);
	std::string GetUniqueName(const std::string& name = "");
	void GetPowerOfTwoValues(int& width, int& height);
	bool IsPowerOfTwo(int value);
	bool IsZeroLength(const Vector3& obj);
	GLushort Transform(GLubyte selected[4]);
	Color Transform(GLushort id);
	std::string CompressBuffer(const std::string& buf);
	std::string DecompressBuffer(const std::string& buffer);
    int ToInt(const std::string& value);
    int ToInt(const char* value);

}
