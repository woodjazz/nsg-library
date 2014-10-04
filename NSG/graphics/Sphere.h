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

namespace NSG
{
	class Sphere
	{
	public:
		Sphere();
		Sphere(const Vertex3& center, float radius);
		Sphere(const Vertex3& center, const Vertex3& pointInSphere);
		~Sphere();
		bool SetCenter(const Vertex3& center);
		bool SetPosition(const Vertex3& pointInSphere);
		bool SetCenterAndPosition(const Vertex3& center, const Vertex3& pointInSphere);
		void IncAngles(float incTheta, float incPhi);
		void SetAngles(float theta, float phi);
		const Vector3& GetPosition() const { return position_; }
		const Vector3& GetUp() const { return up_; }
		const Vertex3& GetCenter() const { return center_; }
	private:
		void CalculatePosition();
		void CalculateUpVector();
		void CalculateAnglesAndRadius();
	private:
		Vertex3 center_;
		float radius_;
		float theta_;
		float phi_;
		Vertex3 position_; // point in the sphere's surface
		Vector3 up_; //up vector for the camera
	};
}