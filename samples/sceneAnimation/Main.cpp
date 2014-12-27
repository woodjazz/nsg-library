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
int NSG_MAIN(int argc, char *argv[])
{
    using namespace NSG;

    App app;
    SignalViewChanged::PSlot resizeSlot;
    auto window = app.GetOrCreateWindow("window", 50, 50, 1024, 768);
    auto scene = std::make_shared<Scene>("scene");
    auto camera = scene->GetOrCreateChild<Camera>("Camera");
    auto resource(app.GetOrCreateResourceFile("data/scene.xml"));
    auto loadSlot = resource->signalLoaded_->Connect([&]()
    {
        scene->SceneNode::Load(resource);

        camera->SetAspectRatio(window->GetWidth(), window->GetHeight());
        resizeSlot = window->signalViewChanged_->Connect([&](int width, int height)
        {
            camera->SetAspectRatio(width, height);
        });

        auto animations = scene->GetAnimationsFor(camera); 
        auto animation = animations[0];
        animation->Play(true);
    });

    
    auto updateSlot = window->signalUpdate_->Connect([&](float deltaTime)
    {
        scene->Update(deltaTime);
    });

    auto renderSlot = window->signalRender_->Connect([&]()
    {
        scene->Render(camera.get());
    });

    return app.Run();
}
