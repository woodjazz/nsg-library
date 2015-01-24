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
    PMaterial material_;
    PTexture lightmapTexture_;
    PTexture diffuseTexture_;
    PTexture normalTexture_;
    PTexture specularTexture_;
    PTexture occTexture_;
    PTexture dispTexture_;
    PCameraControl control_;
    SignalStart::PSlot slotStart_;
	SignalXMLLoaded::PSlot slotXmlLoaded_;

    Sample()
    {
        //AppConfiguration::this_->showStatistics_ = true;
        slotStart_ = signalStart_->Connect([&](int argc, char* argv[])
        {
			scene_ = GetOrCreateScene("scene000");
			SetCurrentScene(scene_);
            scene_->SetAmbientColor(Color(0.3f, 0.3f, 0.3f, 1));

            PResourceFile resource(GetOrCreateResourceFile("data/scene.xml"));
            scene_->Load(resource);
			slotXmlLoaded_ = scene_->signalXMLLoaded_->Connect([&]()
            {
                auto& cameras = scene_->GetCameras();
                PCamera camera = cameras[0].lock();
                camera->SetFOV(60);
                control_ = PCameraControl(new CameraControl(camera));

                PLight light = scene_->GetChild<Light>("Lamp", true);
                light->SetParent(camera);

                camera->Activate();
                camera->SetGlobalPosition(Vector3(0, 5, 5));
                camera->SetGlobalLookAt(VECTOR3_ZERO);

                lightmapTexture_ = GetOrCreateTextureFile("data/lightmap.png");
                diffuseTexture_ = GetOrCreateTextureFile("data/wall_COLOR.png");
                normalTexture_ = GetOrCreateTextureFile("data/wall_NRM.png");
                specularTexture_ = GetOrCreateTextureFile("data/wall_SPEC.png");
                occTexture_ = GetOrCreateTextureFile("data/wall_OCC.png");
                dispTexture_ = GetOrCreateTextureFile("data/wall_DISP.png");

                material_ = scene_->GetChild<SceneNode>("Cube", false)->GetMaterial();
                material_->SetDiffuseMap(diffuseTexture_);
                material_->SetLightMap(lightmapTexture_);
                material_->SetNormalMap(normalTexture_);
                material_->SetSpecularMap(specularTexture_);
                material_->SetAOMap(occTexture_);
                material_->SetDisplacementMap(dispTexture_);
            });
        });
    }

    void RenderGUIWindow() override
    {
        IMGUIBeginHorizontal(100, 10);
        {
            bool on = material_->GetDisplacementMap() != nullptr;
            if (IMGUICheckButton(on, on ? "Disable Parallax" : "Enable Parallax", 25, 100))
                material_->SetDisplacementMap(dispTexture_);
            else
                material_->SetDisplacementMap(nullptr);
        }

        {
            float parallaxScale = material_->GetParallaxScale();
            parallaxScale = IMGUIHSlider(parallaxScale, 75, 100, 10);
            material_->SetParallaxScale(parallaxScale);
        }
        IMGUIEndArea();

        {
            bool on = material_->GetNormalMap() != nullptr;
            if (IMGUICheckButton(on, on ? "Disable normal map" : "Enable normal map", 25, 10))
                material_->SetNormalMap(normalTexture_);
            else
                material_->SetNormalMap(nullptr);
        }


        {
            bool on = material_->GetLightMap() != nullptr;
            if (IMGUICheckButton(on, on ? "Disable Lightmap" : "Enable Lightmap", 25, 10))
                material_->SetLightMap(lightmapTexture_);
            else
                material_->SetLightMap(nullptr);
        }

        {
            bool on = material_->GetSpecularMap() != nullptr;
            if (IMGUICheckButton(on, on ? "Disable Specular map" : "Enable Specular map", 25, 10))
                material_->SetSpecularMap(specularTexture_);
            else
                material_->SetSpecularMap(nullptr);
        }

        {
            bool on = material_->GetAOMap() != nullptr;
            if (IMGUICheckButton(on, on ? "Disable AO map" : "Enable AO map", 25, 10))
                material_->SetAOMap(occTexture_);
            else
                material_->SetAOMap(nullptr);
        }

        {
            bool on = material_->GetDiffuseMap() != nullptr;
            if (IMGUICheckButton(on, on ? "Disable diffuse map" : "Enable diffuse map", 25, 10))
                material_->SetDiffuseMap(diffuseTexture_);
            else
                material_->SetDiffuseMap(nullptr);
        }

    }

};

NSG_MAIN(Sample);
