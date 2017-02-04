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
#include "Quaternion.h"

namespace NSG
{
	class PointOnSphere
	{
	public:
		PointOnSphere();
		PointOnSphere(const Vertex3& center, float radius);
		PointOnSphere(const Vertex3& center, const Vertex3& pointInSphere);
		~PointOnSphere();
		bool SetCenter(const Vertex3& center);
		bool SetPoint(const Vertex3& pointInSphere);
		bool SetCenterAndPoint(const Vertex3& center, const Vertex3& pointInSphere);
		void IncAngles(float incTheta, float incPhi);
		void SetAngles(float theta, float phi);
		const Vector3& GetPoint() const { return point_; }
		const Vector3& GetUp() const { return up_; } // world up vector
		const Vertex3& GetCenter() const { return center_; }
		float GetTheta() const { return theta_; }
		float GetPhi() const { return phi_; }
		float GetRadius() const { return radius_; }
		Quaternion GetOrientation() const;
	private:
		Vector3 CalculatePoint();
		void CalculateUpVector(); // world up vector
		void CalculateAnglesAndRadius();
	private:
		Vertex3 center_;
		float radius_;
		float theta_;
		float phi_;
		Vertex3 point_; // point in the sphere's surface
		Vertex3 initialPoint_;
		Vector3 up_; //up vector for the camera
	};
}
