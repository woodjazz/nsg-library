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

    auto window = app.GetOrCreateWindow("window", 100, 100, 50, 30);
    auto window1 = app.GetOrCreateWindow("window1", 200, 100, 50, 30);


	auto atlas = std::make_shared<FontAtlasTexture>("", window1->GetWidth(), window1->GetHeight());
	auto text = app.CreateTextMesh("text1", atlas);
	auto scene1 = app.GetOrCreateScene("scene1");
	auto node = scene1->GetOrCreateChild<SceneNode>("node1");
	node->SetPosition(Vertex3(0, 0, 0));
	auto material = app.GetOrCreateMaterial("material1");
	material->SetColor(Color(1, 1, 1, 1));
	material->SetTexture0(text->GetTexture());
	auto pass = PPass(new Pass);
	pass->EnableDepthTest(false);
	pass->EnableStencilTest(false);
	pass->SetProgram(text->GetProgram());
	node->Set(material);
	node->Set(text);

	ShowTexture show;
	show.SetNormal(text->GetTexture());
	//show.SetNormal(app.GetWhiteTexture());

	SignalViewChanged::PSlot resizeSlot1;
	SignalRender::PSlot renderSlot1;

	if (window1)
	{
		resizeSlot1 = window1->signalViewChanged_->Connect([&](int width, int height)
		{
			atlas->SetViewSize(width, height);
		});

		renderSlot1 = window1->signalRender_->Connect([&]()
		{
#if 0
			Graphics::this_->SetCamera(nullptr);
			Graphics::this_->SetScene(scene1.get());
			Graphics::this_->Set(material.get());
			Graphics::this_->SetNode(node.get());
			Graphics::this_->Set(text.get());
			text->SetText("Hello World!!!", LEFT_ALIGNMENT, TOP_ALIGNMENT);
			pass->Render();
#endif
			show.Show();
		});
	}

    auto scene = app.GetOrCreateScene("scene000");
    auto resource(app.GetOrCreateResourceFile("data/scene.xml"));

    PCamera camera;
    SignalViewChanged::PSlot resizeSlot;
    SignalUpdate::PSlot updateSlot;
    SignalRender::PSlot renderSlot;

    auto loadSlot = resource->signalLoaded_->Connect([&]()
    {
        scene->SceneNode::Load(resource);
        auto objNode = scene->GetOrCreateChild<SceneNode>("LOD3sp");
        auto objPos = objNode->GetGlobalPosition();
        auto objBB = objNode->GetWorldBoundingBox();
        objBB.max_ *= 1.75f;
        objBB.min_ *= 1.75f;
        camera = scene->GetOrCreateChild<Camera>("camera1");
        camera->SetAspectRatio(window->GetWidth(), window->GetHeight());
        camera->SetGlobalPosition(Vector3(0, objBB.max_.y, objBB.max_.z));
        camera->SetGlobalLookAt(objPos);

        resizeSlot = window->signalViewChanged_->Connect([&](int width, int height)
        {
            camera->SetAspectRatio(width, height);
        });

        auto animation = scene->GetOrCreateAnimation("anim0");
        AnimationTrack track;
        track.node_ = camera;
        track.channelMask_ = (int)AnimationChannel::POSITION | (int)AnimationChannel::ROTATION;

        {
            AnimationKeyFrame key(0, camera.get());
            track.keyFrames_.push_back(key);
        }

        {
            auto node = std::make_shared<Node>("node0");
            node->SetParent(camera->GetParent());
            node->SetGlobalPosition(Vector3(objBB.max_.x, objBB.max_.y, 0));
            node->SetGlobalLookAt(objPos);
            AnimationKeyFrame key(2, node.get());
            track.keyFrames_.push_back(key);
        }

        {
            auto node = std::make_shared<Node>("node1");
            node->SetParent(camera->GetParent());
            node->SetGlobalPosition(Vector3(0, objBB.max_.y, objBB.min_.z));
            node->SetGlobalLookAt(objPos);
            AnimationKeyFrame key(4, node.get());
            track.keyFrames_.push_back(key);
        }

        {
            auto node = std::make_shared<Node>("node2");
            node->SetParent(camera->GetParent());
            node->SetGlobalPosition(Vector3(objBB.min_.x, objBB.max_.y, 0));
            node->SetGlobalLookAt(objPos);
            AnimationKeyFrame key(6, node.get());
            track.keyFrames_.push_back(key);
        }

        animation->AddTrack(track);
        animation->SetLength(8);

        scene->PlayAnimation(animation, false);

        updateSlot = window->signalUpdate_->Connect([&](float deltaTime)
        {
            scene->Update(deltaTime);
        });

        renderSlot = window->signalRender_->Connect([&]()
        {
            scene->Render(camera.get());
        });
    });

    return app.Run();
}

