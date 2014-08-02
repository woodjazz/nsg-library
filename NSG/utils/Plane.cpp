/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

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
#include "Plane.h"

namespace NSG
{
    const Plane Plane::UP(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));

    Plane::Plane() :
        d_(0.0f)
    {
    }

    Plane::Plane(const Plane& plane) :
        normal_(plane.normal_),
        absNormal_(plane.absNormal_),
        d_(plane.d_)
    {
    }

    Plane::Plane(const Vector3& v0, const Vector3& v1, const Vector3& v2)
    {
        Define(v0, v1, v2);
    }

    Plane::Plane(const Vector3& normal, const Vector3& point)
    {
        Define(normal, point);
    }

    void Plane::Define(const Vector3& v0, const Vector3& v1, const Vector3& v2)
    {
        Vector3 dist1 = v1 - v0;
        Vector3 dist2 = v2 - v0;
		Define(glm::cross(dist1, dist2), v0);
    }

    void Plane::Define(const Vector3& normal, const Vector3& point)
    {
        normal_ = glm::normalize(normal);
        absNormal_ = glm::abs(normal_);
        d_ = -glm::dot(normal_, point);
    }

    Vector3 Plane::Project(const Vector3& point) const
    {
        return point - normal_ * (glm::dot(normal_, point) + d_);
    }

    float Plane::Distance(const Vector3& point) const
    {
        return glm::dot(normal_, point) + d_;
    }

    Vector3 Plane::Reflect(const Vector3& direction) const
    {
        return direction - (2.0f * glm::dot(normal_, direction) * normal_);
    }
}
