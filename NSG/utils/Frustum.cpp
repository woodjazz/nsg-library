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
#include "Frustum.h"
#include "Util.h"

namespace NSG
{
	Frustum::Frustum()
	{
		UpdatePlanes();
	}

    Frustum::Frustum(float fov, float aspectRatio, float nearZ, float farZ, const Matrix4& transform)
    {
        nearZ = std::max(nearZ, 0.0f);
        farZ = std::max(farZ, nearZ);
        float halfViewSize = tanf(glm::radians(fov));
        Vector3 nearPoint;
        Vector3 farPoint;

        nearPoint.z = nearZ;
        nearPoint.y = nearPoint.z * halfViewSize;
        nearPoint.x = nearPoint.y * aspectRatio;
        farPoint.z = farZ;
        farPoint.y = farPoint.z * halfViewSize;
        farPoint.x = farPoint.y * aspectRatio;

        Define(nearPoint, farPoint, transform);
    }

	Frustum::Frustum(const Matrix4& transform, float orthoSize, float aspectRatio, float nearZ, float farZ)
    {
        nearZ = std::max(nearZ, 0.0f);
        farZ = std::max(farZ, nearZ);
        float halfViewSize = orthoSize * 0.5f;
        Vector3 nearPoint;
        Vector3 farPoint;

        nearPoint.z = nearZ;
        farPoint.z = farZ;
        farPoint.y = nearPoint.y = halfViewSize;
        farPoint.x = nearPoint.x = nearPoint.y * aspectRatio;

        Define(nearPoint, farPoint, transform);
    }

	void Frustum::Define(const Vector3& nearPoint, const Vector3& farPoint, const Matrix4& transform)
    {
		vertices_[0] = Vector3(transform * Vector4(nearPoint, 1));
		vertices_[1] = Vector3(transform * Vector4(nearPoint.x, -nearPoint.y, nearPoint.z, 1));
		vertices_[2] = Vector3(transform * Vector4(-nearPoint.x, -nearPoint.y, nearPoint.z, 1));
		vertices_[3] = Vector3(transform * Vector4(-nearPoint.x, nearPoint.y, nearPoint.z, 1));
		vertices_[4] = Vector3(transform * Vector4(farPoint, 1));
		vertices_[5] = Vector3(transform * Vector4(farPoint.x, -farPoint.y, farPoint.z, 1));
		vertices_[6] = Vector3(transform * Vector4(-farPoint.x, -farPoint.y, farPoint.z, 1));
		vertices_[7] = Vector3(transform * Vector4(-farPoint.x, farPoint.y, farPoint.z, 1));

        UpdatePlanes();
    }

    void Frustum::UpdatePlanes()
    {
        planes_[PLANE_NEAR].Define(vertices_[2], vertices_[1], vertices_[0]);
        planes_[PLANE_LEFT].Define(vertices_[3], vertices_[7], vertices_[6]);
        planes_[PLANE_RIGHT].Define(vertices_[1], vertices_[5], vertices_[4]);
        planes_[PLANE_UP].Define(vertices_[0], vertices_[4], vertices_[7]);
        planes_[PLANE_DOWN].Define(vertices_[6], vertices_[5], vertices_[1]);
        planes_[PLANE_FAR].Define(vertices_[5], vertices_[6], vertices_[7]);

        // Check if we ended up with inverted planes (reflected transform) and flip in that case
        if (planes_[PLANE_NEAR].Distance(vertices_[5]) < 0.0f)
        {
            for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
            {
                planes_[i].normal_ = -planes_[i].normal_;
                planes_[i].d_ = -planes_[i].d_;
            }
        }

    }

}
