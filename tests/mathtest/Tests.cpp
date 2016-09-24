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
#include "NSG.h"
using namespace NSG;

static void Test01()
{
	auto Encode = [](float depth) -> Vector3
	{
	    Vector3 ret;
	    depth *= 255.0f;
	    ret.x = std::floor(depth);
	    depth = (depth - ret.x) * 255.0f;
	    ret.y = std::floor(depth);
	    ret.z = (depth - ret.y);
	    ret.x *= 1.0f / 255.0f;
		ret.y *= 1.0f / 255.0f;
	    return ret;
	};

	auto Decode = [](Vector3 depth) -> float
	{
	    const Vector3 dotValues = Vector3(1.0f, 1.0f / 255.0f, 1.0f / (255.0f * 255.0f));
	    auto ret = depth.Dot(dotValues);
		return ret;
	};

	float depth = 0.000765f;
	CHECK_CONDITION(std::abs(depth - Decode(Encode(depth))) < 0.0001f);
    
    depth = 0.765f;
    CHECK_CONDITION(std::abs(depth - Decode(Encode(depth))) < 0.0001f);

	depth = 0.01265f;
	CHECK_CONDITION(std::abs(depth - Decode(Encode(depth))) < 0.0001f);
    
    depth = 0.01265f;
    CHECK_CONDITION(std::abs(depth - Decode(Encode(depth))) < 0.0001f);

	auto Encode1 = [](float depth)->Vector3
	{
		const float DISTANCE = 255.0f;
		float value = DISTANCE - depth;
		float v = floor(value);
		float f = value - v;
		float vn = v * 1.0f / DISTANCE;
		return Vector3(vn, f, 0.0f);
	};

	auto Decode1 = [](Vector3 depth) -> float
	{
		const float DISTANCE = 255.0f;
		auto ret = DISTANCE - (depth.x * DISTANCE + depth.y);
		return ret;
	};

	depth = 1;
	CHECK_CONDITION(std::abs(depth - Decode1(Encode1(depth))) < 0.0001f);

	depth = 0.0001265f;
	CHECK_CONDITION(std::abs(depth - Decode1(Encode1(depth))) < 0.0001f);

	auto Encode2 = [](float depth) -> Vector3
	{
	    Vector3 ret;
	    depth *= 255.0f;
	    ret.x = floor(depth);
	    ret.y = depth - ret.x;
	    ret.z = 0.0f;
	    ret.x *= 1.0f / 255.0f;
	    return ret;
	};

	auto Decode2 = [](Vector3 depth) -> float
	{
	    const Vector3 dotValues = Vector3(1.0f, 1.0f / 255.0f, 1.0f / (255.0f * 255.0f));
	    auto ret = depth.Dot(dotValues);
		return ret;
	};

	depth = 0.765f;
	CHECK_CONDITION(std::abs(depth - Decode2(Encode2(depth))) < 0.0001f);

	depth = 0.01265f;
	CHECK_CONDITION(std::abs(depth - Decode2(Encode2(depth))) < 0.0001f);

	auto Encode3 = [](float depth) -> Vector4
	{
		const Vector4 bit_shift = Vector4(256.0*256.0*256.0, 256.0*256.0, 256.0, 1.0);
		const Vector4 bit_mask = Vector4(0.0, 1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0);
        Vector4 res = (bit_shift * depth).Fract();
        res = res - Vector4(res.x, res.x, res.y, res.z) * bit_mask;
		return res;
	};

	auto Decode3 = [](Vector4 rgba_depth) -> float
	{
		const Vector4 bit_shift = Vector4(1.0 / (256.0*256.0*256.0), 1.0 / (256.0*256.0), 1.0 / 256.0, 1.0);
		float depth = rgba_depth.Dot(bit_shift);
		return depth;
	};

	depth = 0.999f;
	CHECK_CONDITION(std::abs(depth - Decode3(Encode3(depth))) < 0.000001f);
    
    depth = 0.99994f;
    CHECK_CONDITION(std::abs(depth - Decode3(Encode3(depth))) < 0.000001f);


	depth = 0.01265f;
	CHECK_CONDITION(std::abs(depth - Decode3(Encode3(depth))) < 0.0001f);

    depth = 0.000022f;
    CHECK_CONDITION(std::abs(depth - Decode3(Encode3(depth))) < 0.000001f);

    depth = 0.000033f;
    CHECK_CONDITION(std::abs(depth - Decode3(Encode3(depth))) < 0.000001f);


	auto Encode4 = [](float depth) -> Vector4
	{
		const Vector4 bit_shift = Vector4(32.0*64.0*32.0, 64.0*32.0, 1.0, 0.0);
		const Vector4 bit_mask = Vector4(0.0, 1.0 / 64.0, 1.0 / 64.0, 0.0);
        Vector4 res = (bit_shift * depth).Fract();
        res = res - Vector4(res.x, res.x, res.y, res.z) * bit_mask;
		return res;
	};

	auto Decode4 = [](Vector4 rgba_depth) -> float
	{
		const Vector4 bit_shift = Vector4(1.0 / (32.0*64.0*32.0), 1.0 / (64.0*32.0), 1.0, 0.0);
		float depth = rgba_depth.Dot(bit_shift);
		return depth;
	};

	depth = 0.999f;
	CHECK_CONDITION(std::abs(depth - Decode4(Encode4(depth))) < 0.03f);

	depth = 0.01265f;
	CHECK_CONDITION(std::abs(depth - Decode4(Encode4(depth))) < 0.03f);

}

static void Test02()
{
	auto value = Vector3(1, 0, 0).Dot(Vector3(0, 1, 0));
	CHECK_CONDITION(value <= 0);

	value = Vector3(1, 0, 0).Dot(Vector3(0.001f, 1, 0));
	CHECK_CONDITION(value > 0);

	value = Vector3(-1, 1, 0).Dot(Vector3(1, 1000, 0).Normalize());
	CHECK_CONDITION(value < 1);

	value = Vector3(-1, 1, 0).Dot(Vector3(-1, 1000, 0).Normalize());
	CHECK_CONDITION(value > 1);
}

static void Test03()
{
	Node node;
	node.SetPosition(Vector3(1, 2, 3));
	node.SetLocalLookAtPosition(Vector3(1, -2, 10));
	Vector4 pos(3, -4, -5, 1);
	auto newPos = node.GetGlobalModelMatrix() * pos;
	CHECK_CONDITION(newPos.w == 1.0);
	node.SetScale(Vector3(0.1f, 2.5, 0.45f));
	newPos = node.GetGlobalModelMatrix() * pos;
	CHECK_CONDITION(newPos.w == 1.0);
}

static void Test04()
{
	Camera camera;
	auto width = 100.f;
	auto height = 50.f;
	camera.SetAspectRatio(width / height);
	camera.SetOrthoScale(width);
	camera.EnableOrtho();
	camera.SetNearClip(0);
	camera.SetFarClip(100);

	auto pos = camera.GetViewProjection() * Vector4(0, 0, -50, 1);
	auto z = pos.z / pos.w;
	z = z * 0.5f + 0.5f;
	CHECK_CONDITION(z == 0.5f);
	pos = camera.GetViewProjection() * Vector4(100, 0, 50, 1);
	z = pos.z / pos.w;
	z = z * 0.5f + 0.5f;
	CHECK_CONDITION(z == -0.5f);
	pos = camera.GetViewProjection() * Vector4(100, 0, 9.9f, 1);
	z = pos.z / pos.w;
	z = z * 0.5f + 0.5f;
	CHECK_CONDITION(z < 0.f && z > -0.1f);
}

static void Test05()
{
	Camera camera;
	camera.SetFarClip(100);
	camera.SetPosition(Vector3(0, 0, -10));
	camera.SetGlobalLookAtDirection(Vector3(1, 0, 0));

	auto pos = camera.GetViewProjection() * Vector4(-10, 0, 1, 1);
	pos = camera.GetViewProjection() * Vector4(0, 0, 100, 1);
	pos = camera.GetViewProjection() * Vector4(0, 0, 101, 1);
}

static void Test06()
{
	auto q = Rotation(VECTOR3_UP, -VECTOR3_FORWARD);
	CHECK_CONDITION(-VECTOR3_FORWARD.Distance(q * VECTOR3_UP) < PRECISION);
}

void Tests()
{
	Test01();
	Test02();
	Test03();
	Test04();
	Test05();
	Test06();
}
