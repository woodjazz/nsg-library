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
#include "Camera.h"

namespace NSG
{
    Frustum::Frustum()
    {
    }

    Frustum::Frustum(const Camera* camera)
    {
		const Matrix4& VP = camera->GetMatViewProjection();
		const float* m = glm::value_ptr(VP);
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
    }
}
