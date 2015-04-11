/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "BoundingBox.h"
#include "Plane.h"
#include "Mesh.h"
#include <algorithm>
namespace NSG
{
    enum FrustumPlane
    {
        PLANE_NEAR = 0,
        PLANE_LEFT,
        PLANE_RIGHT,
        PLANE_UP,
        PLANE_DOWN,
        PLANE_FAR,
        MAX_PLANES
    };

    class Frustum
    {
    public:
        Frustum(const Camera* camera);
        Intersection IsSphereInside(const Vertex3& center, float radius) const;
        Intersection IsInside(const BoundingBox& box) const;
        bool IsVisible(const Node& node, Mesh& mesh) const;
		const Plane& GetPlane(FrustumPlane idx) { return planes_[idx]; }
    private:
        Plane planes_[FrustumPlane::MAX_PLANES];
    };

}
