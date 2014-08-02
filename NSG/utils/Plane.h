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
#pragma once
#include "Types.h"

namespace NSG
{
    struct  Plane
    {
		static const Plane UP;

		Vector3 normal_;
		Vector3 absNormal_;
		float d_;

        Plane();
        Plane(const Plane& plane);
        Plane(const Vector3& v0, const Vector3& v1, const Vector3& v2);
        Plane(const Vector3& normal, const Vector3& point);
        Plane(const Vector4& plane);
        void Define(const Vector3& v0, const Vector3& v1, const Vector3& v2);
        void Define(const Vector3& normal, const Vector3& point);
        Vector3 Project(const Vector3& point) const;
        float Distance(const Vector3& point) const;
        Vector3 Reflect(const Vector3& direction) const;
    };

}
