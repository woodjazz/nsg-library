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
#include "Vector2.h"
#include "Util.h"
#include "Check.h"
#include <cmath>
namespace NSG
{
    Vector2::Vector2()
        : x(0), y(0)
    {
    }

    Vector2::Vector2(float a)
        : x(a), y(a)
    {
    }

    Vector2::Vector2(float a, float b)
        : x(a), y(b)
    {
    }

    Vector2::Vector2(const Vector2& v)
        : x(v.x), y(v.y)
    {
    }

    Vector2::Vector2(const Vector3& v)
        : x(v.x), y(v.y)
    {
    }

    const float& Vector2::operator[](int i) const
    {
        CHECK_ASSERT(i >= 0 && i < 2);
        return (&x)[i];
    }

    float& Vector2::operator[](int i)
    {
        CHECK_ASSERT(i >= 0 && i < 2);
        return (&x)[i];
    }

    const Vector2& Vector2::operator/=(float v)
    {
        x /= v;
        y /= v;
        return *this;
    }

    bool operator!=(const Vector2& v1, const Vector2& v2)
    {
        return (v1.x != v2.x) || (v1.y != v2.y);
    }

    bool operator==(const Vector2& v1, const Vector2& v2)
    {
        return (v1.x == v2.x) && (v1.y == v2.y);
    }

    Vector2 operator+(const Vector2& v1, const Vector2& v2)
    {
        return Vector2(v1.x + v2.x, v1.y + v2.y);   
    }

    Vector2 operator-(const Vector2& v1, const Vector2& v2)
    {
        return Vector2(v1.x - v2.x, v1.y - v2.y);   
    }

    Vector2 operator+(const Vector2& v, float scalar)
    {
        return Vector2(v.x + scalar, v.y + scalar);
    }

    Vector2 operator-(const Vector2& v, float scalar)
    {
        return Vector2(v.x - scalar, v.y - scalar);
    }

    Vector2 operator*(const Vector2& v, float scalar)
    {
        return Vector2(v.x * scalar, v.y * scalar);
    }

    Vector2 operator*(float scalar, const Vector2& v)
    {
        return Vector2(v.x * scalar, v.y * scalar);
    }

    Vector2 operator/(const Vector2& v, float scalar)
    {
        return Vector2(v.x / scalar, v.y / scalar);
    }

}
