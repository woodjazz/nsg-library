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
    PScene scene_;
	PCamera camera_;
	PCameraControl cameraControl_;
	std::string resourcePath_;

	MyApp()
    {
        //AppConfiguration::this_->width_ = 30;
        //AppConfiguration::this_->height_ = 20;
        AppConfiguration::this_->showStatistics_ = false;
    }

    void Start(int argc, char* argv[]) override
    {
		scene_ = GetCurrentScene();
		camera_ = scene_->CreateCamera("EditorCamera");
		camera_->SetSerializable(false);
		camera_->SetInheritScale(false);
		cameraControl_ = PCameraControl(new CameraControl);
		camera_->AddBehavior(cameraControl_);
    }

    void DropFile(const std::string& filePath) override
    {
		Path path(filePath);
		resourcePath_ = path.GetPath();
		App::DropFile(path.GetFilePath());
		SceneConverter scene(path);
		if (!scene.Load())
		{
			std::string extension = path.GetExtension();
			if (extension == "xml")
			{
				PResourceFile resource(GetOrCreateResourceFile(path));
				scene_->Load(resource);
			}
		}

		camera_->Activate();
		cameraControl_->AutoZoom();
    }

    void RenderGUIWindow() override
	{
        using namespace IMGUI;
		IMGUIBeginHorizontal(100, Pixels2PercentageY(25));
		if (IMGUIButton("Save", 20))
		{
			pugi::xml_document doc;
			scene_->Save(doc);
			doc.save_file((resourcePath_ + "/scene.xml").c_str());
		}
		if (IMGUIButton("Point Light", 20))
		{
			PLight light = scene_->CreatePointLight("");
			light->SetPosition(Vertex3(100, 100, 100));
		}
        IMGUIEndArea();
    }
};

NSG_MAIN(MyApp);
