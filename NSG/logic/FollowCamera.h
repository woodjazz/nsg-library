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
#pragma once
#include "Types.h"
#include "Constants.h"

namespace NSG
{
	class FollowCamera
	{
	public:
		FollowCamera(PCamera camera);
		~FollowCamera();
		void Track(PSceneNode track, float distance);
		void SetCameraPhysicsRadius(float radius);
		void SetAngle(float angle); // degrees
	private:
		Vector3 GetDisplacementToAvoidObstruction(const Vector3& dir2Target, const Vector3& hitNormal) const;
		void OnUpdate(float deltaTime);
		bool Obstruction(const Vector3& origin, const Vector3& targetPos) const;
		Vector3 GetBestTargetPoint() const;
		PCamera camera_;
		PSceneNode track_;
		float distance_;
        SignalUpdate::PSlot slotUpdate_;
        float phyRadius_;
        float angle_;
        PPhysicsWorld world_;
	};
}