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
#include "Frustum.h"
#include "Util.h"
#include "Camera.h"

namespace NSG
{
    Frustum::Frustum(const Matrix4& VP)
    : m_(VP)
    {
        Define();
    }

    void Frustum::Define()
    {
        const float* m = glm::value_ptr(m_);
        float t;

        Vector4& left = planes_[PLANE_LEFT].normald_;
        left[0] = m[3] + m[0];
        left[1] = m[7] + m[4];
        left[2] = m[11] + m[8];
        left[3] = m[15] + m[12];
        t = glm::length(Vector3(left));
        left /= t;


        Vector4& right = planes_[PLANE_RIGHT].normald_;
        right[0] = m[3] - m[0];
        right[1] = m[7] - m[4];
        right[2] = m[11] - m[8];
        right[3] = m[15] - m[12];
        t = glm::length(Vector3(right));
        right /= t;


        Vector4& down = planes_[PLANE_DOWN].normald_;
        down[0] = m[3] + m[1];
        down[1] = m[7] + m[5];
        down[2] = m[11] + m[9];
        down[3] = m[15] + m[13];
        t = glm::length(Vector3(down));
        down /= t;


        Vector4& up = planes_[PLANE_UP].normald_;
        up[0] = m[3] - m[1];
        up[1] = m[7] - m[5];
        up[2] = m[11] - m[9];
        up[3] = m[15] - m[13];
        t = glm::length(Vector3(up));
        up /= t;


        Vector4& nearP = planes_[PLANE_NEAR].normald_;
        nearP[0] = m[3] + m[2];
        nearP[1] = m[7] + m[6];
        nearP[2] = m[11] + m[10];
        nearP[3] = m[15] + m[14];
        t = glm::length(Vector3(nearP));
        nearP /= t;


        Vector4& farP = planes_[PLANE_FAR].normald_;
        farP[0] = m[3] - m[2];
        farP[1] = m[7] - m[6];
        farP[2] = m[11] - m[10];
        farP[3] = m[15] - m[14];
        t = glm::length(Vector3(farP));
        farP /= t;

        const Plane& Near = planes_[PLANE_NEAR];
        const Plane& Far = planes_[PLANE_FAR];
        const Plane& Left = planes_[PLANE_LEFT];
        const Plane& Right = planes_[PLANE_RIGHT];
        const Plane& Top = planes_[PLANE_UP];
        const Plane& Bottom = planes_[PLANE_DOWN];

        vertices_[0] = IntersectionPoint(Near, Left, Bottom);
        vertices_[1] = IntersectionPoint(Near, Right, Bottom);
        vertices_[2] = IntersectionPoint(Near, Right, Top);
        vertices_[3] = IntersectionPoint(Near, Left, Top);

        vertices_[4] = IntersectionPoint(Far, Left, Bottom);
        vertices_[5] = IntersectionPoint(Far, Right, Bottom);
        vertices_[6] = IntersectionPoint(Far, Right, Top);
        vertices_[7] = IntersectionPoint(Far, Left, Top);
    }

    std::vector<Vector3> Frustum::GetVerticesTransform(const Matrix4& m) const
    {
        std::vector<Vector3> result;
        for(int i=0; i<NUM_FRUSTUM_VERTICES; i++)
            result.push_back(Vector3(m * Vector4(vertices_[i], 1)));
        return result;
    }

    Intersection Frustum::IsPointInside(const Vector3& point) const
    {
		for (int i = 0; i < FrustumPlane::MAX_PLANES; i++)
            if (planes_[i].Distance(point) < 0.f)
                return Intersection::OUTSIDE;
        return Intersection::INSIDE;
    }

    Intersection Frustum::IsSphereInside(const Vertex3& center, float radius) const
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

    Intersection Frustum::IsInside(const BoundingBox& box) const
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

    bool Frustum::IsVisible(const Node& node, Mesh& mesh) const
    {
        if (mesh.IsReady())
        {
            Vertex3 center = node.GetGlobalPosition();
            float radius = mesh.GetBoundingSphereRadius();
            Vector3 scale = node.GetGlobalScale();
            float maxScale = std::max(std::max(scale.x, scale.y), scale.z);
            radius *= maxScale;

            Intersection sphereFrustumIntersec = IsSphereInside(center, radius);
            if (sphereFrustumIntersec == Intersection::INTERSECTS)
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

    Vector3 Frustum::IntersectionPoint(const Plane& a, const Plane& b, const Plane& c)
    {
        Vector3 n1(a.GetNormalD());
        Vector3 n2(b.GetNormalD());
        Vector3 n3(c.GetNormalD());
        float d1(a.GetNormalD().w);
        float d2(b.GetNormalD().w);
        float d3(c.GetNormalD().w);

        float f = -glm::dot(n1, glm::cross(n2, n3));

        Vector3 v1 = d1 * glm::cross(n2, n3);
        Vector3 v2 = d2 * glm::cross(n3, n1);
        Vector3 v3 = d3 * glm::cross(n1, n2);

        return (v1 + v2 + v3) / f;
    }
}
