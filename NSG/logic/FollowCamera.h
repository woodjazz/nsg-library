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

namespace NSG
{
	class FollowCamera
	{
	public:
		FollowCamera(PCamera camera);
		~FollowCamera();
		void Track(PNode node);
		void SetOffset(const Vector3& offset);
	private:
		Vector3 GetDisplacementToAvoidObstruction(const Vector3& dir2Target, const Vector3& hitPoint, const Vector3& hitNormal);
		void OnUpdate(float deltaTime);
		PCamera camera_;
		PNode trackNode_;
		Vector3 offset_;
		SignalUpdate::PSlot slotUpdate_;
	};
}