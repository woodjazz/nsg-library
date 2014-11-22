/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

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

namespace NSG
{
	Vector3 Lerp(const Vector3& lhs, const Vector3& rhs, float t);
	void DecomposeMatrix(const Matrix4& m, Vertex3& position, Quaternion& q, Vertex3& scale);
    bool CopyFile(const std::string& source, const std::string& target);
    bool SetCurrentDir(const std::string& pathName);
	inline unsigned NextPowerOfTwo(unsigned value)
	{
	    unsigned ret = 1;
	    while (ret < value && ret < 0x80000000)
	        ret <<= 1;
	    return ret;
	}
	std::istream& operator >> (std::istream& s, Vertex2& obj);
	std::istream& operator >> (std::istream& s , Vertex3& obj);
	std::istream& operator >> (std::istream& s , Vertex4& obj);
	std::istream& operator >> (std::istream& s, Matrix4& obj);
	std::istream& operator >> (std::istream& s, Quaternion& obj);
	Vertex2 GetVertex2(const std::string& buffer);
	Vertex3 GetVertex3(const std::string& buffer);
	Vertex4 GetVertex4(const std::string& buffer);
	Matrix4 GetMatrix4(const std::string& buffer);
	std::string ToString(const Matrix4& m);
	Quaternion GetQuaternion(const std::string& buffer);
	std::string GetUniqueName(const std::string& name = "");
	void GetPowerOfTwoValues(unsigned& width, unsigned& height);
	bool IsPowerOfTwo(unsigned value);
}
