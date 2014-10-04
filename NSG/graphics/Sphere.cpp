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
#include "Sphere.h"
#include "Constants.h"

namespace NSG
{
	Sphere::Sphere()
	: center_(0),
	radius_(0),
	theta_(0),
	phi_(0)
	{

	}

    Sphere::Sphere(const Vertex3& center, float radius)
        : center_(center),
          radius_(radius),
          theta_(0),
          phi_(0)
    {
        CalculatePosition();
        CalculateUpVector();
    }

    Sphere::Sphere(const Vertex3& center, const Vertex3& pointInSphere)
        : center_(center),
          radius_(glm::distance(center, pointInSphere)),
          theta_(0),
          phi_(0)
    {
        SetPosition(pointInSphere);
    }

    Sphere::~Sphere()
    {

    }

    void Sphere::IncAngles(float incTheta, float incPhi)
    {
        SetAngles(theta_ + incTheta, phi_ + incPhi);
    }

    void Sphere::SetAngles(float theta, float phi)
    {
        theta_ = theta;
        phi_ = phi;

        CalculatePosition();
        CalculateUpVector();
    }

    bool Sphere::SetCenterAndPosition(const Vertex3& center, const Vertex3& pointInSphere)
    {
		if (glm::distance(pointInSphere, center) > PRECISION)
    	{
    		center_ = center;
    		position_ = pointInSphere;
    		CalculateAnglesAndRadius();
            CalculateUpVector();
			return true;
    	}

		return false;
    }

    bool Sphere::SetCenter(const Vertex3& center)
    {
		if (glm::distance(center, position_) > PRECISION)
        {
            center_ = center;
            CalculateAnglesAndRadius();
            CalculateUpVector();
			return true;
        }

		return false;
    }

    bool Sphere::SetPosition(const Vertex3& pointInSphere)
    {
		if (glm::distance(pointInSphere, center_) > PRECISION)
        {
            position_ = pointInSphere;
            CalculateAnglesAndRadius();
            CalculateUpVector();
			return true;
        }
		return false;
    }

    void Sphere::CalculateAnglesAndRadius()
    {
        Vector3 rn = position_ - center_;
        radius_ = glm::length(rn);
		rn = glm::normalize(rn);

        //calculate phi in order to be in the given point
        float dy = rn.y;
        phi_ = acos(dy);

        //calculate theta in order to be in the given point
        float dx = rn.x;
        float dz = rn.z;

        theta_ = atan(dz / dx);

        if (dx < 0)
            theta_ += PI;

		CHECK_ASSERT(glm::distance(center_ + radius_ * Vertex3(cos(theta_) * sin(phi_), cos(phi_), sin(theta_) * sin(phi_)), position_) < 9 * PRECISION, __FILE__, __LINE__);
    }


    void Sphere::CalculatePosition()
    {
        // Apply spherical coordinates
		position_ = center_ + radius_ * Vertex3(cos(theta_) * sin(phi_), cos(phi_), sin(theta_) * sin(phi_));
    }

    void Sphere::CalculateUpVector()
    {
        // Apply spherical coordinates
		Vertex3 currentPoint(cos(theta_) * sin(phi_), cos(phi_), sin(theta_) * sin(phi_));

		CHECK_ASSERT(glm::distance(center_ + radius_ * currentPoint, position_) < 9*PRECISION, __FILE__, __LINE__);

        // Reduce theta slightly to obtain another point on the same longitude line on the sphere.
        const float dt = 1;
        Vertex3 newUpPoint(cos(theta_) * sin(phi_ - dt), cos(phi_ - dt), sin(theta_) * sin(phi_ - dt));

		up_ = glm::normalize(newUpPoint - currentPoint);
    }


}