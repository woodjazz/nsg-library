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
#pragma once

namespace NSG {
struct Vector3;
struct Vector2 {
    float x, y;
    Vector2();
    Vector2(float a);
    Vector2(float a, float b);
    Vector2(const Vector2& v);
    Vector2(const Vector3& v);
    const float& operator[](int i) const;
    float& operator[](int i);
    const Vector2& operator/=(float v);
};
typedef Vector2 Vertex2;

bool operator!=(const Vector2& v1, const Vector2& v2);
bool operator==(const Vector2& v1, const Vector2& v2);
Vector2 operator+(const Vector2& v1, const Vector2& v2);
Vector2 operator-(const Vector2& v1, const Vector2& v2);
Vector2 operator+(const Vector2& v, float scalar);
Vector2 operator-(const Vector2& v, float scalar);
Vector2 operator*(const Vector2& v, float scalar);
Vector2 operator*(float scalar, const Vector2& v);
Vector2 operator/(const Vector2& v, float scalar);
}
