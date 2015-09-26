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

using namespace NSG;
int NSG_MAIN(int argc, char* argv[])
{
    auto window = Window::Create();
    PCameraControl control;
    PLoaderApp loader;
	PShadowMapDebug shadowDebug;

    std::string status;
    auto load = [&](const std::string & file)
    {
        status = "Loading...";
        Engine::ReleaseMemory();
        control = nullptr;
		shadowDebug = nullptr;
        auto path = Path(file);
        auto resource = Resource::GetOrCreateClass<ResourceFile>(path.GetFilePath());
        loader = std::make_shared<LoaderApp>(resource);
        static SignalEmpty::PSlot slotLoaded;
        slotLoaded = loader->Load()->Connect([&]()
        {
            auto scene = loader->GetScene(0);
            if (scene)
            {
                window->SetScene(scene.get());
                auto camera = scene->GetMainCamera();
                if (camera)
                    control = std::make_shared<CameraControl>(camera);
				auto light = scene->GetChild<Light>("Sun", true);
				if (light)
					shadowDebug = std::make_shared<ShadowMapDebug>(light);
            }
            status = "";
        });
    };

    auto slotButtonClicked = window->SigDropFile()->Connect([&](const std::string & file)
    {
        load(file);
    });

    auto drawGUISlot = window->SigDrawIMGUI()->Connect([&]()
    {
        using namespace ImGui;

		const char* items[] = { 
			"fog", 
			"physics", 
			"shadowdir",
			"shadowpoint",
			"shadowspot",
			"transparency",
			"uvs"
		};

        ImGui::Text("Scenes:");
        static int selection = -1;
		if (ImGui::ListBox(status.c_str(), &selection, items, sizeof(items)/sizeof(char*)))
		{
			std::string file = "data/" + std::string(items[selection]) + "/scene.xml";
			load(file);
		}
    });

    return Engine::Create()->Run();

}