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
int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;

    App app;

    auto window = app.GetOrCreateWindow("window", 100, 100, 10, 10);
    auto scene = std::make_shared<Scene>("scene000");
    auto camera = scene->GetOrCreateChild<Camera>("camera");
    const float FOV = 30.0f;
    camera->SetFOV(FOV);
    auto control = std::make_shared<CameraControl>(camera);
    
    auto light = scene->GetOrCreateChild<Light>("light");
    light->SetType(LightType::SPOT);
    light->SetParent(camera);
    light->SetSpotCutOff(FOV);
    //light->SetDiffuseColor(Color(1, 0, 0, 1));

    auto resource = std::make_shared<ResourceFile>("data/dwarf.xml");
    scene->SceneNode::Load(resource);
    scene->SetAmbientColor(Color(0));
    auto material0 = app.GetMaterial("Material0");
	auto material1 = app.GetMaterial("Material1");
	material0->SetColor(Color(1, 1, 1, 1));
	material0->SetDiffuseColor(Color(1, 1, 1, 1));
	material1->SetColor(Color(1, 1, 1, 1));
	material1->SetDiffuseColor(Color(1, 1, 1, 1));
  
	//material0->GetTechnique()->GetPass(0)->GetProgram()->SetFlags((int)ProgramFlag::UNLIT | (int)ProgramFlag::DIFFUSEMAP);
    material0->GetTechnique()->GetPass(0)->GetProgram()->EnableFlags((int)ProgramFlag::PER_PIXEL_LIGHTING);
	material1->GetTechnique()->GetPass(0)->GetProgram()->EnableFlags((int)ProgramFlag::PER_PIXEL_LIGHTING);

    camera->SetPosition(Vector3(0, 70, 100));
    camera->SetAspectRatio(window->GetWidth(), window->GetHeight());
    auto resizeSlot = window->signalViewChanged_->Connect([&](int width, int height)
    {
        camera->SetAspectRatio(width, height);
    });

    control->AutoZoom();
    //light->SetPosition(Vertex3(100, 0, 0));
    //camera->AddChild(light);

    scene->PlayAnimation("AnimationSet0", true);
    scene->SetAnimationSpeed("AnimationSet0", 0.001f);

    auto node = scene->GetChild<Node>("Body", false);
    CHECK_ASSERT(node, __FILE__, __LINE__);


    auto updateSlot = window->signalUpdate_->Connect([&](float deltaTime)
    {
        scene->Update(deltaTime);
        control->OnUpdate(deltaTime);
    });

    auto renderSlot = window->signalRender_->Connect([&]()
    {
        scene->Render(camera.get());
    });

    auto slotMouseMoved = window->signalMouseMoved_->Connect([&](float x, float y)
    {
        control->OnMousemoved(x, y);
    });

    auto slotMouseDown = window->signalMouseDown_->Connect([&](int button, float x, float y)
    {
        control->OnMouseDown(button, x, y);
    });

    auto slotMouseUp = window->signalMouseUp_->Connect([&](int button, float x, float y)
    {
        control->OnMouseUp(button, x, y);
    });

    auto slotMouseWheel = window->signalMouseWheel_->Connect([&](float x, float y)
    {
        control->OnMousewheel(x, y);
    });

    auto slotMultiGesture = window->signalMultiGesture_->Connect([&](int timestamp, float x, float y, float dTheta, float dDist, int numFingers)
    {
        control->OnMultiGesture(timestamp, x, y, dTheta, dDist, numFingers);
    });

    auto slotKey = window->signalKey_->Connect([&](int key, int action, int modifier)
    {
        control->OnKey(key, action, modifier);
    });


    return app.Run();
}
