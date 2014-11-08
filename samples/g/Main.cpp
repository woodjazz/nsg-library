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

#include "NSG.h"
using namespace NSG;

struct Sample : App
{
    PScene scene_;

    Sample()
    {
        //AppConfiguration::this_->showStatistics_ = true;
    }


    void Start(int argc, char* argv[]) override
    {
		scene_ = GetCurrentScene();
        //scene_->SetAmbientColor(Color(0, 0, 0.7f, 1));

		PResourceFile resource(GetOrCreateResourceFile("data/scene.xml"));
		scene_->Load(resource);

    }

	void OnSceneLoaded() override
	{
		auto& cameras = scene_->GetCameras();
		PCamera camera = cameras[0];

		camera->AddBehavior(PCameraControl(new CameraControl));

		camera->Activate();
		camera->SetGlobalPosition(Vector3(0, 5, 5));
		camera->SetLookAt(VECTOR3_ZERO);

		PTexture lightmapTexture(GetOrCreateTextureFile("data/lightmap.png"));
		PTexture diffuseTexture(GetOrCreateTextureFile("data/MetalFloorsBare_COLOR.png"));
		PTexture normalTexture(GetOrCreateTextureFile("data/MetalFloorsBare_NRM.png"));
		PTexture specularTexture(GetOrCreateTextureFile("data/MetalFloorsBare_SPEC.png"));
		PTexture occTexture(GetOrCreateTextureFile("data/MetalFloorsBare_OCC.png"));
		PTexture dispTexture(GetOrCreateTextureFile("data/MetalFloorsBare_DISP.png"));

		PSceneNode obj = scene_->GetChild<SceneNode>("Cube", true);
        obj->SetDiffuseMap(diffuseTexture, true);
        obj->SetLightMap(lightmapTexture, true);
		obj->SetNormalMap(normalTexture, true);
		obj->SetSpecularMap(specularTexture, true);
		obj->SetAOMap(occTexture, true);
		obj->SetDisplacementMap(dispTexture, true);

	}
};

NSG_MAIN(Sample);
