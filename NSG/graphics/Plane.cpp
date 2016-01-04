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
#include "Plane.h"
#include "Constants.h"
#include "Util.h"
#include "Check.h"

namespace NSG
{
    const Plane Plane::UP(WORLD_Y_COORD, Vector3(0));

    Plane::Plane()
    {
    }

    Plane::Plane(const Plane& plane) :
        normald_(plane.normald_)
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
		CHECK_ASSERT(NSG::Distance(v0, v1) > 0.0001f);
		CHECK_ASSERT(NSG::Distance(v0, v2) > 0.0001f);

        Vector3 dist1 = v1 - v0;
        Vector3 dist2 = v2 - v0;
        Define(Normalize(Cross(dist1, dist2)), v1);
    }

    void Plane::Define(const Vector3& normal, const Vector3& point)
    {
        normald_ = Vector4(normal, -Dot(normal, point));
    }


    float Plane::Distance(const Vector3& point) const
    {
        return Dot(Vector3(normald_), point) + normald_.w;
    }

    Plane::Side Plane::SideOfPlane(const Vector3& point) const
    {
        float dotValue = Distance(point);

        if (dotValue > 0)
            return Side::INFRONT;
        else if (dotValue < 0)
            return Side::BEHIND;
        return Side::INPLANE;
    }
}
