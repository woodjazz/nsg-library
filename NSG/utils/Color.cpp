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
#include "Color.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Check.h"
#include <cmath>
namespace NSG
{
    const Color Color::White(1, 1, 1, 1);
    const Color Color::Black(0, 0, 0, 1);
    const Color Color::Red(1, 0, 0, 1);
    const Color Color::Green(0, 1, 0, 1);
    const Color Color::Blue(0, 0, 1, 1);
    const Color Color::Yellow(1, 1, 0, 1);
    const Color Color::DodgerBlue(30/255.f, 144/255.f, 1, 1);
    const Color Color::DarkOrange(1.f, 140 / 255.f, 0, 1);
    const Color Color::PeachPuff(1.f, 218 / 255.f, 185 / 255.f, 1);
    const Color Color::SkyBlue(135 / 255.f, 206 / 255.f, 235 / 255.f, 1);

    Color::Color()
        : r(0), g(0), b(0), a(0)
    {
    }

    Color::Color(float v)
        : r(v), g(v), b(v), a(1.f)
    {
    }

    Color::Color(float r, float g, float b, float a)
        : x(r), y(g), z(b), w(a)
    {
    }

    Color::Color(const Vector3& v)
        : x(v.x), y(v.y), z(v.z), w(1)
    {
    }

    Color::Color(const Vector4& v)
        : x(v.x), y(v.y), z(v.z), w(v.w)
    {
    }

    Color::Color(const Color& v)
        : x(v.x), y(v.y), z(v.z), w(v.w)
    {
    }

    const float& Color::operator[](int i) const
    {
        CHECK_ASSERT(i >= 0 && i < 4);
        return (&x)[i];
    }

    float& Color::operator[](int i)
    {
        CHECK_ASSERT(i >= 0 && i < 4);
        return (&x)[i];
    }

    bool operator!=(const Color& v1, const Color& v2)
    {
        return (v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z) || (v1.w != v2.w);
    }

    bool operator==(const Color& v1, const Color& v2)
    {
        return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z) && (v1.w == v2.w);
    }

    Color operator*(const Color& v, float scalar)
    {
        return Color(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
    }

    Color operator*(const Color& v1, const Color& v2)
    {
        return Color(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
    }

}
