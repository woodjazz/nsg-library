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

    class  Frustum
    {
    public:
        Frustum();
        Frustum(const Camera* camera);
        Intersection IsSphereInside(const Vertex3& center, float radius) const
        {
            float fDistance;

            // calculate our distances to each of the planes
            for (int i = 0; i < 6; ++i)
            {
                const Plane& plane = planes_[i];
                fDistance = plane.Distance(center);

                if (fDistance < -radius) // if this distance is < -radius, we are outside
                    return Intersection::OUTSIDE;
                else if (fabs(fDistance) < radius) // else if the distance is between +- radius, then we intersect
                    return Intersection::INTERSECTS;
            }

            // otherwise we are fully in view
            return Intersection::INSIDE;
        }

        Intersection IsInside(const BoundingBox& box) const
        {
            int totalIn = 0;

            // get the corners of the box into the vCorner array
            Vector3 vCorner[8];
            box.GetVertices(vCorner);

            // test all 8 corners against the 6 sides
            // if all points are behind 1 specific plane, we are out
            // if we are in with all points, then we are fully in
            for (int p = 0; p < 6; ++p)
            {
                int inCount = 8;
                int ptIn = 1;

                for (int i = 0; i < 8; ++i)
                {
                    const Plane& plane = planes_[p];
                    // test this point against the planes
					if (plane.SideOfPlane(vCorner[i]) == Plane::Side::BEHIND)
                    {
                        ptIn = 0;
                        --inCount;
                    }
                }

                // were all the points outside of plane p?
                if (inCount == 0)
                    return Intersection::OUTSIDE;

                // check if they were all on the right side of the plane
                totalIn += ptIn;
            }

            // so if iTotalIn is 6, then all are inside the view
            if (totalIn == 6)
                return Intersection::INSIDE;

            // we must be partly in then otherwise
            return Intersection::INTERSECTS;
        }

        bool IsVisible(const Node& node, Mesh& mesh) const
        {
            if (mesh.IsReady())
            {
                Vertex3 center = node.GetGlobalPosition();
				float radius = mesh.GetBoundingSphereRadius();
				Vector3 scale = node.GetGlobalScale();
				float maxScale = std::max(std::max(scale.x, scale.y), scale.z);
				radius *= maxScale;

                Intersection sphereFrustumIntersec = IsSphereInside(center, radius);
                if(sphereFrustumIntersec == Intersection::INTERSECTS)
                {
                    BoundingBox bb = mesh.GetBB();
                    bb.Transform(node);
                    return IsInside(bb) != Intersection::OUTSIDE;
                }
                else 
                    return sphereFrustumIntersec !=  Intersection::OUTSIDE;
            }
            return false;
        }

        const Plane& GetPlane(FrustumPlane idx)
        {
            return planes_[idx];
        }

    private:
        void Define(const Vector3& nearPoint, const Vector3& farPoint, const Camera* camera);
        void UpdatePlanes();
        Plane planes_[FrustumPlane::MAX_PLANES];
    };

}
