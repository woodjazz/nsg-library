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
    auto engine = Engine::Create();
    auto& conf = engine->GetAppConfiguration();
    conf.platform_ = AppConfiguration::Platform::UseEGL;

    PAnimationController acontrol;
    auto window = Window::Create();

    LoaderXML loader("loader");
    auto slotLoaded =
        loader.Load(Resource::GetOrCreateClass<ResourceFile>("data/duck.xml"))
            ->Connect([&]() {
                auto scene = loader.GetScene(0);
                auto objNode = scene->GetOrCreateChild<SceneNode>("LOD3sp");
                auto objPos = objNode->GetGlobalPosition();
                auto objBB = objNode->GetWorldBoundingBox();
                objBB.max_ *= 1.75f;
                objBB.min_ *= 1.75f;
                auto camera = scene->GetOrCreateChild<Camera>("camera1");
                // camera->SetGlobalPosition(Vector3(0, objBB.max_.y,
                // objBB.max_.z));
                // camera->SetGlobalLookAtPosition(objPos);
                auto control = std::make_shared<CameraControl>(camera);

                auto animation = Animation::Create("anim0");
                AnimationTrack track;
                track.node_ = camera;
                track.channelMask_ = (int)AnimationChannel::POSITION |
                                     (int)AnimationChannel::ROTATION;

                {
                    AnimationKeyFrame key(0, camera.get());
                    track.keyFrames_.push_back(key);
                }

                {
                    auto node = std::make_shared<Node>("node0");
                    node->SetParent(camera->GetParent());
                    node->SetGlobalPosition(
                        Vector3(objBB.max_.x, objBB.max_.y, 0));
                    node->SetGlobalLookAtPosition(objPos);
                    AnimationKeyFrame key(2, node.get());
                    track.keyFrames_.push_back(key);
                }

                {
                    auto node = std::make_shared<Node>("node1");
                    node->SetParent(camera->GetParent());
                    node->SetGlobalPosition(
                        Vector3(0, objBB.max_.y, objBB.min_.z));
                    node->SetGlobalLookAtPosition(objPos);
                    AnimationKeyFrame key(4, node.get());
                    track.keyFrames_.push_back(key);
                }

                {
                    auto node = std::make_shared<Node>("node2");
                    node->SetParent(camera->GetParent());
                    node->SetGlobalPosition(
                        Vector3(objBB.min_.x, objBB.max_.y, 0));
                    node->SetGlobalLookAtPosition(objPos);
                    AnimationKeyFrame key(6, node.get());
                    track.keyFrames_.push_back(key);
                }

                animation->AddTrack(track);
                animation->SetLength(8);

                acontrol = std::make_shared<AnimationController>(camera);
                acontrol->Play(animation->GetName(), false);
                window->SetScene(scene);
            });
    return engine->Run();
}
