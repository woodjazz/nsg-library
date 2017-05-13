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
#include "Vector3.h"
#include "Vector4.h"
#include <vector>

namespace NSG {
class Plane {
public:
    static const Plane UP;
    enum class Side { INPLANE, BEHIND, INFRONT };
    Plane();
    Plane(const Plane& plane);
    Plane(const Vector3& v0, const Vector3& v1, const Vector3& v2);
    Plane(const Vector3& normal, const Vector3& point);
    Plane(const Vector4& plane);
    void Define(const Vector3& v0, const Vector3& v1, const Vector3& v2);
    void Define(const Vector3& normal, const Vector3& point);
    float Distance(const Vector3& point) const;
    Side SideOfPlane(const Vector3& point) const;
    const Vector4& GetNormalD() const { return normald_; }
    Vector4& GetNormalDRef() { return normald_; }
    Vector3 GetNormal() const {
        return Vector3(normald_.x, normald_.y, normald_.z);
    }

private:
    Vector4 normald_;
};
}
