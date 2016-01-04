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
#pragma once
#include "Types.h"
#include "Constants.h"

namespace NSG
{
	struct ICollision;
	class FollowCamera
	{
	public:
		FollowCamera(PCamera camera);
		~FollowCamera();
		void Track(ICollision* track, float distance);
		void SetAngle(float angle); // degrees
	private:
		void OnUpdate(float deltaTime);
		bool Obstruction(const Vector3& origin, const Vector3& targetPos, float radius) const;
		Vector3 GetBestTargetPoint(const Vector3& center, float radius) const;
		PCamera camera_;
		ICollision* track_;
		float distance_;
        SignalUpdate::PSlot slotUpdate_;
        float angle_;
        PWeakPhysicsWorld world_;
	};
}