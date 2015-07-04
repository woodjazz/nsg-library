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
#include "NSG.h"
#include "UtilConverter.h"
using namespace NSG;
using namespace BlenderConverter;
static void Test01()
{
	std::vector<Vector3> points;
	points.push_back(Vector3(0, 0, 0));
	points.push_back(Vector3(1, 0, 0));
	points.push_back(Vector3(2, 1, 0));
	points.push_back(Vector3(2, 2, 0));
	points.push_back(Vector3(0, 2, 0));

	Vector3 center = points[0];
    Plane plane(points[0], points[1], points[2]);
    auto m = GeneratePointTransformMatrix(plane, center);

    for (auto& point : points)
    {
        Vector4 tmp = m * Vector4(point, 1);
		//Flatten points always belong to XY plane
		CHECK_CONDITION(tmp.z == 0, __FILE__, __LINE__);
    }
}

static void Test02()
{
	std::vector<Vector3> points;
	points.push_back(Vector3(0, 0, 0));
	points.push_back(Vector3(1, 0, 0));
	points.push_back(Vector3(2, 0, 1));
	points.push_back(Vector3(2, 0, 2));
	points.push_back(Vector3(0, 0, 2));

	Vector3 center = points[0];
	Plane plane(points[0], points[1], points[2]);
	auto m = GeneratePointTransformMatrix(plane, center);

	for (auto& point : points)
	{
		Vector4 tmp = m * Vector4(point, 1);
		//Flatten points always belong to XY plane
		CHECK_CONDITION(tmp.z == 0, __FILE__, __LINE__);
	}
}

static void Test03()
{
	std::vector<Vector3> points;
	points.push_back(Vector3(0, 0, 0));
	points.push_back(Vector3(0, 1, 0));
	points.push_back(Vector3(0, 2, 1));
	points.push_back(Vector3(0, 2, 2));
	points.push_back(Vector3(0, 0, 2));

	Vector3 center = points[0];
	Plane plane(points[0], points[1], points[2]);
	auto m = GeneratePointTransformMatrix(plane, center);

	for (auto& point : points)
	{
		Vector4 tmp = m * Vector4(point, 1);
		//Flatten points always belong to XY plane
		CHECK_CONDITION(tmp.z == 0, __FILE__, __LINE__);
	}
}

static void Test04()
{
	std::vector<Vector3> points;
	points.push_back(Vector3(0, 0, 0));
	points.push_back(Vector3(0, 1, 0));
	points.push_back(Vector3(0, 2, 1));
	points.push_back(Vector3(0, 2, 2));
	points.push_back(Vector3(0, 0, 2));

	for (auto& point : points)
		point = Quaternion(Vector3(30, 40, 50)) * point;

	Vector3 center = points[0];
	Plane plane(points[0], points[1], points[2]);
	auto m = GeneratePointTransformMatrix(plane, center);

	for (auto& point : points)
	{
		Vector4 tmp = m * Vector4(point, 1);
		//Flatten points always belong to XY plane
		CHECK_CONDITION(std::abs(tmp.z) < 0.00001f, __FILE__, __LINE__);
	}
}


void Tests()
{
	Test01();
	Test02();
	Test03();
	Test04();
}
