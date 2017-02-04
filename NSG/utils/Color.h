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

namespace NSG
{
    struct Vector3;
    struct Vector4;
    struct Color
    {
        union { float x, r, s; };
        union { float y, g, t; };
        union { float z, b, p; };
        union { float w, a, v; };

        Color();
        Color(float v);
        Color(float r, float g, float b, float a = 1.f);
        Color(const Vector3& v);
        Color(const Vector4& v);
        Color(const Color& v);
        const float& operator[](int i) const;
        float& operator[](int i);
        static const Color White;
        static const Color Black;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;
        static const Color DodgerBlue;
        static const Color DarkOrange;
        static const Color PeachPuff;
        static const Color SkyBlue;
    };
    bool operator!=(const Color& v1, const Color& v2);
    bool operator==(const Color& v1, const Color& v2);
    Color operator*(const Color& v, float scalar);
    Color operator*(const Color& v1, const Color& v2);

}
