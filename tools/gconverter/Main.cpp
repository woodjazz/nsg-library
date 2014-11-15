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
#include "SceneConverter.h"
using namespace NSG;


struct MyApp : App
{
	PCamera camera_;
	std::string resourcePath_;
	std::shared_ptr<SceneConverter> sceneConverter_;

	MyApp()
    {
        //AppConfiguration::this_->width_ = 30;
        //AppConfiguration::this_->height_ = 20;
        AppConfiguration::this_->showStatistics_ = false;
    }

    void Start(int argc, char* argv[]) override
    {
    }

    void DropFile(const std::string& filePath) override
    {
		camera_ = nullptr;
		sceneConverter_ = nullptr;
		ClearAll();
		Path path(filePath);
		resourcePath_ = path.GetPath();
		App::DropFile(path.GetFilePath());
		sceneConverter_ = std::make_shared<SceneConverter>(path);
		if (!sceneConverter_->Load())
		{
			std::string extension = path.GetExtension();
			if (extension == "xml")
			{
				PResourceFile resource(GetOrCreateResourceFile(path));
				sceneConverter_->GetScene()->Load(resource);
			}
		}

		PScene scene = sceneConverter_->GetScene();
		camera_ = scene->GetOrCreateChild<Camera>("EditorCamera");
		camera_->SetSerializable(false);
		camera_->SetInheritScale(false);
		PCameraControl cameraControl = std::make_shared<CameraControl>();
		camera_->AddBehavior(cameraControl);
		cameraControl->Start();
		camera_->Activate();
		cameraControl->AutoZoom();
    }

    void RenderGUIWindow() override
	{
        using namespace IMGUI;
		IMGUIBeginHorizontal(100, Pixels2PercentageY(25));
		if (IMGUIButton("Save", 20))
		{
			sceneConverter_->Save((resourcePath_ + "/scene.xml").c_str());
		}
		if (IMGUIButton("Point Light", 20))
		{
			PLight light = sceneConverter_->GetScene()->GetOrCreateChild<Light>("PointLight");
			light->SetSerializable(false);
			light->SetParent(camera_);
		}
        IMGUIEndArea();
    }
};

NSG_MAIN(MyApp);
