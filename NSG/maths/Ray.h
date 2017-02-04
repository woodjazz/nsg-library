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
#include "Types.h"
#include "Vector3.h"

namespace NSG
{
    struct Matrix4;
    class Ray
    {
    public:
		Ray(const Vertex3& origin, const Vector3& direction, float maxDistance = std::numeric_limits<float>::max());
        Ray(const Ray& ray);
        ~Ray();
		Ray& operator = (const Ray& rhs);
        bool operator == (const Ray& rhs) const;
        bool operator != (const Ray& rhs) const;
		float HitDistance(const Vector3& v0, const Vector3& v1, const Vector3& v2, Vector3* outNormal = nullptr) const;
        float HitDistance(const BoundingBox& box) const;
        Intersection IsInside(const BoundingBox& box) const;
        float HitDistance(const SceneNode* node) const;
        float GetMaxDistance() const { return maxDistance_; }
        Vertex3 GetPoint(float distance) const;
        const Vector3& GetDirection() const { return direction_; }
        const Vertex3& GetOrigin() const { return origin_; }
        Ray Transformed(const Matrix4& transform) const;
    private:
        Vertex3 origin_;
        Vector3 direction_;
        float maxDistance_;
    };
}
