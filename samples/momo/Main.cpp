/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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

int NSG_MAIN(int argc, char* argv[]) {
    using namespace NSG;

    auto window = Window::Create();
    auto resource = Resource::GetOrCreate<ResourceFile>("data/scene.xml");
    LoaderXML loader("loader");
    PCameraControl control;
    auto slotLoaded = loader.Load(resource)->Connect([&]() {
        auto scene = loader.GetScene(0);
        auto camera = scene->GetOrCreateChild<Camera>("Camera");
        camera->SetWindow(window);
        control = std::make_shared<CameraControl>(camera);
        {
            auto armature = scene->GetChild<SceneNode>("RigMomo", true);
            auto controller = armature->GetOrCreateAnimationController();
            controller->Play("Momo_IdleNasty", true);
            control->Track(armature);
        }
        {
            auto armature = scene->GetChild<SceneNode>("RigMomo.001", true);
            auto controller = armature->GetOrCreateAnimationController();
            controller->Play("Momo_Run", true);
        }
        window->SetScene(scene);
    });
    auto engine = Engine::Create();
    return engine->Run();
}
