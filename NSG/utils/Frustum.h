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
    };

    static const unsigned NUM_FRUSTUM_PLANES = 6;
    static const unsigned NUM_FRUSTUM_VERTICES = 8;

    class  Frustum
    {
    public:
		Frustum();
        Frustum(float fov, float aspectRatio, float nearZ, float farZ, const Matrix4& transform);
		Frustum(const Matrix4& transform, float orthoSize, float aspectRatio, float nearZ, float farZ);
        void Define(float fov, float aspectRatio, float nearZ, float farZ, const Matrix4& transform);
        Intersection IsInside(const BoundingBox& box) const
        {
            Vector3 center = box.Center();
            Vector3 edge = center - box.min_;
            bool allInside = true;
            for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
            {
                const Plane& plane = planes_[i];
                float dist = glm::dot(plane.normal_, center) + plane.d_;
                float absDist = glm::dot(plane.absNormal_, edge);

                if (dist < -absDist)
					return Intersection::OUTSIDE;
                else if (dist < absDist)
                    allInside = false;
            }
			return allInside ? Intersection::INSIDE : Intersection::INTERSECTS;
        }

		bool IsVisible(const Node& node, Mesh& mesh) const
		{
			if (mesh.IsReady())
			{
				BoundingBox bb = mesh.GetBB();
				bb.Transform(node);
				return IsInside(bb) != Intersection::OUTSIDE;
			}
			return false;
		}

    private:
		void Define(const Vector3& nearPoint, const Vector3& farPoint, const Matrix4& transform);
        void UpdatePlanes();
        Plane planes_[NUM_FRUSTUM_PLANES];
        Vector3 vertices_[NUM_FRUSTUM_VERTICES];
    };

}
