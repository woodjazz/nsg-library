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
#include "PointOnSphere.h"
#include "Check.h"
#include "Util.h"
#include "Maths.h"
#include <cmath>

namespace NSG
{
    PointOnSphere::PointOnSphere()
        : center_(0),
          radius_(0),
          theta_(0),
          phi_(0),
          up_(Vector3::Up)
    {
    }

    PointOnSphere::PointOnSphere(const Vertex3& center, float radius)
        : center_(center),
          radius_(radius),
          theta_(0),
          phi_(0),
          up_(Vector3::Up)
    {
        initialPoint_ = point_ = CalculatePoint();
        CalculateUpVector();
    }

    PointOnSphere::PointOnSphere(const Vertex3& center, const Vertex3& pointInSphere)
        : center_(center),
          radius_(center.Distance(pointInSphere)),
          theta_(0),
          phi_(0)
    {
        SetPoint(pointInSphere);
		initialPoint_ = point_;
    }

    PointOnSphere::~PointOnSphere()
    {

    }

    void PointOnSphere::IncAngles(float incTheta, float incPhi)
    {
        SetAngles(theta_ + incTheta, phi_ + incPhi);
    }

    void PointOnSphere::SetAngles(float theta, float phi)
    {
        theta_ = theta;
        phi_ = phi;

        point_ = CalculatePoint();
        CalculateUpVector();
    }

    bool PointOnSphere::SetCenterAndPoint(const Vertex3& center, const Vertex3& pointInSphere)
    {
        if (pointInSphere.Distance(center) > PRECISION)
        {
            if (center.Distance(center_) > PRECISION || pointInSphere.Distance(point_) > PRECISION)
            {
                center_ = center;
                point_ = pointInSphere;
                CalculateAnglesAndRadius();
                CalculateUpVector();
                return true;
            }
        }

        return false;
    }

    bool PointOnSphere::SetCenter(const Vertex3& center)
    {
        if (center.Distance(point_) > PRECISION)
        {
            if (center.Distance(center_) > PRECISION)
            {
                center_ = center;
                CalculateAnglesAndRadius();
                CalculateUpVector();
                return true;
            }
        }

        return false;
    }

    bool PointOnSphere::SetPoint(const Vertex3& pointInSphere)
    {
        if (pointInSphere.Distance(center_) > PRECISION)
        {
            if (pointInSphere.Distance(point_) > PRECISION)
            {
                point_ = pointInSphere;
                CalculateAnglesAndRadius();
                CalculateUpVector();
                return true;
            }
        }
        return false;
    }

    void PointOnSphere::CalculateAnglesAndRadius()
    {
        Vector3 rn = point_ - center_;
        radius_ = rn.Length();
        rn = rn.Normalize();

        //calculate phi in order to be in the given point
        float dy = rn.y;
        phi_ = acos(dy);

        //calculate theta in order to be in the given point
        float dx = rn.x;
        float dz = rn.z;

        if (Abs(dx) < EPSILON)
        {
            if (dz < 0)
                theta_ = -PI / 2;
            else
                theta_ = PI / 2;
        }
        else
        {
            theta_ = atan(dz / dx);

            if (dx < 0)
                theta_ += PI;
        }

        //CHECK_ASSERT(Distance(center_ + radius_ * Vertex3(cos(theta_) * sin(phi_), cos(phi_), sin(theta_) * sin(phi_)), point_) < 9 * PRECISION);
    }


    Vector3 PointOnSphere::CalculatePoint()
    {
        // Apply spherical coordinates
        return center_ + radius_ * Vertex3(cos(theta_) * sin(phi_), cos(phi_), sin(theta_) * sin(phi_));
    }

    void PointOnSphere::CalculateUpVector()
    {
        // Apply spherical coordinates
        Vertex3 currentPoint(cos(theta_) * sin(phi_), cos(phi_), sin(theta_) * sin(phi_));

        CHECK_ASSERT((center_ + radius_ * currentPoint).Distance(point_) < 9 * PRECISION);

        // Reduce phi slightly to obtain another point on the same longitude line on the sphere.
        auto newPhi = phi_ - 1;
        Vertex3 newUpPoint(cos(theta_) * sin(newPhi), cos(newPhi), sin(theta_) * sin(newPhi));

        up_ = (newUpPoint - currentPoint).Normalize();
    }

    Quaternion PointOnSphere::GetOrientation() const
    {
		return Rotation((initialPoint_ - center_).Normalize(), (point_ - center_).Normalize());
    }
}
