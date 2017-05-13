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
    PSceneNode armature;
    auto slotLoaded = loader.Load(resource)->Connect([&]() {
        auto scene = loader.GetScene(0);
        auto camera = scene->GetOrCreateChild<Camera>("Camera");
        camera->SetWindow(window);
        control = std::make_shared<CameraControl>(camera);
        armature = scene->GetChild<SceneNode>("RigMomo", true);
        control->Track(armature);

        window->SetScene(scene);

        static auto drawGUISlot = window->SigDrawIMGUI()->Connect([&]() {
            using namespace ImGui;

            static auto itemsGetter = [](void* data, int idx,
                                         const char** out_text) -> bool {
                LoaderXML* loader = static_cast<LoaderXML*>(data);
                auto animations = loader->GetAnimations();
                auto name = animations[idx]->GetName().c_str();
                *out_text = name;
                return true;
            };

            ImGui::Text("Animations:");
            static int selection = -1;
            static float fadeTime = 1.2f;
            ImGui::SliderFloat("Fade time (s)", &fadeTime, 0.0f, 10.0f);
            if (ImGui::ListBox("", &selection, itemsGetter, &loader,
                               (int)loader.GetAnimations().size())) {
                auto controller = armature->GetOrCreateAnimationController();
                controller->CrossFade(
                    loader.GetAnimations()[selection]->GetName(), true,
                    fadeTime);
            }
        });
    });

    auto engine = Engine::Create();
    return engine->Run();
}
