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
using namespace NSG;

static void Test01()
{
	auto Encode = [](float depth) -> Vector3
	{
        depth = 0.5f * depth + 0.5f;
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
	    auto ret = glm::dot(depth, dotValues);
        ret = 2.0 * (ret - 0.5);
		return ret;
	};

	float depth = -0.765f;
	CHECK_CONDITION(std::abs(depth - Decode(Encode(depth))) < 0.0001f, __FILE__, __LINE__);
    
    depth = 0.765f;
    CHECK_CONDITION(std::abs(depth - Decode(Encode(depth))) < 0.0001f, __FILE__, __LINE__);

	depth = -0.01265f;
	CHECK_CONDITION(std::abs(depth - Decode(Encode(depth))) < 0.0001f, __FILE__, __LINE__);
    
    depth = 0.01265f;
    CHECK_CONDITION(std::abs(depth - Decode(Encode(depth))) < 0.0001f, __FILE__, __LINE__);

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

	depth = -1;
	CHECK_CONDITION(std::abs(depth - Decode1(Encode1(depth))) < 0.0001f, __FILE__, __LINE__);

	depth = -0.01265f;
	CHECK_CONDITION(std::abs(depth - Decode1(Encode1(depth))) < 0.0001f, __FILE__, __LINE__);

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
	    auto ret = glm::dot(depth, dotValues);
		return ret;
	};

	depth = 0.765f;
	CHECK_CONDITION(std::abs(depth - Decode2(Encode2(depth))) < 0.0001f, __FILE__, __LINE__);

	depth = 0.01265f;
	CHECK_CONDITION(std::abs(depth - Decode2(Encode2(depth))) < 0.0001f, __FILE__, __LINE__);
}

void Tests()
{
	Test01();
}
