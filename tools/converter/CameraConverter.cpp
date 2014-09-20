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

#include "CameraConverter.h"
#include "TextureFile.h"
#include "Check.h"
#include "Util.h"
#include "Constants.h"
#include "Pass.h"
#include "Technique.h"
#include "assimp/camera.h"

namespace NSG
{
	CameraConverter::CameraConverter(const aiCamera* camera, Scene* scene) 
		: Camera(camera->mName.C_Str(), scene)
	{
		Vector3 lookAt(camera->mLookAt.x, camera->mLookAt.y, camera->mLookAt.z);
		Vector3 up(camera->mUp.x, camera->mUp.y, camera->mUp.z);
		SetLookAt(lookAt, up);
		SetPosition(Vector3(camera->mPosition.x, camera->mPosition.y, camera->mPosition.z));
		SetNearClip(camera->mClipPlaneNear);
		SetFarClip(camera->mClipPlaneFar);
		SetHalfHorizontalFov(camera->mHorizontalFOV);
	}

	CameraConverter::~CameraConverter()
	{

	}
}
