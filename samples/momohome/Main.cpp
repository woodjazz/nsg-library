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
    PSceneNode player;
    LoaderXML loader("loader");
    auto slotLoaded =
        loader.Load(Resource::GetOrCreateClass<ResourceFile>("data/scene.xml"))
            ->Connect([&]() {
                auto scene = loader.GetScene(0);
                auto light = scene->GetChild<Light>("Sun", false);
                static ShadowMapDebug shadowMapDebug(window, light);
                auto camera = scene->GetChild<Camera>("Camera", false);
                camera->SetWindow(window);
                static auto control = std::make_shared<CameraControl>(camera);
                static auto followCamera =
                    std::make_shared<FollowCamera>(camera);
                player = scene->GetChild<SceneNode>("RigMomo", true);
                static auto character = player->GetCharacter();
                followCamera->Track(player->GetCharacter().get(), 25);
                static float turn = 0;

                struct State : FSM::State {
                    bool loop_;
                    float time_;
                    PScene scene_;
                    PSceneNode player_;
                    PCharacter character_;
                    PAnimationController controller_;
                    const char* animName_;
                    float deltaTime_;
                    State(const char* animName, PScene scene)
                        : loop_(true), time_(0), scene_(scene),
                          animName_(animName), deltaTime_(0) {
                        player_ = scene_->GetChild<SceneNode>("RigMomo", true);
                        controller_ = player_->GetOrCreateAnimationController();
                        character_ = player_->GetCharacter();
                    }
                    void Begin() override {
                        controller_->CrossFade(animName_, loop_, 0.5f);
                    }
                    void Stay() override {
                        deltaTime_ = Engine::GetPtr()->GetDeltaTime();
                        time_ += Engine::GetPtr()->GetDeltaTime();
                    }
                    void End() override { time_ = 0; }
                };

                struct Idle : State {
                    Idle(PScene scene) : State("Momo_IdleNasty", scene) {}
                    void Stay() override {
                        State::Stay();
                        character_->SetForwardSpeed(0);
                        character_->SetAngularSpeed(0);
                    }
                } static idle(scene);

                struct Walk : State {
                    Walk(PScene scene) : State("Momo_Walk", scene) {}
                    void Stay() override {
                        State::Stay();
                        character_->SetForwardSpeed(5);
                        character_->SetAngularSpeed(90.f * turn);
                    }
                } static walk(scene);

                struct WalkBack : State {
                    WalkBack(PScene scene) : State("Momo_WalkBack", scene) {}
                    void Stay() override {
                        State::Stay();
                        character_->SetForwardSpeed(-3);
                        character_->SetAngularSpeed(90.f * turn);
                    }
                } static walkBack(scene);

                struct Run : State {
                    Run(PScene scene) : State("Momo_Run", scene) {}
                    void Stay() override {
                        State::Stay();
                        character_->SetForwardSpeed(15);
                        character_->SetAngularSpeed(90.f * turn);
                    }
                } static run(scene);

                struct TurnL : State {
                    TurnL(PScene scene) : State("Momo_Turn.R", scene) {}
                    void Stay() override {
                        State::Stay();
                        character_->SetForwardSpeed(0);
                        character_->SetAngularSpeed(90.f * turn);
                    }
                } static turnL(scene);

                struct TurnR : State {
                    TurnR(PScene scene) : State("Momo_Turn.L", scene) {}
                    void Stay() override {
                        State::Stay();
                        character_->SetForwardSpeed(0);
                        character_->SetAngularSpeed(90.f * turn);
                    }
                } static turnR(scene);

                static bool buttonA = false;
                struct Jump : State {
                    Jump(PScene scene) : State("Momo_Jump", scene) {
                        loop_ = false;
                    }
                    void Begin() override {
                        State::Begin();
                        character_->SetJumpSpeed(20);
                        buttonA = false;
                    }
                    void Stay() override {
                        State::Stay();
                        character_->SetJumpSpeed(0);
                    }
                    void End() override {
                        character_->SetJumpSpeed(0);
                        State::End();
                    }
                } static jump(scene);

                struct Glide : State {
                    Glide(PScene scene) : State("Momo_Glide", scene) {}
                    void Begin() override {
                        character_->EnableFly(true);
                        State::Begin();
                    }
                    void Stay() override {
                        State::Stay();
                        character_->SetForwardSpeed(20);
                        character_->SetAngularSpeed(90.f * turn);
                    }
                    void End() override {
                        character_->EnableFly(false);
                        State::End();
                    }
                } static glide(scene);

                struct Fall : State {
                    Fall(PScene scene) : State("Momo_FallUp", scene) {}
                    void Begin() override { State::Begin(); }
                } static fall(scene);

                static auto IsFalling = [&]() {
                    return character->IsFalling();
                };

                static float speed = 0;
                static FSM::Machine fsm(idle);
                idle.AddTransition(walk).When([&]() { return speed > 0; });
                idle.AddTransition(walkBack).When([&]() { return speed < 0; });
                idle.AddTransition(turnL).When([&]() { return turn < 0; });
                idle.AddTransition(turnR).When([&]() { return turn > 0; });
                idle.AddTransition(jump).When([&]() { return buttonA; });
                idle.AddTransition(fall).When([&]() { return IsFalling(); });
                jump.AddTransition(fall).When([&]() { return IsFalling(); });
                jump.AddTransition(glide).When([&]() { return buttonA; });
                fall.AddTransition(idle).When([&]() { return !IsFalling(); });
                fall.AddTransition(glide).When([&]() { return buttonA; });
                glide.AddTransition(fall).When([&]() { return !buttonA; });
                turnL.AddTransition(idle).When([&]() { return turn == 0; });
                turnL.AddTransition(turnR).When([&]() { return turn > 0; });
                turnL.AddTransition(walk).When([&]() { return speed > 0; });
                turnR.AddTransition(idle).When([&]() { return turn == 0; });
                turnR.AddTransition(turnL).When([&]() { return turn < 0; });
                turnR.AddTransition(walk).When([&]() { return speed > 0; });
                walk.AddTransition(run).When(
                    [&]() { return speed > 0 && walk.time_ > 1.3f; });
                walk.AddTransition(idle).When(
                    [&]() { return speed == 0 && walk.time_ > 1; });
                walk.AddTransition(idle).When([&]() { return speed < 0; });
                walk.AddTransition(jump).When([&]() { return buttonA; });
                walkBack.AddTransition(idle).When([&]() { return speed >= 0; });
                run.AddTransition(walk).When(
                    [&]() { return speed == 0 && run.time_ > 1; });
                run.AddTransition(jump).When([&]() { return buttonA; });
                window->SetScene(scene);
                fsm.Go();

                static auto playerControl =
                    std::make_shared<PlayerControl>(window);

                static auto slotMoved =
                    playerControl->SigMoved()->Connect([&](float x, float y) {
                        speed = y;
                        turn = x;
                    });

                static auto slotStick =
                    playerControl->SigLeftStickMoved()->Connect(
                        [&](float x, float y) {
                            speed = y;
                            turn = x;
                        });

                static auto slotButtonA = playerControl->SigButtonA()->Connect(
                    [&](bool pressed) { buttonA = pressed; });

            });

    auto scene = std::make_shared<Scene>();
    auto font = std::make_shared<FontXMLAtlas>();
    auto loadingNode = scene->CreateChild<SceneNode>();
    auto loadingMaterial = Material::Create();
    font->SetXML(
        Resource::GetOrCreate<ResourceFile>("data/AnonymousPro32.xml"));
    auto atlasTexture = std::make_shared<Texture2D>(
        Resource::GetOrCreate<ResourceFile>("data/AnonymousPro32.png"));
    font->SetTexture(atlasTexture);
    auto camera = scene->CreateChild<Camera>();
    camera->SetPosition(Vector3(0, 0, 1));
    camera->EnableOrtho();
    loadingNode->SetMesh(font->GetOrCreateMesh("Loading...", CENTER_ALIGNMENT,
                                               MIDDLE_ALIGNMENT));
    loadingMaterial->SetFontAtlas(font);
    loadingNode->SetMaterial(loadingMaterial);
    window->SetScene(scene);
    auto engine = Engine::Create();
    return engine->Run();
}
