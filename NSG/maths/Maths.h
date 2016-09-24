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
#pragma once
#include "Constants.h"
namespace NSG
{
	inline bool Equals(float lhs, float rhs) { return lhs + EPSILON >= rhs && lhs - EPSILON <= rhs; }
	unsigned NextPowerOfTwo(unsigned value);
	void GetPowerOfTwoValues(int& width, int& height);
	bool IsPowerOfTwo(int value);
	float Clamp(float value, float minVal, float maxVal);
	int Clamp(int value, int minVal, int maxVal);
	float Radians(float degrees);
	float Degrees(float radians);
	float Distance(float a, float b);
	float Min(float a, float b);
	float Lerp(float a, float b, float t);
    float Mix(float a, float b, float t);
    float Abs(float value);
    float Cos(float value);
    float Floor(float value);
	float Pow2(float x);
	float Pow3(float x);
}
