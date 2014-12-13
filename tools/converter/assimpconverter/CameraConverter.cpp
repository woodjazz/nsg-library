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
#include "Texture.h"
#include "Check.h"
#include "Util.h"
#include "Constants.h"
#include "Pass.h"
#include "Technique.h"
#include "App.h"
#include "Scene.h"
#include "Camera.h"
#include "assimp/camera.h"

namespace NSG
{
	CameraConverter::CameraConverter(const aiCamera* camera, Camera* cameraNode)
	{
		Vector3 lookAt(camera->mLookAt.x, camera->mLookAt.y, camera->mLookAt.z);
		Vector3 up(camera->mUp.x, camera->mUp.y, camera->mUp.z);
		Node node;
		node.SetLocalLookAt(lookAt, up);
		cameraNode->Rotate(node.GetOrientation());
		cameraNode->Translate(Vector3(camera->mPosition.x, camera->mPosition.y, camera->mPosition.z));
		cameraNode->SetNearClip(camera->mClipPlaneNear);
		cameraNode->SetFarClip(camera->mClipPlaneFar);
		cameraNode->SetHalfHorizontalFov(camera->mHorizontalFOV);
	}

	CameraConverter::~CameraConverter()
	{

	}
}
