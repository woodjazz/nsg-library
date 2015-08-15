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
int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;
    auto engine = Engine::Create();
    auto window = Window::Create();
    auto resource = Resource::GetOrCreate<ResourceFile>("data/scene.xml");
    auto appData = std::make_shared<AppData>();
	auto scene = std::make_shared<Scene>();
	auto font = std::make_shared<FontAtlas>();
	auto loadingNode = scene->CreateChild<SceneNode>();
	auto loadingMaterial = Material::Create();
    {
		auto xmlResource = Resource::GetOrCreate<ResourceFile>("data/AnonymousPro32.xml");
		font->Set(xmlResource);
		auto atlasResource = Resource::GetOrCreate<ResourceFile>("data/AnonymousPro32.png");
		auto atlasTexture = std::make_shared<Texture2D>(atlasResource);
		font->SetTexture(atlasTexture);
        
        auto camera = scene->CreateChild<Camera>();
        camera->SetPosition(Vector3(0, 0, 2));
        camera->EnableOrtho();
        camera->SetWindow(nullptr);
		loadingNode->SetMesh(font->GetOrCreateMesh("Loading...", CENTER_ALIGNMENT, MIDDLE_ALIGNMENT));
		loadingMaterial->SetTextMap(atlasTexture);
		loadingNode->SetMaterial(loadingMaterial);
		window->SetScene(scene.get());
    }    
    engine->RenderFrame(); // force load

    PCameraControl control;
    bool loaded = false;
    auto load = [&]()
    {
        auto loader = LoaderXML::GetOrCreate(resource->GetName());
        loader->Set(resource);
        appData->Load(loader);
        auto scene = appData->scenes_.at(0);
        auto camera = scene->GetOrCreateChild<Camera>("Camera");
        control = std::make_shared<CameraControl>(camera);
        {
            auto object = scene->GetChild<SceneNode>("RigMomo", true);
            auto animation = scene->GetAnimationFor("Momo_Carry", object);
            scene->PlayAnimation(animation, true);
        }
        window->SetScene(scene.get());
        loaded = true;
    };

    auto slotUpdate = engine->SigUpdate()->Connect([&](float deltaTime)
    {
		if (!loaded)
		{
			if(AppData::AreReady())
				load();
			else
			{
				static float alpha = loadingMaterial->GetAlpha();
				static float alphaAdd = 0.1f;
				if (alpha > 1 || alpha < 0.1f)
					alphaAdd *= -1;
				loadingMaterial->SetAlpha(alpha);
				alpha += alphaAdd;
			}
		}
    });
	
    return engine->Run();
}
