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
#include "BScene.h"
#include "BlenderDefines.h"
#include "UtilConverter.h"
#include "Check.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Camera.h"
#include <vector>

namespace BlenderConverter
{
    using namespace NSG;
    BScene::BScene(const Blender::Scene* sc)
        : sc_(sc),
		scene_(new Scene(B_IDNAME(sc)))
    {
        const Blender::World* world = sc->world;
        Color ambient(world->ambr, world->ambg, world->ambb, 1);
        scene_->SetAmbientColor(ambient);

        std::vector<const Blender::Object*> armatureLinker;
        for (Blender::Base* base = (Blender::Base*)sc->base.first; base; base = base->next)
        {
            if (!base->object)
                continue;

            const Blender::Object* obj = base->object;

            if (obj->type == OB_MESH && obj->parent != 0 && obj->parent->type == OB_ARMATURE)
                armatureLinker.push_back(obj);

            ConvertObject(obj);
        }
    }

    BScene::~BScene()
    {
    }

    void BScene::ConvertObject(const Blender::Object* obj)
    {
        switch (obj->type)
        {
            case OB_EMPTY:      // SceneNode
                ConvertSceneNode(obj, scene_->CreateChild<SceneNode>(B_IDNAME(obj)));
                break;
            case OB_LAMP:       // Light
                break;
            case OB_CAMERA:     // Camera
				ConvertCamera(obj, scene_->CreateChild<Camera>(B_IDNAME(obj)));
                break;
            case OB_MESH:       // Entity + Mesh
                break;
            case OB_ARMATURE:   // SceneNode + Skeleton
                break;
            case OB_CURVE:      // Curves
                break;
            default:
                break;
        }
    }

    void BScene::ConvertSceneNode(const Blender::Object* obj, PSceneNode sceneNode)
    {

    }

	void BScene::ConvertCamera(const Blender::Object* obj, PCamera camera)
	{
		CHECK_ASSERT(obj->data, __FILE__, __LINE__);

		Blender::Camera* bcamera = static_cast<Blender::Camera*>(obj->data);

		//if (bcamera->type == CAM_ORTHO)
		//else

		camera->SetNearClip(bcamera->clipsta);
		camera->SetFarClip(bcamera->clipend);
		//camera->SetAspectRatio(???);
		camera->SetHalfHorizontalFov(glm::radians(bcamera->lens));
		//isMainCamera = sc_->camera == obj;
	}

}
