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
#include "Sphere.h"
#include "Mesh.h"
#include "SceneNode.h"
#include "Check.h"

namespace NSG
{
    Sphere::Sphere(const Vector3& center, float radius)
        : center_(center),
          radius_(radius)
    {
    }

    Sphere::Sphere(const SceneNode& sceneNode)
    {
        auto mesh = sceneNode.GetMesh();
        CHECK_CONDITION(mesh && mesh->IsReady());
        center_ = sceneNode.GetGlobalPosition();
        radius_ = mesh->GetBoundingSphereRadius();
        Vector3 scale = sceneNode.GetGlobalScale();
        float maxScale = std::max(std::max(scale.x, scale.y), scale.z);
        radius_ *= maxScale;
    }

    Intersection Sphere::IsInside(const Sphere& sphere) const
    {
        float dist = Length(sphere.center_ - center_);
        if (dist >= sphere.radius_ + radius_)
            return Intersection::OUTSIDE;
        else if (dist + sphere.radius_ < radius_)
            return Intersection::INSIDE;
        else
            return Intersection::INTERSECTS;
    }

    Intersection Sphere::IsInside(const BoundingBox& box) const
    {
        float radiusSquared = radius_ * radius_;
        float distSquared = 0;
        float temp;
        Vector3 min = box.min_;
        Vector3 max = box.max_;

        if (center_.x < min.x)
        {
            temp = center_.x - min.x;
            distSquared += temp * temp;
        }
        else if (center_.x > max.x)
        {
            temp = center_.x - max.x;
            distSquared += temp * temp;
        }
        if (center_.y < min.y)
        {
            temp = center_.y - min.y;
            distSquared += temp * temp;
        }
        else if (center_.y > max.y)
        {
            temp = center_.y - max.y;
            distSquared += temp * temp;
        }
        if (center_.z < min.z)
        {
            temp = center_.z - min.z;
            distSquared += temp * temp;
        }
        else if (center_.z > max.z)
        {
            temp = center_.z - max.z;
            distSquared += temp * temp;
        }

        if (distSquared >= radiusSquared)
            return Intersection::OUTSIDE;

        min -= center_;
        max -= center_;

        Vector3 tempVec = min; // - - -
        if (Length2(tempVec) >= radiusSquared)
            return Intersection::INTERSECTS;
        tempVec.x = max.x; // + - -
        if (Length2(tempVec) >= radiusSquared)
            return Intersection::INTERSECTS;
        tempVec.y = max.y; // + + -
        if (Length2(tempVec) >= radiusSquared)
            return Intersection::INTERSECTS;
        tempVec.x = min.x; // - + -
        if (Length2(tempVec) >= radiusSquared)
            return Intersection::INTERSECTS;
        tempVec.z = max.z; // - + +
        if (Length2(tempVec) >= radiusSquared)
            return Intersection::INTERSECTS;
        tempVec.y = min.y; // - - +
        if (Length2(tempVec) >= radiusSquared)
            return Intersection::INTERSECTS;
        tempVec.x = max.x; // + - +
        if (Length2(tempVec) >= radiusSquared)
            return Intersection::INTERSECTS;
        tempVec.y = max.y; // + + +
        if (Length2(tempVec) >= radiusSquared)
            return Intersection::INTERSECTS;

        return Intersection::INSIDE;
    }

}