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

#include "NSG.h"
#include "BScene.h"

using namespace NSG;

int NSG_MAIN(int argc, char* argv[])
{
    auto window = Window::Create("Viewer", 0, 0, 600, 600);
    PAppData data;
    PCamera camera;
    PCameraControl control;
    Path inputFile;

    auto ConvertFile = [&]()
    {
		control = nullptr;
		camera = nullptr;
		data = nullptr;

		if (Path::GetLowercaseFileExtension(inputFile.GetFilename()) == "blend")
		{
			pugi::xml_document doc;
			{
				using namespace BlenderConverter;
				BScene bscene(inputFile, "", true);
				bscene.Load();
				bscene.GenerateXML(doc);
			}
			data = std::make_shared<AppData>(doc);
			if (!data->scenes_.empty())
			{
				auto scene = data->scenes_.at(0);
				camera = scene->GetMainCamera();
				if (!camera)
					camera = scene->CreateChild<Camera>();
				control = std::make_shared<CameraControl>(camera);
			}
		}
    };

	auto slotButtonClicked = window->SigDropFile()->Connect([&](const std::string& file)
    {
    	inputFile = Path(file);
    	ConvertFile();
    });

	Engine engine;
	float totalTime = 0;
    auto updateSlot = engine.SigUpdate()->Connect([&](float deltaTime)
    {
		totalTime += deltaTime;
		const float FREQUENCY = 2; // seconds
		if (inputFile.HasName() && totalTime > FREQUENCY)
    	{
			totalTime = 0;
    		static std::string lastModification;
    		auto mtime = inputFile.GetModificationTime();
    		if(lastModification.empty())
    			lastModification = mtime;
    		else if(lastModification != mtime)
    		{
    			lastModification = mtime;
    			ConvertFile();
    		}
    	}
    });

	return engine.Run();

}