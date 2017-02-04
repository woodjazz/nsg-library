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
#include "Maths.h"
#include <cmath>
#include <algorithm>

namespace NSG
{
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

	float Min(float a, float b)
	{
		return std::min(a, b);
	}

	float Lerp(float a, float b, float t)
	{
		return a * (1.0f - t) + b * t;
	}

	float Mix(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	float Clamp(float value, float minVal, float maxVal)
	{
		return std::min(std::max(value, minVal), maxVal);
	}

	int Clamp(int value, int minVal, int maxVal)
	{
		return std::min(std::max(value, minVal), maxVal);
	}

	float Radians(float degrees)
	{
		return degrees * DEG2RAD;
	}

	float Degrees(float radians)
	{
		return radians * RAD2DEG;
	}

	float Distance(float a, float b)
	{
		return std::abs(b - a);
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

	float Pow2(float x)
	{
		return x * x;
	}

	float Pow3(float x)
	{
		return x * x * x;
	}
}
